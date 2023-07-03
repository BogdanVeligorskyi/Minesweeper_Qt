#include "mainwindow.h"
#include "playwindow.h"
#include "ui_playwindow.h"
#include <QDir>
#include <map>

void generateMines(int[], int, int);

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
    int rows = strRow.toInt();
    int cols = strCol.toInt();

    this->setMinimumSize(45*cols, 45*rows + 40);
    ui->gridLayoutWidget_2->setMinimumSize(44*cols, 40);

    //ui->formLayout->SetFixedSize;
    butArr = new QPushButton*[cols*rows];
    qDebug() << QDir::currentPath();
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            QPushButton *pushButton = new QPushButton(this);
            /*QString objName = "pushButton_%1_%2";
            objName = objName.arg(i, j);*/
            QString buttonName = "pushButton_";
            pushButton->setObjectName(buttonName.append(QString::number(i*cols+j)));
            pushButton->setFixedSize(37, 37);
            pushButton->setIcon(QIcon("1.png"));
            pushButton->setIconSize(QSize(37, 37));
            butArr[i*cols+j] = pushButton;
            qDebug() << pushButton;
            connect(pushButton, SIGNAL(clicked()), this, SLOT(on_button_clicked()));
            ui->gridLayout->addWidget(pushButton, i, j);
        }
    }
    /*for (int k = 0; k < cols*rows; k++) {
        qDebug() << butArr[k]->text();
    }*/
    int minesCounter = this->numOfMines;
    int* mines_arr = new int[minesCounter];
    generateMines(mines_arr, minesCounter, rows*cols);


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
}

void generateMines(int mines_arr[], int numberOfMines, int square) {
    qsrand(time(NULL)|clock());
    for (int i = 0; i < numberOfMines; i++) {
        mines_arr[i] = qrand() % square-1;
    }
    qDebug() << "Mines Array:";
    for (int j = 0; j < numberOfMines; j++) {
        qDebug() << mines_arr[j] << " ";
    }
    /*if () {

    }*/

}
