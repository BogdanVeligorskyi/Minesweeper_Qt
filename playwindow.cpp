#include "mainwindow.h"
#include "playwindow.h"
#include "ui_playwindow.h"
#include <QDir>
#include <map>

PlayWindow::PlayWindow(QWidget *parent, int numOfMines, QString boardSize) :
        QDialog(parent),
        ui(new Ui::PlayWindow)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
    this->numOfMines = numOfMines;
    this->boardSize = boardSize;
    QString text = "Mines Left: ";
    QString num = QString::number(this->numOfMines);
    text.append(num);
    ui->labeMinesLeft->setText(text);
    QString strRow = (this->boardSize.split("x"))[0];
    QString strCol = (this->boardSize.split("x"))[1];
    int rows = strRow.toInt();
    int cols = strCol.toInt();

    butArr = new QPushButton*[cols*rows];
    qDebug() << QDir::currentPath();
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            QPushButton *pushButton = new QPushButton(this);
            /*QString objName = "pushButton_%1_%2";
            objName = objName.arg(i, j);*/
            QString buttonName = "pushButton_";
            pushButton->setObjectName(buttonName.append(QString::number(i*cols+j)));
            pushButton->setIcon(QIcon("1.png"));
            butArr[i*cols+j] = pushButton;
            qDebug() << pushButton;
            connect(pushButton, SIGNAL(clicked()), this, SLOT(on_button_clicked()));
            ui->gridLayout->addWidget(pushButton, i, j);
        }
    }
    /*for (int k = 0; k < cols*rows; k++) {
        qDebug() << butArr[k]->text();
    }*/

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
    //qDebug() << "4545454";
 }
