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
#include "ui_ChartUi.h"

#include "insets/InsetChartParams.h"
#include "ChartConverter.h"

#include <vector>
#include <DataSourceManager.h>
#include <LapTable.h>

class QString;

namespace lyx {

namespace frontend {

class GuiChart : public GuiDialog, public Ui::ChartUi
{
	Q_OBJECT

public:
	GuiChart(GuiView & lv);
    ~GuiChart();
	void setAutoText();
    void clearSeries();
    void removeLayout(QLayout *layout);

private Q_SLOTS:
    void changeAdaptor();
    void on_addSeriesB_clicked();
    void on_removeSeriesB_clicked();
    void on_addPointB_clicked();
    void on_removePointB_clicked();
    void on_seriesList_currentRowChanged(int currentRow);
    void on_linesList_currentRowChanged(int currentRow);
    void showSeries(ChartLine *line);
    void showLine(ChartLine *line);
    void on_dataTable_itemChanged(QTableWidgetItem *item);
    void on_seriesList_itemChanged(QListWidgetItem *item);
    void on_smoothChk_stateChanged(int arg1);
    void on_colorCB_currentIndexChanged(int index);
    void on_lineCB_currentIndexChanged(int index);
    void on_markerCB_currentIndexChanged(int index);
    void on_selectDataButtonPressed();
    void on_dataSelected(LapTable *table);
    void on_show();
    void on_close();
    void on_OK();
    void on_Apply();
    void on_Converted();

private:
    bool guiSender;
    DataSourceManager *dsManager;
    InsetChartConverter converter;
    QString actionAfterConvert;
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
    void paramsToDialog(InsetChartParams const & params);

	/// get bounding box from file
	void getBB();
	/// Browse for a file
	QString browse(QString const &) const;
	/// Read the Bounding Box from a eps or ps-file
	std::string readBoundingBox(std::string const & file);
	/// test if file exist
	bool isFileNameValid(std::string const & fname) const;

	/// Control the bb
	bool bbChanged;
	/// Store the LaTeX names for the rotation origins.
	std::vector<std::string> origin_ltx;
	///
    InsetChartParams params_;
};

} // namespace frontend
} // namespace lyx

#endif // GUICHART_H
