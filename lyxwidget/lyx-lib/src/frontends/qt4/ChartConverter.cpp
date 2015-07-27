
#include "ChartConverter.h"

#include "../insets/InsetChartParams.h"
#include "../support/TempFile.h"
#include <QObject>
#include <QProcess>
#include <QFile>
#include <QDir>
#include <QDebug>

using namespace std;
using namespace lyx;
using namespace lyx::support;

InsetChartConverter::InsetChartConverter()
    :QObject(0), inProcess(false)
{
}

void InsetChartConverter::startConvertation(const lyx::InsetChartParams *params)
{
    if (inProcess)
        return;
    inProcess = true;
    QString fileName = writeLatex(params);
    if (fileName=="")
        return;
    this->fileName = fileName;
    QFileInfo f(fileName);
    fileOnlyPath = f.absolutePath();
    fileOnlyName = f.fileName();
    mProcess.setWorkingDirectory(fileOnlyPath);
    QString cmd = "pdflatex";
    disconnect(&mProcess, SIGNAL(finished(int)), this, SLOT(onGsProcessFinished(int)));
    disconnect(&mProcess, SIGNAL(readyRead()), this, SLOT(onGsReadyRead()));
    disconnect(&mProcess, SIGNAL(error(QProcess::ProcessError)), this, SLOT(onError()));
    connect(&mProcess, SIGNAL(finished(int)), this, SLOT(onLatexProcessFinished(int)));
    connect(&mProcess, SIGNAL(readyRead()), this, SLOT(onLatexReadyRead()));
    connect(&mProcess, SIGNAL(error(QProcess::ProcessError)), this, SLOT(onError()));
    mProcess.start(cmd, QStringList()<<fileOnlyName);
}

QByteArray InsetChartConverter::getImageData()
{
    return mImageData;
}

QString InsetChartConverter::writeLatex(const lyx::InsetChartParams *params)
{
    tempfile = new TempFile("chartXXXXXX.tex");
    tempfile->setAutoRemove(false);
    string outfile = tempfile->name().toFilesystemEncoding();
    ofstream texfile(outfile.c_str());
    texfile<<"\\documentclass{standalone}\n\
             \\usepackage{tikz}\n\
             \\usepackage[T2A,T1]{fontenc}\n\
             \\usepackage[utf8]{inputenc}\n\
             \\usepackage[english,russian]{babel}\n\
             \\usepackage{pgfplots}\n\
             \\usetikzlibrary{plotmarks}\n\
             \n\
             \\begin{document}\n";
    texfile<<"\\begin{tikzpicture}\n";
    texfile<<"\\begin{axis}[\n";
    texfile<<"title="<<to_utf8(params->title)<<",\n";
    if (params->legend){
        texfile<<"legend style={xshift=3.5cm,yshift=-.2cm},\n";
    }
    if (params->grid){
        texfile<<"grid=major,\n";
    }
    texfile<<"xlabel="<<to_utf8(params->xLabel)<<",\n";
    texfile<<"ylabel="<<to_utf8(params->yLabel)<<"\n";
    texfile<<"]\n";

    for (std::vector<ChartLine* >::const_iterator i=params->lines.begin(); i!=params->lines.end();i++){
        ChartLine *line = *i;
        string smoothValue = "";
        if (line->smooth){
            smoothValue= ", smooth";
        }
        texfile<<"\\addplot["<<line->lineType<<", mark = "<<line->markerType<<smoothValue<<", mark options={solid}, color="<<line->lineColor<<"] coordinates {\n";
        for (std::vector<ChartPoint* >::iterator i=line->data.begin(); i!=line->data.end(); i++){
            ChartPoint *point = *i;
            texfile<<"( "<<point->x<<", "<<point->y<<" )\n";
        }
        texfile<<"};\n";
        if (params->legend){
            texfile<<"\\addlegendentry{"<<to_utf8(line->name)<<"}\n";
        }
    }
    texfile<<"\\end{axis}\n";
    texfile<<"\\end{tikzpicture}\n";
    texfile<<"\\end{document}\n";
    texfile.flush();
    texfile.close();
    delete tempfile;
    return outfile.c_str();
}

void InsetChartConverter::onLatexProcessFinished(int code)
{
    qDebug()<<"Latex finished: "+QString::number(code);
    QString cmd = "mgs";
    QStringList args;
    args<<"-dSAFER";
    args<<"-dBATCH";
    args<<"-dNOPAUSE";
    args<<"-sDEVICE=png16m";
    args<<"-dGraphicsAlphaBits=4";
    args<<"-r150";
    args<<"-sOutputFile=-";
    args<<"-q";
    QString gsFileName = fileOnlyName;
    if (gsFileName.endsWith(".tex"))
        gsFileName = gsFileName.left(gsFileName.length()-4);
    gsFileName = gsFileName + ".pdf";
    args<<gsFileName;
    disconnect(&mProcess, SIGNAL(finished(int)), this, SLOT(onLatexProcessFinished(int)));
    disconnect(&mProcess, SIGNAL(readyRead()), this, SLOT(onLatexReadyRead()));
    disconnect(&mProcess, SIGNAL(error(QProcess::ProcessError)), this, SLOT(onError()));
    connect(&mProcess, SIGNAL(finished(int)), this, SLOT(onGsProcessFinished(int)));
    connect(&mProcess, SIGNAL(readyRead()), this, SLOT(onGsReadyRead()));
    connect(&mProcess, SIGNAL(error(QProcess::ProcessError)), this, SLOT(onError()));
    mProcess.start(cmd, args);
    mImageData.clear();
}

void InsetChartConverter::onGsProcessFinished(int code)
{
    inProcess = false;
    Q_EMIT finished();
}

void InsetChartConverter::onLatexReadyRead()
{
    qDebug()<<mProcess.readAll();
}

void InsetChartConverter::onGsReadyRead()
{
    mImageData.append(mProcess.readAll());
}

void InsetChartConverter::onError()
{
    qDebug()<<"Error"+mProcess.errorString();
}

#include "moc_ChartConverter.cpp"
