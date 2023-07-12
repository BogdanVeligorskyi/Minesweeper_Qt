#include "settingswindow.h"
#include "ui_settingswindow.h"


bool checkBoardSize(QString, int &, QString &);
bool checkIfDigit(QChar);
bool checkNumOfMines(int, int &, QString &);


// constructor
SettingsWindow::SettingsWindow(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::SettingsWindow)
{
    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
    ui->setupUi(this);
    if (!readSettingsFromFile()) {
        this->numOfMines = 10;
        this->boardSize = "10x10";
    }

    ui->lineEditNumOfMines->setText(QString::number(this->numOfMines));
    ui->lineEditSize->setText(this->boardSize);

    ui->labelErrorText->setStyleSheet("QLabel {color : red}");
    ui->labelErrorText->setText("");
}

// destructor
SettingsWindow::~SettingsWindow() {
    delete ui;
}


// handler for 'Save' button
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
    saveSettingsToFile();

    this->hide();
}


// check entered number of mines
bool checkNumOfMines(int num, int &square, QString &strError) {

    if (num < 1 || num > square / 3) {
        strError = "NUMBEROFMINES IS INCORRECT!";
        qDebug() << strError;
        return false;
    }
    return true;
}


// check entered board size
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


// determine whether char is digit or not
bool checkIfDigit(QChar c) {
    if(c.isDigit()) {
        return true;
    }
    return false;
}


// read settings from settings.txt file
bool SettingsWindow::readSettingsFromFile() {
    char buf[80];
    FILE *fr = fopen("files/settings.txt", "rt");
    if (fr == NULL) {
        qDebug() << "Can`t open settings.txt!";
        return false;
    }
    QString line = "";
    while (fgets(buf, 80, fr) != NULL) {
        line = QString(buf);
    }
    QStringList strList = line.split(",");
    QString boardStr = strList.at(0);
    QString minesNumStr = strList.at(1);
    this->boardSize = boardStr;
    this->numOfMines = minesNumStr.toInt();
    fclose(fr);
    return true;
}


// save settings to settings.txt file
void SettingsWindow::saveSettingsToFile() {
    char filename[] = "files/settings.txt";
    FILE *fw = fopen(filename, "w");
    if (fw == NULL) {
        qDebug() << "settings.txt hasn`t been found!";
        return;
    }

    QByteArray boardBA = this->boardSize.toLocal8Bit();
    const char* charBoardSize = boardBA.data();

    fprintf(fw, "%s,%d", charBoardSize, this->numOfMines);
    fclose(fw);
}
