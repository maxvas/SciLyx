#include "syncdialog.h"
#include "ui_syncdialog.h"
#include <QLabel>
#include <QMovie>
#include <QHBoxLayout>
#include <QVBoxLayout>

SyncDialog::SyncDialog(QWidget *parent) :
    QLabel(parent)
{
    this->setGeometry(parent->geometry());
    this->setStyleSheet("background-color: rgba(119, 136, 153, 128);");
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    syncImage = new QMovie(":/images/sync.gif");
    syncImage->setScaledSize(QSize(50, 50));
    iconLabel = new QLabel();
    iconLabel->setMovie(syncImage);
    iconLabel->setAlignment(Qt::AlignCenter);
    iconLabel->setStyleSheet("background-color: transparent;");
    titleLabel = new QLabel("Синхронизация...");
    titleLabel->setStyleSheet("background-color: transparent;");
    titleLabel->setAlignment(Qt::AlignCenter);
    progress = new QProgressBar;
    progress->setMinimumWidth(200);
    progress->setMaximumHeight(15);
    progress->setStyleSheet("border: 1px solid grey; background-color: green;");
    progress->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    progress->setMaximum(100);
    progress->setTextVisible(false);
    QVBoxLayout *tpLayout = new QVBoxLayout;
    tpLayout->addSpacerItem(new QSpacerItem(10, 10, QSizePolicy::Minimum, QSizePolicy::Expanding));
    tpLayout->addWidget(titleLabel);
    tpLayout->addWidget(progress);
    tpLayout->addSpacerItem(new QSpacerItem(10, 10, QSizePolicy::Minimum, QSizePolicy::Expanding));

    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->addSpacerItem(new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Minimum));
    hLayout->addWidget(iconLabel);
    hLayout->addLayout(tpLayout);
    hLayout->addSpacerItem(new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Minimum));

    mainLayout->addSpacerItem(new QSpacerItem(10, 10, QSizePolicy::Minimum, QSizePolicy::Expanding));
    mainLayout->addLayout(hLayout);
    mainLayout->addSpacerItem(new QSpacerItem(10, 10, QSizePolicy::Minimum, QSizePolicy::Expanding));
    syncImage->start();
    this->setLayout(mainLayout);
//    this->hide();
    progress->setVisible(false);
}

SyncDialog::~SyncDialog()
{
}

void SyncDialog::start(QString title, bool withProgressBar)
{
    titleLabel->setText(title);
    iconLabel->setMovie(syncImage);
    if (withProgressBar)
    {
        iconLabel->setVisible(false);
        progress->setVisible(true);
    } else
    {
        iconLabel->setVisible(true);
        progress->setVisible(false);
    }
    this->show();
}

void SyncDialog::stop()
{
    this->hide();
}

void SyncDialog::setProgress(int value)
{
    progress->setValue(value);
}

void SyncDialog::showError(QString errorString)
{

}

void SyncDialog::clear()
{

}
