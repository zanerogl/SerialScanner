#ifndef SERIALSCANNER_MAINWINDOW_H
#define SERIALSCANNER_MAINWINDOW_H

#include <QWidget>
#include <QTimerEvent>
#include <QSerialPort>


#include <QDebug>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QWidget {
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow() override;

protected:
    void timerEvent(QTimerEvent *event) override;

private:
    void init();

private:
    Ui::MainWindow *ui;

    QSerialPort *m_serial;
    QStringList m_comNum;
    int m_timerID{};

};


#endif //SERIALSCANNER_MAINWINDOW_H
