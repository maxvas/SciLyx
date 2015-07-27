#ifndef QANIMATEDPUSHBUTTON_H
#define QANIMATEDPUSHBUTTON_H

#include <QPushButton>
#include <QString>
#include <QObject>

class QMovie;
class QAnimatedPushButton: public QPushButton
{
    Q_OBJECT
public:
    explicit QAnimatedPushButton(QWidget * parent = 0);
    void setAnimatedIcon(QString fileName);
    void showAnimation();
    void hideAnimation();

private Q_SLOTS:
    void setButtonIcon();
private:
    QMovie *myMovie;
};

#endif // QANIMATEDPUSHBUTTON_H
