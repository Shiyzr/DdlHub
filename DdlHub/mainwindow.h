#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include "TopMenu.h"
#include "SlideMenu.h"
#include "SlideMenuButton.h"
#include "ItemWidget.h"
#include "CourseSchedule.h"
#include "DeadlineWidget.h"
#include "AchievementWidget.h"
#include "CircularProgressBar.h"
#include "TodayTaskWindow.h"
#include "ChangePasswordDialog.h"


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(User& user,QWidget *parent = nullptr);
    ~MainWindow();

    TopMenu* tp;
    SlideMenu *menu;
    ChangePasswordDialog* profile;

    SlideMenuButton* btnTodoList;
    SlideMenuButton* btnDDL;
    SlideMenuButton* btnAchievement;
    SlideMenuButton* btnPomodoro;


    void resizeEvent(QResizeEvent *event) override;

    User& currentUser;
    DataManager dataManager;

    CourseSchedule *s;


    QWidget *achievementContainer;
    QVBoxLayout *achievementLayout;

    QWidget *currentContent;  // 当前显示的内容
    TodayTaskWindow *todoListContent; // To-Do List 页面
    DeadlineWidget *ddlContent;      // DDL 页面
    QWidget *achievementContent; // 成就页面
    QWidget *pomodoroContent; // 番茄钟页面
    QWidget *durationArea;
    QLabel *times;

    //QPushButton* refreshButton;
    AchievementWidget* 零点冲刺王;
    AchievementWidget* 早鸟先锋;
    AchievementWidget* 专注达人;
    AchievementWidget* 初出茅庐;
    AchievementWidget* 夜猫子;
    AchievementWidget* 百战百胜;
    AchievementWidget* 长期规划师;
    AchievementWidget* 小试牛刀;




    void initAchievementPage();  // 声明


private:
    Ui::MainWindow *ui;

public slots:
    void onRefreshClicked();
};
#endif // MAINWINDOW_H
