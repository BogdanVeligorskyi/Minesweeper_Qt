#include "mainwindow.h"
#include "playwindow.h"
#include "ui_playwindow.h"


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
    butArr = new QRightClickButton*[cols*rows];

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            QRightClickButton *pushButton = new QRightClickButton(this);
            QString buttonName = "pushButton_";
            pushButton->setObjectName(buttonName.append(QString::number(i*cols+j)));
            pushButton->setFixedSize(37, 37);
            pushButton->setIconSize(QSize(37, 37));
            butArr[i*cols+j] = pushButton;
            visited_arr[i*cols+j] = 0;
            pushButton->setEnabled(true);
            connect(pushButton, SIGNAL(clicked()), this, SLOT(on_button_clicked()));
            connect(pushButton, SIGNAL(rightClicked()), this, SLOT(on_right_clicked()));
            ui->gridLayout->addWidget(pushButton, i, j);
        }
    }
    this->minesCounter = this->numOfMines;
    this->mines_arr = new int[this->minesCounter];
    mines_arr = generateMines(rows*cols);

}


// destructor
PlayWindow::~PlayWindow() {
    delete ui;
}


// handler for LMB click on cell
void PlayWindow::on_button_clicked() {

    QRightClickButton* buttonSender = qobject_cast<QRightClickButton*>(sender());
    QString textStart = "You have clicked buttton: ";
    QString text = textStart.append(buttonSender->objectName());
    qDebug()<< text;

    QStringList strList = buttonSender->objectName().split("_");
    QString buttonNumStr = strList.value(1);
    int buttonNumInt = buttonNumStr.toInt();
    this->clickedButton = buttonNumInt;

    // if cell has icon - skip it
    if (visited_arr[buttonNumInt] == 2 || visited_arr[buttonNumInt] == 1) {
        return;
    }

    // check if clicked button contains mine and
    // stop the game if it`s true
    if (checkIfMine(buttonNumInt)) {
        setIconToButton(buttonSender, -3);
        QMessageBox::about(this, "Game over", "Unfortunately, you`ve lost the game!");
        delete this->butArr;
        delete this->visited_arr;
        this->hide();
    // otherwise start checking surrounded (neighbour) cells
    } else {
        checkNeighbourCells(buttonNumInt);
    }

    if (checkIfEnd()) {
        QMessageBox::about(this, "Game over", "Congratulations, you`ve won the game!");
        delete this->butArr;
        delete this->visited_arr;
        this->hide();
    }
}


// handler for RMB click on cell
void PlayWindow::on_right_clicked() {

    QRightClickButton* buttonSender = qobject_cast<QRightClickButton*>(sender());
    QString textStart = "You have right-clicked buttton: ";
    QString text = textStart.append(buttonSender->objectName());
    qDebug()<< text;
    QStringList strList = buttonSender->objectName().split("_");
    QString buttonNumStr = strList.value(1);
    int buttonNumInt = buttonNumStr.toInt();



    // if cell hasn`t been clicked or visited
    if (visited_arr[buttonNumInt] != 2 && visited_arr[buttonNumInt] != 1 && this->minesCounter > 0) {
        setIconToButton(buttonSender, -1);
        this->minesCounter--;
        visited_arr[buttonNumInt] = 2;

    // if cell has been already clicked with RMB - return one mine
    } else if (visited_arr[buttonNumInt] == 2) {
        setIconToButton(buttonSender, -2);
        this->minesCounter++;
        visited_arr[buttonNumInt] = 0;
    }

    // change label text according to number of mines left
    text = "Mines Left: ";
    QString num = QString::number(this->minesCounter);
    text.append(num);
    ui->labelMinesLeft->setText(text);

    if (checkIfEnd()) {
        QMessageBox::about(this, "Game over", "Congratulations, you`ve won the game!");
        delete this->butArr;
        delete this->visited_arr;
        this->hide();
    }
}


// generate fixed number of mines
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


// check if clicked button contains a mine
bool PlayWindow::checkIfMine(int buttonNumInt) {
    for (int i = 0; i < numOfMines; i++) {
        if (buttonNumInt == mines_arr[i]) {
            return true;
        }
    }
    return false;
}


// draw route until cell with mine as neighbour is found
int PlayWindow::checkNeighbourCells(int butNum) {

    QQueue <int> queue;
    queue.enqueue(butNum);
    visited_arr[butNum] = 1;
    int* arr_of_neighbours = new int[8];

    // draw route to expand all needed cells
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

            int res = checkNeighbourCell(butNum, arr_of_neighbours[i]);
            if (res == 0 &&
                visited_arr[arr_of_neighbours[i]] == 0) {
                queue.enqueue(arr_of_neighbours[i]);
                visited_arr[arr_of_neighbours[i]] = 1;
            } else if (res == 1 && visited_arr[arr_of_neighbours[i]] == 0) {
                visited_arr[arr_of_neighbours[i]] = 1;
            }
        }

    }
    qDebug() << "jjjjj";
    delete arr_of_neighbours;
    int minesInNeighbourCells = 0;
    QRightClickButton* button;

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
           button = ui->gridLayoutWidget->findChild<QRightClickButton *>("pushButton_" + QString::number(j));
           setIconToButton(button, minesInNeighbourCells);
        }
        minesInNeighbourCells = 0;
    }

    return 0;
}


// check neighbour cell
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

    int *arr = new int[8];
    arr[0] = butNum - cols;
    arr[1] = butNum - cols + 1;
    arr[2] = butNum + 1;
    arr[3] = butNum + cols + 1;
    arr[4] = butNum + cols;
    arr[5] = butNum + cols - 1;
    arr[6] = butNum - 1;
    arr[7] = butNum - cols - 1;

    for (int i = 0; i < numOfMines; i++) {
        for (int j = 0; j < 8; j++) {
            if (checkIfValidCoord(butNum, arr[j]) && mines_arr[i] == arr[j]) {
                //qDebug() << "1. This button" << butNum << " has mine neighbours:" << upCellNum;
                return 1;
            }
        }
    }
    return 0;
}


// count number of mines in neighbour cells
int PlayWindow::checkForMinesCount(int firstParam, int num) {
    if (!checkIfValidCoord(firstParam, num)) {
        return 10;
    }
    for (int i = 0; i < numOfMines; i++) {
        if (mines_arr[i] == num) {
            return 1;
        }
    }
    return 0;
}


// check if coordinates of button are valid
bool PlayWindow::checkIfValidCoord(int firstParam, int num) {

    int x = num % cols;
    int y = num / rows;
    int x_prev = firstParam % cols;

    // if cell is out of range
    if (x < 0 || y < 0 || x >= cols || y > rows ||
        (x == 0 && x_prev == (cols-1)) || (x == (cols-1) && x_prev == 0)) {
        return false;
    }
    return true;
}


// set icon to button in order to show additional information needed for user
void PlayWindow::setIconToButton(QRightClickButton* button, int minesNum) {

    //qDebug() << button->objectName();

    if (button == NULL) {
        qDebug() << "Button was not found!";
        return;
    }

    QStringList strList = button->objectName().split("_");
    QString buttonNumStr = strList.value(1);
    int buttonNumInt = buttonNumStr.toInt();
    qDebug() << "buttonNumInt =" << buttonNumInt;

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
    } else if (minesNum == -1) {
        button->setIcon(QIcon("flag.png"));
    } else if (minesNum == -2) {
        button->setIcon(QIcon());
    } else if(minesNum == -3) {
        button->setIcon(QIcon("mine_clicked.png"));
    }

}


// check if all cells were visited
bool PlayWindow::checkIfEnd() {
    for (int i = 0; i < rows*cols; i++) {
        if (visited_arr[i] == 0) {
            return false;
        }
    }
    return true;
}
