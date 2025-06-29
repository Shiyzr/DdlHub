#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "DurationDialog.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDebug>
#include <QScrollArea>
#include <QTimer>
#include <QMessageBox>

MainWindow::MainWindow(User& user,QWidget *parent)
    : QMainWindow(parent)
    , currentUser(user)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    setWindowTitle("DdlHub");
    setFixedSize(800, 600);

    tp=new TopMenu(this);


    todoListContent=new TodayTaskWindow(currentUser,this);
    ui->stackedWidget1->addWidget(todoListContent);








    ddlContent=new DeadlineWidget(currentUser,this);
    ui->stackedWidget1->addWidget(ddlContent);










    // 成就页面初始化
    achievementContainer = new QWidget;
    achievementLayout = new QVBoxLayout;
    achievementLayout->setAlignment(Qt::AlignTop); // 否则组件会集中在中间
    achievementLayout->addStretch();
    achievementContainer->setLayout(achievementLayout);
    // 添加成就内容
    零点冲刺王 = new AchievementWidget("零点冲刺王", "你的速度就是你的武器，冲刺时刻从不掉链子！", QPixmap("C:\\Users\\syzr_\\Downloads\\newuntitled\\DdlHub_Logo").scaled(40, 40, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    零点冲刺王->setConditionText("任务在截止当天的最后2小时内完成");
    早鸟先锋 = new AchievementWidget("早鸟先锋", "早起的鸟儿有虫吃，提前完成让你领先一步！", QPixmap("C:\\Users\\syzr_\\Downloads\\newuntitled\\DdlHub_Logo").scaled(40, 40, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    早鸟先锋->setConditionText(" 任务至少提前3天完成");
    专注达人 = new AchievementWidget("专注达人", "25分钟的专注，打造出无坚不摧的意志力！", QPixmap("C:\\Users\\syzr_\\Downloads\\newuntitled\\DdlHub_Logo").scaled(40, 40, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    专注达人->setConditionText("累计完成50个番茄钟专注时长");
    初出茅庐 = new AchievementWidget("初出茅庐", "你的征途，从第一个任务开始，未来无可限量！", QPixmap("C:\\Users\\syzr_\\Downloads\\newuntitled\\DdlHub_Logo").scaled(40, 40, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    初出茅庐->setConditionText("添加第一个DDL任务");
    夜猫子 = new AchievementWidget("夜猫子", "黑夜是你的战场，灵感在星光下闪耀！", QPixmap("C:\\Users\\syzr_\\Downloads\\newuntitled\\DdlHub_Logo").scaled(40, 40, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    夜猫子->setConditionText("在午夜至凌晨4点间完成任务");
    百战百胜 = new AchievementWidget("百战百胜", " 百次出击，百次凯旋，你是名副其实的任务终结者！", QPixmap("C:\\Users\\syzr_\\Downloads\\newuntitled\\DdlHub_Logo").scaled(40, 40, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    百战百胜->setConditionText("累计完成100个任务");
    长期规划师 = new AchievementWidget("长期规划师", "眼光看得远，计划布得深，你是未来的掌舵人！", QPixmap("C:\\Users\\syzr_\\Downloads\\newuntitled\\DdlHub_Logo").scaled(40, 40, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    长期规划师->setConditionText("添加一个截止日期在3个月后的任务");
    小试牛刀 = new AchievementWidget("小试牛刀", "10个任务轻松搞定，实力不容小觑！", QPixmap("C:\\Users\\syzr_\\Downloads\\newuntitled\\DdlHub_Logo").scaled(40, 40, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    小试牛刀->setConditionText("累计完成10个任务");
    // 设置已完成状态
    currentUser.checkAchievement();
    for (int i = 0; i < currentUser.achievements.size(); ++i) {
        Achievement target = currentUser.achievements[i];

        if (target.title == "零点冲刺王") {
            if (零点冲刺王->completed != true){
                零点冲刺王->setCompleted(true);
            }
            零点冲刺王->setCompleted(true);
        }
        if (target.title == "早鸟先锋") {
            if (早鸟先锋->completed != true){
                早鸟先锋->setCompleted(true);
            }
            早鸟先锋->setCompleted(true);
        }
        if (target.title == "初出茅庐") {
            if (初出茅庐->completed != true){
                初出茅庐->setCompleted(true);
            }
            初出茅庐->setCompleted(true);
        }
        if (target.title == "夜猫子") {
            if (夜猫子->completed != true){
                夜猫子->setCompleted(true);
            }
            夜猫子->setCompleted(true);
        }
        if (target.title == "小试牛刀") {
            if (小试牛刀->completed != true){
                小试牛刀->setCompleted(true);
            }
            小试牛刀->setCompleted(true);
        }
        if (target.title == "百战百胜") {
            if (百战百胜->completed != true){
                百战百胜->setCompleted(true);
            }
            百战百胜->setCompleted(true);
        }
        if (target.title == "长期规划师") {
            if (长期规划师->completed != true){
                长期规划师->setCompleted(true);
            }
            长期规划师->setCompleted(true);
        }
        if (target.title == "专注达人") {
            if (专注达人->completed != true){
                专注达人->setCompleted(true);
            }
            专注达人->setCompleted(true);
        }
    }
    // 添加到布局
    achievementLayout->addWidget(零点冲刺王);
    achievementLayout->addWidget(早鸟先锋);
    achievementLayout->addWidget(专注达人);
    achievementLayout->addWidget(初出茅庐);
    achievementLayout->addWidget(夜猫子);
    achievementLayout->addWidget(百战百胜);
    achievementLayout->addWidget(长期规划师);
    achievementLayout->addWidget(小试牛刀);
    // 放入滚动区域
    QScrollArea *scrollArea = new QScrollArea;
    QHBoxLayout* tLayout = new QHBoxLayout;
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(achievementContainer);
    //achievementContent = scrollArea;
    achievementContent = new QWidget(this);
    achievementContent->setLayout(tLayout);
    tLayout->addWidget(scrollArea);
    // // 创建按钮
    // refreshButton = new QPushButton("刷新成就");
    // tLayout->addWidget(refreshButton);

    // //ui->stackedWidget1->widget(2)->set

    // // 绑定槽函数

    // 添加到 stackedWidget 第2页
    //ui->stackedWidget1->addWidget(achievementContent);
    ui->stackedWidget1->addWidget(achievementContent);


    // connect(refreshButton, &QPushButton::clicked, this, &MainWindow::onRefreshClicked);
    // bool ok = connect(refreshButton, &QPushButton::clicked, this, &MainWindow::onRefreshClicked);
    // qDebug() << "连接结果：" << ok; // 通常 true


    QTimer* t = new QTimer(this);
    connect(t, &QTimer::timeout, this, &MainWindow::onRefreshClicked);
    t->start(10);




    pomodoroContent=new QWidget(this);
    ui->stackedWidget1->addWidget(pomodoroContent);
    CircularProgressBar* progressBar = new CircularProgressBar;
    // circular->move(300, 100);
    progressBar->setUser(&currentUser);

    QPushButton *startBtn = new QPushButton("启动");
    QPushButton *pauseBtn = new QPushButton("暂停");
    QPushButton *resetBtn = new QPushButton("重置");
    QPushButton *setDurationBtn = new QPushButton("自定义");
    durationArea = new QWidget;
    QVBoxLayout *durationLayout = new QVBoxLayout;
    QLabel *workLabel = new QLabel;
    QLabel *breakLabel = new QLabel;
    times = new QLabel;
    workLabel->setText("工作时长：" + QString::number(progressBar->m_workDuration / 60) + "min");
    breakLabel->setText("休息时长：" + QString::number(progressBar->m_breakDuration / 60) + "min");
    times->setText("共完成番茄钟：" + QString::number(currentUser.tomatoclocktime) + "个");

    durationLayout->addWidget(workLabel);
    durationLayout->addWidget(breakLabel);
    durationLayout->addWidget(times);

    durationArea->setLayout(durationLayout);
    durationArea->setFixedSize(140,75);

    QObject::connect(startBtn, &QPushButton::clicked, progressBar, &CircularProgressBar::start);
    QObject::connect(pauseBtn, &QPushButton::clicked, progressBar, &CircularProgressBar::pause);
    QObject::connect(resetBtn, &QPushButton::clicked, progressBar, &CircularProgressBar::reset);


    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->addWidget(startBtn);
    btnLayout->addWidget(pauseBtn);
    btnLayout->addWidget(resetBtn);
    btnLayout->addWidget(setDurationBtn);

    QHBoxLayout *wLayout = new QHBoxLayout;
    wLayout->addWidget(progressBar);
    wLayout->addWidget(durationArea);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addLayout(wLayout);
    //layout->addWidget(progressBar);
    layout->addLayout(btnLayout);
    pomodoroContent->setLayout(layout);


    ui->stackedWidget1->setCurrentWidget(todoListContent);

    connect(setDurationBtn, &QPushButton::clicked, [=]() {
        DurationDialog dialog;
        if (dialog.exec() == QDialog::Accepted) {
            int workMin = dialog.workMinutes();
            int breakMin = dialog.breakMinutes();
            progressBar->setDurations(workMin, breakMin);

            // 实时更新文字显示
            workLabel->setText("工作时长：" + QString::number(workMin) + "min");
            breakLabel->setText("休息时长：" + QString::number(breakMin) + "min");
        }
    });

    //呼出左边菜单
    menu=new SlideMenu(this);
    QObject::connect(tp->customBtn, &QPushButton::clicked, menu, &SlideMenu::toggle);
    SlideMenuButton* btnTodoList = new SlideMenuButton("To-Do List");
    SlideMenuButton* btnDDL = new SlideMenuButton("我的ddl");
    SlideMenuButton* btnAchievement = new SlideMenuButton("成就");
    SlideMenuButton* btnPomodoro = new SlideMenuButton("番茄钟");
    menu->addWidget(btnTodoList);
    menu->addWidget(btnDDL);
    menu->addWidget(btnAchievement);
    menu->addWidget(btnPomodoro);
    connect(btnTodoList, &QPushButton::clicked, this, [&](){ui->stackedWidget1->setCurrentWidget(todoListContent);});
    connect(btnDDL, &QPushButton::clicked, this, [&]() {ui->stackedWidget1->setCurrentWidget(ddlContent);});
    connect(btnAchievement, &QPushButton::clicked, this, [&](){ ui->stackedWidget1->setCurrentWidget(achievementContent); });
    connect(btnPomodoro, &QPushButton::clicked, this, [&](){ui->stackedWidget1->setCurrentWidget(pomodoroContent);});

    //右上角课程表
    s=new CourseSchedule(dataManager,currentUser);
    s->setWindowTitle("课程表");
    s->resize(700, 550);
    QObject::connect(tp->scheduleBtn, &QPushButton::clicked, s, &CourseSchedule::show);

    profile = new ChangePasswordDialog(currentUser, this);

    connect(tp->profileButton, &QPushButton::clicked, profile, &ChangePasswordDialog::show);

}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    tp->setGeometry(0, 0, this->width(), this->height() * (60.0/600));
    tp->setStyleSheet("background-color: rgba(0,0,0,75);");
    tp->setAttribute(Qt::WA_StyledBackground, true);
}

void MainWindow::onRefreshClicked() {

    //todoListContent->loadTasks();
    // qDebug()<<"1:"<<todoListContent->longTermTasks.size() + todoListContent->todayTasks.size()<<Qt::endl <<"2: " << currentUser.getShownTasks().size()<<Qt::endl;
    // for (const Task &task : currentUser.getShownTasks()) {
    //     qDebug() << task.title << Qt::endl;
    // }
    // qDebug() << "totalTasks" <<Qt::endl;
    // for (const Task &task : currentUser.tasks) {
    //     qDebug() << task.title << Qt::endl;
    // }
    // qDebug() << "totalTasksSize:" << currentUser.tasks.size() << Qt::endl;
    if (todoListContent->longTermTasks.size() + todoListContent->todayTasks.size() != currentUser.getShownTasks().size()) {
        todoListContent->todayTasks.clear();
        todoListContent->longTermTasks.clear();
        todoListContent->loadTasks();
    }
    // qDebug()<<"no"<<Qt::endl;

    if ("共完成番茄钟：" + QString::number(currentUser.tomatoclocktime) + "个" != times->text()) {
        times->setText("共完成番茄钟：" + QString::number(currentUser.tomatoclocktime) + "个");
        dataManager.saveUser(currentUser);
    }


    currentUser.checkAchievement();

    if(todoListContent->completedTasks){
        QVector<DDLItem> v;
        for(auto i=currentUser.tasks.begin();i!=currentUser.tasks.end();i++){
            DDLItem x;
            x.name=i->title;
            x.deadline=i->deadline;
            if(i->type==Assignment){
                x.type="任务";
            }
            else if(i->type==Paper){
                x.type="论文";
            }
            else if(i->type==Exam){
                x.type="考试";
            }
            if(i->cycle.length()==0){
                x.cycle="NoCycle";
            }
            else if(i->cycle.length()==7){
                x.cycle="Daily";
            }
            else{
                QList<QString> tmp;
                tmp.append("周一");
                tmp.append("周二");
                tmp.append("周三");
                tmp.append("周四");
                tmp.append("周五");
                tmp.append("周六");
                tmp.append("周日");
                bool isfirst=true;
                for(auto j=i->cycle.begin();j!=i->cycle.end();j++){
                    if(isfirst){
                        x.cycle=tmp[*j];
                        isfirst=false;
                    }
                    else{
                        x.cycle+=","+tmp[*j];
                    }
                }
            }
            v.append(x);
        }
        ddlContent->refreshDDLList(v);
    }

    for (int i = 0; i < currentUser.achievements.size(); ++i) {
        Achievement target = currentUser.achievements[i];

        if (target.title == "零点冲刺王") {
            if (零点冲刺王->completed != true){
                QMessageBox::information(this, "触发成就", "达成成就：" + target.title + '!');
                零点冲刺王->setCompleted(true);
            }
            零点冲刺王->setCompleted(true);
        }
        if (target.title == "早鸟先锋") {
            if (早鸟先锋->completed != true){
                QMessageBox::information(this, "触发成就", "达成成就：" + target.title + '!');
                早鸟先锋->setCompleted(true);
            }
            早鸟先锋->setCompleted(true);
        }
        if (target.title == "初出茅庐") {
            if (初出茅庐->completed != true){
                QMessageBox::information(this, "触发成就", "达成成就：" + target.title + '!');
                初出茅庐->setCompleted(true);
            }
            初出茅庐->setCompleted(true);
        }
        if (target.title == "夜猫子") {
            if (夜猫子->completed != true){
                QMessageBox::information(this, "触发成就", "达成成就：" + target.title + '!');
                夜猫子->setCompleted(true);
            }
            夜猫子->setCompleted(true);
        }
        if (target.title == "小试牛刀") {
            if (小试牛刀->completed != true){
                QMessageBox::information(this, "触发成就", "达成成就：" + target.title + '!');
                小试牛刀->setCompleted(true);
            }
            小试牛刀->setCompleted(true);
        }
        if (target.title == "百战百胜") {
            if (百战百胜->completed != true){
                QMessageBox::information(this, "触发成就", "达成成就：" + target.title + '!');
                百战百胜->setCompleted(true);
            }
            百战百胜->setCompleted(true);
        }
        if (target.title == "长期规划师") {
            if (长期规划师->completed != true){
                QMessageBox::information(this, "触发成就", "达成成就：" + target.title + '!');
                长期规划师->setCompleted(true);
            }
            长期规划师->setCompleted(true);
        }
        if (target.title == "专注达人") {
            if (专注达人->completed != true){
                QMessageBox::information(this, "触发成就", "达成成就：" + target.title + '!');
                专注达人->setCompleted(true);
            }
            专注达人->setCompleted(true);
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
