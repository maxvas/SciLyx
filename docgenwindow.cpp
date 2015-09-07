#include "docgenwindow.h"
#include "scilyx.h"

DocGenWindow::DocGenWindow(QWidget *parent)
    :QDialog(parent)
{
	
}

DocGenWindow::~DocGenWindow()
{

}

SciLyx *DocGenWindow::scilyx()
{
    return mSciLyx;
}
