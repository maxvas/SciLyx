#include <config.h>

#include "GuiSelectData.h"
#include "qt_helpers.h"

using namespace std;
using namespace lyx::support;

namespace lyx {
namespace frontend {

GuiSelectData::GuiSelectData(GuiView & lv)
    : DialogView(lv, "select-data", qt_("Выбор данных"))
{
	setupUi(this);
}

Dialog * createGuiSelectData(GuiView & lv) {
    return new GuiSelectData(lv);
}


} // namespace frontend
} // namespace lyx

#include "moc_GuiSelectData.cpp"
