#ifndef SHOW_CURSOR_H
#define SHOW_CURSOR_H

#include <QWidget>

namespace Ui {
class show_cursor;
}

class show_cursor : public QWidget
{
    Q_OBJECT

public:
    explicit show_cursor(QWidget *parent = nullptr);
    ~show_cursor();
    Ui::show_cursor *ui2;
    void settext_1(float value);
    void settext_2(float value);
    void settext_3(float value);
    void settext_4(float value);
    void settext_5(float value,int a);
    void settext_6(float value);

};

#endif // SHOW_CURSOR_H
