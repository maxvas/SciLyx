#include "lyxgen.h"
#include <QDebug>

LyxGen::LyxGen()
{
    mLyxFormat = 478;
    mPreamble = "\\usepackage{indentfirst}\n"
            "\\date{ }";
    mTextClass = "article";
}


LyxGen::~LyxGen()
{

}

void LyxGen::addLyxCode(QString lyxCode)
{
    mBody += lyxCode;
}

QByteArray LyxGen::generate()
{
    QByteArray data = "";
    data.append("\\lyxformat ");
    data.append(QString::number(mLyxFormat));
    data.append("\n");
    qDebug()<<data;
    data+="\\begin_document\n";
    qDebug()<<data;
    data+="\\begin_header\n";
    data+="\\textclass article "+mTextClass+"\n";
    data+="\\begin_preamble\n";
    data+=mPreamble+"\n";
    qDebug()<<data;
    data+="\\end_preamble\n";
    data+="\\use_default_options true\n"
//            "\\begin_modules\n"
//            "\\theorems-ams\n"
//            "\\eqs-within-sections\n"
//            "\\figs-within-sections\n"
//            "\\end_modules\n"
            "\\maintain_unincluded_children false\n"
            "\\language english\n"
            "\\language_package default\n"
            "\\inputencoding auto\n"
            "\\fontencoding global\n"
            "\\font_roman default\n"
            "\\font_sans default\n"
            "\\font_typewriter default\n"
            "\\font_math auto\n"
            "\\font_default_family default\n"
            "\\use_non_tex_fonts false\n"
            "\\font_sc false\n"
            "\\font_osf false\n"
            "\\font_sf_scale 100\n"
            "\\font_tt_scale 100\n"
            "\\graphics default\n"
            "\\default_output_format default\n"
            "\\output_sync 0\n"
            "\\bibtex_command default\n"
            "\\index_command default\n"
            "\\paperfontsize default\n"
            "\\spacing single\n"
            "\\use_hyperref false\n"
            "\\papersize a4paper\n"
            "\\use_geometry true\n"
            "\\use_package amsmath 1\n"
            "\\use_package amssymb 1\n"
            "\\use_package cancel 1\n"
            "\\use_package esint 1\n"
            "\\use_package mathdots 1\n"
            "\\use_package mathtools 1\n"
            "\\use_package mhchem 1\n"
            "\\use_package stackrel 1\n"
            "\\use_package stmaryrd 1\n"
            "\\use_package undertilde 1\n"
            "\\cite_engine basic\n"
            "\\cite_engine_type default\n"
            "\\biblio_style plain\n"
            "\\use_bibtopic false\n"
            "\\use_indices false\n"
            "\\paperorientation portrait\n"
            "\\suppress_date false\n"
            "\\justification true\n"
            "\\use_refstyle 1\n"
            "\\index Index\n"
            "\\shortcut idx\n"
            "\\color #008000\n"
            "\\end_index\n"
            "\\leftmargin 2cm\n"
            "\\topmargin 2cm\n"
            "\\rightmargin 1cm\n"
            "\\bottommargin 2cm\n"
            "\\secnumdepth 2\n"
            "\\tocdepth 2\n"
            "\\paragraph_separation indent\n"
            "\\paragraph_indentation 1cm\n"
            "\\quotes_language english\n"
            "\\papercolumns 1\n"
            "\\papersides 1\n"
            "\\paperpagestyle default\n"
            "\\tracking_changes false\n"
            "\\output_changes false\n"
            "\\html_math_output 0\n"
            "\\html_css_as_file 0\n"
            "\\html_be_strict false\n";
    data+="\\end_header\n\n";
    data+="\\begin_body\n";
    data+=mBody+"\n";
    data+="\\end_body\n";
    data+="\\end_document\n";
    QString str = data.data();
    return str.toUtf8();
}
