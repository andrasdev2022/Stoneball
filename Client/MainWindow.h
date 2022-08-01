#ifndef __MAINWINDOW_H__
#define __MAINWINDOW_H__
#include <QMainWindow>
#include <QRect>
#include <fstream>
#include <thread>
#include <mutex>
#include <atomic>

class ClientCanvas;
class ProfileWindow;
class AboutWindow;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(double w, double h);
    ~MainWindow();

    void resetCentralWidget();
public slots:
    void tick();

protected:

signals:

private slots:

private:
    void saveSettings();
    void loadSettings();
    void changeWindowSize() noexcept;

    ClientCanvas* canvas_{};
    QWidget* centralWidget_{};
    ProfileWindow* profileWindow_{};
    AboutWindow* aboutWindow_{};
};

#endif // __MAINWINDOW_H__
