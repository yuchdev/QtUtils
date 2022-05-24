#include <qt_directory_hook/application_window.h>
#include <QApplication>


int main(int argc, char **argv)
{
    setlocale(LC_ALL, "en-US");

    QApplication::setApplicationName("Filesystem Changes Hook");
    QApplication app(argc, argv);

    FilesystemHookApplicationWindow wnd;
    const int height = 500;
    const int goldenRatioWidth = 500*1.6;
    wnd.setMinimumHeight(height);
    wnd.setMinimumWidth(goldenRatioWidth);
    wnd.show();

    return app.exec();
}
