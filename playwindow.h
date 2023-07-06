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
    int minesCounter;
    QString boardSize;
    QPushButton **butArr;

    int *mines_arr;
    int *found_mines_arr;

private slots:
    void on_button_clicked();
    bool checkIfMine(QPushButton*, int[], int, int);
};

#endif // PLAYWINDOW_H
