#ifndef SERIALSCANNER_MAINWINDOW_H
#define SERIALSCANNER_MAINWINDOW_H

#include <QWidget>
#include <QTimerEvent>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QTimer>

#include <QDebug>

#define SEARCH  0
#define CONNECT 1


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QWidget {
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

    void setDev(const QString& name);

    void scanCom(const QString& comName);

    void setPkg(const QByteArray& pkg);

protected:
    void timerEvent(QTimerEvent *event) override;

private:
    void init();

    void readBuff();

    void serialError(QSerialPort::SerialPortError error);

private:
    Ui::MainWindow *ui;

    QSerialPort *m_serial;
    QStringList m_coms;
    int m_timerID;
    QByteArray m_pkg;
    uint8_t m_status = SEARCH;

    QString m_devName;

    QTimer *m_scanTimer;

};


#endif //SERIALSCANNER_MAINWINDOW_H
