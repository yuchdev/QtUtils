#pragma once

#include <QDialog>
#include <QString>

class QPushButton;

/// @brief The TranslateDialog class
/// Converts TS-file into a valid XML and performs a custom XSLT transformation
class XslTrassformerDlg : public QDialog
{
    Q_OBJECT

public:
    
    /// @brief Create controls and handlers
    explicit XslTrassformerDlg(QWidget *parent = 0);

    /// @brief virtual
    virtual ~XslTrassformerDlg();

protected:

    /// @brief Open TS-file and convert into valid XML
    void onButtonTsOpenClicked();

    /// @brief Transform to HTML table
    void onButtonXslOpenClicked();

    /// @brief Transform to tab-delimited file to be open in excel
    void onButtonTransformClicked();

    /// @brief Perform XSLT transformation
    void performTransformation(const QString& outputFile);

private:

    Q_DISABLE_COPY(XslTrassformerDlg)

    /// Layout
    QPushButton* openTsButton_ = nullptr;
    QPushButton* openXslButton_ = nullptr;
    QPushButton* transformButton_ = nullptr;

    /// Temporary XML file path (empty if not created yet)
    QString tempXmlPath_;

    /// XSL transformation file path (empty if not opened yet)
    QString xslPath_;

    /// Temporary XML file name
    static const char* tempXmlFile;
};
