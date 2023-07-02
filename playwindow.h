#ifndef PLAYWINDOW_H
#define PLAYWINDOW_H

#include <QDialog>
#include <QPushButton>

namespace Ui {
    class PlayWindow;
}

class PlayWindow : public QDialog {
    Q_OBJECT

public:
    explicit PlayWindow(QWidget *parent = 0, int num = 5, QString size = "10x10");
    ~PlayWindow();

private:
    Ui::PlayWindow *ui;
    int numOfMines;
    QString boardSize;
    QPushButton **butArr;

private slots:
    void on_button_clicked();
};

#endif // PLAYWINDOW_H
