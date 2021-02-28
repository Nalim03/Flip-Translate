#include <QtWidgets>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    QWidget window;
    window.setWindowTitle("Hello world!");
    window.resize(320, 240);
    window.show();
    return app.exec();
}
