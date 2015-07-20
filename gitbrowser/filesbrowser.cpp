#include "filesbrowser.h"
#include "ui_filesbrowser.h"

#include <QMessageBox>
#include <QLabel>
#include <QMovie>
#include <QDebug>
#include <QListWidgetItem>
#include <QMenu>
#include <QFont>
#include <QDebug>
#include <QUuid>
#include <QTextStream>
#include <QDate>
#include <QToolBar>

#include "versionbrowserdialog.h"
#include "syncdialog.h"
#include "texteditor.h"

GitBrowser::GitBrowser(QString localRepoFolder, QWidget *parent) :
    QWidget(parent)
{
    Q_INIT_RESOURCE(gitbrowser);

    ui = new Ui::GitBrowser;
    ui->setupUi(this);

    repoParams = new RepoParams();
    model = new QFileSystemModel;
    model->setNameFilters(QStringList()<<"*.lyx");
    model->setNameFilterDisables(false);
    repoFolder = localRepoFolder;
    model->setRootPath(repoFolder);
    model->setReadOnly(false);

    ui->listView->setModel(model);
    ui->listView->setEditTriggers(QAbstractItemView::SelectedClicked|QAbstractItemView::EditKeyPressed);
    connect(model, SIGNAL(fileRenamed(QString,QString,QString)), this, SLOT(onFileRenamed(QString,QString,QString)));

    go("/");
    syncWidget = new SyncDialog(this);
    repoSettingsDialog = new RepoSettings(this);
    connect(repoSettingsDialog, SIGNAL(changed()), this, SLOT(updateRepoParams()));
    git = new GitManager(repoFolder);
    actUpdate = new QAction("Обновить", this);
    connect(actUpdate, SIGNAL(triggered()), this, SLOT(callUpdate()));
    actOpen = new QAction("Открыть", this);
    connect(actOpen, SIGNAL(triggered()), this, SLOT(callOpen()));
    actVersions = new QAction("Версии...", this);
    connect(actVersions, SIGNAL(triggered()), this, SLOT(callVersions()));
    actRename = new QAction("Переименовать", this);
    connect(actRename, SIGNAL(triggered()), this, SLOT(callRename()));
    actRemove = new QAction("Удалить", this);
    connect(actRemove, SIGNAL(triggered()), this, SLOT(callRemove()));
    actNewDir = new QAction("Новая папка", this);
    connect(actNewDir, SIGNAL(triggered()), this, SLOT(callNewDir()));
    actNewDocument = new QAction("Новый документ", this);
    connect(actNewDocument, SIGNAL(triggered()), this, SLOT(callNewDocument()));
    actCopy = new QAction("Копировать", this);
    connect(actCopy, SIGNAL(triggered()), this, SLOT(callCopy()));
    actCut = new QAction("Вырезать", this);
    connect(actCut, SIGNAL(triggered()), this, SLOT(callCut()));
    actPaste = new QAction("Вставить", this);
    connect(actPaste, SIGNAL(triggered()), this, SLOT(callPaste()));
    actRepoManager = new QAction("Репозитории", this);
    connect(actRepoManager, SIGNAL(triggered(bool)), SLOT(callRepoSettings()));
    ui->listView->setContextMenuPolicy(Qt::CustomContextMenu);
    QList<QAction* > toolBarActions;
    toolBarActions.append(actRepoManager);
    ui->toolBar->addActions(toolBarActions);
}

void GitBrowser::readRepoConfigOrAskUser()
{
    if (!readRepoConfig())
        callRepoSettings();
    else
        updateRepoParams();
}

void GitBrowser::updateRepoParams()
{
    git->setRepoParams(repoParams);
    QMetaObject::invokeMethod(this, "init", Qt::QueuedConnection);
}

bool GitBrowser::readRepoConfig()
{
    return repoParams->readFromConfig();
}

GitBrowser::~GitBrowser()
{
    delete ui;
    delete syncWidget;
    delete repoSettingsDialog;
    delete repoParams;
    git->deleteLater();
}

//QString FilesBrowser::currentPath()
//{
//    return currentPathWithSlash();
//}

QString GitBrowser::currentAbsolutePath()
{
    QDir dir(currentPathWithSlash());
    return dir.absolutePath();
}

bool GitBrowser::openEditor(QString fileName)
{
    QFile file(fileName);
    if (!file.exists())
        return false;
    emit openEditorTriggered(fileName);
    return true;
}

//Выполняет переход к каталогу
void GitBrowser::go(QString path)
{
    QDir dir(repoFolder);
    QString path_ =  path == "/" ? "/" : path.mid(1);
    if (!(dir.exists(path_) || path == "/"))
    {
        QMessageBox::information(this, "Указанный путь не найден", "Путь '"+currentPath+"' не найден!");
        return;
    }
    currentPath = path;
    ui->lineEdit->setText(currentPath);
    path_ = repoFolder+currentPath;
    ui->listView->setRootIndex(model->setRootPath(path_));
}

//Выполняет переход на уровень выше
void GitBrowser::up()
{
    QString newPath = currentPath;
    if (currentPath!="/")
    {
        int i = currentPath.length()-1;
        while (i>1 && currentPath[i]!='/')
            i--;
        newPath = currentPath.mid(0, i);
    };
    go(newPath);
}

void GitBrowser::on_goBtn_clicked()
{
    go(ui->lineEdit->text());
}

void GitBrowser::on_upBtn_clicked()
{
    up();
}

void GitBrowser::on_updateBtn_clicked()
{
    update();
}

//Создание нового документа
void GitBrowser::on_newDocumentBtn_clicked()
{
    QString path = currentPathWithSlash();
    path = repoFolder + path;
}

void GitBrowser::on_listView_doubleClicked(const QModelIndex &index)
{
    if (model->fileInfo(index).isDir())
    {
        QString path = currentPathWithSlash();
        path += model->fileInfo(index).fileName();
        go(path);
    }
    if (model->fileInfo(index).isFile())
    {
        actOpen->trigger();
    }
}

void GitBrowser::on_lineEdit_returnPressed()
{
    go(ui->lineEdit->text());
}

//Меню
void GitBrowser::on_listView_customContextMenuRequested(const QPoint &pos)
{
    const QModelIndex &index = ui->listView->indexAt(pos);
    if (index.isValid()) {
        on_listView_showItemContextMenu(index, ui->listView->viewport()->mapToGlobal(pos));
    } else
    {
        on_listView_showSpaceContextMenu(ui->listView->viewport()->mapToGlobal(pos));
    }
}

//Вызов контекстного меню элемента
void GitBrowser::on_listView_showItemContextMenu(const QModelIndex &index, const QPoint& globalPos)
{
    QMenu menu;
    if (model->fileInfo(index).isDir())
    {
        menu.addAction(actRename);
        menu.addAction(actRemove);
    }
    if (model->fileInfo(index).isFile())
    {
        menu.addAction(actOpen);
        menu.addAction(actVersions);
        menu.addAction(actRename);
        menu.addAction(actRemove);
    }
    menu.exec(globalPos);
}

//Вызов контекстного меню при клике на свободное пространство
void GitBrowser::on_listView_showSpaceContextMenu(const QPoint& globalPos)
{
    QMenu menu;
    menu.addAction(actNewDir);
    menu.exec(globalPos);
}

void GitBrowser::resizeEvent(QResizeEvent *event)
{
    UNUSED(event);
    syncWidget->setGeometry(geometry());
}

QString GitBrowser::currentPathWithSlash()
{
    return currentPath == "/" ? currentPath : currentPath + "/";
}

bool GitBrowser::registerAction(QString name, QAction *action)
{
    if (actions.contains(name))
        return false;
    actions[name] = action;
    QList<QAction* > list;
    list.append(action);
    ui->toolBar->addActions(list);
    return true;
}

bool GitBrowser::unregisterAction(QString name)
{
    return actions.remove(name);
}

//Этот слот вызывается только после успешного переименования файла или папки
void GitBrowser::onFileRenamed(const QString &path, const QString &oldName, const QString &newName)
{
    UNUSED(path);
    UNUSED(oldName);
    UNUSED(newName);
    commitChanges();
}

void GitBrowser::init()
{
    init_remote = repoParams->login+"@"+repoParams->url;
    QDir dir(repoFolder);
    if (!dir.exists())
    {
        QDir root("/");
        if (!root.mkpath(repoFolder))
        {
            initFailure("Ошибка инициализации репозитория", "Невозможно создать каталог '"+repoFolder+"'. Проверьте наличие соответствующих прав у пользователя");
            return;
        }
    }
    QFile gitFile(repoFolder+"/.git");
    if (!gitFile.exists())
    {
        syncWidget->start("Инициализация локального репозитория...", true);
        syncWidget->setProgress(50);
        connect(&fileManager, SIGNAL(progressChanged(int)), this, SLOT(initRemoveProgressChanged(int)));
        connect(&fileManager, SIGNAL(removeSuccess()), this, SLOT(initRemoveSuccess()));
        connect(&fileManager, SIGNAL(removeError(QString,QString)), this, SLOT(initRemoveFailure(QString,QString)));
        fileManager.remove(repoFolder);
    } else
    {
        update();
    }
}

void GitBrowser::initRemoveProgressChanged(int value)
{
    syncWidget->setProgress((int)((float)value/2.0));
}

void GitBrowser::initRemoveSuccess()
{
    syncWidget->setProgress(50);
    connect(git, SIGNAL(cloneSuccess()), this, SLOT(initSuccess()));
    git->clone(init_remote);
}

void GitBrowser::initRemoveFailure(QString error, QString details)
{
    UNUSED(error);
    initFailure("Ошибка на этапе инициализации", details);
}

void GitBrowser::initCloneProgressChanged(int value)
{
    syncWidget->setProgress(50+(int)((float)value/2.0));
}

void GitBrowser::initSuccess()
{
    syncWidget->setProgress(100);
    syncWidget->stop();
    QString path_ = repoFolder+currentPath;
    ui->listView->setRootIndex(model->setRootPath(path_));
}

void GitBrowser::initFailure(QString error, QString details)
{
    UNUSED(error);
    UNUSED(details);
    //TODO: дописать обработку ошибки инициализации репозитория
}

void GitBrowser::repair(QString error, QString details)
{
    UNUSED(error);
    UNUSED(details);
    //TODO: дописать
}

void GitBrowser::repairSuccess()
{
    //TODO: дописать
}

void GitBrowser::repairFailure(QString error, QString details)
{
    UNUSED(error);
    UNUSED(details);
    //TODO: дописать
}

//Update = git pull
void GitBrowser::update()
{
    syncWidget->start("Синхронизация...");
    connect(git, SIGNAL(pullSuccess()), this, SLOT(updateSuccess()));
    git->pull();
}
//Обработчик успешного update
void GitBrowser::updateSuccess()
{
    disconnect(git, SIGNAL(pullSuccess()), this, SLOT(updateSuccess()));
    syncWidget->stop();
}
//Обработчик ошибки при update
void GitBrowser::updateFailure(QString error, QString details)
{
    UNUSED(details);
    disconnect(git, SIGNAL(pullSuccess()), this, SLOT(updateSuccess()));
    syncWidget->showError(error);
}

void GitBrowser::commitChanges()
{
    //Сигналы и слоты соединяются в граф асинхронного выполнения процесса commitChanges
    //После выхода из графа предусмотрено их разъединение (disconnect)
    //Выхода из графа всего два: commitChangesSuccess и commitChangesFailure
    connect(git, SIGNAL(addSuccess()), git, SLOT(commit()));
    connect(git, SIGNAL(addFailure(QString,QString)), this, SLOT(commitChangesFailure(QString,QString)));
    connect(git, SIGNAL(commitSuccess()), git, SLOT(pull()));
    connect(git, SIGNAL(commitFailure(QString,QString)), this, SLOT(commitChangesFailure(QString,QString)));
    connect(git, SIGNAL(pullSuccess()), git, SLOT(push()));
    connect(git, SIGNAL(pullFailure(QString,QString)), this, SLOT(commitChangesFailure(QString,QString)));
    connect(git, SIGNAL(pushSuccess()), this, SLOT(commitChangesSuccess()));
    connect(git, SIGNAL(pushFailure(QString,QString)), this, SLOT(commitChangesFailure(QString,QString)));
    syncWidget->start("Синхронизация...");
    git->add();
}

void GitBrowser::commitChangesSuccess()
{
    //Необходимо скрыть виджет синхронизации
    syncWidget->stop();
    //Разъединяем граф commitChanges
    commitChangesDisconnectSignals();
}

void GitBrowser::commitChangesFailure(QString error, QString details)
{
    UNUSED(details);
    //Необходимо скрыть виджет синхронизации
    syncWidget->showError(error);
    //Разъединяем граф commitChanges
    commitChangesDisconnectSignals();
}

void GitBrowser::commitChangesDisconnectSignals()
{
    //Дисконнектим сигналы
    disconnect(git, SIGNAL(addSuccess()), git, SLOT(commit()));
    disconnect(git, SIGNAL(addFailure(QString,QString)), this, SLOT(commitChangesFailure(QString,QString)));
    disconnect(git, SIGNAL(commitSuccess()), git, SLOT(pull()));
    disconnect(git, SIGNAL(commitFailure(QString,QString)), this, SLOT(commitChangesFailure(QString,QString)));
    disconnect(git, SIGNAL(pullSuccess()), git, SLOT(push()));
    disconnect(git, SIGNAL(pullFailure(QString,QString)), this, SLOT(commitChangesFailure(QString,QString)));
    disconnect(git, SIGNAL(pushSuccess()), this, SLOT(commitChangesSuccess()));
    disconnect(git, SIGNAL(pushFailure(QString,QString)), this, SLOT(commitChangesFailure(QString,QString)));
}

void GitBrowser::configureStarted()
{
    syncWidget->start("Конфигурирование SciLyx...");
}

void GitBrowser::configureFinished()
{
    syncWidget->stop();
}

void GitBrowser::callUpdate()
{
    update();
}

void GitBrowser::callOpen()
{
    const QModelIndex &index = ui->listView->currentIndex();
    if (model->fileInfo(index).isFile())
    {
        QString fileName = model->fileInfo(index).absoluteFilePath();
        emit openEditorTriggered(fileName);
    }
}

void GitBrowser::callVersions()
{
    const QModelIndex &index = ui->listView->currentIndex();
    if (model->fileInfo(index).isDir())
    {

    }
    if (model->fileInfo(index).isFile())
    {
        QFile file(model->fileInfo(index).absoluteFilePath());
        if (file.exists())
        {
            QString filePath = currentPathWithSlash()+model->fileInfo(index).fileName();
            filePath = filePath.mid(1);
            VersionBrowserDialog *f = new VersionBrowserDialog(this, filePath, git);
            connect(f, SIGNAL(openedOldFile(QString,QString,QByteArray)), this, SIGNAL(openOldVersionInEditorTriggered(QString,QString,QByteArray)));
            f->show();
        }
    }
}

void GitBrowser::callRename()
{
    const QModelIndex &index = ui->listView->currentIndex();
    if (!index.isValid())
        return;
    ui->listView->setCurrentIndex(index);
    ui->listView->edit(index);
}

void GitBrowser::callRemove()
{
    const QModelIndex &index = ui->listView->currentIndex();
    QString fileName = model->fileInfo(index).fileName();
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Подтверждение", "Вы действительно хотите удалить файл '"+fileName+"'?", QMessageBox::Yes|QMessageBox::No);
    QString name = "файла";
    if (model->fileInfo(index).isDir())
    {
        name = "каталога";
    }
    if (reply == QMessageBox::Yes)
    {
        if (!model->remove(index))
        {
            QMessageBox::information(this, "Ошибка", "Ошибка при удалении "+name+" '"+fileName+"'. Возможно, у Вас недостаточно прав");
        }else
        {
            commitChanges();
        }
    }
}

//слот, вызываемый на действие "Новый каталог"
void GitBrowser::callNewDir()
{
    QString newDir = "Новая папка";
    int i=0;
    while (true)
    {
        QFile file(model->rootPath()+"/"+newDir);
        if (file.exists())
        {
            i++;
            newDir = "Новая папка " + i;
        } else {
            break;
        }
    }
    const QModelIndex &index = model->mkdir(ui->listView->rootIndex(), newDir);
    QFile gitignore(model->fileInfo(index).absoluteFilePath()+"/"+".gitignore");
    gitignore.open(QFile::ReadWrite);
    gitignore.write(QString("!.gitignore").toUtf8());
    gitignore.close();
    ui->listView->edit(index);
}

//Слот, вызываемый на действие "Новый документ"
void GitBrowser::callNewDocument()
{

}

//Слот, вызываемый на действие "Копировать"
void GitBrowser::callCopy()
{

}

//Слот, вызываемый на действие "Вырезать"
void GitBrowser::callCut()
{

}

//Слот, вызываемый на действие "Вставить"
void GitBrowser::callPaste()
{

}

//Слот, вызываемый на действие "Открыть настройки репозитория"
void GitBrowser::callRepoSettings()
{
    repoSettingsDialog->startEdit(repoParams);
}