#ifndef TEXTEDITOR_H
#define TEXTEDITOR_H

#include <QMainWindow>

namespace Ui {
class TextEditor;
}

class TextEditor : public QMainWindow
{
    Q_OBJECT

public:
    explicit TextEditor(QString fileName);
    ~TextEditor();

private slots:
    void on_actSave_triggered();

private:
    Ui::TextEditor *ui;
    QString fileName;
    void closeEvent(QCloseEvent *event);

signals:
    void saved(QString fileName);
};

#endif // TEXTEDITOR_H
