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
    m_serial = new QSerialPort(this);
    m_serial->setBaudRate(QSerialPort::Baud115200);
    m_serial->setDataBits(QSerialPort::Data8);
    m_serial->setParity(QSerialPort::NoParity);
    m_serial->setStopBits(QSerialPort::OneStop);
    m_serial->setFlowControl(QSerialPort::NoFlowControl);
    m_timerID = startTimer(1000);
    setDev("USB-SERIAL CH340");
    setPkg("\xFF\xFF\x01\x01");

    m_scanTimer = new QTimer(this);
    connect(m_scanTimer, &QTimer::timeout, this, &MainWindow::rev);

    connect(this, &MainWindow::next, this, &MainWindow::scanComs);
}

void MainWindow::setDev(const QString &name) {
    m_devName = name;
}

void MainWindow::setPkg(const QByteArray &pkg) {
    m_pkg = pkg;
}

void MainWindow::scanComs() {
    m_serial->setPortName(m_coms.at(m_portIndex));
    if (m_serial->open(QIODevice::ReadWrite)) {
        qDebug() << "Open: " << m_coms.at(m_portIndex);
        m_portName = m_coms.at(m_portIndex);
        QByteArray data;
        data[0] = 0xFE;
        data[1] = 0x5A;
        data[2] = 0x49;
        data[3] = 0x61;
        data[4] = 0x6E;
        data[5] = 0xEF;
        qint64 byte = m_serial->write(data);
        qDebug()<<"byte"<<byte;
        m_scanTimer->start(1000);
    } else {
        qDebug()<<"Can not open "<<m_coms.at(m_portIndex);
    }
}

void MainWindow::rev() {
    QByteArray buff = m_serial->read(6).toHex();
    qDebug() << buff << "size: " << buff.size();
    if (buff == "fe5a49616eef") {
        ui->status->setText("CONNECT");
        ui->status->setStyleSheet(QString::fromUtf8("QLabel{color: green}"));
        m_scanTimer->stop();
        m_status = CONNECT;
        connect(m_serial, &QSerialPort::readyRead, this, &MainWindow::readBuff);
        connect(m_serial, &QSerialPort::errorOccurred, this, &MainWindow::serialError);
        m_serial->readAll();    //clear buffer2
    } else if (m_portIndex+1 < m_coms.size()) {
        qDebug()<<"m_portIndex: "<<m_portIndex;
        m_serial->close();
        m_scanTimer->stop();
        m_portIndex++;
        emit next();
    } else {
        m_serial->close();
        m_scanTimer->stop();
        m_timerID = startTimer(1000);
    }
}

void MainWindow::readBuff() {
    QByteArray buff = m_serial->readAll().toHex();
    qDebug() << buff;
}

void MainWindow::serialError(QSerialPort::SerialPortError error) {
    if (error == QSerialPort::PermissionError && m_serial->isOpen()) {
        m_status = SEARCH;
        qDebug() << "Serial port disconnected.";
        disconnect(m_serial, &QSerialPort::readyRead, this, &MainWindow::readBuff);
        disconnect(m_serial, &QSerialPort::errorOccurred, this, &MainWindow::serialError);
        m_scanTimer->stop();
        m_serial->close();
        m_coms.clear();
        ui->status->setText("SEARCH");
        ui->status->setStyleSheet(QString::fromUtf8("QLabel{color: red}"));
        m_timerID = startTimer(1000);
    }
}

void MainWindow::timerEvent(QTimerEvent *event) {
    if (event->timerId() == m_timerID && m_status == SEARCH) {
        m_coms.clear();
        QStringList coms;
        foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
            qDebug() << info.portName();
            coms.append(info.portName());
            if (info.description() == m_devName) {
                m_coms.append(info.portName());
                qDebug() << info.portName() << "is CH340";
            }
        }
        if (!m_coms.empty()) {
            killTimer(m_timerID);
            m_portIndex = 0;
            qDebug()<<"m_coms.size(): "<<m_coms.size();
            emit next();
        }
    }
}

void MainWindow::closeEvent(QCloseEvent *event) {
    if (m_status == CONNECT) {
        QByteArray buff;
        buff[0] = 0xFE;
        buff[1] = 0x6E;
        buff[2] = 0x61;
        buff[3] = 0x49;
        buff[4] = 0x5A;
        buff[5] = 0xEF;
        m_serial->write(buff);
    }
    QWidget::closeEvent(event);
}

