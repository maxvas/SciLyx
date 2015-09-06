
#include <config.h>

#include "GuiSelectData.h"
#include "qt_helpers.h"
#include "LyX.h"
#include <QVBoxLayout>
#include <QLabel>
#include "docgenwindow.h"

using namespace std;
using namespace lyx::support;

namespace lyx {
namespace frontend {

GuiSelectData::GuiSelectData(GuiView & lv)
    : DialogView(lv, "select-data", qt_("Выбор данных"))
{
	setupUi(this);
    signalMapper = new QSignalMapper(this);
    QVBoxLayout *layout1 = new QVBoxLayout(groupBox);
    groupBox->setLayout(layout1);
    lyx::LyX *lyx = &lyx::Singleton<lyx::LyX>::Instance();
    Q_FOREACH(QString name, lyx->docGenWindows.keys())
    {
        QPushButton *button = new QPushButton(lyx->docGenWindows[name]->buttonName());
        connect(button, SIGNAL(clicked()), signalMapper, SLOT(map()));
        signalMapper->setMapping(button, name);
        QLabel *label = new QLabel(lyx->docGenWindows[name]->description());
        QHBoxLayout *layout2 = new QHBoxLayout;
        layout2->addWidget(button);
        layout2->addWidget(label);
        layout1->addLayout(layout2);
    }
    layout1->addStretch(1);
    connect(signalMapper, SIGNAL(mapped(QString)), this, SLOT(onButtonPressed(QString)));
}

GuiSelectData::~GuiSelectData()
{
    delete signalMapper;
}

void GuiSelectData::onButtonPressed(QString name)
{
    lyx::LyX *lyx = &lyx::Singleton<lyx::LyX>::Instance();
    lyx->docGenWindows[name]->execute();
}

Dialog * createGuiSelectData(GuiView & lv) {
    return new GuiSelectData(lv);
}


} // namespace frontend
} // namespace lyx

#include "moc_GuiSelectData.cpp"
