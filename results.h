#ifndef RESULTS_H
#define RESULTS_H

#include <QDialog>
#include <QTableWidget>
#include <string.h>
#include <QDebug>
#include <QString>

namespace Ui {
    class Results;
}

// Results Window class
class Results : public QDialog {
    Q_OBJECT

public:
    explicit Results(QWidget *parent = 0);
    ~Results();

private:
    Ui::Results *ui;

private slots:
    void readResultsFromFile();

};

#endif // RESULTS_H
