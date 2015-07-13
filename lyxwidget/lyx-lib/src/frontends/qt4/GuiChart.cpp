/**
 * \file GuiChart.cpp
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Maxim Kuznetcov
 *
 * Full author contact details are available in file CREDITS.
 */

#include <config.h>

#include "GuiChart.h"
#include "frontends/alert.h"
#include "qt_helpers.h"
#include "Validator.h"

#include "Buffer.h"
#include "FuncRequest.h"
#include "LengthCombo.h"
#include "Length.h"
#include "LyXRC.h"

#include "graphics/epstools.h"
#include "graphics/GraphicsCache.h"
#include "graphics/GraphicsCacheItem.h"
#include "graphics/GraphicsImage.h"

#include "insets/InsetChart.h"
#include "insets/InsetChartParams.h"

#include "support/convert.h"
#include "support/debug.h"
#include "support/filetools.h"
#include "support/gettext.h"
#include "support/lstrings.h"
#include "support/os.h"
#include "support/Package.h"
#include "support/types.h"

#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QValidator>

#include <cmath>
#include <utility>
#include <QDebug>
#include <QStringList>
#include <QObject>
#include <DataSourceManager.h>
#include <QDesktopWidget>

using namespace std;
using namespace lyx::support;

namespace {

// These are the strings that are stored in the LyX file and which
// correspond to the LaTeX identifiers shown in the comments at the
// end of each line.
char const * const rorigin_lyx_strs[] = {
	// the LaTeX default is leftBaseline
	"",
	"leftTop",  "leftBottom", "leftBaseline", // lt lb lB
	"center", "centerTop", "centerBottom", "centerBaseline", // c ct cb cB
	"rightTop", "rightBottom", "rightBaseline" }; // rt rb rB

// These are the strings, corresponding to the above, that the GUI should
// use. Note that they can/should be translated.
char const * const rorigin_gui_strs[] = {
	N_("Default"),
	N_("Top left"), N_("Bottom left"), N_("Baseline left"),
	N_("Center"), N_("Top center"), N_("Bottom center"), N_("Baseline center"),
	N_("Top right"), N_("Bottom right"), N_("Baseline right") };

size_t const rorigin_size = sizeof(rorigin_lyx_strs) / sizeof(char *);

static string const autostr = N_("automatically");

} // namespace anon


namespace lyx {
namespace frontend {

//FIXME setAutoTextCB should really take an argument, as indicated, that
//determines what text is to be written for "auto". But making
//that work involves more extensive revisions than we now want
//to make, since "auto" also appears in paramsToDialog().
//The right way to do this, I think, would be to define a class
//checkedLengthSet (and a partnering labeledLengthSete) that encapsulated
//the checkbox, line edit, and length combo together, and then made e.g.
//lengthToWidgets, widgetsToLength, etc, all public methods of that class.
//Perhaps even the validator could be exposed through it.

GuiChart::GuiChart(GuiView & lv)
    : GuiDialog(lv, "chart", qt_("График")), guiSender(true)
{
	setupUi(this);
    dataTable->setColumnCount(2);
    dataTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    dataTable->setSelectionMode(QAbstractItemView::SingleSelection);
    dataTable->setHorizontalHeaderLabels(QStringList()<<"X"<<"Y");
    seriesList->setEditTriggers( QAbstractItemView::DoubleClicked );
    lineCB->addItem("Сплошная", QVariant("solid"));
    lineCB->addItem("Пунктирная", QVariant("dashed"));
    lineCB->addItem("Штрих-пунктирная", QVariant("dashdotted"));
    markerCB->addItem("Нет маркера", QVariant("none"));
    markerCB->addItem("Круг", QVariant("*"));
    markerCB->addItem("Квадрат", QVariant("square*"));
    markerCB->addItem("Треугольник", QVariant("triangle*"));
    markerCB->addItem("Ромб", QVariant("diamond*"));
    markerCB->addItem("Крестик", QVariant("x"));
    markerCB->addItem("Круг полый", QVariant("o"));
    markerCB->addItem("Квадрат полый", QVariant("square"));
    markerCB->addItem("Треугольник полый", QVariant("triangle"));
    markerCB->addItem("Ромб полый", QVariant("diamond"));
    markerCB->addItem("Плюс", QVariant("+"));
    markerCB->addItem("Минус", QVariant("-"));
    markerCB->addItem("Вертикальная черта", QVariant("|"));
    markerCB->addItem("Звездочка", QVariant("star"));
    markerCB->addItem("Звездочка 2", QVariant("asterisk"));
    markerCB->addItem("Звездочка 3", QVariant("10-pointed star"));
//    markerCB->addItem("Плюс в круге", QVariant("oplus*"));
//    markerCB->addItem("Крестик в круге", QVariant("otimes*"));
    markerCB->addItem("Плюс в полом круге", QVariant("oplus"));
    markerCB->addItem("Крестик в полом круге", QVariant("otimes"));
    colorCB->addItem("Черный", QVariant("black"));
    colorCB->addItem("Синий", QVariant("blue"));
    colorCB->addItem("Красный", QVariant("red"));
    colorCB->addItem("Зеленый", QVariant("green"));
    colorCB->addItem("Желтый", QVariant("yellow"));
    colorCB->addItem("Оранжевый", QVariant("orange"));
    colorCB->addItem("Голубой", QVariant("cyan"));
    colorCB->addItem("Фиолетовый", QVariant("violet"));
    colorCB->addItem("Розовый", QVariant("pink"));
    colorCB->addItem("Серый", QVariant("gray"));
    colorCB->addItem("Темно-серый", QVariant("darkgray"));
    colorCB->addItem("Светло-серый", QVariant("lightgray"));
    colorCB->addItem("Лайм", QVariant("lime"));
    colorCB->addItem("Пурпурный", QVariant("purpure"));
    colorCB->addItem("Пурпурно-красный", QVariant("magenta"));
    colorCB->addItem("Белый", QVariant("white"));
    QVBoxLayout *layout = new QVBoxLayout;
    dataSourcesGB->setLayout(layout);

    connect(this, SIGNAL(onWindowShow()), this, SLOT(on_show()));
    connect(this, SIGNAL(onWindowHide()), this, SLOT(on_close()));

    connect(gridChk, SIGNAL(clicked()), this, SLOT(changeAdaptor()));
    connect(legendChk, SIGNAL(clicked()), this, SLOT(changeAdaptor()));
    connect(lineCB, SIGNAL(currentIndexChanged(int)), this, SLOT(changeAdaptor()));
    connect(colorCB, SIGNAL(currentIndexChanged(int)), this, SLOT(changeAdaptor()));
    connect(markerCB, SIGNAL(currentIndexChanged(int)), this, SLOT(changeAdaptor()));
	//main buttons
	connect(okPB, SIGNAL(clicked()), this, SLOT(slotOK()));
	connect(applyPB, SIGNAL(clicked()), this, SLOT(slotApply()));
	connect(closePB, SIGNAL(clicked()), this, SLOT(slotClose()));
	connect(restorePB, SIGNAL(clicked()), this, SLOT(slotRestore()));

	bc().setPolicy(ButtonPolicy::NoRepeatedApplyReadOnlyPolicy);
	bc().setOK(okPB);
	bc().setApply(applyPB);
	bc().setRestore(restorePB);
    bc().setCancel(closePB);
    changed();
}

GuiChart::~GuiChart()
{
    clearSeries();
}

void GuiChart::clearSeries()
{
    for (int i=0; i<seriesList->count(); i++)
    {
        QListWidgetItem *it = seriesList->item(i);
        ChartLine *line = (ChartLine *)it->data(Qt::UserRole).toLongLong();
        delete line;
    }
    seriesList->clear();
    linesList->clear();
    dataTable->clear();
}

void GuiChart::changeAdaptor()
{
    changed();
}

void GuiChart::applyView()
{
    InsetChartParams & igp = params_;
    igp.title = fromqstr(titleLine->text());
    igp.legend = legendChk->isChecked();
    igp.xLabel = fromqstr(xLabelLine->text());
    igp.yLabel = fromqstr(yLabelLine->text());
    igp.grid = gridChk->isChecked();
    igp.lines.clear();
    for (int i=0; i<seriesList->count(); i++)
    {
        QListWidgetItem *it = seriesList->item(i);
        ChartLine *line = (ChartLine *)it->data(Qt::UserRole).toLongLong();
        igp.lines.push_back(line->clone());
    }
}

bool GuiChart::isValid()
{
    return true;
}


bool GuiChart::initialiseParams(string const & data)
{
    InsetChart::string2params(data, buffer(), params_);
	paramsToDialog(params_);
	return true;
}

void GuiChart::clearParams()
{
    params_ = InsetChartParams();
}

void GuiChart::dispatchParams()
{
    InsetChartParams tmp_params(params_);
    string const lfun = InsetChart::params2string(tmp_params, buffer());
    dispatch(FuncRequest(getLfun(), lfun));
}

void GuiChart::paramsToDialog(const InsetChartParams &params)
{
    clearSeries();
    titleLine->setText(params.title.c_str());
    legendChk->setChecked(params.legend);
    xLabelLine->setText(params.xLabel.c_str());
    yLabelLine->setText(params.yLabel.c_str());
    gridChk->setChecked(params.grid);
    for (std::vector<ChartLine* >::const_iterator i=params.lines.begin(); i!=params.lines.end(); ++i)
    {
        ChartLine *line = (*i);
        QListWidgetItem *it = new QListWidgetItem(line->name.c_str(), seriesList);
        new QListWidgetItem(line->name.c_str(), linesList);
        it->setFlags(it->flags() | Qt::ItemIsEditable);
        it->setData(Qt::UserRole, (qlonglong)(line->clone()));
    }
    if (seriesList->count()>0){
        seriesList->setCurrentRow(0);
    }
}
//MAXVAS
Dialog * createGuiChart(GuiView & lv) {
    return new GuiChart(lv);
}


} // namespace frontend
} // namespace lyx

#include "moc_GuiChart.cpp"

void lyx::frontend::GuiChart::on_addSeriesB_clicked()
{
    QListWidgetItem *it = new QListWidgetItem(QString("Ряд %1").arg(seriesList->count()+1), seriesList);
    new QListWidgetItem(QString("Ряд %1").arg(seriesList->count()+1), linesList);
    it->setFlags(it->flags() | Qt::ItemIsEditable);
    ChartLine *line = new ChartLine;
    it->setData(Qt::UserRole, (qlonglong)line);
    line->name = it->text().toStdString();
    seriesList->setCurrentItem(it);
    changeAdaptor();
}

void lyx::frontend::GuiChart::on_removeSeriesB_clicked()
{
    int row = seriesList->currentRow();
    QListWidgetItem *it = seriesList->takeItem(row);
    linesList->takeItem(row);
    if (!it){
        return;
    }
    ChartLine *line = (ChartLine *)it->data(Qt::UserRole).toLongLong();
    delete line;
    delete it;
    changeAdaptor();
}

void lyx::frontend::GuiChart::on_addPointB_clicked()
{
    //take selected QListWidgetItem
    QListWidgetItem *lit = seriesList->item(seriesList->currentRow());
    if (!lit){
        return;
    }
    int row = dataTable->rowCount();
    dataTable->setRowCount(row+1);
    dataTable->setRowHeight(row, 25);
    QTableWidgetItem *it = new QTableWidgetItem("0");
    dataTable->setItem(row, 0, it);
    it = new QTableWidgetItem("0");
    dataTable->setItem(row, 1, it);
    on_dataTable_itemChanged(it);
    changeAdaptor();
}

void lyx::frontend::GuiChart::on_removePointB_clicked()
{
    //take selected QListWidgetItem
    QListWidgetItem *it = seriesList->item(seriesList->currentRow());
    if (!it){
        return;
    }
    int row = dataTable->currentRow();
    if (row<0){
        return;
    }
    //take line
    ChartLine *line = (ChartLine *)it->data(Qt::UserRole).toLongLong();
    line->data.erase(line->data.begin()+row);
    dataTable->removeRow(row);
    changeAdaptor();
}

void lyx::frontend::GuiChart::on_seriesList_currentRowChanged(int currentRow)
{
    //take selected QListWidgetItem
    QListWidgetItem *it = seriesList->item(currentRow);
    if (!it){
        return;
    }
    //take line
    ChartLine *line = (ChartLine *)it->data(Qt::UserRole).toLongLong();
    //show line in first tab
    showSeries(line);
    if (guiSender){
        guiSender = false;
        linesList->setCurrentRow(currentRow);
        guiSender = true;
    }
}

void lyx::frontend::GuiChart::on_linesList_currentRowChanged(int currentRow)
{
    //take selected QListWidgetItem
    QListWidgetItem *it = seriesList->item(currentRow);
    if (!it){
        return;
    }
    //take line
    ChartLine *line = (ChartLine *)it->data(Qt::UserRole).toLongLong();
    //show line in second tab
    showLine(line);
    if (guiSender){
        guiSender = false;
        seriesList->setCurrentRow(currentRow);
        guiSender = true;
    }
}

void lyx::frontend::GuiChart::showSeries(lyx::ChartLine *line)
{
    int count = line->data.size();
    dataTable->clear();
    dataTable->setHorizontalHeaderLabels(QStringList()<<"X"<<"Y");
    dataTable->setRowCount(count);
    for (int row=0; row<count; row++)
    {
        QTableWidgetItem *it = new QTableWidgetItem(tr("%1").arg(line->data.at(row)->x));
        dataTable->setItem(row, 0, it);
        it = new QTableWidgetItem(tr("%1").arg(line->data.at(row)->y));
        dataTable->setItem(row, 1, it);
        dataTable->setRowHeight(row, 25);
    }
}

void lyx::frontend::GuiChart::showLine(lyx::ChartLine *line)
{
    guiSender = false;
    smoothChk->setChecked(line->smooth);
    colorCB->setCurrentIndex(colorCB->findData(QVariant(line->lineColor.c_str())));
    lineCB->setCurrentIndex(lineCB->findData(QVariant(line->lineType.c_str())));
    markerCB->setCurrentIndex(markerCB->findData(QVariant(line->markerType.c_str())));
    guiSender = true;
}

void lyx::frontend::GuiChart::on_dataTable_itemChanged(QTableWidgetItem *item)
{
    //take selected QListWidgetItem
    QListWidgetItem *it = seriesList->item(seriesList->currentRow());
    if (!it){
        return;
    }
    //take line
    ChartLine *line = (ChartLine *)it->data(Qt::UserRole).toLongLong();
    int column = item->column();
    int row = item->row();
    if ((int)line->data.size()<=row)
    {
        ChartPoint *p = new ChartPoint;
        p->x = 0;
        p->y = 0;
        line->data.push_back(p);
    }
    bool success = false;
    if (column==0){
        line->data[row]->x = item->text().toDouble(&success);
    } else {
        line->data[row]->y = item->text().toDouble(&success);
    }
    if (success){
        item->setBackgroundColor(Qt::white);
    } else {
        item->setBackgroundColor(Qt::red);
    }
    changeAdaptor();
}

void lyx::frontend::GuiChart::on_seriesList_itemChanged(QListWidgetItem *item)
{
    ChartLine *line = (ChartLine *)item->data(Qt::UserRole).toLongLong();
    if (!line){
        return;
    }
    line->name = item->text().toStdString();
    QListWidgetItem *it = linesList->item(linesList->currentRow());
    if (!it){
        return;
    }
    it->setText(line->name.c_str());
    changeAdaptor();
}

void lyx::frontend::GuiChart::on_smoothChk_stateChanged(int arg1)
{
    if (!guiSender){
        return;
    }
    //take selected QListWidgetItem
    QListWidgetItem *it = seriesList->item(seriesList->currentRow());
    if (!it){
        return;
    }
    //take line
    ChartLine *line = (ChartLine *)it->data(Qt::UserRole).toLongLong();
    line->smooth = (arg1==2);
    changeAdaptor();
}

void lyx::frontend::GuiChart::on_colorCB_currentIndexChanged(int index)
{
    if (!guiSender){
        return;
    }
    //take selected QListWidgetItem
    QListWidgetItem *it = seriesList->item(seriesList->currentRow());
    if (!it){
        return;
    }
    //take line
    ChartLine *line = (ChartLine *)it->data(Qt::UserRole).toLongLong();
    line->lineColor = colorCB->itemData(index).toString().toStdString();
    changeAdaptor();
}

void lyx::frontend::GuiChart::on_lineCB_currentIndexChanged(int index)
{
    if (!guiSender){
        return;
    }
    //take selected QListWidgetItem
    QListWidgetItem *it = seriesList->item(seriesList->currentRow());
    if (!it){
        return;
    }
    //take line
    ChartLine *line = (ChartLine *)it->data(Qt::UserRole).toLongLong();
    line->lineType = lineCB->itemData(index).toString().toStdString();
    changeAdaptor();
}

void lyx::frontend::GuiChart::on_markerCB_currentIndexChanged(int index)
{
    if (!guiSender){
        return;
    }
    //take selected QListWidgetItem
    QListWidgetItem *it = seriesList->item(seriesList->currentRow());
    if (!it){
        return;
    }
    //take line
    ChartLine *line = (ChartLine *)it->data(Qt::UserRole).toLongLong();
    line->markerType = markerCB->itemData(index).toString().toStdString();
    changeAdaptor();
}

void lyx::frontend::GuiChart::on_selectDataButtonPressed()
{
    QPushButton *button = (QPushButton* )(QObject::sender());
    dsManager->execute(button->text());
}

void lyx::frontend::GuiChart::on_dataSelected(LapTable *table)
{
    //take selected QListWidgetItem
    QListWidgetItem *it = seriesList->item(seriesList->currentRow());
    if (!it){
        return;
    }
    //take line
    ChartLine *line = (ChartLine *)it->data(Qt::UserRole).toLongLong();
    for (std::vector<ChartPoint* >::iterator i = line->data.begin(); i!=line->data.end(); ++i)
    {
        ChartPoint *p = *i;
        delete p;
    }
    line->data.clear();
    for (int i=0; i<table->rowsCount(); i++)
    {
        ChartPoint *p = new ChartPoint;
        for (int j=0; j<qMin(2, table->columnsCount()); j++)
        {
            double value = table->data[i][j].toDouble();
            if (j==0)
            {
                p->x = value;
            } else
            {
                p->y = value;
            }
        }
        line->data.push_back(p);
    }
    showSeries(line);
}

void lyx::frontend::GuiChart::removeLayout(QLayout* layout)
{
    QLayoutItem* child;
    while(layout->count()!=0)
    {
        child = layout->takeAt(0);
        if(child->layout() != 0)
        {
            removeLayout(child->layout());
        }
        else if(child->widget() != 0)
        {
            delete child->widget();
        }

        delete child;
    }
}

void lyx::frontend::GuiChart::on_show()
{
    dsManager = new DataSourceManager();
    QStringList ds = dsManager->list();
    QVBoxLayout *layout = (QVBoxLayout* )dataSourcesGB->layout();
    removeLayout(layout);
    for (QStringList::const_iterator i = ds.begin(); i!=ds.end(); ++i)
    {
        QString dataSourceName = *i;
        QPushButton *button = new QPushButton(dataSourceName);
        connect(button, SIGNAL(clicked()), this, SLOT(on_selectDataButtonPressed()));
        layout->addWidget(button);
    }
    QSpacerItem *spacer = new QSpacerItem(1,1, QSizePolicy::Fixed, QSizePolicy::Expanding);
    layout->addSpacerItem(spacer);
    connect(dsManager, SIGNAL(dataSelected(LapTable*)), this, SLOT(on_dataSelected(LapTable*)));
    QDesktopWidget *desktopWidget = QApplication::desktop();
    QRect screenRect = desktopWidget->geometry();
    int x = screenRect.center().x();
    int y = screenRect.center().y();
    this->setGeometry(x - this->width()/2, y - this->height()/2, this->width(), this->height());
}

void lyx::frontend::GuiChart::on_close()
{
    delete dsManager;
}
