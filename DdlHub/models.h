#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <QString>
#include <QTime>
#include <QDateTime>
#include <QDate>
#include <QList>
#include <QVector>
#include <QMessageBox>

enum TaskType { Assignment, Paper, Exam };

class Course {
public:
    QString name; // Course name
    QTime starttime; // Start time
    QTime endtime; // End time
    QString date; // Day of the week
    QString location; // Location

    Course() = default;
    Course(QString name, QTime st, QTime en, QString dat, QString lo);

    bool operator==(const Course &c) const;
};

class Task {
public:
    QString title; // Task title
    QDateTime deadline; // Deadline
    int estimatedTimeMinutes; // Estimated time in minutes
    bool isCompleted = false; // Completion status
    bool failed = false; // Failure status
    TaskType type; // Task type
    QList<int> cycle; // Task cycle
    int daystocomplete = 1; // Expected days to complete
    int priority = 5; // Priority (1-10)

    Task();
    Task(QString t, QDateTime d, int time);
    Task(QString t, QDateTime d, int time, TaskType tp, QList<int> ct, int dtc, int p = 5);

    bool operator==(const Task &t) const;
    int hoursRemaining() const;
    void reducedaystocomplete();
};

class Achievement {
public:
    QString title; // Achievement title
    QDateTime unlocktime; // Unlock time
    QString description; // Achievement description

    Achievement() = default;
    Achievement(QString t);
    Achievement(QString t, QDateTime ti, QString d);

    bool operator==(const Achievement &other) const;
};

class User : public QObject {
    Q_OBJECT

public:
    QString username; // Username
    QString password; // Password
    QDate lastlogindate; // Last login date
    QList<Course> courses; // List of courses
    QList<Task> tasks; // List of tasks
    QVector<Achievement> achievements; // List of achievements
    int tomatoclocktime = 0; // Tomato clock count
    int taskcompletetime = 0; // Task completion count

    explicit User(QObject *parent = nullptr);
    User(QString name, QString word, QObject *parent = nullptr);

    void copyFrom(const User &other);
    bool verify(QString name, QString word);
    bool is0To4();
    bool addcourse(Course c);
    bool removecourse(Course c);
    bool removecoursebyname(QString n);
    bool removecoursebytime(QTime st, QTime en, QString date);
    bool addtask(Task t);
    bool removetask(QString t);
    bool completetask(QString t);
    void checktask();
    void addAchievement(const Achievement &a);
    void tomatoclockfinished();

signals:
    void achievementUnlocked(const Achievement &achievement);
};

#endif // MODEL_H
