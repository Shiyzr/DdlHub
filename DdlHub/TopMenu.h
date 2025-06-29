#ifndef TOPMENU_H
#define TOPMENU_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QPushButton>

class TopMenu : public QWidget
{
    Q_OBJECT
public:
    explicit TopMenu(QWidget *parent = nullptr);

    QHBoxLayout *layout;
    QPushButton *profileButton;//右上角github
    QPushButton *scheduleBtn;
    QPushButton *customBtn;

protected:
    void resizeEvent(QResizeEvent *event) override;
};

#endif // TOPMENU_H
