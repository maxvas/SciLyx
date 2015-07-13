/**
 * \file GuiTabularCreate.cpp
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Max Kuznetcov
 *
 * Full author contact details are available in file CREDITS.
 */

#include <config.h>

#include "GuiTabularDataCreate.h"

#include "EmptyTable.h"
#include "FuncRequest.h"

#include "support/convert.h"

#include <QSpinBox>
#include <QPushButton>
#include <sstream>
#include <QDesktopWidget>

using namespace std;

namespace lyx {
namespace frontend {

GuiTabularDataCreate::GuiTabularDataCreate(GuiView & lv)
	: GuiDialog(lv, "tabulardatacreate", qt_("Insert Table"))
{
    setupUi(this);
    QVBoxLayout *layout = new QVBoxLayout;
    groupBox->setLayout(layout);
	bc().setPolicy(ButtonPolicy::IgnorantPolicy);
    params_ = new LapTable(2, 2);
    connect(this, SIGNAL(onWindowShow()), this, SLOT(on_show()));
    connect(this, SIGNAL(onWindowHide()), this, SLOT(on_close()));
    //bc().setCancel(closePB);
}

void GuiTabularDataCreate::on_selectDataButtonPressed()
{
    QPushButton *button = (QPushButton* )(QObject::sender());
    dsManager->execute(button->text());
}

void GuiTabularDataCreate::on_dataSelected(LapTable *table)
{
    if (params_)
    {
        delete params_;
    }
    params_ = table;
    slotOK();
}

void GuiTabularDataCreate::applyView()
{

}


bool GuiTabularDataCreate::initialiseParams(string const &)
{
    clearParams();
	return true;
}


void GuiTabularDataCreate::clearParams()
{
    if (params_)
    {
        delete params_;
    }
    params_ = new LapTable(2, 2);
}


void GuiTabularDataCreate::dispatchParams()
{
    ostringstream data;
    data << "tabular Tabular \n";
    data << "<lyxtabular version=\"3\" rows=\""<<params_->rowsCount()<<"\" columns=\""<<params_->columnsCount()<<"\">\n";
    data << "<features tabularvalignment=\"middle\">\n";
    data << "<column alignment=\"center\" valignment=\"top\">\n";
    data << "<column alignment=\"center\" valignment=\"top\">\n";
    for (int i=0; i<params_->rowsCount(); i++)
    {
        data << "<row>\n";
        for (int j=0; j<params_->columnsCount(); j++)
        {
            data << "<cell alignment=\"decimal\" valignment=\"middle\" topline=\"true\" ";
            if (i==params_->rowsCount()-1)
            {
                data << "bottomline=\"true\" ";
            }
            if (j==params_->columnsCount()-1)
            {
                data << "rightline=\"true\" ";
            }
            data << "leftline=\"true\" ";
            data << "usebox=\"none\">\n";
            data << "\\begin_inset Text\n";
            data << "\n";
            data << "\\begin_layout Plain Layout\n";
            data << params_->data[i][j].toDouble()<<"\n";
            data << "\\end_layout\n";
            data << "\n";
            data << "\\end_inset\n";
            data << "</cell>\n";
        }
        data << "</row>\n";
    }
    data << "</lyxtabular>\n";
    data << "\n";
    data << "\\end_inset\n";
    dispatch(FuncRequest(getLfun(), data.str()));
}

void GuiTabularDataCreate::removeLayout(QLayout* layout)
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

void GuiTabularDataCreate::on_show()
{
    dsManager = new DataSourceManager();
    connect(dsManager, SIGNAL(dataSelected(LapTable*)), this, SLOT(on_dataSelected(LapTable*)));
    QStringList ds = dsManager->list();
    QVBoxLayout *layout = (QVBoxLayout* )groupBox->layout();
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
    QDesktopWidget *desktopWidget = QApplication::desktop();
    QRect screenRect = desktopWidget->geometry();
    int x = screenRect.center().x();
    int y = screenRect.center().y();
    this->setGeometry(x - this->width()/2, y - this->height()/2, this->width(), this->height());
}

void GuiTabularDataCreate::on_close()
{
    if (params_)
    {
        delete params_;
    }
    params_ = 0;
    delete dsManager;
}


Dialog * createGuiTabularDataCreate(GuiView & lv)
{
	return new GuiTabularDataCreate(lv);
}


} // namespace frontend
} // namespace lyx

#include "moc_GuiTabularDataCreate.cpp"
