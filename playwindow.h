#ifndef PLAYWINDOW_H
#define PLAYWINDOW_H

#include <QDialog>
#include <QPushButton>
#include "qrightclickbutton.h"

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
    int rows;
    int cols;
    int clickedButton;
    QString boardSize;
    QRightClickButton **butArr;

    int *mines_arr;
    int *found_mines_arr;
    int* visited_arr;

private slots:
    void on_button_clicked();
    void on_right_clicked();
    bool checkIfMine(QRightClickButton*, int);
    int* generateMines(int);
    int checkNeighbourCells(int);
    int checkNeighbourCell(int, int);
    void setIconToButton(QRightClickButton*, int);
    bool checkIfValidCoord(int, int);
    int checkForMinesCount(int, int);

};

#endif // PLAYWINDOW_H
