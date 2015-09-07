#ifndef LYXGEN_H
#define LYXGEN_H

#include <QByteArray>
#include <QFile>
#include <QDataStream>

class GitBrowser;
class LyxWidget;
class QVBoxLayout;
class SciLyxPlugin;
class DocGenWindow;

class LyxGen
{
public:
    LyxGen();
    ~LyxGen();
    void addLyxCode(QString lyxCode);
    QByteArray generate();

private:
    QByteArray mHeader;
    QByteArray mPreamble;
    QByteArray mBody;
    QByteArray mFooter;
    QString mTextClass;
    int mLyxFormat;

};

#endif // LYXGEN_H
