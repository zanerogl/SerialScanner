#ifndef SERIALSCANNER_MAINWINDOW_H
#define SERIALSCANNER_MAINWINDOW_H

#include <QWidget>
#include <QTimerEvent>
#include <QCloseEvent>
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
    void scanComs();
    void setPkg(const QByteArray& pkg);

protected:
    void timerEvent(QTimerEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

signals:
    void portPlugin(QString portName);
    void next();

private:
    void init();
    void readBuff();
    void rev();
    void serialError(QSerialPort::SerialPortError error);

private:
    Ui::MainWindow *ui;

    QSerialPort *m_serial = nullptr;
    QStringList m_coms;
    int m_timerID = 0;
    QByteArray m_pkg;
    uint8_t m_status = SEARCH;

    QString m_devName;
    QString m_portName;

    QTimer *m_scanTimer = nullptr;
    int m_portIndex = 0;
};


#endif //SERIALSCANNER_MAINWINDOW_H
