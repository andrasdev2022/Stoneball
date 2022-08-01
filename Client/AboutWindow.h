#ifndef __ABOUTWINDOW_H__
#define __ABOUTWINDOW_H__
#include <QWidget>
#include <QPixmap>
#include <string>

class QLabel;
class Q_DECL_EXPORT AboutWindow : public QWidget
{
    Q_OBJECT
public:
    AboutWindow(double w, double h);
    void paintEvent(QPaintEvent* pe) override;
    void mouseReleaseEvent(QMouseEvent*me) override;
    void updateLabels() noexcept;

public slots:
signals:

private slots:
private:
    void loadAboutText(QLabel* w) noexcept;

    QPixmap bgImage_;
    QPixmap backButton_;
    double width_;
    double height_;
};

#endif // __ABOUTWINDOW_H__
