#include "AboutWindow.h"
#include <QPainter>
#include <QGridLayout>
#include <QLabel>
#include <QMouseEvent>
#include "MainWindow.h"
#include "TranslatorFromAndroidStrings.h"
#include "MediaData.h"
#include "Global.h"
#include <QTextStream>
#include <QScrollArea>
#include "CommonUtil.h"
#include "Application.h"

namespace ClientApplication {
extern std::unique_ptr<TranslatorFromAndroidStrings> translator;
}

AboutWindow::AboutWindow(double w, double h)
: width_(w)
, height_(h) {
    bgImage_ = QPixmap("../res/drawable/menu_background.png");
    backButton_ = QPixmap("../res/drawable/backbutton.png");

    QGridLayout* grid = new QGridLayout;
    int row = 0;

    QWidget* temp = new QWidget;
    // ZRH: distance from top
    temp->setMaximumHeight(71._ZRH);
    temp->setFixedHeight(71._ZRH);
    grid->addWidget(temp, row++, 0);

    QLabel* profileLabel = new QLabel();
    profileLabel->setObjectName("Title");
    grid->addWidget(profileLabel, row++ ,0, Qt::AlignTop | Qt::AlignHCenter);
    int titleTextSize = 30._ZRD;
    profileLabel->setStyleSheet("color: #ddc69f;font-size: " + QString::number(titleTextSize) +
                                "px;font-family: ubuntu mono; font-weight: bold;text-decoration: underline");

    QLabel* content = new QLabel();
    content->setObjectName("content");
    content->setTextFormat(Qt::RichText);
    content->setWordWrap(true);
    // ZRD: width of content widget
    content->setMaximumWidth(320._ZRD - 20);
    content->setFixedWidth(320._ZRD - 20);
    loadAboutText(content);

    QScrollArea* scrollArea = new QScrollArea;
    scrollArea->setObjectName("scrollArea");
    //scrollArea->setBackgroundRole(QPalette::Dark);
    //scrollArea->setBackgroundRole(QPalette::Window);
    scrollArea->setStyleSheet("background-color:transparent;");
    scrollArea->setWidget(content);
    // ZRD: width of content widget
    scrollArea->setMaximumWidth(320._ZRD);
    scrollArea->setFixedWidth(320._ZRD);
    scrollArea->setWidgetResizable(true);

    grid->addWidget(scrollArea, row++ ,0, Qt::AlignTop | Qt::AlignHCenter);

    temp = new QWidget;
    // ZRH: distance from bottom
    temp->setMaximumHeight(60._ZRH);
    temp->setFixedHeight(60._ZRH);
    grid->addWidget(temp, row++, 0);

    setLayout(grid);
    //grid->setRowStretch(row - 1, 100);
}

void AboutWindow::paintEvent(QPaintEvent* pe) {
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

void AboutWindow::mouseReleaseEvent(QMouseEvent*me) {
    if(me->button() == Qt::LeftButton) {
        //QRectF desc((width() - width_ - 10) / 2, (height() - height_ - 10) / 2, ZRD(backButton_.width()), ZRD(backButton_.height()));
        QRectF desc(9, 9, ZRD(backButton_.width()), ZRD(backButton_.height()));
        if(desc.contains(me->x(), me->y())) {
            ((MainWindow*)parent())->resetCentralWidget();
        }
    }
    QWidget::mouseReleaseEvent(me);
}

void AboutWindow::updateLabels() noexcept {
    QString label = QString::fromStdWString((*ClientApplication::translator)(L"about"));
    findChild<QLabel*>("Title")->setText(label);

    loadAboutText(nullptr);
}

void AboutWindow::loadAboutText(QLabel* w) noexcept {
    QString lang("eng");
    switch(Application::language()) {
    case TranslateInterface::Language::ENGLISH: lang = "eng"; break;
    case TranslateInterface::Language::BULGARIAN: lang = "bul"; break;
    case TranslateInterface::Language::HUNGARIAN: lang = "hun"; break;
    case TranslateInterface::Language::LAST_ONE: break;
    }

    QFile f("../assets/about_htmls/about_" + lang + ".html");
    if(f.open(QFile::ReadOnly | QFile::Text)) {
        QTextStream in(&f);
        if(!w) {w = findChild<QLabel*>("content");}
        // the scrollarea's size does not change if I update the text only
        QScrollArea* scrollArea = findChild<QScrollArea*>("scrollArea");
        if(scrollArea) scrollArea->takeWidget();
        QString content = in.readAll().replace("<img src=\"", "<img src=\"../assets/about_htmls/");
        switch(Application::resolution()) {
        case 0: break;
        case 1:
            content = content.replace("font-size: 12pt;", "font-size:14pt;");
            content = content.replace("font-size: x-large;", "font-size: 27pt;");
            content = content.replace("font-size: large;", "font-size: 18pt;");
            break;
        case 2:
            content = content.replace("font-size: 12pt;", "font-size:16pt;");
            content = content.replace("font-size: x-large;", "font-size: 29pt;");
            content = content.replace("font-size: large;", "font-size: 20pt;");
            break;
        case 3:
            content = content.replace("font-size: 12pt;", "font-size:18pt;");
            content = content.replace("font-size: x-large;", "font-size: 31pt;");
            content = content.replace("font-size: large;", "font-size: 22pt;");
            break;
        case 4:
            content = content.replace("font-size: 12pt;", "font-size:20pt;");
            content = content.replace("font-size: x-large;", "font-size: 33pt;");
            content = content.replace("font-size: large;", "font-size: 24pt;");
            break;
        }

        w->setText(content);
        w->setTextFormat(Qt::RichText);
        if(scrollArea) scrollArea->setWidget(w);
    }
}
