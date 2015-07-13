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

#include "GuiTableData.h"
#include "frontends/alert.h"
#include "qt_helpers.h"
#include "Validator.h"

#include "Buffer.h"
#include "FuncRequest.h"
#include "LengthCombo.h"
#include "Length.h"
#include "LyXRC.h"

#include "insets/InsetTabular.h"

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

using namespace std;
using namespace lyx::support;

namespace lyx {
namespace frontend {

GuiTableData::GuiTableData(GuiView & lv)
    : GuiDialog(lv, "tabledata", qt_("Таблица из источника")), guiSender(true)
{
	setupUi(this);

	//main buttons
	connect(okPB, SIGNAL(clicked()), this, SLOT(slotOK()));
	connect(applyPB, SIGNAL(clicked()), this, SLOT(slotApply()));
	connect(closePB, SIGNAL(clicked()), this, SLOT(slotClose()));
	connect(restorePB, SIGNAL(clicked()), this, SLOT(slotRestore()));

    connect(&dsManager, SIGNAL(dataSelected(LapTable*)), this, SLOT(on_dataSelected(LapTable*)));

	bc().setPolicy(ButtonPolicy::NoRepeatedApplyReadOnlyPolicy);
	bc().setOK(okPB);
	bc().setApply(applyPB);
	bc().setRestore(restorePB);
    bc().setCancel(closePB);
    changed();
}

GuiTableData::~GuiTableData()
{
}

void GuiTableData::changeAdaptor()
{
    changed();
}

void GuiTableData::applyView()
{

}

bool GuiTableData::isValid()
{
    return true;
}


bool GuiTableData::initialiseParams(string const & data)
{
	return true;
}

void GuiTableData::clearParams()
{
}

void GuiTableData::dispatchParams()
{

}

Dialog * createGuiTableData(GuiView & lv) {
    return new GuiTableData(lv);
}


} // namespace frontend
} // namespace lyx

#include "moc_GuiTableData.cpp"
