#include <QtWidgets>
#include "view/flip_widget.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    FlipWidget window;
    window.show();
    return app.exec();
}
