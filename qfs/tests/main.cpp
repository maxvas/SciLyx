#include <iostream>
#include <qfilemanager.h>

using namespace std;

int main()
{
	QFileManager fm;
	QCoreApplication app;
	fm.rm("test-dir")
}