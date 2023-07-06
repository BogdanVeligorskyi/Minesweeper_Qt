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
    setWindowFlags(Qt::Window | Qt::MSWindowsFixedSizeDialogHint);
    this->setWindowIcon(QIcon("mine_usual.png"));
    /*mines_counter = 0;
    mines_left = 10;
    columns = 11;
    rows = 11;
    mines_arr = new mines_struct[10];*/
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
    qDebug()<< "Returned to Main menu";
    int mines = setWindow->getNumOfMines();
    QString boardSize = setWindow->getBoardSize();
    qDebug() << boardSize;
    this->show();
}
