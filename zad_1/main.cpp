#include <gtkmm.h>
#include <iostream>
#include <string>


Gtk::Entry* eAx = nullptr;
Gtk::Entry* eAy = nullptr;
Gtk::Entry* eBx = nullptr;
Gtk::Entry* eBy = nullptr;
Gtk::Entry* ePx = nullptr;
Gtk::Entry* ePy = nullptr;

Gtk::Label* lAnswer = nullptr;

Gtk::Label* lDeterminant = nullptr;
Gtk::Label* lProduct = nullptr;
Gtk::Label* lDifference = nullptr;

struct point {
    long double x;
    long double y;
};

long double det(point A, point B, point P) {
    return A.x * B.y - A.x * P.y + B.x * P.y - B.x * A.y + P.x * A.y - P.x * B.y;
}

long double vec_mul(point A, point B, point P) {
    return (A.x - B.x) * (A.y - P.y) - (A.x - P.x) * (A.y - B.y);
}

static void on_compute_clicked() {
    eAx->get_style_context()->remove_class("error");
    eAy->get_style_context()->remove_class("error");
    eBx->get_style_context()->remove_class("error");
    eBy->get_style_context()->remove_class("error");
    ePx->get_style_context()->remove_class("error");
    ePy->get_style_context()->remove_class("error");
    
    point A;
    try {
        A.x = std::stold(eAx->get_text());
    } catch (std::invalid_argument&) {
        eAx->get_style_context()->add_class("error");
        return;
    }
    try {
        A.y = std::stold(eAy->get_text());
    } catch (std::invalid_argument&) {
        eAy->get_style_context()->add_class("error");
        return;
    }

    point B;
    try {
        B.x = std::stold(eBx->get_text());
    } catch (std::invalid_argument&) {
        eBx->get_style_context()->add_class("error");
        return;
    }
    try {
        B.y = std::stold(eBy->get_text());
    } catch (std::invalid_argument&) {
        eBy->get_style_context()->add_class("error");
        return;
    }

    point P;
    try {
        P.x = std::stold(ePx->get_text());
    } catch (std::invalid_argument&) {
        ePx->get_style_context()->add_class("error");
        return;
    }
    try {
        P.y = std::stold(ePy->get_text());
    } catch (std::invalid_argument&) {
        ePy->get_style_context()->add_class("error");
        return;
    }
    
    long double determinant = det(A, B, P);
    long double product = vec_mul(A, B, P);
    long double difference = determinant - product;
    
    if (determinant > 0) {
        lAnswer->set_text("Punkt P położony jest po prawej stronie odcinka AB");
    } else if (determinant < 0) {
        lAnswer->set_text("Punkt P położony jest po lewej stronie odcinka AB");
    } else {
        lAnswer->set_text("Punkty A, B i P są współliniowe");
    }
    
    lDeterminant->set_text(std::to_string(determinant));
    lProduct->set_text(std::to_string(product));
    lDifference->set_text(std::to_string(difference));
}

int main(int argc, char *argv[]) {
    auto app = Gtk::Application::create(argc, argv, "pl.lebihan.zad_1");

    Glib::RefPtr<Gtk::Builder> builder;
    builder = Gtk::Builder::create_from_resource("/main.glade");

    Gtk::Window *window;
    builder->get_widget("window", window);
    
    Gtk::Button* bCompute = nullptr;
    builder->get_widget("compute", bCompute);
    
    bCompute->signal_clicked().connect( sigc::ptr_fun(on_compute_clicked) );

    builder->get_widget("ax", eAx);
    builder->get_widget("ay", eAy);
    builder->get_widget("bx", eBx);
    builder->get_widget("by", eBy);
    builder->get_widget("px", ePx);
    builder->get_widget("py", ePy);
    
    builder->get_widget("answer", lAnswer);
    
    builder->get_widget("determinant", lDeterminant);
    builder->get_widget("product", lProduct);
    builder->get_widget("difference", lDifference);

    return app->run(*window);
}
