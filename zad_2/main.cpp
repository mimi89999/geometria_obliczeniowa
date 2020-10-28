#include <gtkmm.h>
#include <json/json.h>
#include <fstream>
#include <iostream>
#include <string>
#include <unistd.h>


struct point {
    long double x;
    long double y;
};

struct line {
    point begin;
    point end;
    Gdk::RGBA color;
    double width;
};

struct drawingData {
    line line1;
    line line2;
};

Gtk::Entry* eAx = nullptr;
Gtk::Entry* eAy = nullptr;
Gtk::Entry* eBx = nullptr;
Gtk::Entry* eBy = nullptr;
Gtk::Entry* eCx = nullptr;
Gtk::Entry* eCy = nullptr;
Gtk::Entry* eDx = nullptr;
Gtk::Entry* eDy = nullptr;

Gtk::FileChooserButton* fcbImport = nullptr;
Gtk::FileChooserButton* fcbExport = nullptr;

Gtk::Label* lPx = nullptr;
Gtk::Label* lPy = nullptr;

Gtk::ColorButton* cbLine1 = nullptr;
Gtk::ColorButton* cbLine2 = nullptr;

Gtk::SpinButton* spbLine1 = nullptr;
Gtk::SpinButton* spbLine2 = nullptr;

Gtk::DrawingArea* visualizationArea = nullptr;
drawingData visualizationData_obj;
drawingData* visualizationData = &visualizationData_obj;

static void on_import_clicked() {
    std::ifstream input_file(fcbImport->get_filename());
    Json::Value input_json;
    input_file >> input_json;

    eAx->set_text(input_json["A"]["x"].asString());
    eAy->set_text(input_json["A"]["y"].asString());
    eBx->set_text(input_json["B"]["x"].asString());
    eBy->set_text(input_json["B"]["y"].asString());
    eCx->set_text(input_json["C"]["x"].asString());
    eCy->set_text(input_json["C"]["y"].asString());
    eDx->set_text(input_json["D"]["x"].asString());
    eDy->set_text(input_json["D"]["y"].asString());
}

static bool on_draw(const ::Cairo::RefPtr< ::Cairo::Context>& visualization) {
    if (visualizationData == nullptr) {
        return true;
    }

    point A = visualizationData->line1.begin;
    point B = visualizationData->line1.end;
    point C = visualizationData->line2.begin;
    point D = visualizationData->line2.end;

    Gdk::RGBA rgbaLine1 = visualizationData->line1.color;
    Gdk::RGBA rgbaLine2 = visualizationData->line2.color;

    visualization->set_source_rgb(rgbaLine1.get_red(), rgbaLine1.get_green(), rgbaLine1.get_blue());
    visualization->set_line_width(visualizationData->line1.width);
    visualization->move_to(A.y, visualizationArea->get_height() - A.x);
    visualization->line_to(B.y, visualizationArea->get_height() - B.x);
    visualization->stroke();

    visualization->set_source_rgb(rgbaLine2.get_red(), rgbaLine2.get_green(), rgbaLine2.get_blue());
    visualization->set_line_width(visualizationData->line2.width);
    visualization->move_to(C.y, visualizationArea->get_height() - C.x);
    visualization->line_to(D.y, visualizationArea->get_height() - D.x);
    visualization->stroke();

    return true;
}

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
    visualizationData->line1.begin = A;

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
    visualizationData->line1.end = B;

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
    visualizationData->line2.begin = C;

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
    visualizationData->line2.end = D;

    visualizationData->line1.color = cbLine1->get_rgba();
    visualizationData->line2.color = cbLine2->get_rgba();

    visualizationData->line1.width = spbLine1->get_value();
    visualizationData->line2.width = spbLine2->get_value();

    double t = ((C.x - A.x) * (D.y - C.y) - (C.y - A.y) * (D.x - C.x)) /
        ((B.x - A.x) * (D.y - C.y) - (B.y - A.y) * (D.x - C.x));

    lPx->set_text(std::to_string(A.x + t * (B.x - A.x)));
    lPy->set_text(std::to_string(A.y + t * (B.y - A.y)));

    visualizationArea->signal_draw().connect( sigc::ptr_fun(on_draw) );
    visualizationArea->queue_draw();
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

    builder->get_widget("import", fcbImport);
    builder->get_widget("export", fcbExport);

    builder->get_widget("px", lPx);
    builder->get_widget("py", lPy);

    builder->get_widget("line1", cbLine1);
    builder->get_widget("line2", cbLine2);

    builder->get_widget("line1_width", spbLine1);
    builder->get_widget("line2_width", spbLine2);

    builder->get_widget("drawing_area", visualizationArea);

    fcbImport->signal_file_set().connect( sigc::ptr_fun(on_import_clicked) );

    return app->run(*window);
}
