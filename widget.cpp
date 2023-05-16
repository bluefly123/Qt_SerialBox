#include "widget.h"
#include "ui_widget.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QTabWidget>
#include <QDebug>

// 用于测试不同tab页面下的操作
Widget::Widget(QWidget *parent)
    : QWidget(parent), ui(new Ui::Widget)
{
    ui->setupUi(this);

    QChart *chart_realTime = Chart_Init();

    QChartView *chartView = new QChartView(); // ui->tab_2
    QPushButton *button_Test = new QPushButton;

    QVBoxLayout *QVBoxLayout_graphPage = new QVBoxLayout;             // tab2的布局
    QHBoxLayout *QHBoxLayout_graphPage_buttonGroup = new QHBoxLayout; // 按钮布局
    QPushButton *button_graphPage_pause = new QPushButton;
    QPushButton *button_graphPage_clear = new QPushButton;
    QWidget *widget_graphPage = new QWidget;             // tab2的widget
    QWidget *widget_graphPage_buttonGroup = new QWidget; // tab2的widget

    //    //主要布局为一个tabWidget
    //    mainLayout->addWidget(ui->tabWidget);
    //    //设置tab1界面的布局-使用QtDesigner设计

    //    //设置tab2界面的布局
    button_graphPage_pause->setText("暂停");
    button_graphPage_clear->setText("清除");

    QHBoxLayout_graphPage_buttonGroup->addWidget(button_graphPage_pause);
    QHBoxLayout_graphPage_buttonGroup->addWidget(button_graphPage_clear);
    widget_graphPage_buttonGroup->setLayout(QHBoxLayout_graphPage_buttonGroup);
    button_Test->setText("测试");
    chartView->setChart(chart_realTime);                            // 将chart添加到View
    QVBoxLayout_graphPage->addWidget(chartView);                    // 添加chartView到widget
    QVBoxLayout_graphPage->addWidget(button_Test);                  // 添加按钮到widget
    QVBoxLayout_graphPage->addWidget(widget_graphPage_buttonGroup); // 添加按钮组到widget

    widget_graphPage->setLayout(QVBoxLayout_graphPage); // 将布局添加进widget
    ui->tabWidget->addTab(widget_graphPage, "图形显示");
}

QChart *Widget::Chart_Init()
{
    // 初始化QChart的实例
    QChart *chart;
    QSplineSeries *lineSeries;
    chart = new QChart();
    // 初始化QSplineSeries的实例
    lineSeries = new QSplineSeries();
    // 设置曲线的名称
    lineSeries->setName("随机数测试曲线");
    // 把曲线添加到QChart的实例chart中
    chart->addSeries(lineSeries);
    // 声明并初始化X轴、两个Y轴
    QValueAxis *axisX = new QValueAxis();
    QValueAxis *axisY = new QValueAxis();
    // 设置坐标轴显示的范围
    axisX->setMin(0);
    axisX->setMax(MAX_X);
    axisY->setMin(0);
    axisY->setMax(MAX_Y);
    // 设置坐标轴上的格点
    axisX->setTickCount(10);
    axisY->setTickCount(10);
    // 设置坐标轴显示的名称
    QFont font("Microsoft YaHei", 8, QFont::Normal); // 微软雅黑。字体大小8
    axisX->setTitleFont(font);
    axisY->setTitleFont(font);
    axisX->setTitleText("X-Test");
    axisY->setTitleText("Y-Test");
    // 设置网格不显示
    axisY->setGridLineVisible(false);
    // 下方：Qt::AlignBottom，左边：Qt::AlignLeft
    // 右边：Qt::AlignRight，上方：Qt::AlignTop
    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);
    // 把曲线关联到坐标轴
    lineSeries->attachAxis(axisX);
    lineSeries->attachAxis(axisY);
    // 把chart显示到窗口上
    // ui->graphicsView->setChart(chart);
    //  设置渲染：抗锯齿，如果不设置那么曲线就显得不平滑
    // ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    return chart;
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_tab1_clicked()
{
}

void Widget::int2Chars(char str[], long long int a, int &length)
{
    length = 0;
    char temp[20];
    while (a)
    {
        temp[length] = '0' + a % 10;
        a = a / 10;
        length++;
    }
    str[length] = 0;
    int i, j;
    i = 0;
    j = length - 1;
    for (; i < length; i++, j--)
    {
        str[i] = temp[j];
    }
    return;
}

// 按下刷新按钮，检查可用串口
void Widget::on_pushButton_refreshSerial_clicked()
{
    qDebug() << "刷新串口" << endl;
    ui->comboBox_serialPort->clear();
    // 通过QSerialPortInfo查找可用串口
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) // 把后面每个项 依次赋值给 info
    {
        ui->comboBox_serialPort->addItem(info.portName());
    }
    QList<int> baudList = QSerialPortInfo::standardBaudRates();
    for (int i = 0; i < baudList.length(); i++)
    {
        char str[20];
        int length;
        int2Chars(str, baudList.at(i), length);
        // ui->baudSerial->addItem(QString(QByteArray(str,length )));
    }
    ui->checkBox_openSerial->setEnabled(true); // 使能打开串口
}
// 开启按钮 按下
void Widget::on_checkBox_openSerial_toggled(bool checked)
{
    qDebug() << checked << endl;
    if (true == checked) // 判断是否是打开串口
    {
        serial->setPortName(ui->comboBox_serialPort->currentText());
        serial->setBaudRate(QSerialPort::Baud9600);         // 设置波特率
        serial->setDataBits(QSerialPort::Data8);            // 设置数据位数  默认8位
        serial->setParity(QSerialPort::NoParity);           // 设置奇偶校验  默认无奇偶
        serial->setStopBits(QSerialPort::OneStop);          // 设置停止位    默认无停止
        serial->setFlowControl(QSerialPort::NoFlowControl); // 设置流控制    默认无
        if (serial->open(QIODevice::ReadWrite))
        {
            qDebug() << "打开串口成功" << endl;
            // QMessageBox::about(NULL, "提示", "打开串口成功");
            ui->checkBox_openSerial->setText("关闭串口");
        }

        timer_cycleReceive->start(10);
        connect(timer_cycleReceive, SIGNAL(timeout()), this, SLOT(on_readSerial()));

        //关闭下拉框的输入使能
        ui->comboBox_stopBit->setEnabled(false);
        ui->comboBox_baudRate->setEnabled(false);
        ui->comboBox_checkBit->setEnabled(false);
        ui->comboBox_serialPort->setEnabled(false);
        ui->comboBox_flowControl->setEnabled(false);

    }
    else
    {
        serial->close();
        timer_cycleReceive->stop();
        //开启下拉框的输入使能
        ui->comboBox_stopBit->setEnabled(true);
        ui->comboBox_baudRate->setEnabled(true);
        ui->comboBox_checkBit->setEnabled(true);
        ui->comboBox_serialPort->setEnabled(true);
        ui->comboBox_flowControl->setEnabled(true);
    }
}

void Widget::on_readSerial()
{
    static unsigned counter = 0;
    counter++;
    if (counter >= 4000000000)
        counter = 0;
    if (counter % 100 == 0) // 10ms*100 = 1s
        qDebug() << "串口检测" << counter / 100 << "s";
    QByteArray buffer = serial->readAll(); // 串口接受数据
    // buffer = QString::fromLocal8Bit(buffer);//设置成 可以从中文GBK 变成 utf8
    buffer = buffer.toHex(' ').trimmed().toUpper(); /*hex显示*/
    // ui->textBrowser_receiveShow->clear();// 清空显示

    ui->plainTextEdit_receiveDataShow->insertPlainText(buffer);
    //**************************************************************
    //另一种接收方法-类似于中断接收

    //connect(&uSerial,&QSerialPort::readyRead,this,&serial::uSerialReceiveHandle);
}
void Widget::send_data(){
    QString message = ui->lineEdit_sendEdit->text();
    QByteArray messageSend;
    messageSend.append(message);
    serial->write(messageSend);
    //uSerial.write(arg.data(),arg.length());	//串口发送数据

//    connect(serial, &QSerialPort::readyRead, this, [=]()
//        {
//            QByteArray data = serial->readAll();
//            emit haveReceive(data);
//        });

}

//数据转换 字符串转十六进制
void Widget::StringToHex(QString str, QByteArray &senddata)
{
    int hexdata,lowhexdata;
    int hexdatalen = 0;
    int len = str.length();
    senddata.resize(len/2);
    char lstr,hstr;
    for(int i=0; i<len; )
    {
        hstr=str[i].toLatin1();
        if(hstr == ' ')
        {
            i++;
            continue;
        }
        i++;
        if(i >= len)
            break;
        lstr = str[i].toLatin1();
        hexdata = convertHexChart(hstr);
        lowhexdata = convertHexChart(lstr);
        if((hexdata == 16) || (lowhexdata == 16))
            break;
        else
            hexdata = hexdata*16+lowhexdata;
        i++;
        senddata[hexdatalen] = (char)hexdata;
        hexdatalen++;
    }
    senddata.resize(hexdatalen);
}

//数据转换 字符串转16进制数据处理
char Widget::convertHexChart(char ch)
{
    if((ch >= '0') && (ch <= '9'))
        return ch-0x30;  // 0x30 对应 ‘0’
    else if((ch >= 'A') && (ch <= 'F'))
        return ch-'A'+10;
    else if((ch >= 'a') && (ch <= 'f'))
        return ch-'a'+10;
    else
        return ch-ch;//不在0-f范围内的会发送成0
}



