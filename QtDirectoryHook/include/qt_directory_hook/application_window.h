#pragma once
#include <QWidget>
#include <QScopedPointer>

class QString;
class QPushButton;
class QLineEdit;
class QCheckBox;
class QListWidget;
class QFileSystemWatcher;
class QComboBox;

/// @brief Hook to directory or file
class FilesystemHookApplicationWindow : public QWidget
{
    Q_OBJECT

    enum{
        FileFSType,
        DirFSType,
        FSTypeCount
    };
public:

    /// @brief Create main window
    FilesystemHookApplicationWindow(QWidget* parent = nullptr);

    /// @brief default
    virtual ~FilesystemHookApplicationWindow();

private:

    /// Cross-platform LastError code
    static int getLastError();

    /// Get path o directory from QFileDialog
    /// param fileType: filesystem object type, file or directory
    QString getDirectoryName(int fileType);

    /// Add file or directory under watch
    void addPathUnderWatch(const QString& pathToAdd, const QString& fileType);

    /// Remove file or directory from watch
    void removePathFromWatch(const QString& pathToRemove);

    /// Get wildcards list
    static QStringList& fileWildcards();

private slots:

    /// Signal accepted when new path is selected
    void selectHookPath();

    /// Signal accepted when new path is added
    void addHookPath();

    /// Signal accepted when new path is removed
    void removeHookPath();

    /// Signal accepted when selected directory where to copy changed file
    void selectDestinationPath();

    /// Signal accepted when selected checkbox to copy changed file
    void selectCopyChangedFiles();

    /// Signal accepted when file under watch is changed
    void fileChanged(const QString& path);

    /// Signal accepted when directory under watch is changed
    void directoryChanged(const QString& path);

    /// Make wildcards list
    void composeWildcardList();

private:

    bool copyChanged_;
    QComboBox* filesystemObjType_;
    QPushButton* lookupPathButton_;
    QPushButton* addPathButton_;
    QPushButton* removePathButton_;
    QPushButton* copyToDirButton_;
    QLineEdit* wildcardEdit_;
    QLineEdit* addPathEdit_;
    QLineEdit* copyToDirEdit_;
    QCheckBox* copyToDirChck_;
    QListWidget* pathsList_;
    QListWidget* eventsList_;
    QScopedPointer<QFileSystemWatcher> filesystemHook_;
};