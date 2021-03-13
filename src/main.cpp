#include <QtWidgets>

#include "model/flip_model.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    FlipModel model;
    model.translate(model.German, model.Hungarian, "Deutschland");
    QWidget window;
    window.setWindowTitle("Hello world!");
    window.resize(320, 240);
    //window.show();

    return app.exec();
}
