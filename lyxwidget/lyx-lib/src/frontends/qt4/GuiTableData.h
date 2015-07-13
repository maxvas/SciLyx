// -*- C++ -*-
/**
 * \file GuiChart.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Maxim Kuznetcov
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef GUICHART_H
#define GUICHART_H

#include "GuiDialog.h"
#include "ui_TableDataUi.h"

#include "insets/InsetTabular.h"

#include <vector>
#include <DataSourceManager.h>
#include <LapTable.h>

class QString;

namespace lyx {

namespace frontend {

class GuiTableData : public GuiDialog, public Ui::TableDataUi
{
	Q_OBJECT

public:
    GuiTableData(GuiView & lv);
    ~GuiTableData();

private Q_SLOTS:
    void changeAdaptor();

private:
    bool guiSender;
    DataSourceManager dsManager;
	///
	bool isValid();
	/// Dialog inherited methods
	//@{
	void applyView();
	void updateContents() {}
	bool initialiseParams(std::string const & data);
	void clearParams();
	void dispatchParams();
	bool isBufferDependent() const { return true; }
	//@}

	///
    void paramsToDialog(InsetTabular const & params);

};

} // namespace frontend
} // namespace lyx

#endif // GUICHART_H
