// #ifndef TODAYTASKWINDOW_H
// #define TODAYTASKWINDOW_H

// #include <QWidget>
// #include <QList>
// #include "ItemWidget.h" // 包含你的User类

// class QVBoxLayout;
// class QScrollArea;
// class QPushButton;

// class TodayTaskWindow : public QWidget
// {
//     Q_OBJECT

// public:
//     explicit TodayTaskWindow(User &user, QWidget *parent = nullptr);

// private:
//     void setupUI();
//     void loadTasks();
//     void addTaskWidget(const Task &task, bool isTodayTask);

//     User &m_user;
//     QVBoxLayout *m_mainLayout;
//     QScrollArea *m_scrollArea;
//     QWidget *m_tasksContainer;
//     QVBoxLayout *m_tasksLayout;

// private slots:
//     void onTaskCompleted(QString taskIndex);
// };

// #endif // TODAYTASKWINDOW_H



#ifndef TODAYTASKWINDOW_H
#define TODAYTASKWINDOW_H

#include <QWidget>
#include <QList>
#include "ItemWidget.h"  // Assuming User.h contains Task definition

class QVBoxLayout;
class QScrollArea;
class QPushButton;
class QLabel;

class TodayTaskWindow : public QWidget
{
    Q_OBJECT

public:
    explicit TodayTaskWindow(User &user, QWidget *parent = nullptr);
    void loadTasks();
    QList<Task> todayTasks;
    QList<Task> longTermTasks;
    void setupUI();
    bool completedTasks=false;

private:
    void addTaskWidget(const Task &task, bool isTodayTask);

    User &m_user;
    QVBoxLayout *m_mainLayout;
    QScrollArea *m_scrollArea;
    QWidget *m_tasksContainer;
    QVBoxLayout *m_tasksLayout;

private slots:
    void onTaskCompleted(const QString &taskTitle);  // Changed parameter type
};

#endif // TODAYTASKWINDOW_H
