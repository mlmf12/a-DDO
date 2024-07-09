#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <cstdio> // 包含C标准输入输出头文件
int frequency=10000000;
#define TIMER_TIMEOUT  100
#define TIMER_TIMEOUT1  90
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    timer->start(TIMER_TIMEOUT);
    if(!fpgainit())
    {
        QMessageBox::warning(this, "WARNING", "Unable to connect to FPGA");
    }
    ui->setupUi(this);
    zuobiao();

    connect(timer, SIGNAL(timeout()), this, SLOT(handleTimeout()));
    connect(timer1, &QTimer::timeout, this, [&](){
        ui->pushButton_2->click();
    });
    connect(ui->dial,SIGNAL(sliderMoved(int)),this,SLOT(dialChanged(int)));
    ui->dial->setValue(20);
    ui->dial->setRange(0,40);
    ui->dial->setNotchesVisible(true);
    ui->dial->setSingleStep(1);//tri level

    voltage = new QButtonGroup(this);
    voltage->addButton(ui->radioButton_9,0);
    voltage->addButton(ui->radioButton_10,1);
    voltage->addButton(ui->radioButton_11,2);
    voltage->addButton(ui->radioButton_12,3);

    time = new QButtonGroup(this);
    time->addButton(ui->radioButton,0);
    time->addButton(ui->radioButton_4,1);
    time->addButton(ui->radioButton_2,2);
    ui->radioButton->setChecked(1);
    ui->radioButton_9->setChecked(1);
    ui->lineEdit->setText(tr("Tri %1 V").arg(0));
    connect(voltage, SIGNAL(buttonClicked(int)), this, SLOT(voltageClicked(int)));
    connect(time, SIGNAL(buttonClicked(int)), this, SLOT(timeClicked(int)));

    connect(ui->pushButton_4, SIGNAL(clicked()),this, SLOT(FFT_WAVE()));

    connect(ui->pushButton_2, SIGNAL(clicked()),this, SLOT(reset()));
    ui->dial_3->setValue(100);
    ui->dial_3->setRange(0,200);
    ui->dial_3->setNotchesVisible(true);
    ui->dial_3->setSingleStep(20);
    ui->dial_2->setValue(100);
    ui->dial_2->setRange(0,200);
    ui->dial_2->setNotchesVisible(true);
    ui->dial_2->setSingleStep(20);
    ui->dial_5->setValue(100);
    ui->dial_5->setRange(0,200);
    ui->dial_5->setNotchesVisible(true);
    ui->dial_5->setSingleStep(20);
    ui->dial_4->setValue(100);
    ui->dial_4->setRange(0,200);
    ui->dial_4->setNotchesVisible(true);
    ui->dial_4->setSingleStep(20);
    connect(ui->dial_3,SIGNAL(sliderMoved(int)),this,SLOT(dial5Changed(int)));
    connect(ui->dial_5,SIGNAL(sliderMoved(int)),this,SLOT(dial2Changed(int)));
    connect(ui->dial_4,SIGNAL(sliderMoved(int)),this,SLOT(dial3Changed(int)));
    connect(ui->dial_2,SIGNAL(sliderMoved(int)),this,SLOT(dial4Changed(int)));
    connect(ui->pushButton_7, SIGNAL(clicked()),this, SLOT(cursor()));
    connect(ui->pushButton_6, SIGNAL(clicked()),this, SLOT(single()));
    connect(ui->pushButton_3, SIGNAL(clicked()),this, SLOT(save()));
    connect(ui->pushButton_5, SIGNAL(clicked()),this, SLOT(open()));
    connect(ui->pushButton, SIGNAL(clicked()),this, SLOT(runorstop()));
    showfreq();
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::zuobiao()
{
        QFont labelsFont;
        labelsFont.setPixelSize(15);   //参数字号，数字越小，字就越小
        axisX = new QValueAxis();//X轴
        axisX->setRange(0,max_x);            //设置X轴范围
        axisX->setTitleText("时间t");      //设置X轴标题
        axisX->setLabelsFont(labelsFont);
        axisY = new QValueAxis();//定义Y轴
        axisY->setRange(-4,4);           //设置Y轴范围
        axisY->setTitleText("电压/V");      //设置Y轴标题
        axisY->setTickCount(9);
        axisY->setMinorTickCount(10);
        axisX->setTickCount(11);
        axisY->setLabelsFont(labelsFont);
        chart->addAxis(axisX,Qt::AlignBottom);//把横轴添加到图表
        chart->addAxis(axisY,Qt::AlignLeft);//把纵轴添加到图表

        series_fft->attachAxis(axisX);
        series_fft->attachAxis(axisY);
        for (double i = 0; i < max_x; i+=1)
        {
            series->append(i, 10*sin(i*3.14*4/180));
        }

        // 设置折线的标题
        series->setName("aduiduidui-loongson");
        // 折线系列添加到图表,//openGl 加速
        series->setUseOpenGL(true);
        chart->addSeries(series);
        ui->widget->setChart(chart);

}

void MainWindow::dialChanged(int value)
{
ui->lineEdit->setText(tr("Tri %1 V").arg(float(value) / 5 - 4));
}
void MainWindow::updateData_sigle(int m,int maxy)
{
    int i;
    float data_fpga[200]={0};
    float data_fpga1[500]={0};
    QList<QPointF> data;
    int max=200,das=500;
    if(m<8)
    {
    getdata_fpga(data_fpga,&frequency,m,ui->dial->value());
    axisX->setRange(0,max_x);
    series->attachAxis(axisX);
    for(i = 0; i < max; ++i)
     {
        if(data_fpga[i]>max_data)
            max_data=data_fpga[i];
        if(data_fpga[i]<min_data)
            min_data=data_fpga[i];
        data.append(QPointF(i ,data_fpga[i]*2.5*maxy));
     }
     series->replace(data);
    }
    else
    {
    getdata_fpga(data_fpga1,&frequency,m,ui->dial->value());
    axisX->setRange(0,500);
    series->attachAxis(axisX);
    for(i = 0; i < das; ++i)
     {
        if(data_fpga1[i]>max_data)
            max_data=data_fpga1[i];
        if(data_fpga1[i]<min_data)
            min_data=data_fpga1[i];
        data.append(QPointF(i ,data_fpga1[i]*2.5*maxy));
     }
     series->replace(data);
    }
     showfreq();
     max_data=0;
     min_data=0;

}

void MainWindow::handleTimeout()
{

    if(isrun)
    {
        if(maxx==0){
                timer1->start(TIMER_TIMEOUT1);
                updateData_sigle(maxx,max_y);
            }
        else{
            timer1->stop();
            updateData_sigle(maxx,max_y);
        }
    }
    if(frequency<102)
    {
        ui->radioButton->setText("20ms/div");
        ui->radioButton_4->setText("10ms/div");
        ui->radioButton_2->setText("1ms/div");
        diff_cursor_x=0;
        ui->radioButton_2->setStyleSheet("");
        ui->radioButton->setStyleSheet("background-color: rgb(138, 226, 52);");
    }
    else if(frequency<100000)
    {
        ui->radioButton->setText("20us/div");
        ui->radioButton_4->setText("40us/div");
        ui->radioButton_2->setText("160us/div");
        diff_cursor_x=2000;
        ui->radioButton->setStyleSheet("");
        ui->radioButton_2->setStyleSheet("");

    }
    else
    {
        ui->radioButton->setText("100ns/div");
        ui->radioButton_4->setText("200ns/div");
        ui->radioButton_2->setText("2us/div");
        diff_cursor_x=4000;
        ui->radioButton->setStyleSheet("background-color: rgb(138, 226, 52);");
        ui->radioButton_2->setStyleSheet("background-color: rgb(138, 226, 52);");
    }
}

void MainWindow::voltageClicked(int m)
{
    switch(m)
    {
        case 0:max_y=1;axisY->setRange(-4,4);break; //1v
        case 1:max_y=10;axisY->setRange(-0.4,0.4);break;  //0.1v
        case 2:max_y=500;axisY->setRange(-0.008,0.008);break;   //0.002v
        case 3:max_y=2;axisY->setRange(-2,2);break;
    default:break;
    }
}
void MainWindow::timeClicked(int m)
{
    if(frequency<100)
    {
        switch(m)
        {
            case 0:maxx=0;break;
            case 1:maxx=1;break;
            case 2:maxx=2;break;
        }
    }
    else if(frequency<100000)
    {
        switch(m)
        {
            case 0:maxx=3;break;
            case 1:maxx=4;break;
            case 2:maxx=5;break;
        }
    }
    else
    {
        switch(m)
        {
            case 0:maxx=6;break;
            case 1:maxx=7;break;
            case 2:maxx=8;break;
        }
    }
}
void MainWindow::runorstop()
{
    if(isrun)
    {
       isrun=0;
    }
    else
       isrun=1;
}
void MainWindow::showfreq()
{
    if(frequency > 1000000)
    {
         ui->lineEdit_4->setText(tr("%1").arg((float)frequency/1000000)+"  MHZ");
    }
    else if (frequency > 1000)
    {
         ui->lineEdit_4->setText(tr("%1").arg((float)frequency/1000)+"  KHZ");
    }
    else
    {
         ui->lineEdit_4->setText(tr("%1").arg((float)frequency)+"  HZ");
    }
    ui->lineEdit_3->setText(tr("%1").arg(max_data-min_data)+"  V");
    ui->lineEdit_2->setText(tr("%1").arg(max_data)+"  V");
    ui->lineEdit_5->setText(tr("%1").arg(min_data)+"  V");

}
void MainWindow::FFT_WAVE()
{
    series_fft->setName("FFT_WAVE");
    float FFT_real[256] = {0};
    float FFT_image[256] = {0};
    float amp[256] = {0};
    float real_freq[256] = {0};
    QList<QPointF> oldData = series->points();
    QList<QPointF> Data;

    // 使用零填充将数据扩展到256个点
    for(int i = 0; i < 256; i++)
    {
        if(i < oldData.size())
        {
            FFT_real[i] = oldData.at(i).y();
        }
        else
        {
            FFT_real[i] = 0; // 零填充
        }
    }

    FFT(FFT_real, FFT_image, 256);

    for (int i = 0; i < 256; i++)
    {
        amp[i] = 2 * (sqrt((FFT_real[i] * FFT_real[i] + FFT_image[i] * FFT_image[i])) / 256);
        real_freq[i] = 200 * i / 256;
        Data.append(QPointF(real_freq[i], amp[i]));
    }

    series_fft->replace(Data);
    chart->addSeries(series_fft);
}

void MainWindow::reset()
{
    chart->zoomReset();
    ui->dial_3->setValue(100);
    ui->dial_2->setValue(100);
    ui->dial_5->setValue(100);
    ui->dial_4->setValue(100);
    chart->removeSeries(series_fft);
    one->close();
}

void MainWindow::dial5Changed(int value)
{
    if(cursor_run)
    {
        huizhishuxian_x1(value);
    }
    else
    {
        if(value>lastvalue)
            chart->zoom(1.12);
        else if(value<lastvalue)
            chart->zoom(0.9);
        lastvalue=value;
    }
}
void MainWindow::dial2Changed(int value)
{
    if(cursor_run)
    {
        huizhishuxian_y2(value);
    }
    else
    {
      if(value>lastx_position)
            chart->scroll(-3,0);
      else
            chart->scroll(3,0);
      lastx_position=value;
    }
}
void MainWindow::dial3Changed(int value)
{
    if(cursor_run)
    {
        huizhishuxian_y1(value);
    }
    else
    {
        if(value>lasty_position)
              chart->scroll(0,-3);
        else
              chart->scroll(0,3);
        lasty_position=value;
    }
}
//波形的扩张显示
void MainWindow::dial4Changed(int value)
{
    if(cursor_run)
    {
        huizhishuxian_x2(value);
    }
    else
    {
        QList<QPointF> oldData = series->points();
        QList<QPointF> data;
        for(int i = 0; i < oldData.size(); ++i){
            data.append(QPointF(i*value/50,oldData.at(i).y()));
        series->replace(data);
        }
    }
}

void MainWindow::cursor()
{
    if(cursor_run==0)
    {
        m.show();
        chart->addSeries(series_x1);
        series_x1->attachAxis(axisX);
        m.settext_1(90.0);
        series_x1->append(90,5);
        series_x1->append(90,-5);
        chart->addSeries(series_x2);
        series_x2->attachAxis(axisX);
        m.settext_2(110.0);
        series_x2->append(110,5);
        series_x2->append(110,-5);
        chart->addSeries(series_y1);
        series_y1->attachAxis(axisX);
        series_y1->attachAxis(axisY);
        m.settext_3(0.02);
        series_y1->append(1,0.02);
        if(maxx!=8)
        {
            series_y1->append(200,0.02);
        }
        else {
            series_y1->append(500,0.02);
        }
        chart->addSeries(series_y2);
        series_y2->attachAxis(axisX);
        series_y2->attachAxis(axisY);
        series_y2->setColor(QColor(Qt::red));
        m.settext_4(-0.02);
        series_y2->append(1,-0.02);
        if(maxx!=8)
        {
            series_y2->append(200,-0.02);
        }
        else {
            series_y2->append(500,-0.02);
        }
        cursor_run=1;
        m.settext_5(-20.0,0);
        m.settext_6(0.04);
    }
    else
    {
        m.close();
        chart->removeSeries(series_x1);
        series_x1->clear();
        chart->removeSeries(series_x2);
        series_x2->clear();
        chart->removeSeries(series_y1);
        series_y1->clear();
        chart->removeSeries(series_y2);
        series_y2->clear();
        cursor_run=0;
    }
}

void MainWindow::huizhishuxian_x1(int x)
{
    QList<QPointF> Data;
    if(maxx!=8)
    {
        Data.append(QPointF(x,5));
        Data.append(QPointF(x,-5));
    }
    else {
        Data.append(QPointF(x*2.5,5));
        Data.append(QPointF(x*2.5,-5));
    }
    series_x1->replace(Data);
    cursor_x1=(float)x;
    switch(maxx)
    {
        case 0:cursor_x1=cursor_x1+diff_cursor_x;break;
        case 1:cursor_x1=cursor_x1/2+diff_cursor_x;break;
        case 2:cursor_x1=cursor_x1/20+diff_cursor_x;break;
        case 3:cursor_x1=cursor_x1+diff_cursor_x;break;
        case 4:cursor_x1=cursor_x1*2+diff_cursor_x;break;
        case 5:cursor_x1=cursor_x1*8+diff_cursor_x;break;
        case 6:cursor_x1=cursor_x1*5+diff_cursor_x;break;
        case 7:cursor_x1=cursor_x1*10+diff_cursor_x;break;
        case 8:cursor_x1=cursor_x1*100+diff_cursor_x;break;
    default:break;
    }
    m.settext_1(cursor_x1);
    diff();
}
void MainWindow::huizhishuxian_x2(int x)
{
    QList<QPointF> Data;
    if(maxx!=8)
    {
        Data.append(QPointF(x,5));
        Data.append(QPointF(x,-5));
    }
    else {
        Data.append(QPointF(x*2.5,5));
        Data.append(QPointF(x*2.5,-5));
    }
    series_x2->replace(Data);
    cursor_x2=(float)x;
    switch(maxx)
    {   case 0:cursor_x2=cursor_x2+diff_cursor_x;break;
        case 1:cursor_x2=cursor_x2/2+diff_cursor_x;break;
        case 2:cursor_x2=cursor_x2/20+diff_cursor_x;break;
        case 3:cursor_x2=cursor_x2+diff_cursor_x;break;
        case 4:cursor_x2=cursor_x2*2+diff_cursor_x;break;
        case 5:cursor_x2=cursor_x2*8+diff_cursor_x;break;
        case 6:cursor_x2=cursor_x2*5+diff_cursor_x;break;
        case 7:cursor_x2=cursor_x2*10+diff_cursor_x;break;
        case 8:cursor_x2=cursor_x2*100+diff_cursor_x;break;
    default:break;
    }
    m.settext_2(cursor_x2);
    diff();
}
void MainWindow::huizhishuxian_y1(int y)
{
    QList<QPointF> Data;
    float y1=0;
    if(abs(max_y-1)<0.000001)
    {
       y1=(float)(y-100)/25;
    }
    else if(abs(max_y-10)<0.000001)
    {

        y1=(float)(y-100)/250;
    }
    else if(abs(max_y-500)<0.000001)
    {
        y1=(float)(y-100)/12500;
    }
    else{
        y1=(float)(y-100)/50;
    }
    Data.append(QPointF(1,y1));
    cursor_y1=y1;
    m.settext_3(cursor_y1);
    if(maxx!=8)
    {
        Data.append(QPointF(200,y1));
    }
    else {
        Data.append(QPointF(500,y1));
    }
    series_y1->replace(Data);
    diff();
}
void MainWindow::huizhishuxian_y2(int y)
{
    QList<QPointF> Data;
    float y2=0;
    if(abs(max_y-1)<0.000001)
    {
       y2=(float)(y-100)/25;
    }
    else if(abs(max_y-10)<0.000001)
    {
        y2=(float)(y-100)/250;
    }
    else if(abs(max_y-500)<0.000001)
    {
        y2=(float)(y-100)/12500;
    }
    else{
        y2=(float)(y-100)/50;
    }
    cursor_y2=y2;
    Data.append(QPointF(1,y2));
    m.settext_4(cursor_y2);
    if(maxx!=8)
    {
        Data.append(QPointF(200,y2));
    }
    else {
        Data.append(QPointF(500,y2));
    }
    series_y2->replace(Data);
    diff();
}

void MainWindow::diff()
{
    m.settext_5(cursor_x1-cursor_x2,diff_cursor_x);
    m.settext_6(cursor_y1-cursor_y2);
}
void MainWindow::save()
{
    QString SaveFile;
    QScreen * screen = QGuiApplication::primaryScreen();
    QPixmap p = screen->grabWindow(ui->widget->winId());
    QImage image = p.toImage();
    //image.save("/home/linaro/Desktop/chart.png");
    SaveFile =QFileDialog::getSaveFileName(this,tr("保存文件"),"wave.jpg");
    if (SaveFile != "")
    {
        image.save(SaveFile);
    }
    else
    {
         QMessageBox::warning(this, "WARNING", "Incorrect file name");
    }
}

void MainWindow::open()
{

one->resize(780,540);
QString OpenFile;
QImage image;
QColor oldColor;
OpenFile = QFileDialog::getOpenFileName(this,
            "please choose an image file",
            "",
            "Image Files(*.jpg *.png *.bmp *.pgm *.pbm);;All(*.*)");
if (OpenFile != "")
        {
            if (image.load(OpenFile))
            {
                one->setPixmap(QPixmap::fromImage(image).scaled(one->size()));
                one->show();
            }
        }
else
    {
            QMessageBox::warning(this, "WARNING", "Incorrect file name");
    }

}
void MainWindow:: single()
{
    updateData_sigle1(maxx,max_y);
    isrun=0;
}
void MainWindow::updateData_sigle1(int m,int maxy)
{
    int i;
    float data_fpga[200]={0};
    float data_fpga1[500]={0};
    QList<QPointF> data;
    int max=200,das=500;
    if(m<8)
    {
    getdata_fpga_single(data_fpga,&frequency,m,ui->dial->value());
    axisX->setRange(0,max_x);
    series->attachAxis(axisX);
    for(i = 0; i < max; ++i)
     {
        if(data_fpga[i]>max_data)
            max_data=data_fpga[i];
        if(data_fpga[i]<min_data)
            min_data=data_fpga[i];
        data.append(QPointF(i ,data_fpga[i]*2.5*maxy));
     }
     series->replace(data);
    }
    else
    {
    getdata_fpga_single(data_fpga1,&frequency,m,ui->dial->value());
    axisX->setRange(0,500);
    series->attachAxis(axisX);
    for(i = 0; i < das; ++i)
     {
        if(data_fpga1[i]>max_data)
            max_data=data_fpga1[i];
        if(data_fpga1[i]<min_data)
            min_data=data_fpga1[i];
        data.append(QPointF(i ,data_fpga1[i]*2.5*maxy));

     }
     series->replace(data);
    }
     showfreq();
     max_data=0;
     min_data=0;
}
