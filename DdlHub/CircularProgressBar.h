#ifndef CIRCULARPROGRESSBAR_H
#define CIRCULARPROGRESSBAR_H

#include <QWidget>

class User; // 前向声明

class CircularProgressBar : public QWidget {
    Q_OBJECT

public:
    explicit CircularProgressBar(QWidget *parent = nullptr);

    void setValue(int value);     // 设置当前值
    void setMaximum(int maximum); // 设置最大值
    void start();
    void pause();
    void reset();
    void setUser(User *user); // 设置用户对象以调用其函数
    void setDurations(int workMinutes, int breakMinutes);
    int m_workDuration = 25 * 60;
    int m_breakDuration = 5 * 60;



private slots:
    void onTimeout();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    int m_value = 0;
    int m_maximum = 25 * 60;
    bool m_isBreak = false; // 是否为休息时间
    bool m_isPause = false;
    QTimer *m_timer;
    User *m_user = nullptr;
};

#endif // CIRCULARPROGRESSBAR_H
