#include <QMouseEvent>
#include <iostream>
#include <math.h>
#include <fstream>
#include <QDir>
#include "ClientCanvas.h"
#include "ClientApplication.h"

ClientCanvas::ClientCanvas(QWidget *parent)
: QWidget(parent)
{
    setMouseTracking(true);
}

void ClientCanvas::paintEvent(QPaintEvent* pe) {
    QPainter p(this);
    p.fillRect(0, 0, width(), height(), QBrush(Qt::GlobalColor::black));
    if(!canvasImage.isNull()) {
        p.drawImage(0, 0, canvasImage);
    }
    QWidget::paintEvent(pe);
}


void ClientCanvas::mouseMoveEvent(QMouseEvent*me) {
    //mouseX = me->x();
    //mouseY = me->y();
    ClientApplication::mouseMoveEvent(me->x(), me->y());
    repaint();
    QWidget::mouseMoveEvent(me);
}

void ClientCanvas::mousePressEvent(QMouseEvent*me) {
    if(me->button() != Qt::LeftButton) {
        QWidget::mousePressEvent(me);
        return;
    }
    
    //mousePressed = true;
    //mousePressedX = me->x();
    //mousePressedY = me->y();
    //setCursor(Qt::OpenHandCursor);
    ClientApplication::mousePressEvent(me->x(), me->y());
    repaint();
    QWidget::mousePressEvent(me);
}

void ClientCanvas::mouseReleaseEvent(QMouseEvent*me) {
    if(me->button() == Qt::RightButton) {
        QWidget::mouseReleaseEvent(me);
        repaint();
        return;
    }
    ClientApplication::mouseReleaseEvent(me->x(), me->y());
    //setCursor(Qt::ArrowCursor);
    QWidget::mouseReleaseEvent(me);
}

void ClientCanvas::wheelEvent(QWheelEvent *event) {
    // Let's change the selected rect in case of overlapping
/*
    if(event->modifiers() == Qt::ControlModifier) {
        if(event->delta() > 0 && mZoom > 0.1 + 1e-6) {
            mFocusedRect = -1;
            emit boundingboxSelection("", "", "");
            centeredZoom(mZoom - 0.1);
        } else if(event->delta() < 0) {
            mFocusedRect = -1;
            emit boundingboxSelection("", "", "");
            centeredZoom(mZoom + 0.1);
        }
    } else {
        nextSelectedRectUnderMouse();
    }
*/
    repaint();
    QWidget::wheelEvent(event);
}

void ClientCanvas::updateImage(QImage image) {
    canvasImage = image;
    repaint();
}

