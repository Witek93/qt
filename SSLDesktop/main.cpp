#include <QApplication>

#include "SslApp/sslapp.h"
#include "SslBox/sslbox.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SslApp app;
    SslBox box;

    app.setGeometry(0, 0, 500, 300);
    box.setGeometry(500, 0, 500, 300);

    app.show();
    box.show();


    return a.exec();
}
