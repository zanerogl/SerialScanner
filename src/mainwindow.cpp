#include "../include/mainwindow.h"
#include "../forms/ui_MainWindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    init();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::init() {
    m_timerID = startTimer(1000);

}

void MainWindow::timerEvent(QTimerEvent *event) {
    if(event->timerId() == m_timerID){
        qDebug()<<"Time up";
    }
    QObject::timerEvent(event);

}
