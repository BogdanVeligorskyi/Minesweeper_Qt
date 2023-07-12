#include "mainwindow.h"
#include "playwindow.h"
#include "settingswindow.h"
#include "results.h"
#include "ui_mainwindow.h"


// window constructor
MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow) {
    ui->setupUi(this);
    setWindow = new SettingsWindow(this);
    setWindowFlags(Qt::Window | Qt::MSWindowsFixedSizeDialogHint);
    this->setWindowIcon(QIcon("icons/mine_usual.png"));
}


// window destructor
MainWindow::~MainWindow() {
    delete ui;
}


// handler for 'Play' button
void MainWindow::on_pushPlayButton_clicked() {
    plWindow = new PlayWindow(this, setWindow->getNumOfMines(), setWindow->getBoardSize());
    this->hide();
    plWindow->exec();
    this->show();
}


// handler for 'Settings' button
void MainWindow::on_pushSettingsButton_clicked() {
    this->hide();
    setWindow->exec();
    qDebug()<< "Returned to Main menu";
    QString boardSize = setWindow->getBoardSize();
    qDebug() << boardSize;
    this->show();
}


// handler for 'Results' button
void MainWindow::on_pushResultsButton_clicked() {
    this->hide();
    resWindow = new Results(this);
    resWindow->exec();
    this->show();
}
