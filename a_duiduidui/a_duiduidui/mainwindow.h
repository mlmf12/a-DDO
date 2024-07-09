#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCharts>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QtCharts/QLineSeries>
#include <QTimer>
#include <QVector>
#include <QThread>
#include <QRadioButton>
#include <cctype>
#include "getdata_fpga.h"
#include "FFT.h"
#include"show_cursor.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    int max_x=200;
    int max_y=1,maxx=0;
    float max_data=0,min_data=0;
    unsigned char isrun=1;
    QChart* chart = new QChart();
    // 创建折线系列对象
    QLineSeries *series = new QLineSeries();
    QLineSeries *series_fft = new QLineSeries();
    QLineSeries *series_x1 = new QLineSeries();
    QLineSeries *series_x2 = new QLineSeries();
    QLineSeries *series_y1 = new QLineSeries();
    QLineSeries *series_y2 = new QLineSeries();
    QValueAxis *axisY,*axisX;

    QTimer *timer = new QTimer(this);
    QTimer *timer1 = new QTimer(this);

    QButtonGroup *voltage;
    QButtonGroup *time;

    show_cursor m;
    unsigned char cursor_run=0;
    float cursor_x1,cursor_x2,cursor_y1=3.0,cursor_y2=-3.0;
    float diff_cursor_x;
    unsigned char lasty_position=100,lastx_position=100;
    int lastvalue=100;

    QLabel * one = new QLabel;
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void zuobiao();
    void showfreq();
    void huizhishuxian_x1(int x);
    void huizhishuxian_x2(int x);
    void huizhishuxian_y1(int y);
    void huizhishuxian_y2(int y);
    void diff();


public slots:
    void handleTimeout();
    void dialChanged(int value);
    void updateData_sigle(int m,int maxy);
    void voltageClicked(int m);
    void timeClicked(int);
    void runorstop();
    void FFT_WAVE();
    void reset();
    void dial5Changed(int value);
    void dial2Changed(int value);
    void dial3Changed(int value);
    void dial4Changed(int value);
    void cursor();
    void save();
    void open();
    void single();
    void updateData_sigle1(int m,int maxy);
private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
