#ifndef FORM4_H
#define FORM4_H

#include <QWidget>
#include <QFileSystemModel>
#include <QProcess>
#include <QAction>
#include <QTextStream>
#include <QToolBar>

#include "gitmanager.h"
#include "reposettings.h"
#include "../qfs/qfilemanager.h"

#define UNUSED(expr) do { (void)(expr); } while (0)

class SyncDialog;

namespace Ui {
class GitBrowser;
}

class GitBrowser : public QWidget
{
    Q_OBJECT

public:
    GitBrowser(QString localRepoFolder, QWidget *parent = 0);
    void readRepoConfigOrAskUser();
    bool readRepoConfig();
    ~GitBrowser();
    //QString currentPath();
    QString currentAbsolutePath();
    bool openEditor(QString fileName);
    bool registerAction(QString name, QAction *action);
    bool unregisterAction(QString name);

private:
    Ui::GitBrowser *ui;
    QString repoFolder;//Каталог, в котором хранится локальная копия репозитория
    QString currentPath;//Текущий путь (отображается в lineEdit)
    QFileSystemModel *model;//Модель файловой системы
    SyncDialog *syncWidget;//Виджет, затемняющий окно проводника во время синхронизации и при ошибках синхронизации
    RepoSettings *repoSettingsDialog;//Диалог настройки репозитория
    GitManager *git;//GitManager осуществляет взаимодействие с git
    //Действия
    QAction *actUpdate;
    QAction *actOpen;
    QAction *actVersions;
    QAction *actRename;
    QAction *actRemove;
    QAction *actNewDir;
    QAction *actNewDocument;
    QAction *actCopy;
    QAction *actCut;
    QAction *actPaste;
    QAction *actRepoManager;
    void go(QString path);//Выполняет переход к каталогу
    void up();//Выполняет переход на уровень выше
    void on_listView_showItemContextMenu(const QModelIndex &index, const QPoint &globalPos);//Контекстное меню файла или папки
    void on_listView_showSpaceContextMenu(const QPoint &globalPos);//Контекстное меню пустого пространства
    void resizeEvent (QResizeEvent *event);
    QFileManager fileManager;
    QString init_remote;
    QString currentPathWithSlash();
    QHash<QString, QAction* > actions;
    RepoParams *repoParams;

private slots:
    //Слоты, вызываемые напрямую из GUI
    void on_goBtn_clicked();//При нажатии на кнопку "Перейти"
    void on_upBtn_clicked();//При нажатии на кнопку "Вверх"
    void on_updateBtn_clicked();//При нажатии на кнопку "Обновить"
    void on_newDocumentBtn_clicked();
    void on_listView_doubleClicked(const QModelIndex &index);//Двойной клик на папке или каталоге
    void on_lineEdit_returnPressed();//Нажатие Enter, когда фокус находится на lineEdit
    void on_listView_customContextMenuRequested(const QPoint &pos);//Вызов меню из listView
    void onFileRenamed(const QString & path, const QString & oldName, const QString & newName);//Вызывется после Переименования файла или каталога в listView

    //Асинхронный процесс инициализации
    void init();
    void initRemoveProgressChanged(int value);
    void initRemoveSuccess();
    void initRemoveFailure(QString error, QString details);
    void initCloneProgressChanged(int value);
    void initSuccess();
    void initFailure(QString error, QString details);

    void repair(QString error, QString details);
    void repairSuccess();
    void repairFailure(QString error, QString details);

    //Асинхронный процесс обновления (синхронизации)
    void update();//Запуск процесса
    void updateSuccess();//Обработка успешного завершения
    void updateFailure(QString error, QString details);//Обработка завершения с ошибками

    //Асинхронный процесс отправки изменений на сервер
    void commitChangesSuccess();//В случае успешного завершения
    void commitChangesFailure(QString error, QString details);//В случае каких-либо ошибок
    void commitChangesDisconnectSignals();//Отсоединение сигналов и слотов

    //Обработка процесса конфигурирования Lyx
    void configureStarted();
    void configureFinished();

    //Слоты обработки действий (actions)
    void callUpdate();
    void callOpen();
    void callVersions();
    void callRename();
    void callRemove();
    void callNewDir();
    void callNewDocument();
    void callCopy();
    void callCut();
    void callPaste();
    void callRepoSettings();

    void updateRepoParams();

signals:
    void openEditorTriggered(QString fileName);
    void openOldVersionInEditorTriggered(QString fileName, QString commit, QByteArray data);

public slots:
    void commitChanges();//Запуск процесса отправки изменений на сервер
};

#endif // FORM4_H