#ifndef __CLIENTAPPLICATION_H__
#define __CLIENTAPPLICATION_H__

#include <QImage>

namespace ClientApplication {
    int run(int argc, char* argv[]);
    QImage updateWorld();
    void mousePressEvent(int x, int y);
    void mouseReleaseEvent(int x, int y);
    void mouseMoveEvent(int x, int y);
}

#endif // #ifndef __CLIENTAPPLICATION_H__
