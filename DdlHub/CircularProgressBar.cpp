#include "CircularProgressBar.h"
#include "DeadlineWidget.h"
#include <QPainter>
#include <QPen>
#include <QtMath>
#include <QTimer>

CircularProgressBar::CircularProgressBar(QWidget *parent)
    : QWidget(parent)
{
    // setMinimumSize(300, 300);
    setFixedSize(400, 400);
    //setMaximum(25 * 60);

    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &CircularProgressBar::onTimeout);
}

void CircularProgressBar::setUser(User *user) {
    m_user = user;
}

void CircularProgressBar::setValue(int value) {
    if (value != m_value) {
        m_value = value;
        update(); // 触发重绘
    }
}

void CircularProgressBar::setMaximum(int maximum) {
    if (maximum > 0) {
        m_maximum = maximum;
        update();
    }
}

void CircularProgressBar::setDurations(int workMinutes, int breakMinutes) {
    if (workMinutes > 0 && breakMinutes > 0) {
        m_workDuration = workMinutes * 60;
        m_breakDuration = breakMinutes * 60;

        // 设置当前阶段的最大时间（不重置整个状态）
        m_maximum = m_isBreak ? m_breakDuration : m_workDuration;
        update();
    }
}



void CircularProgressBar::start() {
    m_isPause = false;
    update();
    if (!m_timer->isActive())
        m_timer->start(1000); // 每秒更新一次
}

void CircularProgressBar::pause() {
    m_timer->stop();
    m_isPause = true;

    update();
}

void CircularProgressBar::reset() {
    m_timer->stop();
    m_isBreak = false;
    m_isPause = false;
    m_value = 0;
    m_maximum = m_workDuration; // 不再写死为 25*60
    update();
}


void CircularProgressBar::onTimeout() {
    if (++m_value > m_maximum) {
        if (!m_isBreak && m_user) {
            m_user->tomatoclockfinished(); // 通知用户完成工作阶段
        }

        m_isBreak = !m_isBreak;
        m_maximum = m_isBreak ? m_breakDuration : m_workDuration;
        m_value = 0;
    }
    update();
}


void CircularProgressBar::paintEvent(QPaintEvent *) {
    QColor workColor(243, 75, 125);
    QColor restColor(68, 165, 28);
    QColor pauseColor(241, 224, 90);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // 计算尺寸
    int width = this->width();
    int height = this->height();
    int side = qMin(width, height);

    QRectF rect((width - side) / 2 + 10, (height - side) / 2 + 10, side - 20, side - 20);

    // 画底部圆环
    QPen basePen(Qt::gray, 20);
    painter.setPen(basePen);
    painter.drawArc(rect, 0, 360 * 16);

    // 画进度圆环
    QPen progressPen;
    progressPen.setWidth(20);
    if (m_isPause) {
        progressPen.setColor(pauseColor);

    }
    else {
        progressPen.setColor(m_isBreak ? restColor : workColor);

    }
    // QPen progressPen(Qt::red, 10);
    painter.setPen(progressPen);

    double angle = (double)m_value / m_maximum * 360.0;
    painter.drawArc(rect, 90 * 16, -angle * 16); // 顺时针

    // 画中间文本
    // painter.setPen(Qt::white);
    if (m_isPause) {
        painter.setPen(pauseColor);
    }
    else {
        painter.setPen(m_isBreak ? restColor : workColor);
    }
    QFont font("Consolas", 45);
    // font.setPointSize(60);
    painter.setFont(font);

    int remainingSeconds = m_maximum - m_value;
    int minutes = remainingSeconds / 60;
    int seconds = remainingSeconds % 60;
    QString text = QString("%1:%2")
                       .arg(minutes, 2, 10, QChar('0'))
                       .arg(seconds, 2, 10, QChar('0')); // 补零显示为 2 位数

    //QString text = QString("%1%").arg((int)((double)m_value / m_maximum * 100));
    //painter.drawText(rect, Qt::AlignCenter, text);
    painter.drawText(rect, Qt::AlignCenter, text);
}
