#include <QtWidgets>

#include "persistence/sql_persistence.h"

int main(int argc, char* argv[])
{
    try
    {
        SQLPersistence sqlPersistence("FlipTranslate.db");
        QApplication app(argc, argv);
        QWidget window;
        window.setWindowTitle("Hello world!");
        window.resize(320, 240);
        window.show();
        return app.exec();
    }
    catch(const std::exception& e)
    {
        qFatal(e.what());
    }
}
