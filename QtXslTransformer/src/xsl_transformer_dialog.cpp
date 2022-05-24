#include <xsl_transformer/xsl_transformer_dialog.h>

#include <QLayout>
#include <QFile>
#include <QXmlQuery>
#include <QUrl>
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QStandardPaths>

const char* XslTrassformerDlg::tempXmlFile = "E36EA2C6-FD7D-4649-8518-5ADBFEB7824E.xml";

XslTrassformerDlg::XslTrassformerDlg(QWidget *parent /*= 0*/)
{
    QHBoxLayout* buttons = new QHBoxLayout;
    openTsButton_ = new QPushButton("Open TS");
    openXslButton_ = new QPushButton("Open XSL");
    transformButton_ = new QPushButton("Transform");
    buttons->addWidget(openTsButton_);
    buttons->addWidget(openXslButton_);
    buttons->addWidget(transformButton_);

    connect(openTsButton_, &QPushButton::clicked, this, &XslTrassformerDlg::onButtonTsOpenClicked);
    connect(openXslButton_, &QPushButton::clicked, this, &XslTrassformerDlg::onButtonXslOpenClicked);
    connect(transformButton_, &QPushButton::clicked, this, &XslTrassformerDlg::onButtonTransformClicked);
    
    this->setLayout(buttons);
}

XslTrassformerDlg::~XslTrassformerDlg()
{

}

void XslTrassformerDlg::onButtonTsOpenClicked()
{
    QString tsFilePath = QFileDialog::getOpenFileName(
        this, QString("Select files"),
        QString(),
        QString("Qt translation files (*.ts)"));

    if (tsFilePath.isEmpty()) {
        return;
    }

    QFile tsFile(tsFilePath);
    if (!tsFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(nullptr, "Error", QString("Unable to open translation file %1").arg(tsFilePath));
        return;
    }
    
    QTextStream tsFileStream(&tsFile);
    QString content;
    QString line = tsFileStream.readLine();
    while (!line.isNull()) {
        line = tsFileStream.readLine();
        if(line == QString("<!DOCTYPE TS>")) continue;
        content.append(line);
        content.append('\n');
    }
    
    QStringList tempPaths = QStandardPaths::standardLocations(QStandardPaths::TempLocation);
    if (tempPaths.isEmpty()) {
        return;
    }

    tempXmlPath_ = QDir(tempPaths[0]).filePath(tempXmlFile);
    QFile file(tempXmlPath_);
    file.open(QIODevice::WriteOnly);
    QTextStream outstream(&file);
    outstream << content;
    file.close();
}

void XslTrassformerDlg::onButtonXslOpenClicked()
{
    xslPath_ = QFileDialog::getOpenFileName(
        this, QString("Select files"),
        QString(),
        QString("XSL transformation files (*.xsl; *.xslt)"));
}

void XslTrassformerDlg::onButtonTransformClicked()
{
    QString htmlFilePath = QFileDialog::getSaveFileName(nullptr, "Save file",
        QString(),
        QString("All files (*.*)"));
    performTransformation(htmlFilePath);
}

void XslTrassformerDlg::performTransformation(const QString& outputFile)
{
    QFile file(outputFile);
    if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
        QMessageBox::warning(nullptr, "Error", QString("Unable to open file for writing %1").arg(outputFile));
        return;
    }

    QXmlQuery query(QXmlQuery::XSLT20);
    query.setFocus(QUrl(tempXmlPath_));
    query.setQuery(QUrl(xslPath_));
    bool writeSuccesful = query.evaluateTo(&file);
    
    if (!writeSuccesful) {
        QMessageBox::warning(nullptr, "Error", QString("Unable to evaluate file %1").arg(outputFile));
        return;
    }
}
