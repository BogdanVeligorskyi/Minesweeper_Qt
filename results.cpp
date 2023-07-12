#include "results.h"
#include "ui_results.h"

// constructor
Results::Results(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Results) {
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
    readResultsFromFile();
}


// destructor
Results::~Results() {
    delete ui;
}


// read results from results.txt file
void Results::readResultsFromFile() {
    char buf[80];
    FILE *fr = fopen("files/results.txt", "rt");
    if (fr == NULL) {
        qDebug() << "Can`t open results.txt!";
        return;
    }

    int countRows = 0;
    while (fgets(buf, 80, fr) != NULL) {
        countRows++;
    }
    fclose(fr);

    char newBuf[80];
    FILE *frNew = fopen("files/results.txt", "rt");

    if (frNew == NULL) {
        qDebug() << "Can`t open results.txt!";
        return;
    }

    QString line = "";
    QTableWidget* tableWidget = ui->tableWidget;
    tableWidget->setRowCount(10);
    QTableWidgetItem *item;

    int i = 0;
    while (fgets(newBuf, 80, frNew) != NULL) {

        item = new QTableWidgetItem();
        line = QString(newBuf);
        QStringList strList = line.split(",");
        QString name = strList.at(0);
        QString boardSize = strList.at(1);
        QString numOfMines = strList.at(2);
        QString time = strList.at(3);

        item->setText(name);
        tableWidget->setItem(i, 0, item);

        item = new QTableWidgetItem();

        item->setText(time);
        tableWidget->setItem(i, 1, item);

        item = new QTableWidgetItem();

        item->setText(boardSize);
        tableWidget->setItem(i, 2, item);

        item = new QTableWidgetItem();

        item->setText(numOfMines);
        tableWidget->setItem(i, 3, item);
        i++;
    }

    fclose(fr);

    return;
}
