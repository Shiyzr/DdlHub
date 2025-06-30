#include "TodayTaskWindow.h"
#include <QVBoxLayout>
#include <QScrollArea>
#include <QLabel>
#include <QPushButton>
#include <QDateTime>
#include <QMessageBox>
#include <QDebug>

TodayTaskWindow::TodayTaskWindow(User &user, QWidget *parent)
    : QWidget(parent), m_user(user)
{
    setupUI();
    loadTasks();
    completedTasks=false;
}

void TodayTaskWindow::setupUI()
{
    // 全局样式设置（深色背景 + 白色粗体文字）
    this->setStyleSheet(R"(
        QWidget {
            background-color: #1e1e1e;
            color: white;
            font-weight: bold;
        }
        QLabel {
            color: white;
        }
        QScrollArea {
            background-color: #1e1e1e;
        }
        QPushButton {
            color: white;
            font-weight: bold;
        }
    )");

    // 主布局
    m_mainLayout = new QVBoxLayout(this);

    // 标题
    QLabel *titleLabel = new QLabel("今日任务清单", this);
    titleLabel->setStyleSheet("font-size: 18px; margin-bottom: 10px;");
    m_mainLayout->addWidget(titleLabel);

    // 滚动区域
    m_scrollArea = new QScrollArea(this);
    m_scrollArea->setWidgetResizable(true);

    // 任务容器
    m_tasksContainer = new QWidget();
    m_tasksLayout = new QVBoxLayout(m_tasksContainer);
    m_tasksLayout->setAlignment(Qt::AlignTop);
    m_tasksLayout->setSpacing(10);

    m_scrollArea->setWidget(m_tasksContainer);
    m_mainLayout->addWidget(m_scrollArea);
}

void TodayTaskWindow::loadTasks()
{
    QLayoutItem *child;
    while ((child = m_tasksLayout->takeAt(0))) {
        delete child->widget();
        delete child;
    }

    int currentDay = QDate::currentDate().dayOfWeek() - 1;
    if (currentDay < 0) currentDay = 6;

    QDateTime todayEnd = QDateTime(QDate::currentDate(), QTime(23, 59, 59));



    for (const Task &task : m_user.tasks) {
        if (task.isCompleted || task.failed) continue;

        if (!task.cycle.empty()) {
            if (task.cycle.contains(currentDay)) {
                todayTasks.append(task);
                continue;
            }
        }

        int daysRemaining = QDateTime::currentDateTime().daysTo(task.deadline);
        if (daysRemaining < 7) {
            longTermTasks.append(task);
        }
    }

    auto sortTasks = [&](Task &a, Task &b) {
        int aRemaining, bRemaining;

        if (&a >= todayTasks.constBegin() && &a < todayTasks.constEnd()) {
            aRemaining = QDateTime::currentDateTime().secsTo(todayEnd);
        } else {
            aRemaining = QDateTime::currentDateTime().secsTo(a.deadline);
        }

        if (&b >= todayTasks.constBegin() && &b < todayTasks.constEnd()) {
            bRemaining = QDateTime::currentDateTime().secsTo(todayEnd);
        } else {
            bRemaining = QDateTime::currentDateTime().secsTo(b.deadline);
        }

        if (aRemaining != bRemaining) {
            return aRemaining < bRemaining;
        }

        return a.type < b.type;
    };

    std::sort(todayTasks.begin(), todayTasks.end(), sortTasks);
    std::sort(longTermTasks.begin(), longTermTasks.end(), sortTasks);

    if (!todayTasks.isEmpty()) {
        QLabel *todayLabel = new QLabel("今日任务", m_tasksContainer);
        todayLabel->setStyleSheet("font-size: 16px; margin-top: 10px;");
        m_tasksLayout->addWidget(todayLabel);

        for (const Task &task : todayTasks) {
            addTaskWidget(task, true);
        }
    }

    if (!longTermTasks.isEmpty()) {
        QLabel *longTermLabel = new QLabel("即将到期 (7天内)", m_tasksContainer);
        longTermLabel->setStyleSheet("font-size: 16px; margin-top: 10px;");
        m_tasksLayout->addWidget(longTermLabel);

        for (const Task &task : longTermTasks) {
            addTaskWidget(task, false);
        }
    }

    if (todayTasks.isEmpty() && longTermTasks.isEmpty()) {
        QLabel *noTaskLabel = new QLabel("今天没有任务，好好休息吧！", m_tasksContainer);
        noTaskLabel->setStyleSheet("font-size: 14px; color: #bdc3c7;");
        noTaskLabel->setAlignment(Qt::AlignCenter);
        m_tasksLayout->addWidget(noTaskLabel);
    }
}

void TodayTaskWindow::addTaskWidget(const Task &task, bool isTodayTask)
{
    QWidget *taskWidget = new QWidget(m_tasksContainer);
    taskWidget->setStyleSheet("QWidget { background-color: #2c2c2c; border-radius: 4px; padding: 5px; }");

    QHBoxLayout *taskLayout = new QHBoxLayout(taskWidget);
    taskLayout->setContentsMargins(8, 4, 8, 4);
    taskLayout->setSpacing(12);

    QPushButton *completeButton = new QPushButton("完成", taskWidget);
    completeButton->setStyleSheet(
        "QPushButton { background-color: #2ecc71; color: white; border: none; padding: 4px 8px; border-radius: 3px; font-weight: bold; font-size: 12px; }"
        "QPushButton:hover { background-color: #58d68d; }"
        );

    // Store task title instead of ID
    completeButton->setProperty("taskTitle", task.title);
    connect(completeButton, &QPushButton::clicked, this, [this, task]() {
        onTaskCompleted(task.title);
    });

    QLabel *nameLabel = new QLabel(task.title, taskWidget);
    nameLabel->setStyleSheet("font-size: 13px; font-weight: bold; color: white;");
    nameLabel->setFixedWidth(150);

    QString typeStr;
    switch (task.type) {
    case Assignment: typeStr = "任务"; break;
    case Paper: typeStr = "论文"; break;
    case Exam: typeStr = "考试"; break;
    }
    QLabel *typeLabel = new QLabel("类型: " + typeStr, taskWidget);
    typeLabel->setStyleSheet("font-size: 12px; color: #95a5a6;");
    typeLabel->setFixedWidth(80);

    QString timeRemainingStr;
    if (isTodayTask) {
        int secs = QDateTime::currentDateTime().secsTo(QDateTime(QDate::currentDate(), QTime(23, 59, 59)));
        int hours = secs / 3600;
        int mins = (secs % 3600) / 60;
        timeRemainingStr = QString("剩余: %1小时%2分钟").arg(hours).arg(mins);
    } else {
        int days = QDateTime::currentDateTime().daysTo(task.deadline);
        if (days == 0) {
            int secs = QDateTime::currentDateTime().secsTo(task.deadline);
            int hours = secs / 3600;
            int mins = (secs % 3600) / 60;
            timeRemainingStr = QString("剩余: %1小时%2分钟").arg(hours).arg(mins);
        } else {
            timeRemainingStr = QString("剩余: %1天").arg(days);
        }
    }

    QLabel *timeLabel = new QLabel(timeRemainingStr, taskWidget);
    timeLabel->setStyleSheet("font-size: 12px; color: #e74c3c;");
    timeLabel->setMinimumWidth(110);
    timeLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    taskLayout->addWidget(completeButton);
    taskLayout->addWidget(nameLabel);
    taskLayout->addWidget(typeLabel);
    taskLayout->addWidget(timeLabel);
    taskLayout->addStretch();

    m_tasksLayout->addWidget(taskWidget);
}

void TodayTaskWindow::onTaskCompleted(const QString& taskTitle)
{
    // Find task by title
    auto it = std::find_if(m_user.tasks.begin(), m_user.tasks.end(),
                           [taskTitle](const Task& t) { return t.title == taskTitle; });

    if (it != m_user.tasks.end()) {
        Task& task = *it;

        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "确认完成",
                                      QString("确认完成任务「%1」吗？").arg(task.title),
                                      QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::Yes) {
            if (task.cycle.empty()) {
                // Non-recurring task - delete it
                m_user.completetask(it->title);
                m_user.tasks.erase(it);
                completedTasks=true;
            } else {
                // Recurring task - just mark as completed
                m_user.completetask(it->title);
            }

            m_user.taskcompletetime++;
            loadTasks();

            DataManager manager;
            manager.saveUser(m_user);
        }
    }
}
