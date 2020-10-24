#include <gtkmm.h>
#include <iostream>
#include <string>
#include <unistd.h>


Gtk::Entry* eAx = nullptr;
Gtk::Entry* eAy = nullptr;
Gtk::Entry* eBx = nullptr;
Gtk::Entry* eBy = nullptr;
Gtk::Entry* eCx = nullptr;
Gtk::Entry* eCy = nullptr;
Gtk::Entry* eDx = nullptr;
Gtk::Entry* eDy = nullptr;

Gtk::Label* lPx = nullptr;
Gtk::Label* lPy = nullptr;

Gtk::ColorButton* cbLine1 = nullptr;
Gtk::ColorButton* cbLine2 = nullptr;

Gtk::DrawingArea* visualizationArea = nullptr;

struct point {
    long double x;
    long double y;
};

static void on_compute_clicked() {
    eAx->get_style_context()->remove_class("error");
    eAy->get_style_context()->remove_class("error");
    eBx->get_style_context()->remove_class("error");
    eBy->get_style_context()->remove_class("error");
    eCx->get_style_context()->remove_class("error");
    eCy->get_style_context()->remove_class("error");
    eDx->get_style_context()->remove_class("error");
    eDy->get_style_context()->remove_class("error");

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

    point C;
    try {
        C.x = std::stold(eCx->get_text());
    } catch (std::invalid_argument&) {
        eCx->get_style_context()->add_class("error");
        return;
    }
    try {
        C.y = std::stold(eCy->get_text());
    } catch (std::invalid_argument&) {
        eCy->get_style_context()->add_class("error");
        return;
    }

    point D;
    try {
        D.x = std::stold(eDx->get_text());
    } catch (std::invalid_argument&) {
        eDx->get_style_context()->add_class("error");
        return;
    }
    try {
        D.y = std::stold(eDy->get_text());
    } catch (std::invalid_argument&) {
        eDy->get_style_context()->add_class("error");
        return;
    }

    double t = ((C.x - A.x) * (D.y - C.y) - (C.y - A.y) * (D.x - C.x)) /
        ((B.x - A.x) * (D.y - C.y) - (B.y - A.y) * (D.x - C.x));

    lPx->set_text(std::to_string(A.x + t * (B.x - A.x)));
    lPy->set_text(std::to_string(A.y + t * (B.y - A.y)));

    Cairo::RefPtr<Cairo::Context> visualization = visualizationArea->get_window()->create_cairo_context();
    Gdk::RGBA rgbaLine1 = cbLine1->get_rgba();
    visualization->set_source_rgb(rgbaLine1.get_red(), rgbaLine1.get_green(), rgbaLine1.get_blue());
    visualization->set_line_width(2.0);
    visualization->move_to(A.y, visualizationArea->get_height() - A.x);
    visualization->line_to(B.y, visualizationArea->get_height() - B.x);
    visualization->stroke();
    Gdk::RGBA rgbaLine2 = cbLine2->get_rgba();
    visualization->set_source_rgb(rgbaLine2.get_red(), rgbaLine2.get_green(), rgbaLine2.get_blue());
    visualization->move_to(C.y, visualizationArea->get_height() - C.x);
    visualization->line_to(D.y, visualizationArea->get_height() - D.x);
    visualization->stroke();
}

int main(int argc, char *argv[]) {
    auto app = Gtk::Application::create(argc, argv, "pl.lebihan.zad_1");

    Glib::RefPtr<Gtk::Builder> builder;
    builder = Gtk::Builder::create_from_resource("/main.glade");

    Gtk::Window *window = nullptr;
    builder->get_widget("window", window);

    Gtk::Button* bCompute = nullptr;
    builder->get_widget("compute", bCompute);

    bCompute->signal_clicked().connect( sigc::ptr_fun(on_compute_clicked) );

    builder->get_widget("ax", eAx);
    builder->get_widget("ay", eAy);
    builder->get_widget("bx", eBx);
    builder->get_widget("by", eBy);
    builder->get_widget("cx", eCx);
    builder->get_widget("cy", eCy);
    builder->get_widget("dx", eDx);
    builder->get_widget("dy", eDy);

    builder->get_widget("px", lPx);
    builder->get_widget("py", lPy);

    builder->get_widget("line1", cbLine1);
    builder->get_widget("line2", cbLine2);

    builder->get_widget("drawing_area", visualizationArea);

    return app->run(*window);
}
