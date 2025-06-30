#include <QTime>
#include <QDateTime>
#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QMessageBox>

enum TaskType { Assignment, Paper, Exam };
//enum CycleType { NoCycle, Weekly, Daily };


class Course{
public:
    QString name;//课程名
    QTime starttime;//开始时间
    QTime endtime;//结束时间
    QString date;//星期几
    QString location;//上课地点

    Course()=default;
    Course(QString name,QTime st,QTime en,QString dat,QString lo):name(name),starttime(st),endtime(en),date(dat),location(lo){}
    //创建课程
    bool operator==(const Course &c) const
    {
        if (name==c.name&&starttime==c.starttime&&endtime==c.endtime&&date==c.date)
            return 1;
        return 0;
    }//==的重载

};

class Task {
public:
    QString title;//任务名
    QDateTime deadline;//ddl
    int estimatedTimeMinutes=0;//预计时间
    bool isCompleted = 0;//是否完成
    bool failed = 0;//是否已经寄了
    TaskType type;//表示任务类型
    //CycleType cycle;//表示任务周期
    QList<int> cycle;
    int daystocomplete = 1;//表示预期天数
    int priority = 5;//任务的优先级,在1-10中取值

    //三种构造函数，兼容过去的类型
    Task() : title(""), deadline(QDateTime::currentDateTime()), estimatedTimeMinutes(0), type(Assignment), daystocomplete(1) {
        priority=5;
        cycle.clear();
    }
    Task(QString t, QDateTime d):title(t), deadline(d), estimatedTimeMinutes(0),type(Assignment), daystocomplete(1),priority(5){}
    Task(QString t, QDateTime d,int stime,TaskType tp,QList<int> ct,int dtc=1,int p=5) :title(t), deadline(d), estimatedTimeMinutes(stime),type(tp),cycle(ct),daystocomplete(dtc),priority(p){}

    //创建任务
    bool operator==(const Task& t) const
    {
        return title == t.title;
    }
    //==的重载

    int hoursRemaining() {
        int secs = QDateTime::currentDateTime().secsTo(deadline);
        return (secs > 0) ? secs / 3600 : -1;  // 处理已过期情况
    }//返回任务距离ddl的剩余小时数
    void reducedaystocomplete() {
        if (daystocomplete > 0) {
            daystocomplete--;
            if (daystocomplete == 0) {
                isCompleted = true;
            }
        }
    }//减少任务的预期天数

};

class Achievement {
public:
    QString title;//成就名
    QDateTime unlocktime;//解锁时间
    QString description;//成就描述

    Achievement(QString t,QDateTime ti,QString d):title(t),unlocktime(ti),description(d){}
    Achievement(QString t):title(t){}
    Achievement()=default;
    //添加成就
    bool operator==(const Achievement& other) const {
        return title == other.title;
    }
};
class User {
public:

    QString username;//用户名
    QString password;//密码
    QDate lastlogindate;//上次登录时间

    User(QString name, QString word) :username(name), password(word),lastlogindate(QDate::currentDate()), tomatoclocktime(0),taskcompletetime(0) {}//创建新用户
    User():lastlogindate(QDate::currentDate()), tomatoclocktime(0),taskcompletetime(0) {}


    bool verify(QString name,QString word)
    {
        if (name==username&&word==password)
            return 1;
        return 0;
    }//登录

    bool is0To4()
    {
        // 获取当前时间（不含日期部分）
        QTime currentTime = QDateTime::currentDateTime().time();

        // 定义时间范围：00:00:00 至 04:00:00
        QTime start(0, 0);    // 午夜00:00
        QTime end(4, 0);      // 凌晨04:00

        // 判断当前时间是否在区间内
        return (currentTime >= start && currentTime <= end);
    }//为“夜猫子”成就服务的函数


    QList<Course> courses;//课程
    QList<Task> tasks;//任务
    QList<Task> getShownTasks(){
        QList<Task> ret;

        int currentDay = QDate::currentDate().dayOfWeek() - 1;
        if (currentDay < 0) currentDay = 6;

        for (const Task &task : tasks) {
            if (task.isCompleted || task.failed) continue;
            if (!task.cycle.empty()) {
                if (task.cycle.contains(currentDay)) {
                    ret.append(task);
                    continue;
                }
            }

            int daysRemaining = QDateTime::currentDateTime().daysTo(task.deadline);
            if (daysRemaining < 7) {
                ret.append(task);
            }
        }
        return ret;
    }


    QVector<Achievement> achievements;//成就
    int tomatoclocktime=0;//完成番茄钟数
    int taskcompletetime=0;//完成任务数

    bool addcourse(Course c)
    {
        bool canadd=1;
        if (c.endtime<=c.starttime)
            canadd=0;
        for (auto i=courses.begin();i!=courses.end();i++)
        {
            if (i->date==c.date)
            {
                if(c.starttime < i->endtime && c.endtime > i->starttime)
                {
                    canadd=0;
                    break;
                }
            }
        }
        if(canadd)  {courses.push_back(c);return 1;}
        else
        {
            QMessageBox::warning(nullptr,"错误","时间冲突，无法添加该课程！");
            return 0;
        }
    }//添加课程,时间冲突的不进行添加,开始时间晚于结束时间的不添加，已改为bool型

    bool removecourse(Course c)
    {
        bool successfully_removed = 0;
        for (auto i = courses.begin(); i != courses.end(); )
        {
            if (*i == c) {i = courses.erase(i);successfully_removed = 1;}
            else ++i;
        }
        if (!successfully_removed)
            QMessageBox::warning(nullptr,"错误","找不到该课程！");
        return successfully_removed;
    }//用课程类移除课程,已改为bool型

    bool removecoursebyname(QString n)
    {
        bool successfully_removed = 0;
        for (auto i=courses.begin();i!=courses.end();)
        {
            if (i->name==n)  {i=courses.erase(i);successfully_removed = 1;}
            else ++i;
        }
        if (!successfully_removed)
            QMessageBox::warning(nullptr,"错误","找不到该课程！");
        return successfully_removed;
    }//用课程名移除课程,已改为bool型

    bool removecoursebytime(QTime st,QTime en,QString date)
    {
        bool successfully_removed = 0;
        for (auto i=courses.begin();i!=courses.end();)
        {
            if (i->starttime==st && i->endtime==en&&i->date==date)  {i=courses.erase(i);successfully_removed = 1;}
            else ++i;
        }
        if (!successfully_removed)
            QMessageBox::warning(nullptr,"错误","找不到该课程！");
        return successfully_removed;
    }//用课程起止时间与日期移除课程,已改为bool型



    bool addtask(Task t)
    {
        bool canadd=1;
        for (auto i=tasks.begin();i!=tasks.end();i++)
        {
            if (i->title==t.title)  canadd=0;
        }
        if(canadd)
        {
            tasks.push_back(t);
            if (!achievements.contains(Achievement("初出茅庐")))
            {
                //如果需要弹窗的话让我在这里添加语句
                Achievement acv("初出茅庐",QDateTime::currentDateTime(),"添加第一个DDL任务");
                addAchievement(acv);
            }//获取“初出茅庐”成就
            if (t.hoursRemaining()>=2160&&!achievements.contains(Achievement("长期规划师")))
            {
                //如果需要弹窗的话让我在这里添加语句
                Achievement acv("长期规划师",QDateTime::currentDateTime(),"添加一个截止日期在3个月后的任务");
                addAchievement(acv);
            }//获取“长期规划师”成就
        }
        else
            QMessageBox::warning(nullptr,"错误","已有此任务！请调整任务名称。");
        return canadd;
    }//添加任务,名字重复的不进行添加，已改为bool型

    bool removetask(QString t)
    {
        bool successfully_removed = 0;
        for (int i=0;i<tasks.length();i++){
            if(tasks[i].title==t){
                tasks.erase(tasks.begin()+i);
                successfully_removed=1;
                break;
            }
        }
        // for (auto i=tasks.begin();i!=tasks.end();)
        // {
        //     if (i->title==t)  {i=tasks.erase(i);successfully_removed = 1;}
        //     else ++i;
        // }
        if (!successfully_removed)
            QMessageBox::warning(nullptr,"错误","找不到该任务！");
        return successfully_removed;
    }//删除任务,已改为bool型
    bool completetask(QString t)
    {
        bool cando=0;
        for (auto i=tasks.begin();i!=tasks.end();i++)
        {
            if (i->title==t&&i->failed==false)
            {
                cando=1;
                i->isCompleted=1;
                taskcompletetime++;

                if (taskcompletetime==10 && !achievements.contains(Achievement("小试牛刀")))
                {
                    //如果需要弹窗的话让我在这里添加语句
                    Achievement acv("小试牛刀",QDateTime::currentDateTime(),"累计完成10个任务");
                    addAchievement(acv);
                }//获取“小试牛刀”成就
                if (taskcompletetime==100 && !achievements.contains(Achievement("百战百胜")))
                {
                    //如果需要弹窗的话让我在这里添加语句
                    Achievement acv("百战百胜",QDateTime::currentDateTime(),"累计完成100个任务");
                    addAchievement(acv);
                }//获取“百战百胜”成就

                if (i->hoursRemaining()>=0 && i->hoursRemaining()<=2 && !achievements.contains(Achievement("零点冲刺王")))
                {
                    //如果需要弹窗的话让我在这里添加语句
                    Achievement acv1("零点冲刺王",QDateTime::currentDateTime(),"任务在截止当天的最后2小时内完成。 你的速度就是你的武器，冲刺时刻从不掉链子！");
                    addAchievement(acv1);
                }//获取“零点冲刺王”成就
                else if (i->hoursRemaining()>=72 && !achievements.contains(Achievement("早鸟先锋")))
                {
                    //如果需要弹窗的话让我在这里添加语句
                    Achievement acv2("早鸟先锋",QDateTime::currentDateTime(),"任务至少提前3天完成。 早起的鸟儿有虫吃，提前完成让你领先一步！");
                    addAchievement(acv2);
                }//获取“早鸟先锋”成就

                if (is0To4() && !achievements.contains(Achievement("夜猫子")))
                {
                    //如果需要弹窗的话让我在这里添加语句
                    Achievement acv("夜猫子",QDateTime::currentDateTime(),"在午夜至凌晨4点间完成任务");
                    addAchievement(acv);
                }//获取“夜猫子”成就
            }
        }
        if (!cando)
            QMessageBox::warning(nullptr,"错误","找不到该任务！");
        return cando;
    }//完成任务,已改为bool型
    void checktask()
    {
        for (auto i=tasks.begin();i!=tasks.end();i++)
        {
            if (QDateTime::currentDateTime() > i->deadline && i->isCompleted==false)
                i->failed=true;
        }
    }//如果到ddl还没完成，把这个任务标记为failed



    void addAchievement(Achievement a)
    {
        if (!achievements.contains(a)) {
            if (a.description.isEmpty()) {
                QMessageBox::warning(nullptr, "添加成就失败", "成就描述不能为空！");
                return;
            }
            achievements.append(a);
        }
    }//获得成就

    void tomatoclockfinished()
    {
        tomatoclocktime++;
        if (tomatoclocktime==50)
        {
            //如果需要弹窗的话让我在这里添加语句
            Achievement acv3("专注达人",QDateTime::currentDateTime(),"累计完成50个番茄钟专注时长。 25分钟的专注，打造出无坚不摧的意志力！");
            addAchievement(acv3);
        }//获得“专注达人”成就
    }//番茄钟结束时调用这个函数

    void checkAchievement()
    {
        if (taskcompletetime>=10 && !achievements.contains(Achievement("小试牛刀")))
        {
            //如果需要弹窗的话让我在这里添加语句
            Achievement acv("小试牛刀",QDateTime::currentDateTime(),"累计完成10个任务");
            addAchievement(acv);
        }//获取“小试牛刀”成就
        if (taskcompletetime>=100 && !achievements.contains(Achievement("百战百胜")))
        {
            //如果需要弹窗的话让我在这里添加语句
            Achievement acv("百战百胜",QDateTime::currentDateTime(),"累计完成100个任务");
            addAchievement(acv);
        }//获取“百战百胜”成就
        if (tomatoclocktime>=50 && !achievements.contains(Achievement("专注达人")))
        {
            //如果需要弹窗的话让我在这里添加语句
            Achievement acv3("专注达人",QDateTime::currentDateTime(),"累计完成50个番茄钟专注时长。 25分钟的专注，打造出无坚不摧的意志力！");
            addAchievement(acv3);
        }//获得“专注达人”成就
        if (taskcompletetime>0 && !achievements.contains(Achievement("初出茅庐")))
        {
            //如果需要弹窗的话让我在这里添加语句
            Achievement acv("初出茅庐",QDateTime::currentDateTime(),"添加第一个DDL任务");
            addAchievement(acv);
        }//获取“初出茅庐”成就
        for (auto i=tasks.begin();i!=tasks.end();i++)
        {
            if (i->hoursRemaining()>=2160&&!achievements.contains(Achievement("长期规划师")))
            {
                //如果需要弹窗的话让我在这里添加语句
                Achievement acv("长期规划师",QDateTime::currentDateTime(),"添加一个截止日期在3个月后的任务");
                addAchievement(acv);
            }//获取“长期规划师”成就
        }
    }

};

class DataManager {
public:
    bool saveUser(const User &user)
    {
        QFile file("user_" + user.username + ".txt");
        if (!file.open(QIODevice::WriteOnly)) {
            qWarning() << "无法打开用户文件";
            return false;
        }

        QTextStream out(&file);
        // 写入用户信息
        out << "USERINFO|" << user.username << "|" << user.password << "\n";

        // 写入课程数据
        out << "\n[COURSES]\n";
        for (const Course &c : user.courses) {
            out << c.name << "|"
                << c.starttime.toString("HH:mm") << "|"
                << c.endtime.toString("HH:mm") << "|"
                << c.date << "|"
                << c.location << "\n";
        }

        // 写入任务数据
        out << "\n[TASKS]\n";
        for (const Task& t : user.tasks) {
            QString tmp="";
            if(!t.cycle.empty()){
                for(auto i=t.cycle.begin();i!=t.cycle.end();i++){
                    tmp+=QString::number(*i);
                }
            }
            out << t.title << "|"
                << t.deadline.toString("yyyy-MM-dd HH:mm") << "|"
                << t.estimatedTimeMinutes << "|"
                << t.isCompleted << "|"
                << t.failed << "|"
                << t.type << "|"
                <<tmp<<"|"
                << t.daystocomplete << "|"
                << t.priority << "\n";
        }

        // 写入成就数据
        out << "\n[ACHIEVEMENTS]\n";
        for (const Achievement &a : user.achievements) {
            out << a.title << "|"
                << a.unlocktime.toString("yyyy-MM-dd HH:mm") << "|"
                << a.description << "\n";
        }

        //写入番茄钟使用次数
        out << "\n[TOMATOCLOCK]\n";
        out << user.tomatoclocktime << "\n";

        //写入任务完成次数
        out << "\n[TASKCOMPLETE]\n";
        out << user.taskcompletetime << "\n";

        //写入上次登录时间
        out << "\n[LASTLOGIN]\n";
        out << QDate::currentDate().toString("yyyy-MM-dd") << "\n";

        file.close();
        return true;
    }//保存用户数据


    User loadUser(const QString &username)
    {
        User user;
        QFile file("user_" + username + ".txt");

        if (!file.open(QIODevice::ReadOnly))
        {
            qWarning() << "用户文件不存在";
            return user;
        }

        QString currentSection;

        QTextStream in(&file);
        QString header = in.readLine();
        QStringList headerParts = header.split("|");
        if (headerParts.size() >= 3 && headerParts[0] == "USERINFO")
        {
            user.username = headerParts[1];  // 从文件内容读取用户名
            user.password = headerParts[2];  // 读取密码
        }
        else
        {
            qWarning() << "文件格式错误";
            return user;
        }
        while (!in.atEnd())
        {
            QString line = in.readLine().trimmed();

            if (line.startsWith("[COURSES]"))
            {
                currentSection = "COURSES";
            }
            else if (line.startsWith("[TASKS]"))
            {
                currentSection = "TASKS";
            }
            else if (line.startsWith("[ACHIEVEMENTS]"))
            {
                currentSection = "ACHIEVEMENTS";
            }
            else if (line.startsWith("[TOMATOCLOCK]"))
            {
                currentSection = "TOMATOCLOCK";
            }
            else if (line.startsWith("[TASKCOMPLETE]"))
            {
                currentSection = "TASKCOMPLETE";
            }
            else if (line.startsWith("[LASTLOGIN]"))
            {
                currentSection = "LASTLOGIN";
            }
            else if (!line.isEmpty())
            {
                if (currentSection == "COURSES")
                {
                    QStringList parts = line.split("|");
                    if (parts.size() == 5)
                    {
                        user.courses.append(Course(
                            parts[0],
                            QTime::fromString(parts[1], "HH:mm"),
                            QTime::fromString(parts[2], "HH:mm"),
                            parts[3],
                            parts[4]
                            ));
                    }
                }
                else if (currentSection == "TASKS")
                {
                    QStringList parts = line.split("|");
                    if (parts.size() == 9) {
                        QString tmp=parts[6];
                        QList<int>cycletmp;
                        for (QChar c : tmp){
                            cycletmp.append(c.digitValue());
                        }
                        Task task(
                            parts[0],
                            QDateTime::fromString(parts[1], "yyyy-MM-dd HH:mm"),
                            parts[2].toInt(),
                            static_cast<TaskType>(parts[5].toInt()),
                            //static_cast<CycleType>(parts[6].toInt()),
                            cycletmp,
                            parts[7].toInt(),
                            parts[8].toInt()
                            );
                        task.isCompleted = parts[3].toInt();
                        task.failed = parts[4].toInt();
                        user.tasks.append(task);
                    }
                }
                else if (currentSection == "ACHIEVEMENTS")
                {
                    QStringList parts = line.split("|");
                    if (parts.size() == 3)
                    {
                        user.achievements.append(Achievement(
                            parts[0],
                            QDateTime::fromString(parts[1], "yyyy-MM-dd HH:mm"),
                            parts[2]
                            ));
                    }
                }
                else if (currentSection == "TOMATOCLOCK")
                {
                    QString parts = line;
                    user.tomatoclocktime = parts.toInt();
                }
                else if (currentSection == "TASKCOMPLETE")
                {
                    QString parts = line;
                    user.taskcompletetime = parts.toInt();
                }
                else if (currentSection == "LASTLOGIN")
                {
                    user.lastlogindate = QDate::fromString(line, "yyyy-MM-dd");
                }
            }
        }

        file.close();
        return user;
    }//加载用户数据
};

class LoginSystem {
public:

    bool userExists(const QString &username)
    {
        return QFile::exists("user_" + username + ".txt");
    }//判断用户是否存在


    bool registerUser(const QString &username, const QString &password)
    {
        if (userExists(username)) {
            qWarning() << "用户名已存在！";
            return false;
        }

        QFile file("user_" + username + ".txt");
        if (!file.open(QIODevice::WriteOnly)) {
            qWarning() << "无法创建用户文件";
            return false;
        }

        QTextStream out(&file);
        out << "USERINFO|" << username << "|" << password << "\n";
        file.close();
        return true;
    }//用户注册


    bool login(const QString &username, const QString &password)
    {
        if (!userExists(username)) return false;

        QFile file("user_" + username + ".txt");
        if (!file.open(QIODevice::ReadOnly)) return false;

        QTextStream in(&file);
        QString firstLine = in.readLine();
        file.close();

        QStringList parts = firstLine.split("|");
        return (parts.size() >= 3 && parts[0] == "USERINFO"
                && parts[1] == username && parts[2] == password);
    }//用户登录

};
