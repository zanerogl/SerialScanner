#include "../include/mainwindow.h"
#include "../forms/ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    init();
    setDev("USB-SERIAL CH340");
    setPkg("\xFF\xFF\x01\x01");
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::init() {
    m_timerID = startTimer(1000);
    m_serial = new QSerialPort(this);
    m_serial->setBaudRate(QSerialPort::Baud115200);
    m_serial->setDataBits(QSerialPort::Data8);
    m_serial->setParity(QSerialPort::NoParity);
    m_serial->setStopBits(QSerialPort::OneStop);
    m_serial->setFlowControl(QSerialPort::NoFlowControl);
    m_scanTimer = new QTimer(this);
}

void MainWindow::setDev(const QString& name) {
    m_devName = name;
}

void MainWindow::setPkg(const QByteArray& pkg){
    m_pkg = pkg;
}

void MainWindow::scanCom(const QString& comName) {
    m_serial->setPortName(comName);
    if(m_serial->open(QIODevice::ReadWrite)){
        qDebug()<<comName;
        m_scanTimer->start(1000);
        connect(m_scanTimer, &QTimer::timeout, this, &MainWindow::readBuff);
    }
    m_status = SEARCH;
}

void MainWindow::readBuff() {
    QByteArray buff = m_serial->readAll();
    QByteArray header = buff.mid(0, 4);
    qDebug()<<"m_pkg:"<<m_pkg;
    qDebug()<<"header"<<header;
    if (header == m_pkg){
        m_status = CONNECT;
        m_scanTimer->stop();
        disconnect(m_scanTimer, &QTimer::timeout, this, &MainWindow::readBuff);
        ui->status->setText("CONNECT");
        ui->status->setStyleSheet(QString::fromUtf8("QLabel{color: green}"));
        connect(m_serial, &QSerialPort::errorOccurred, this, &MainWindow::serialError);
        killTimer(m_timerID);
    }
}

void MainWindow::serialError(QSerialPort::SerialPortError error) {
    if (error == QSerialPort::PermissionError  && m_serial->isOpen()) {// 串口意外断开
        m_status = SEARCH;
        qDebug() << "Serial port disconnected.";
        disconnect(m_serial, &QSerialPort::errorOccurred, this, &MainWindow::serialError);
        m_serial->close();
        m_scanTimer->stop();
        ui->status->setText("SEARCH");
        ui->status->setStyleSheet(QString::fromUtf8("QLabel{color: red}"));
        m_timerID = startTimer(1000);
    }
}

void MainWindow::timerEvent(QTimerEvent *event) {
    if(event->timerId() == m_timerID && m_status == SEARCH){
        m_coms.clear();
        foreach(const QSerialPortInfo &info,QSerialPortInfo::availablePorts()){
            qDebug()<<info.portName();
            if (info.description() == m_devName){
                m_coms.append(info.portName());
            }
            for (const auto & m_com : m_coms) {
                scanCom(m_com);
            }
        }
    }
    QObject::timerEvent(event);
}





