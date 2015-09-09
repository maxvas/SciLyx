#include "eqwindow.h"
#include "ui_eqwindow.h"
#include <QFile>
#include <QDir>
#include <QStringList>
#include <QTextStream>
#include <QDebug>

#define UNUSED(expr) do { (void)(expr); } while (0)

EqWindow::EqWindow(QWidget *parent) :
    DocGenWindow(parent), ui(new Ui::EqWindow)
{
    ui->setupUi(this);
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
    return tr("Плагин для выбора оборудования.");
}

void EqWindow::execute()
{
    LyxGen doc;
    QString lyx =
    "\\begin_layout Standard\n"
    "\\readonly\n"
    "Varian\n"
    "\\end_layout\n";

    doc.addLyxCode(lyx);
    scilyx()->insertDocumentFragment(doc.generate());
}

EqWindow::~EqWindow()
{
    delete ui;
}
