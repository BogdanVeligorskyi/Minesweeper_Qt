#include "mainwindow.h"
#include "playwindow.h"
#include "ui_playwindow.h"
#include <QDir>
#include <map>
#include <QQueue>

PlayWindow::PlayWindow(QWidget *parent, int numOfMines, QString boardSize) :
        QDialog(parent),
        ui(new Ui::PlayWindow) {
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
    this->numOfMines = numOfMines;
    this->boardSize = boardSize;

    QString text = "Mines Left: ";
    QString num = QString::number(this->numOfMines);
    text.append(num);
    ui->labelMinesLeft->setText(text);

    QString strRow = (this->boardSize.split("x"))[0];
    QString strCol = (this->boardSize.split("x"))[1];
    rows = strRow.toInt();
    cols = strCol.toInt();

    this->setMinimumSize(45*cols, 45*rows + 40);
    ui->gridLayoutWidget_2->setMinimumSize(44*cols, 40);

    visited_arr = new int[rows*cols];
    butArr = new QPushButton*[cols*rows];
    qDebug() << QDir::currentPath();
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            QPushButton *pushButton = new QPushButton(this);
            QString buttonName = "pushButton_";
            pushButton->setObjectName(buttonName.append(QString::number(i*cols+j)));
            pushButton->setFixedSize(37, 37);
            pushButton->setIconSize(QSize(37, 37));
            butArr[i*cols+j] = pushButton;
            visited_arr[i*cols+j] = 0;
            connect(pushButton, SIGNAL(clicked()), this, SLOT(on_button_clicked()));
            ui->gridLayout->addWidget(pushButton, i, j);
        }
    }
    this->minesCounter = this->numOfMines;
    this->found_mines_arr = new int[this->minesCounter];
    for (int i = 0; i < this->minesCounter; i++) {
        found_mines_arr[i] = 0;
    }
    this->mines_arr = new int[this->minesCounter];
    mines_arr = generateMines(rows*cols);

}

// destructor
PlayWindow::~PlayWindow() {
    delete ui;
}

void PlayWindow::on_button_clicked() {
    QPushButton* buttonSender = qobject_cast<QPushButton*>(sender());
    QString textStart = "You have clicked buttton: ";
    QString text = textStart.append(buttonSender->objectName());
    qDebug()<< text;

    QStringList strList = buttonSender->objectName().split("_");
    QString buttonNumStr = strList.value(1);
    int buttonNumInt = buttonNumStr.toInt();
    this->clickedButton = buttonNumInt;
    if (checkIfMine(buttonSender, buttonNumInt)) {
        QString text = "Mines Left: ";
        QString num = QString::number(this->minesCounter);
        text.append(num);
        ui->labelMinesLeft->setText(text);
    } else {
        checkNeighbourCells(buttonNumInt);
    }
}

int* PlayWindow::generateMines(int square) {
    qsrand(time(NULL)|clock());

    for (int i = 0; i < numOfMines; i++) {
        bool isUnique = false;
        while (!isUnique) {
            int randNum = qrand() % square-1;
            for (int k = 0; k <= i; k++) {
                if (mines_arr[k] == randNum || randNum <= 0) {
                    isUnique = false;
                    break;
                }
                if (k == i) {
                    mines_arr[i] = randNum;
                    isUnique = true;
                }
            }
        }
    }
    qDebug() << "Mines Array:";
    for (int j = 0; j < numOfMines; j++) {
        qDebug() << mines_arr[j] << " ";
    }
    return mines_arr;

}

bool PlayWindow::checkIfMine(QPushButton* but, int buttonNumInt) {
    for (int i = 0; i < numOfMines; i++) {
        if (buttonNumInt == mines_arr[i]) {
            this->minesCounter--;
            but->setIcon(QIcon("mine_clicked.png"));
            but->setIconSize(QSize(37, 37));
            bool isAlreadyClicked = false;
            for (int j = 0; j < numOfMines; j++) {
                if (this->found_mines_arr[j] == buttonNumInt) {
                    qDebug() << "Already clicked!";
                    isAlreadyClicked = true;
                    break;
                }
            }
            if (isAlreadyClicked == true) {
                this->minesCounter ++;
            } else {
                this->found_mines_arr[this->numOfMines - this->minesCounter] = buttonNumInt;

            }
            return true;
        }

    }
    return false;
}

int PlayWindow::checkNeighbourCells(int butNum) {

    QQueue <int> queue;
    queue.enqueue(butNum);
    visited_arr[butNum] = 1;
    int* arr_of_neighbours = new int[8];

    while (queue.length() > 0) {
        butNum = queue.at(0);
        qDebug() << "ButNum =" << butNum;
        arr_of_neighbours[0] = butNum - cols;
        arr_of_neighbours[1] = butNum - cols + 1;
        arr_of_neighbours[2] = butNum + 1;
        arr_of_neighbours[3] = butNum + cols + 1;
        arr_of_neighbours[4] = butNum + cols;
        arr_of_neighbours[5] = butNum + cols - 1;
        arr_of_neighbours[6] = butNum - 1;
        arr_of_neighbours[7] = butNum - cols - 1;
        queue.dequeue();
        for (int i = 0; i < 8; i++) {
            // check cell
            int res = checkNeighbourCell(butNum, arr_of_neighbours[i]);
            if (res == 0 &&
                visited_arr[arr_of_neighbours[i]] == 0) {
                qDebug() << "i =" << i << ", arr_of_neighbours[i] =" << arr_of_neighbours[i];
                queue.enqueue(arr_of_neighbours[i]);
                visited_arr[arr_of_neighbours[i]] = 1;
            } else if (res == 1 && visited_arr[arr_of_neighbours[i]] == 0) {
                qDebug() << "i =" << i << ", arr_of_neighbours[i] =" << arr_of_neighbours[i];
                visited_arr[arr_of_neighbours[i]] = 1;
            }
        }

    }
    qDebug() << "jjjjj";
    delete arr_of_neighbours;
    int minesInNeighbourCells = 0;
    QPushButton* button;

    for (int j = 0; j < rows*cols; j++) {
        if (visited_arr[j] == 1) {
            if (checkForMinesCount(j - cols, j - cols) == 1) {
                qDebug() << "MinesCounter0:" << minesInNeighbourCells;
                minesInNeighbourCells++;
            }
            if (((j % cols) != (cols-1)) && checkForMinesCount(j - cols + 1, j - cols + 1) == 1) {
                //qDebug() << "j:" << j;
                qDebug() << "MinesCounter1:" << minesInNeighbourCells;
                minesInNeighbourCells++;
            }
            if (((j % cols) != (cols - 1)) && checkForMinesCount(j + 1, j + 1) == 1) {
                qDebug() << "MinesCounter2:" << minesInNeighbourCells;
                //qDebug() << "j:" << j;
                minesInNeighbourCells++;
            }
            if (((j % cols) != (cols - 1)) && checkForMinesCount(j + cols + 1, j + cols + 1) == 1) {
                qDebug() << "MinesCounter3:" << minesInNeighbourCells;
                //qDebug() << "j:" << j;
                minesInNeighbourCells++;
            }
            if (checkForMinesCount(j + cols, j + cols) == 1) {
                qDebug() << "MinesCounter4:" << minesInNeighbourCells;
                minesInNeighbourCells++;
            }
            if (((j % cols) != 0) && checkForMinesCount(j + cols - 1, j + cols - 1) == 1) {
                qDebug() << "MinesCounter5:" << minesInNeighbourCells;
                //qDebug() << "j:" << j;
                minesInNeighbourCells++;
            }
            if (((j % cols) != 0) && checkForMinesCount(j - 1, j - 1) == 1) {
                qDebug() << "MinesCounter6:" << minesInNeighbourCells;
                //qDebug() << "j:" << j;
                minesInNeighbourCells++;
            }
            if (((j % cols) != 0) && checkForMinesCount(j - cols - 1, j - cols - 1) == 1) {
                qDebug() << "MinesCounter7:" << minesInNeighbourCells;
                //qDebug() << "j:" << j;
                minesInNeighbourCells++;
            }
           button = ui->gridLayoutWidget->findChild<QPushButton *>("pushButton_" + QString::number(j));
           setIconWithDigit(button, minesInNeighbourCells);
        }
        minesInNeighbourCells = 0;
    }

    return 0;
}

int PlayWindow::checkNeighbourCell(int firstParam, int butNum) {

    if (!checkIfValidCoord(firstParam, butNum)) {
        qDebug() << "Invalid button!";
        return 10;
    }
    for (int i = 0; i < numOfMines; i++) {
        if (mines_arr[i] == butNum) {
            //qDebug() << "This button contains mine!";
            return 2;
        }
    }

    int upCellNum = butNum - cols;
    int upRightCellNum = butNum - cols + 1;
    int rightCellNum = butNum + 1;
    int downRightCellNum = butNum + cols + 1;
    int downCellNum = butNum + cols;
    int downLeftCellNum = butNum + cols - 1;
    int leftCellNum = butNum - 1;
    int upLeftCellNum = butNum - cols - 1;

    for (int i = 0; i < numOfMines; i++) {
        if (checkIfValidCoord(butNum, upCellNum) && mines_arr[i] == upCellNum) {
            qDebug() << "1. This button" << butNum << " has mine neighbours:" << upCellNum;
            return 1;
        }
        if (checkIfValidCoord(butNum, upRightCellNum) && mines_arr[i] == upRightCellNum) {
            qDebug() << "2. This button" << butNum << " has mine neighbours:" << upRightCellNum;
            return 1;
        }
        if (checkIfValidCoord(butNum, rightCellNum) && mines_arr[i] == rightCellNum) {
            qDebug() << "3. This button" << butNum << " has mine neighbours:" << rightCellNum;
            return 1;
        }
        if (checkIfValidCoord(butNum, downRightCellNum) && mines_arr[i] == downRightCellNum) {
            qDebug() << "4. This button" << butNum << " has mine neighbours:" << downRightCellNum;
            return 1;
        }
        if (checkIfValidCoord(butNum, downCellNum) && mines_arr[i] == downCellNum) {
            qDebug() << "5. This button" << butNum << " has mine neighbours:" << downCellNum;
            return 1;
        }
        if (checkIfValidCoord(butNum, downLeftCellNum) && mines_arr[i] == downLeftCellNum) {
            qDebug() << "6. This button" << butNum << " has mine neighbours:" << downLeftCellNum;
            return 1;
        }
        if (checkIfValidCoord(butNum, leftCellNum) && mines_arr[i] == leftCellNum) {
            qDebug() << "7. This button" << butNum << " has mine neighbours:" << leftCellNum;
            return 1;
        }
        if (checkIfValidCoord(butNum, upLeftCellNum) && mines_arr[i] == upLeftCellNum) {
            qDebug() << "8. This button" << butNum << " has mine neighbours:" << upLeftCellNum;
            return 1;
        }

    }
    return 0;
}


int PlayWindow::checkForMinesCount(int firstParam, int num) {
    if (!checkIfValidCoord(firstParam, num)) {
        return 10;
    }
    for (int i = 0; i < numOfMines; i++) {
        if (mines_arr[i] == num) {
            //qDebug() << "This button has mine neighbours!";
            return 1;
        }
    }
    return 0;
}


bool PlayWindow::checkIfValidCoord(int firstParam, int num) {
    int x = num % cols;
    int y = num / rows;
    int x_prev = firstParam % cols;
    //qDebug() << "X: " << x << " Y: " << y;
    // if cell is out of range
    if (x < 0 || y < 0 || x >= cols || y > rows || (x == 0 && x_prev == (cols-1)) || (x == (cols-1) && x_prev == 0)) {
        //qDebug() << "Out of range!";
        return false;
    }
    return true;
}


void PlayWindow::setIconWithDigit(QPushButton* button, int minesNum) {
    qDebug() << button->objectName();
    if (button == NULL) {
        qDebug() << "Button was not found!";
        return;
    }
    button->setIconSize(QSize(37, 37));
    if (minesNum == 0) {
        button->setIcon(QIcon("empty_cell.png"));
    } else if (minesNum == 1) {
        button->setIcon(QIcon("digit_1.png"));
    } else if (minesNum == 2) {
        button->setIcon(QIcon("digit_2.png"));
    } else if (minesNum == 3) {
        button->setIcon(QIcon("digit_3.png"));
    } else if (minesNum == 4) {
        button->setIcon(QIcon("digit_4.png"));
    } else if (minesNum == 5) {
        button->setIcon(QIcon("digit_5.png"));
    } else if (minesNum == 6) {
        button->setIcon(QIcon("digit_6.png"));
    }
}
