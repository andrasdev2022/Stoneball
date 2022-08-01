#ifndef __CLIENTCANVAS_H__
#define __CLIENTCANVAS_H__

#include <QWidget>
#include <QPixmap>
#include <QPainter>

class Q_DECL_EXPORT ClientCanvas : public QWidget {
    Q_OBJECT
public:

    explicit ClientCanvas(QWidget *parent = 0);

    void paintEvent(QPaintEvent* pe) override;
    void mouseMoveEvent(QMouseEvent*me) override;
    void mousePressEvent(QMouseEvent*me) override;
    void mouseReleaseEvent(QMouseEvent*me) override;
    void wheelEvent(QWheelEvent *event) override;

    void updateImage(QImage image);
    
signals:

public slots:

private:
    QImage canvasImage;
};

#endif // __CLIENTCANVAS_H__
