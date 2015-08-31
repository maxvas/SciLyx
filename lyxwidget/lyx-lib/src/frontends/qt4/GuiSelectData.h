#ifndef GUISELECTDATA_H
#define GUISELECTDATA_H

#include "DialogView.h"
#include "ui_SelectDataUi.h"

class QString;

namespace lyx {

namespace frontend {

class GuiSelectData : public DialogView, public Ui::SelectDataUi
{
	Q_OBJECT

public:
	GuiSelectData(GuiView & lv);

private:
    /// Controller stuff
    ///@{
    void updateView() {}
    void dispatchParams() {}
    bool isBufferDependent() const { return false; }
    ///@}
};

} // namespace frontend
} // namespace lyx

#endif // GUISELECTDATA_H
