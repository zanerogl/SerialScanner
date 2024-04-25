//
// Created by Ian on 25/04/2024.
//

#ifndef SERIALSCANNER_MAINWINDOW_H
#define SERIALSCANNER_MAINWINDOW_H

#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QWidget {
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow() override;

private:
    Ui::MainWindow *ui;
};


#endif //SERIALSCANNER_MAINWINDOW_H
