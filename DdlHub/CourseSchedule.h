#ifndef COURSESCHEDULE_H
#define COURSESCHEDULE_H

#include <QMainWindow>
#include <QTableWidget>
#include <QPushButton>
#include <QDialog>
#include <QLineEdit>
#include <QSpinBox>
#include <QComboBox>
#include <QLabel>
#include <QMessageBox>
#include <QMenu>
#include <QAction>
#include <QFormLayout>
#include <QHeaderView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStringList>
#include <QVariant>
#include "ItemWidget.h"

class CourseDialog : public QDialog {
    Q_OBJECT
public:
    explicit CourseDialog(QWidget *parent = nullptr);

    QString getCourseName() const;
    int getDay() const;
    int getStartSection() const;
    int getEndSection() const;
    QString getLocation() const;

private:
    QLineEdit *nameEdit;
    QComboBox *dayCombo;
    QSpinBox *startSpin;
    QSpinBox *endSpin;
    QLineEdit *locationEdit;
};

class CourseSchedule : public QMainWindow {
    Q_OBJECT

public:
    CourseSchedule(DataManager& dm,User& user,QWidget *parent = nullptr);
    ~CourseSchedule();
    QString Coursetime[12]={"08:00-08:50","09:00-09:50","10:10-11:00","11:10-12:00","13:00-13:50",
                              "14:00-14:50","15:10-16:00","16:10-17:00","17:10-18:00","18:40-19:30","19:40-20:30","20:40-21:30"};
    QString week[7]={"Monday","Tuesday","Wednesday","Thursday","Friday","Saturday","Sunday"};

private slots:
    void addCourse();//添加课程
    void showCourseDetails(int row, int column);//点开课程详情
    void showContextMenu(const QPoint &pos);//右键点开删除
    void deleteCourse();//删除课程

private:
    User currentuser;
    DataManager datamanager;

    void setupUI();//初始化界面
    void createActions();//创建加号？
    void populateTable();//填充课程
    bool hasTimeConflict(int day, int start, int end, const QString &name);//课程冲突
    void loadUserCourses();

    QWidget *centralWidget;//主界面
    QVBoxLayout *mainLayout;//垂直
    QHBoxLayout *buttonLayout;//水平
    QPushButton *addButton;//添加课程
    QTableWidget *scheduleTable;//显示课程表

    QMenu *contextMenu;//右键菜单
    QAction *deleteAction;//删除动作

    int selectedRow;//记录选中的表格行
    int selectedCol;//。。。。列
};

#endif // COURSESCHEDULE_H
