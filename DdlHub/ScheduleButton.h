#ifndef SCHEDULEBUTTON_H
#define SCHEDULEBUTTON_H

#include <QPushButton>

class ScheduleButton : public QPushButton {
    Q_OBJECT

public:
    explicit ScheduleButton(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
};

#endif // SCHEDULEBUTTON_H
