
#ifndef INSETCC_H
#define INSETCC_H

#include <QObject>
#include <QProcess>

#include "../support/TempFile.h"
#include "../insets/InsetChartParams.h"

using namespace lyx::support;

namespace lyx {
    class InsetChartParams;
}

class ChartConverter: public QObject
{
    Q_OBJECT
public:
    explicit ChartConverter();
    void startConvertation(const lyx::InsetChartParams *params);
    QByteArray getImageData();

private:
    QByteArray mImageData;
    QProcess mProcess;
    TempFile *tempfile;
    bool inProcess;
    QString writeLatex(const lyx::InsetChartParams *params);
    QString fileName;
    QString fileOnlyName;
    QString fileOnlyPath;


private Q_SLOTS:
    void onLatexProcessFinished(int code);
    void onGsProcessFinished(int code);
    void onLatexReadyRead();
    void onGsReadyRead();
    void onError();

Q_SIGNALS:
    void finished();
    void error();

};

#endif //INSETCC_H
