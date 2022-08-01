#include "ProfileWindow.h"
#include <QPainter>
#include <QGridLayout>
#include <QLineEdit>
#include <QLabel>
#include <QMouseEvent>
#include "MainWindow.h"
#include "TranslatorFromAndroidStrings.h"
#include "MediaData.h"
#include "Global.h"

using MyLineEdit=QLineEdit;

namespace ClientApplication {
extern std::unique_ptr<TranslatorFromAndroidStrings> translator;
}

ProfileWindow::ProfileWindow(double w, double h)
: width_(w)
, height_(h) {
    bgImage_ = QPixmap("../res/drawable/menu_background.png");
    backButton_ = QPixmap("../res/drawable/backbutton.png");

    QGridLayout* grid = new QGridLayout;
    int row = 0;

    QWidget* temp = new QWidget;
    temp->setMaximumHeight(71._ZRH);
    temp->setFixedHeight(71._ZRH);
    grid->addWidget(temp, row++, 0);

    QLabel* profileLabel = new QLabel();
    profileLabel->setObjectName("profileLabel");
    grid->addWidget(profileLabel, row++ ,0, Qt::AlignTop | Qt::AlignHCenter);
    int titleTextSize = 25._ZRD;
    profileLabel->setStyleSheet("color: #ddc69f;font-size: " + QString::number(titleTextSize) + "px;font-family: ubuntu mono; font-weight: bold;text-decoration: underline");

    temp = new QWidget;
    temp->setMaximumHeight(40._ZRD);
    temp->setFixedHeight(40._ZRD);
    grid->addWidget(temp, row++, 0);

    QLabel* yourNameLabel = new QLabel();
    yourNameLabel->setObjectName("yourNameLabel");
    grid->addWidget(yourNameLabel, 2 ,0, Qt::AlignBottom);
    int generalTextSize = 14._ZRD;
    yourNameLabel->setStyleSheet("color: #ddc69f;font-size: " + QString::number(generalTextSize) + "px;font-family: ubuntu mono;");

    edit_ = new MyLineEdit;
    grid->addWidget(edit_, row++, 0, Qt::AlignTop);
    edit_->setMaximumWidth(300._ZRD);
    edit_->setFixedWidth(300._ZRD);
    edit_->setStyleSheet("color: black;font-size: " + QString::number(generalTextSize) + "px;font-family: ubuntu mono;");
    edit_->setMaxLength(13);

    setLayout(grid);
    grid->setRowStretch(3, 10);
}

void ProfileWindow::paintEvent(QPaintEvent* pe) {
    QPainter p(this);
    p.fillRect(0, 0, width(), height(), QBrush(Qt::GlobalColor::black));

    if(!bgImage_.isNull()) {
        //QRectF desc((width() - ZRD(bgImage_.width()) - 10) / 2, (height() - ZRD(bgImage_.height()) - 10) / 2, ZRD(bgImage_.width()), ZRD(bgImage_.height()));
        QRectF desc(9, 9, ZRD(bgImage_.width()), ZRD(bgImage_.height()));
        QRectF src(0, 0, bgImage_.width(), bgImage_.height());
        p.drawPixmap(desc, bgImage_, src);
    }
    if(!backButton_.isNull()) {
        //QRectF desc((width() - width_ - 10) / 2, (height() - height_ - 10) / 2, ZRD(backButton_.width()), ZRD(backButton_.height()));
        QRectF desc(9, 9, ZRD(backButton_.width()), ZRD(backButton_.height()));
        QRectF src(0, 0, backButton_.width(), backButton_.height());
        p.drawPixmap(desc, backButton_, src);
    }
    QWidget::paintEvent(pe);
}

void ProfileWindow::mouseReleaseEvent(QMouseEvent*me) {
    if(me->button() == Qt::LeftButton) {
        //QRectF desc((width() - width_ - 10) / 2, (height() - height_ - 10) / 2, ZRD(backButton_.width()), ZRD(backButton_.height()));
        QRectF desc(9, 9, ZRD(backButton_.width()), ZRD(backButton_.height()));
        if(desc.contains(me->x(), me->y())) {
            ((MainWindow*)parent())->resetCentralWidget();
        }
    }
    QWidget::mouseReleaseEvent(me);
}

std::wstring ProfileWindow::profileName() noexcept {
    return  edit_->text().toStdWString();
}

void ProfileWindow::setProfileName(std::wstring name) noexcept {
    edit_->setText(QString::fromStdWString(name));
}

void ProfileWindow::updateLabels() noexcept {
    QString label = QString::fromStdWString((*ClientApplication::translator)(L"profile"));
    findChild<QLabel*>("profileLabel")->setText(label);

    label = QString::fromStdWString((*ClientApplication::translator)(L"your_name"));
    findChild<QLabel*>("yourNameLabel")->setText(label);
}
