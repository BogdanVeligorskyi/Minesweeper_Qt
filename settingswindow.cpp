#include "settingswindow.h"
#include "ui_settingswindow.h"

bool checkBoardSize(QString, int &, QString &);
bool checkIfDigit(QChar);
bool checkNumOfMines(int, int &, QString &);

SettingsWindow::SettingsWindow(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::SettingsWindow)
{
    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
    ui->setupUi(this);
    this->numOfMines = 10;
    this->boardSize = "10x10";
    ui->labelErrorText->setStyleSheet("QLabel {color : red}");
    ui->labelErrorText->setText("");
}

// destructor
SettingsWindow::~SettingsWindow() {
    delete ui;
}


void SettingsWindow::on_pushSaveButton_clicked() {
    QString strNumOfMines = ui->lineEditNumOfMines->text();
    QString strBoardSize = ui->lineEditSize->text();
    int square = 0;
    QString strError = "";
    if (!checkBoardSize(strBoardSize, square, strError) || !checkNumOfMines(strNumOfMines.toInt(), square, strError)) {
        ui->labelErrorText->setText(strError);
        return;
    }
    this->numOfMines = strNumOfMines.toInt();
    this->boardSize = strBoardSize;
    ui->labelErrorText->setText("");
    this->hide();
}

bool checkNumOfMines(int num, int &square, QString &strError) {

    if (num < 1 || num > square / 3) {
        strError = "NUMBEROFMINES IS INCORRECT!";
        qDebug() << strError;
        return false;
    }
    return true;
}

bool checkBoardSize(QString str, int &square, QString &strError) {
    if (str.isEmpty()) {
        strError = "Line in BOARDSIZE is empty!";
        qDebug() << strError;
        return false;
    }
    if (!str.contains('x')) {
        strError = "BOARDSIZE doesn`t contain 'x'!";
        qDebug() << strError;
        return false;
    }
    QStringList strArr = str.split('x');
    QString testString_1 = strArr.value(0);
    QString testString_2 = strArr.value(1);
    if (testString_1.startsWith('0') || testString_2.startsWith('0')) {
        strError = "Number in BOARDSIZE starts with '0'!";
        qDebug() << strError;
        return false;
    }
    if (testString_1.length() >= 3 || testString_2.length() >= 3) {
        strError = "Number in BOARDSIZE is too large!";
        qDebug() << strError;
        return false;
    }
    for(int i = 0; i < testString_1.length(); i++) {
        QChar ch = testString_1.at(i);
        if (!checkIfDigit(ch)) {
            strError = "BOARDSIZE (1) has non-digit chars!";
            qDebug() << strError;
            return false;
        }
    }
    for(int i = 0; i < testString_2.length(); i++) {
        QChar ch = testString_2.at(i);
        if (!checkIfDigit(ch)) {
            strError = "BOARDSIZE (2) has non-digit chars!";
            qDebug() << strError;
            return false;
        }
    }
    QString strRow = (testString_1);
    QString strCol = (testString_2);
    int rows = strRow.toInt();
    int cols = strCol.toInt();
    square = rows*cols;
    qDebug() << "Square=" << square;
    return true;
}

bool checkIfDigit(QChar c) {
    if(c.isDigit()) {
        return true;
    }
    return false;
}
