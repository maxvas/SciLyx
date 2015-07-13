// -*- C++ -*-
/**
 * \file GuiTabularCreate.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Max Kuznetcov
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef GUITABULARDATACREATE_H
#define GUITABULARDATACREATE_H

#include "GuiDialog.h"
#include "ui_TabularDataCreateUi.h"

#include <utility>
#include <DataSourceManager.h>

namespace lyx {
namespace frontend {


class GuiTabularDataCreate : public GuiDialog, public Ui::TabularDataCreateUi
{
	Q_OBJECT

public:
    GuiTabularDataCreate(GuiView & lv);

    void removeLayout(QLayout *layout);
private Q_SLOTS:
    void on_selectDataButtonPressed();
    void on_dataSelected(LapTable *table);

private:
	/// Apply changes
	void applyView();
	///
	bool initialiseParams(std::string const & data);
	/// clean-up on hide.
	void clearParams();
	///
	void dispatchParams();
	///
	bool isBufferDependent() const { return true; }
	///
	FuncCode getLfun() const { return LFUN_TABULAR_DATA; }
	///
    LapTable *params() { return params_; }

private Q_SLOTS:
    void on_show();
    void on_close();

private:
	/// rows, cols params
    LapTable *params_;
    DataSourceManager *dsManager;
};

} // namespace frontend
} // namespace lyx

#endif // GUITABULARDATACREATE_H
