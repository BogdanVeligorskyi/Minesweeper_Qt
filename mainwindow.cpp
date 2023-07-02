#include "mainwindow.h"
#include "playwindow.h"
#include "settingswindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QString>

// window constructor
MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow) {
    ui->setupUi(this);
    setWindow = new SettingsWindow(this);
    /*mines_counter = 0;
    mines_left = 10;
    columns = 11;
    rows = 11;
    mines_arr = new mines_struct[10];*/


    //ui->labelMinesLeft->setText("Mines left: 10");
    //mines_generator(mines_arr, 10, 10);
    //connect(ui->pushPlayButton, clicked(), this, &MainWindow::onPlayClicked());
    //connect(ui->pushSettingsButton, clicked(), this, &MainWindow::onSettingsClicked());

}

// window destructor
MainWindow::~MainWindow() {
    delete ui;
}


void MainWindow::on_pushPlayButton_clicked() {
    plWindow = new PlayWindow(this, setWindow->getNumOfMines(), setWindow->getBoardSize());
    this->hide();
    plWindow->exec();
    this->show();
}

void MainWindow::on_pushSettingsButton_clicked() {
    this->hide();
    setWindow->exec();
    qDebug()<< "HERE";
    int mines = setWindow->getNumOfMines();
    QString boardSize = setWindow->getBoardSize();
    qDebug() << boardSize;
    this->show();
}
