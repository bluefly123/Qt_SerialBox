#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtCharts/QChartView>
#include <QtCharts/QSplineSeries>
#include <QValueAxis>
#include <QGraphicsView>
#include <QChartView>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>



#include <QMessageBox>
#include <QDebug>
#include <QTimer>


#define MAX_X   50
#define MAX_Y   70
QT_CHARTS_USE_NAMESPACE
QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    QSerialPort *serial = new QSerialPort;
    QTimer *timer_cycleReceive = new QTimer;

    Widget(QWidget *parent = nullptr);
    QChart * Chart_Init();

    void int2Chars(char str[],long long int a,int &length);

    void send_data();

    char convertHexChart(char ch);

    void StringToHex(QString str, QByteArray &senddata);

    ~Widget();

private slots:
    void on_pushButton_tab1_clicked();

    void on_pushButton_refreshSerial_clicked();

    void on_checkBox_openSerial_toggled(bool checked);

    void on_readSerial();

private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
