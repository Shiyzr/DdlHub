#include "DataManager.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>

bool DataManager::saveUser(const User &user) {
    QFile file("user_" + user.username + ".txt");
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "无法打开用户文件";
        return false;
    }

    QTextStream out(&file);
    out << "USERINFO|" << user.username << "|" << user.password << "\n";
    out << "\n[COURSES]\n";
    for (const Course &c : user.courses) {
        out << c.name << "|"
            << c.starttime.toString("HH:mm") << "|"
            << c.endtime.toString("HH:mm") << "|"
            << c.date << "|"
            << c.location << "\n";
    }
    out << "\n[TASKS]\n";
    for (const Task &t : user.tasks) {
        out << t.title << "|"
            << t.deadline.toString("yyyy-MM-dd HH:mm") << "|"
            << t.estimatedTimeMinutes << "|"
            << t.isCompleted << "|"
            << t.failed << "|"
            << t.type << "|"<<"0";
        for (auto i=t.cycle.begin();i!=t.cycle.end();i++)
            out<< *i ;
        out <<"|"<< t.daystocomplete << "|"
            << t.priority << "\n";
    }
    out << "\n[ACHIEVEMENTS]\n";
    for (const Achievement &a : user.achievements) {
        out << a.title << "|"
            << a.unlocktime.toString("yyyy-MM-dd HH:mm") << "|"
            << a.description << "\n";
    }
    out << "\n[TOMATOCLOCK]\n";
    out << user.tomatoclocktime << "\n";
    out << "\n[TASKCOMPLETE]\n";
    out << user.taskcompletetime << "\n";
    out << "\n[LASTLOGIN]\n";
    out << QDate::currentDate().toString("yyyy-MM-dd") << "\n";

    file.close();
    return true;
}

bool DataManager::loadUser(QString &username, User &userOut) {
    QFile file("user_" + username + ".txt");
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "用户文件不存在";
        return false;
    }

    QString currentSection;
    QTextStream in(&file);
    QString header = in.readLine();
    QStringList headerParts = header.split("|");
    if (headerParts.size() >= 3 && headerParts[0] == "USERINFO") {
        userOut.username = headerParts[1];
        userOut.password = headerParts[2];
    } else {
        qWarning() << "文件格式错误";
        return false;
    }

    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.startsWith("[COURSES]")) {
            currentSection = "COURSES";
        } else if (line.startsWith("[TASKS]")) {
            currentSection = "TASKS";
        } else if (line.startsWith("[ACHIEVEMENTS]")) {
            currentSection = "ACHIEVEMENTS";
        } else if (line.startsWith("[TOMATOCLOCK]")) {
            currentSection = "TOMATOCLOCK";
        } else if (line.startsWith("[TASKCOMPLETE]")) {
            currentSection = "TASKCOMPLETE";
        } else if (line.startsWith("[LASTLOGIN]")) {
            currentSection = "LASTLOGIN";
        } else if (!line.isEmpty()) {
            if (currentSection == "COURSES") {
                QStringList parts = line.split("|");
                if (parts.size() == 5) {
                    userOut.courses.append(Course(
                        parts[0],
                        QTime::fromString(parts[1], "HH:mm"),
                        QTime::fromString(parts[2], "HH:mm"),
                        parts[3],
                        parts[4]
                        ));
                }
            } else if (currentSection == "TASKS") {
                QStringList parts = line.split("|");
                if (parts.size() == 9) {
                    int x=parts[6].toInt();
                    QList<int> current_cycle;
                    while(x!=0)
                    {
                        current_cycle.append(x%10);
                        x/=10;
                    }
                    Task task(
                        parts[0],
                        QDateTime::fromString(parts[1], "yyyy-MM-dd HH:mm"),
                        parts[2].toInt(),
                        static_cast<TaskType>(parts[5].toInt()),
                        current_cycle,
                        parts[7].toInt(),
                        parts[8].toInt()
                        );
                    task.isCompleted = parts[3].toInt();
                    task.failed = parts[4].toInt();
                    userOut.tasks.append(task);
                }
            } else if (currentSection == "ACHIEVEMENTS") {
                QStringList parts = line.split("|");
                if (parts.size() == 3) {
                    userOut.achievements.append(Achievement(
                        parts[0],
                        QDateTime::fromString(parts[1], "yyyy-MM-dd HH:mm"),
                        parts[2]
                        ));
                }
            } else if (currentSection == "TOMATOCLOCK") {
                userOut.tomatoclocktime = line.toInt();
            } else if (currentSection == "TASKCOMPLETE") {
                userOut.taskcompletetime = line.toInt();
            } else if (currentSection == "LASTLOGIN") {
                userOut.lastlogindate = QDate::fromString(line, "yyyy-MM-dd");
            }
        }
    }

    file.close();
    return true;
}
