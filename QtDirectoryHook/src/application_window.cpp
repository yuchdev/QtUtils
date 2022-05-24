#include <qt_directory_hook/application_window.h>

#include <QDir>
#include <QDirIterator>
#include <QLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QComboBox>
#include <QFileDialog>
#include <QListWidget>
#include <QTableWidgetItem>
#include <QFileSystemWatcher>
#include <QListIterator>
#include <QStringList>

#if defined(Q_OS_WIN)
#include <Windows.h>
#else defined(Q_OS_LINUX) || defined(Q_OS_MAC)
#include <errno.h>
#endif

#include <cassert>

FilesystemHookApplicationWindow::FilesystemHookApplicationWindow(QWidget* parent/* = nullptr*/) : 
    QWidget(parent),
    copyChanged_(false),
    filesystemObjType_(),
    lookupPathButton_(),
    addPathButton_(),
    removePathButton_(),
    copyToDirButton_(),
    wildcardEdit_(),
    addPathEdit_(),
    copyToDirEdit_(),
    copyToDirChck_(),
    pathsList_(),
    eventsList_(),
    filesystemHook_(new QFileSystemWatcher(parent))
{
    QVBoxLayout* setHookLayout = new QVBoxLayout;

    // 1 row - select type to monitor
    QHBoxLayout* fileType = new QHBoxLayout;
    QLabel* fileTypeLbl = new QLabel("Filesystem object type: ");
    fileTypeLbl->setAlignment(Qt::AlignRight);
    filesystemObjType_ = new QComboBox;
    QStringList filesystemTypes;
    filesystemTypes << "File" << "Directory";
    filesystemObjType_->addItems(filesystemTypes);
    filesystemObjType_->setCurrentIndex(DirFSType);
    assert(filesystemObjType_->count() == FSTypeCount);

    fileType->addWidget(fileTypeLbl);
    fileType->addWidget(filesystemObjType_);
    

    // 2 row - add path
    QHBoxLayout* addPath = new QHBoxLayout;
    QLabel* addPathLbl = new QLabel("Add path: ");
    addPathEdit_ = new QLineEdit;
    lookupPathButton_ = new QPushButton("...");
    addPathButton_ = new QPushButton("+");
    removePathButton_ = new QPushButton("-");
    addPath->addWidget(addPathLbl);
    addPath->addWidget(addPathEdit_);
    addPath->addWidget(lookupPathButton_);
    addPath->addWidget(addPathButton_);
    addPath->addWidget(removePathButton_);
    connect(lookupPathButton_, &QPushButton::clicked, this, &FilesystemHookApplicationWindow::selectHookPath);
    connect(addPathButton_, &QPushButton::clicked, this, &FilesystemHookApplicationWindow::addHookPath);
    connect(removePathButton_, &QPushButton::clicked, this, &FilesystemHookApplicationWindow::removeHookPath);

    // 3 row - paths list
    pathsList_ = new QListWidget;

    // 4 row - copy to dir
    QHBoxLayout* copyToDir = new QHBoxLayout;
    copyToDirChck_ = new QCheckBox("Copy changed files to dir");
    copyToDirEdit_ = new QLineEdit;
    copyToDirButton_ = new QPushButton("...");
    copyToDir->addWidget(copyToDirChck_);
    copyToDir->addWidget(copyToDirEdit_);
    copyToDir->addWidget(copyToDirButton_);
    copyToDirChck_->setChecked(false);
    connect(copyToDirButton_, &QPushButton::clicked, this, &FilesystemHookApplicationWindow::selectDestinationPath);
    connect(copyToDirChck_, &QCheckBox::clicked, this, &FilesystemHookApplicationWindow::selectCopyChangedFiles);
    selectCopyChangedFiles();
    
    // 4 row - file wildcards
    QHBoxLayout* wildcards = new QHBoxLayout;
    QLabel* wildcardsLbl = new QLabel("Wildcards (separated with ';'):");
    wildcardEdit_ = new QLineEdit;
    wildcardEdit_->setText("*.sh");
    wildcards->addWidget(wildcardsLbl);
    wildcards->addWidget(wildcardEdit_);
    connect(wildcardEdit_, &QLineEdit::textEdited, this, &FilesystemHookApplicationWindow::composeWildcardList);
    composeWildcardList();

    // 6 row - events list
    eventsList_ = new QListWidget;

    // Hook
    connect(filesystemHook_.data(), &QFileSystemWatcher::fileChanged, this, &FilesystemHookApplicationWindow::fileChanged);
    connect(filesystemHook_.data(), &QFileSystemWatcher::directoryChanged, this, &FilesystemHookApplicationWindow::directoryChanged);

    // Setting layout
    setHookLayout->addLayout(fileType);
    setHookLayout->addLayout(addPath);
    setHookLayout->addWidget(pathsList_);
    setHookLayout->addLayout(copyToDir);
    setHookLayout->addLayout(wildcards);
    setHookLayout->addWidget(eventsList_);
    this->setLayout(setHookLayout);
}

FilesystemHookApplicationWindow::~FilesystemHookApplicationWindow()
{
}

QString FilesystemHookApplicationWindow::getDirectoryName(int fileType)
{
    QFileDialog openDir;
    openDir.setDirectory(QDir::homePath());
    if(fileType == FileFSType) {
        openDir.setFileMode(QFileDialog::ExistingFile);
    }
    else if (fileType == DirFSType) {
        openDir.setOptions(QFileDialog::ShowDirsOnly);
        openDir.setFileMode(QFileDialog::Directory);
    }
    
    QStringList fileName;
    if (openDir.exec()) {
        fileName = openDir.selectedFiles();
        assert(fileName.size() == 1);
    }
    return (fileName.size() == 1) ? fileName[0] : QString();
}

// static
int FilesystemHookApplicationWindow::getLastError()
{
#if defined(Q_OS_WIN)
    return ::GetLastError();
#else defined(Q_OS_LINUX) || defined(Q_OS_MAC)
    return errno;
#endif
}

void FilesystemHookApplicationWindow::addPathUnderWatch(const QString& pathToAdd, const QString& fileType)
{
    if(!filesystemHook_->addPath(pathToAdd)){
        eventsList_->addItem(QString("Unable to put the %1 %2 under watch").arg(fileType).arg(pathToAdd));
        eventsList_->addItem(QString("System Error Code = %1").arg(this->getLastError()));
    }
    else{
        eventsList_->addItem(QString("The %1 %2 is put under watch").arg(fileType).arg(pathToAdd));
    }
}

void FilesystemHookApplicationWindow::removePathFromWatch(const QString& pathToRemove)
{
    if(filesystemHook_->removePath(pathToRemove)){
        eventsList_->addItem(QString("Unable to remove the file %1 from watch").arg(pathToRemove));
        eventsList_->addItem(QString("System Error Code = %1").arg(this->getLastError()));
    }
    else{
        eventsList_->addItem(QString("The file %1 is removed from watch").arg(pathToRemove));
    }
}

QStringList& FilesystemHookApplicationWindow::fileWildcards()
{
    static QStringList wildcards;
    return wildcards;
}

void FilesystemHookApplicationWindow::selectHookPath()
{
    addPathEdit_->setText(getDirectoryName(filesystemObjType_->currentIndex()));
}

void FilesystemHookApplicationWindow::addHookPath()
{
    QString hookPath = addPathEdit_->text();
    QFileInfo hookFile(hookPath);
    
    if(hookFile.isFile()){
        pathsList_->addItem(hookPath);
        addPathUnderWatch(hookPath, QString("file"));
    }
    else if(hookFile.isDir()){
        pathsList_->addItem(hookPath);
        addPathUnderWatch(hookPath, QString("directory"));
    }
    else{
        eventsList_->addItem(QString("The file %1 does not exist").arg(hookPath));
    }
}

void FilesystemHookApplicationWindow::removeHookPath()
{
    QList<QListWidgetItem*> eraseUs = pathsList_->selectedItems();
    QListIterator<QListWidgetItem*> iterErasedItems(eraseUs);
    while(iterErasedItems.hasNext()) {
        removePathFromWatch(iterErasedItems.next()->text());
        pathsList_->removeItemWidget(iterErasedItems.next());
    }
}

void FilesystemHookApplicationWindow::selectDestinationPath()
{
    copyToDirEdit_->setText(getDirectoryName(DirFSType));
}

void FilesystemHookApplicationWindow::selectCopyChangedFiles()
{
    copyChanged_ = copyToDirChck_->isChecked();
    copyToDirEdit_->setEnabled(copyChanged_);
    copyToDirButton_->setEnabled(copyChanged_);
}

void FilesystemHookApplicationWindow::fileChanged(const QString& path)
{
    if(copyChanged_){
        QFileInfo fileInfo(path);
        QString destinationFilePath = QDir::cleanPath(QString("%1%2%3")
            .arg(copyToDirEdit_->text())
            .arg(QDir::separator())
            .arg(fileInfo.fileName()));
        QFile::copy(path, destinationFilePath);
    }
    eventsList_->addItem(QString("Changed file signal, path = %1 in the watched directory").arg(path));
}

void FilesystemHookApplicationWindow::directoryChanged(const QString& path)
{
    eventsList_->addItem(QString("Changed directory signal, path = %1 in the watched directory").arg(path));
    if(copyChanged_){
        QDirIterator it(path, fileWildcards(), QDir::Files);
        while (it.hasNext()){
            QFileInfo fileInfo(it.next());
            QString destinationFilePath = QDir::cleanPath(QString("%1%2%3")
                .arg(copyToDirEdit_->text())
                .arg(QDir::separator())
                .arg(fileInfo.fileName()));

            QFile::copy(fileInfo.filePath(), destinationFilePath);
            eventsList_->addItem(QString("Source path = %1; Destination path = %2").arg(fileInfo.filePath()).arg(destinationFilePath));
        }
    }
}

void FilesystemHookApplicationWindow::composeWildcardList()
{
    QStringList& wildcards = fileWildcards();
    wildcards = wildcardEdit_->text().split(QChar(';'));
}
