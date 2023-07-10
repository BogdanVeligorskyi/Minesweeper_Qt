#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QIcon>
#include <QPushButton>
#include <QDebug>
#include <QString>
#include <time.h>
#include <stdlib.h>
#include <settingswindow.h>
#include <playwindow.h>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    PlayWindow *plWindow;
    SettingsWindow *setWindow;

private slots:
    void on_pushSettingsButton_clicked();
    void on_pushPlayButton_clicked();

};

#endif // MAINWINDOW_H
