#include "eqwindow.h"
#include "ui_eqwindow.h"
#include <QFile>
#include <QDir>
#include <QStringList>
#include <QTextStream>
#include <QDebug>
#include <QSqlQueryModel>
#include <QMessageBox>
#include <QSqlQueryModel>

#define UNUSED(expr) do { (void)(expr); } while (0)

EqWindow::EqWindow(QWidget *parent) :
    DocGenWindow(parent), ui(new Ui::EqWindow)
{
    ui->setupUi(this);

    connect(ui->btnOk, SIGNAL(clicked(bool)),
            this, SLOT(ok()));
}

QString EqWindow::name()
{
    return "equipment-chooser-plugin";
}

QString EqWindow::buttonName()
{
    return tr("Оборудование");
}

QString EqWindow::description()
{
    return tr("Выбор оборудования ИЛ.");
}

void EqWindow::execute()
{

    eqname = "";

    QSqlQueryModel * model = new QSqlQueryModel(this);
    model->setQuery("select inv, eqid from equipment where status = 'Y' order by 2 ");
    model->setHeaderData(0, Qt::Horizontal, tr("Инв. номер"));
    model->setHeaderData(1, Qt::Horizontal, tr("Наименование оборудования"));

    ui->tbl->setModel(model);
    ui->tbl->horizontalHeader()->setStretchLastSection(true);
    ui->tbl->horizontalHeader()->setHighlightSections(false);

    exec();

    if(eqname.isEmpty())
        return;

    LyxGen doc;
    QString lyx =
    "\\begin_layout Standard\n"
    "Работа ведется на приборе "
    "\\begin_inset Text\n"
    "\\readonly\n"
    "\\begin_layout Default\n";

    lyx += eqname + "\n";

    lyx +=
    "\\end_layout\n"
    "\\end_inset\n"
    "\\end_layout\n";

    doc.addLyxCode(lyx);
    scilyx()->insertDocumentFragment(doc.generate());
}

EqWindow::~EqWindow()
{
    delete ui;
}

void EqWindow::ok()
{
    QModelIndex ind = ui->tbl->currentIndex();
    if(ind.isValid())
    {
        eqname = ui->tbl->model()->index(ind.row(), 1).data().toString();
        accept();
    }
    else
    {
        QMessageBox::information(this, tr("Ошибка"),
                                 tr("Пожалуйста, выберите прибор из списка."));
        return;
    }
}
