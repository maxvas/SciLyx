// -*- C++ -*-
/**
 * \file GuiGraphics.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author John Levon
 * \author Herbert Voß
 * \author Baruch Even
 * \author Angus Leeming
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef GUIGRAPHICS_H
#define GUIGRAPHICS_H

#include "GuiDialog.h"
#include "ui_GraphicsUi.h"
#include <QObject>

#include "insets/InsetGraphicsParams.h"

#include <vector>

class QString;

namespace lyx {

namespace frontend {

class GraphicsPreview: public QWidget
{
    Q_OBJECT

public:
    GraphicsPreview(QWidget *parent = 0);
    void setImage(QByteArray data);
    bool hasImage();
private:
    void paintEvent(QPaintEvent *);
    QImage image_;

Q_SIGNALS:
    void changed();

};

class GuiGraphics : public GuiDialog, public Ui::GraphicsUi
{
	Q_OBJECT

public:
	GuiGraphics(GuiView & lv);
	void setAutoText();

private Q_SLOTS:
	void change_adaptor();
	void changeGroup(int);
	void changeBB();
	void on_newGroupPB_clicked();
	void on_browsePB_clicked();
	void on_getPB_clicked();
	void on_scaleCB_toggled(bool);
	void on_WidthCB_toggled(bool);
	void on_HeightCB_toggled(bool);
	void on_angle_textChanged(const QString &);

private:
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
	void paramsToDialog(InsetGraphicsParams const & params);

	/// get bounding box from file
	void getBB();
	/// Read the Bounding Box from a eps or ps-file
    std::string readBoundingBox(QByteArray const &imageData);
	/// test if file exist
	bool isFileNameValid(std::string const & fname) const;

	/// Control the bb
	bool bbChanged;
	/// Store the LaTeX names for the rotation origins.
	std::vector<std::string> origin_ltx;
	///
	InsetGraphicsParams params_;
	/// the current graphics group
	std::string current_group_;
    QByteArray imageData;
    GraphicsPreview *preview;
};

} // namespace frontend
} // namespace lyx

#endif // GUIGRAPHICS_H
