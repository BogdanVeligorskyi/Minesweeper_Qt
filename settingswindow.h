#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QDialog>
#include <string.h>
#include <QString>
#include <QDebug>

namespace Ui {
    class SettingsWindow;
}

// Settings Window class
class SettingsWindow : public QDialog {
    Q_OBJECT

public:
    explicit SettingsWindow(QWidget *parent = 0);
    ~SettingsWindow();

    int getNumOfMines() {
        return numOfMines;
    }

    QString getBoardSize() {
        return boardSize;
    }

private:
    Ui::SettingsWindow *ui;
    int numOfMines;
    QString boardSize;

private slots:
    void on_pushSaveButton_clicked();
    bool readSettingsFromFile();
    void saveSettingsToFile();
};

#endif // SETTINGSWINDOW_H
