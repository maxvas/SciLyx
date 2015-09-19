#include "matwindow.h"
#include "ui_matwindow.h"
#include <QFile>
#include <QDir>
#include <QStringList>
#include <QTextStream>
#include <QDebug>
#include <QSqlQueryModel>
#include <QMessageBox>
#include <QSqlQueryModel>

#define UNUSED(expr) do { (void)(expr); } while (0)

MatWindow::MatWindow(QWidget *parent) :
    DocGenWindow(parent), ui(new Ui::MatWindow)
{
    ui->setupUi(this);

    connect(ui->btnOk, SIGNAL(clicked(bool)),
            this, SLOT(ok()));
}

QString MatWindow::name()
{
    return "material-chooser-plugin";
}

QString MatWindow::buttonName()
{
    return tr("Реагенты");
}

QString MatWindow::description()
{
    return tr("Выбор оборудования ИЛ.");
}

void MatWindow::execute()
{

    setWindowTitle(tr("Выбор реагентов"));

    material = "";

    QSqlQueryModel * model = new QSqlQueryModel(this);
    model->setQuery("select r.n, m.material, r.expdate, r.lot, r.comments "
                    "from reags2 r inner join TRANSRES2 t on t.N = r.n "
                    "inner join MATERIALS m on m.id = r.MATID order by 1 ");

    model->setHeaderData(0, Qt::Horizontal, tr("Номер"));
    model->setHeaderData(1, Qt::Horizontal, tr("Наименование регаента"));
    model->setHeaderData(2, Qt::Horizontal, tr("Срок годности"));
    model->setHeaderData(3, Qt::Horizontal, tr("Лот"));
    model->setHeaderData(4, Qt::Horizontal, tr("Комментарий"));

    ui->tbl->setModel(model);
    ui->tbl->horizontalHeader()->setStretchLastSection(true);
    ui->tbl->horizontalHeader()->setHighlightSections(false);

    show();
    ui->tbl->resizeRowsToContents();

    exec();

    if(material.isEmpty())
        return;

    LyxGen doc;
    QString lyx =
    "\\begin_layout Standard\n"
    "\\begin_inset Text\n"
    "\\readonly\n"
    "\\begin_layout Standard\n\\noindent\n";

    lyx += material + "\n";

    lyx +=
    "\\end_layout\n"
    "\\end_inset\n"
    "\\end_layout\n";

    doc.addLyxCode(lyx);
    scilyx()->insertDocumentFragment(doc.generate());
}

MatWindow::~MatWindow()
{
    delete ui;
}

void MatWindow::ok()
{
    QModelIndex ind = ui->tbl->currentIndex();
    if(ind.isValid())
    {
        material = ui->tbl->model()->index(ind.row(), 1).data().toString().trimmed();

        QString lot =  ui->tbl->model()->index(ind.row(), 3).data().toString().trimmed();
        QString comments =  ui->tbl->model()->index(ind.row(), 4).data().toString().trimmed();

        if(!comments.isEmpty())
        {
            material += " (" + comments;
            if(!lot.isEmpty())
                material += ", лот: " + lot + ")";
        }
        else
        {
            if(!lot.isEmpty())
                material += " (лот: " + lot + ")";
        }

        accept();
    }
    else
    {
        QMessageBox::information(this, tr("Ошибка"),
                                 tr("Пожалуйста, выберите реагент из списка."));
        return;
    }
}
