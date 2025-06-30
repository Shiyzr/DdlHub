#include "models.h"

// Course Implementation
Course::Course(QString name, QTime st, QTime en, QString dat, QString lo)
    : name(name), starttime(st), endtime(en), date(dat), location(lo) {}

bool Course::operator==(const Course &c) const {
    return name == c.name && starttime == c.starttime && endtime == c.endtime && date == c.date;
}

// Task Implementation
Task::Task() : deadline(QDateTime::currentDateTime()), estimatedTimeMinutes(0), type(Assignment), cycle({0}), daystocomplete(1) {}

Task::Task(QString t, QDateTime d, int time)
    : title(t), deadline(d), estimatedTimeMinutes(time), type(Assignment), cycle({0}), daystocomplete(1), priority(5) {}

Task::Task(QString t, QDateTime d, int time, TaskType tp, QList<int> ct, int dtc, int p)
    : title(t), deadline(d), estimatedTimeMinutes(time), type(tp), cycle(ct), daystocomplete(dtc), priority(qBound(1, p, 10)) {}

bool Task::operator==(const Task &t) const {
    return title == t.title;
}

int Task::hoursRemaining() const {
    int secs = QDateTime::currentDateTime().secsTo(deadline);
    return (secs > 0) ? secs / 3600 : -1; // Handle overdue tasks
}

void Task::reducedaystocomplete() {
    if (daystocomplete > 0) {
        daystocomplete--;
        if (daystocomplete == 0) {
            isCompleted = true;
        }
    }
}

// Achievement Implementation
Achievement::Achievement(QString t) : title(t) {}

Achievement::Achievement(QString t, QDateTime ti, QString d)
    : title(t), unlocktime(ti), description(d) {}

bool Achievement::operator==(const Achievement &other) const {
    return title == other.title;
}

// User Implementation
User::User(QObject *parent)
    : QObject(parent), lastlogindate(QDate::currentDate()), tomatoclocktime(0), taskcompletetime(0) {}

User::User(QString name, QString word, QObject *parent)
    : QObject(parent), username(name), password(word), lastlogindate(QDate::currentDate()), tomatoclocktime(0), taskcompletetime(0) {}

void User::copyFrom(const User &other) {
    username = other.username;
    password = other.password;
    lastlogindate = other.lastlogindate;
    courses = other.courses;
    tasks = other.tasks;
    achievements = other.achievements;
    tomatoclocktime = other.tomatoclocktime;
    taskcompletetime = other.taskcompletetime;
}

bool User::verify(QString name, QString word) {
    return name == username && word == password;
}

bool User::is0To4() {
    QTime currentTime = QDateTime::currentDateTime().time();
    QTime start(0, 0);
    QTime end(4, 0);
    return (currentTime >= start && currentTime <= end);
}

bool User::addcourse(Course c) {
    bool canadd = true;
    if (c.endtime <= c.starttime)
        canadd = false;
    for (const auto &course : courses) {
        if (course.date == c.date) {
            if (c.starttime < course.endtime && c.endtime > course.starttime) {
                canadd = false;
                break;
            }
        }
    }
    if (canadd) {
        courses.push_back(c);
        return true;
    } else {
        QMessageBox::warning(nullptr, "错误", "时间冲突，无法添加该课程！");
        return false;
    }
}

bool User::removecourse(Course c) {
    bool successfully_removed = false;
    for (auto i = courses.begin(); i != courses.end();) {
        if (*i == c) {
            i = courses.erase(i);
            successfully_removed = true;
        } else {
            ++i;
        }
    }
    if (!successfully_removed)
        QMessageBox::warning(nullptr, "错误", "找不到该课程！");
    return successfully_removed;
}

bool User::removecoursebyname(QString n) {
    bool successfully_removed = false;
    for (auto i = courses.begin(); i != courses.end();) {
        if (i->name == n) {
            i = courses.erase(i);
            successfully_removed = true;
        } else {
            ++i;
        }
    }
    if (!successfully_removed)
        QMessageBox::warning(nullptr, "错误", "找不到该课程！");
    return successfully_removed;
}

bool User::removecoursebytime(QTime st, QTime en, QString date) {
    bool successfully_removed = false;
    for (auto i = courses.begin(); i != courses.end();) {
        if (i->starttime == st && i->endtime == en && i->date == date) {
            i = courses.erase(i);
            successfully_removed = true;
        } else {
            ++i;
        }
    }
    if (!successfully_removed)
        QMessageBox::warning(nullptr, "错误", "找不到该课程！");
    return successfully_removed;
}

bool User::addtask(Task t) {
    bool canadd = true;
    for (const auto &task : tasks) {
        if (task.title == t.title)
            canadd = false;
    }
    if (canadd) {
        tasks.push_back(t);
        if (!achievements.contains(Achievement("初出茅庐"))) {
            Achievement acv("初出茅庐", QDateTime::currentDateTime(), "添加第一个DDL任务");
            addAchievement(acv);
        }
        if (t.hoursRemaining() >= 2160 && !achievements.contains(Achievement("长期规划师"))) {
            Achievement acv("长期规划师", QDateTime::currentDateTime(), "添加一个截止日期在3个月后的任务");
            addAchievement(acv);
        }
    } else {
        QMessageBox::warning(nullptr, "错误", "已有此任务！请调整任务名称。");
    }
    return canadd;
}

bool User::removetask(QString t) {
    bool successfully_removed = false;
    for (auto i = tasks.begin(); i != tasks.end();) {
        if (i->title == t) {
            i = tasks.erase(i);
            successfully_removed = true;
        } else {
            ++i;
        }
    }
    if (!successfully_removed)
        QMessageBox::warning(nullptr, "错误", "找不到该任务！");
    return successfully_removed;
}

bool User::completetask(QString t) {
    bool cando = false;
    for (auto i = tasks.begin(); i != tasks.end(); i++) {
        if (i->title == t && !i->failed) {
            cando = true;
            i->isCompleted = true;
            taskcompletetime++;

            if (taskcompletetime == 10 && !achievements.contains(Achievement("小试牛刀"))) {
                Achievement acv("小试牛刀", QDateTime::currentDateTime(), "累计完成10个任务");
                addAchievement(acv);
            }
            if (taskcompletetime == 100 && !achievements.contains(Achievement("百战百胜"))) {
                Achievement acv("百战百胜", QDateTime::currentDateTime(), "累计完成100个任务");
                addAchievement(acv);
            }
            if (i->hoursRemaining() >= 0 && i->hoursRemaining() <= 2 && !achievements.contains(Achievement("零点冲刺王"))) {
                Achievement acv1("零点冲刺王", QDateTime::currentDateTime(), "任务在截止当天的最后2小时内完成。 你的速度就是你的武器，冲刺时刻从不掉链子！");
                addAchievement(acv1);
            } else if (i->hoursRemaining() >= 72 && !achievements.contains(Achievement("早鸟先锋"))) {
                Achievement acv2("早鸟先锋", QDateTime::currentDateTime(), "任务至少提前3天完成。 早起的鸟儿有虫吃，提前完成让你领先一步！");
                addAchievement(acv2);
            }
            if (is0To4() && !achievements.contains(Achievement("夜猫子"))) {
                Achievement acv("夜猫子", QDateTime::currentDateTime(), "在午夜至凌晨4点间完成任务");
                addAchievement(acv);
            }
        }
    }
    if (!cando)
        QMessageBox::warning(nullptr, "错误", "找不到该任务！");
    return cando;
}

void User::checktask() {
    for (auto i = tasks.begin(); i != tasks.end(); i++) {
        if (QDateTime::currentDateTime() > i->deadline && !i->isCompleted)
            i->failed = true;
    }
}

void User::addAchievement(const Achievement &a) {
    if (!achievements.contains(a)) {
        if (a.description.isEmpty()) {
            QMessageBox::warning(nullptr, "添加成就失败", "成就描述不能为空！");
            return;
        }
        achievements.append(a);
        emit achievementUnlocked(a);
    }
}

void User::tomatoclockfinished() {
    tomatoclocktime++;
    if (tomatoclocktime == 50) {
        Achievement acv3("专注达人", QDateTime::currentDateTime(), "累计完成50个番茄钟专注时长。 25分钟的专注，打造出无坚不摧的意志力！");
        addAchievement(acv3);
    }
}
