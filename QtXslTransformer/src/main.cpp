#include <xsl_transformer/xsl_transformer_dialog.h>

#include <QApplication>

int main(int argc, char** argv)
{
    QApplication::setApplicationName("Qt XSL Transformer");
    QApplication app(argc, argv);

    XslTrassformerDlg wnd;
    wnd.show();

    return app.exec();
}
