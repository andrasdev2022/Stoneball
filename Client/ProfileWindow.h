#ifndef __PROFILEWINDOW_H__
#define __PROFILEWINDOW_H__
#include <QWidget>
#include <QPixmap>
#include <string>

class QLineEdit;

class Q_DECL_EXPORT ProfileWindow : public QWidget
{
    Q_OBJECT
public:
    ProfileWindow(double w, double h);
    void paintEvent(QPaintEvent* pe) override;
    void mouseReleaseEvent(QMouseEvent*me) override;
    std::wstring profileName() noexcept;
    void setProfileName(std::wstring name) noexcept;
    void updateLabels() noexcept;

public slots:
signals:

private slots:
private:
    QPixmap bgImage_;
    QPixmap backButton_;
    double width_;
    double height_;
    QLineEdit* edit_;
};

#endif // __PROFILEWINDOW_H__
