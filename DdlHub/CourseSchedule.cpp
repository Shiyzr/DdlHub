#include "CourseSchedule.h"
#include <QTime>

CourseDialog::CourseDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle("添加课程");
    setMinimumSize(200, 200);  // 设置最小尺寸
    setFixedSize(200, 200);    // 固定尺寸

    QFormLayout *formLayout = new QFormLayout(this);

    nameEdit = new QLineEdit(this);
    formLayout->addRow("课程名称:", nameEdit);

    dayCombo = new QComboBox(this);
    dayCombo->addItems({"星期一", "星期二", "星期三", "星期四", "星期五", "星期六", "星期日"});
    formLayout->addRow("星期:", dayCombo);

    startSpin = new QSpinBox(this);
    startSpin->setRange(1, 12);
    startSpin->setValue(1);
    formLayout->addRow("开始节数:", startSpin);

    endSpin = new QSpinBox(this);
    endSpin->setRange(1, 12);
    endSpin->setValue(1);
    formLayout->addRow("结束节数:", endSpin);

    locationEdit = new QLineEdit(this);
    formLayout->addRow("地点(可选):", locationEdit);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *okButton = new QPushButton("确定", this);
    QPushButton *cancelButton = new QPushButton("取消", this);

    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);

    formLayout->addRow(buttonLayout);

    connect(okButton, &QPushButton::clicked, this, &QDialog::accept);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

QString CourseDialog::getCourseName() const {
    return nameEdit->text();
}

int CourseDialog::getDay() const {
    return dayCombo->currentIndex();
}

int CourseDialog::getStartSection() const {
    return startSpin->value();
}

int CourseDialog::getEndSection() const {
    return endSpin->value();
}

QString CourseDialog::getLocation() const {
    return locationEdit->text();
}

void CourseSchedule::loadUserCourses(){
    for (int row = 0; row < 12; ++row) {
        for (int col = 0; col < 7; ++col) {
            scheduleTable->item(row, col)->setText("");
        }
    }
    for(const Course& x:currentuser.courses){
        int day=-1;
        if(x.date=="Monday") day=0;
        else if(x.date=="Tuesday") day=1;
        else if(x.date=="Wednesday") day=2;
        else if(x.date=="Thursday") day=3;
        else if(x.date=="Friday") day=4;
        else if(x.date=="Saturday") day=5;
        else if(x.date=="Sunday") day=6;
        if(day==-1)continue;
        QTime st=x.starttime;
        QTime en=x.endtime;
        int stindex=-1,enindex=-1;
        for (int i=0;i<12;i++){
            if(Coursetime[i].startsWith(st.toString("HH:mm"))){
                stindex=i;
            }
            if(Coursetime[i].endsWith(en.toString("HH:mm"))){
                enindex=i;
            }
        }
        if(stindex==-1||enindex==-1)continue;
        QString str=x.name;
        if(x.location!=""){
            str+=+"\n地点: "+x.location;
        }
        for (int i=stindex;i<=enindex;i++){
            QTableWidgetItem *item = scheduleTable->item(i, day);
            item->setText(str);
            item->setData(Qt::UserRole, QVariant::fromValue<QString>(str));
        }
    }
}

CourseSchedule::CourseSchedule(DataManager& dm,User &user,QWidget *parent) : QMainWindow(parent) {
    datamanager=dm;
    currentuser=user;
    setupUI();
    createActions();
    populateTable();
    loadUserCourses();
}

CourseSchedule::~CourseSchedule() {
    delete centralWidget;
}

void CourseSchedule::setupUI() {
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    setMinimumSize(700, 550);

    mainLayout = new QVBoxLayout(centralWidget);

    buttonLayout = new QHBoxLayout();
    addButton = new QPushButton("+", this);
    addButton->setFixedSize(30, 30);

    QFont font = addButton->font();
    font.setPointSize(16);  // 字体大小
    font.setBold(true);     // 加粗
    addButton->setFont(font);

    buttonLayout->addWidget(addButton);
    buttonLayout->addStretch();

    mainLayout->addLayout(buttonLayout);

    scheduleTable = new QTableWidget(this);
    scheduleTable->setRowCount(12);
    scheduleTable->setColumnCount(7);

    QStringList days;
    days << "星期一" << "星期二" << "星期三" << "星期四" << "星期五" << "星期六" << "星期日";
    scheduleTable->setHorizontalHeaderLabels(days);

    QStringList sections;
    for (int i = 1; i <= 12; ++i) {
        sections << QString("第%1节\n"+CourseSchedule::Coursetime[i-1]).arg(i);
    }
    scheduleTable->setVerticalHeaderLabels(sections);

    scheduleTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    scheduleTable->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    scheduleTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    scheduleTable->setSelectionMode(QAbstractItemView::SingleSelection);

    mainLayout->addWidget(scheduleTable);

    connect(addButton, &QPushButton::clicked, this, &CourseSchedule::addCourse);
    connect(scheduleTable, &QTableWidget::cellClicked, this, &CourseSchedule::showCourseDetails);
    scheduleTable->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(scheduleTable, &QTableWidget::customContextMenuRequested, this, &CourseSchedule::showContextMenu);
}

void CourseSchedule::createActions() {
    contextMenu = new QMenu(this);
    deleteAction = new QAction("删除课程", this);
    contextMenu->addAction(deleteAction);

    connect(deleteAction, &QAction::triggered, this, &CourseSchedule::deleteCourse);
}

void CourseSchedule::populateTable() {
    for (int row = 0; row < 12; ++row) {
        for (int col = 0; col < 7; ++col) {
            QTableWidgetItem *item = new QTableWidgetItem();
            item->setTextAlignment(Qt::AlignCenter);
            scheduleTable->setItem(row, col, item);
        }
    }
}

bool CourseSchedule::hasTimeConflict(int day, int start, int end, const QString &name) {
    for (int i = start; i <= end; ++i) {
        QTableWidgetItem *item = scheduleTable->item(i, day);
        if (item && !item->text().isEmpty() && !item->text().startsWith(name)) {
            return true;
        }
    }
    return false;
}

void CourseSchedule::addCourse() {
    CourseDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        QString name = dialog.getCourseName();
        if (name.isEmpty()) {
            QMessageBox::warning(this, "警告", "课程名称不能为空!");
            return;
        }

        int day = dialog.getDay();
        int start = dialog.getStartSection() - 1;
        int end = dialog.getEndSection() - 1;

        if (start > end) {
            QMessageBox::warning(this, "警告", "开始节数不能大于结束节数!");
            return;
        }

        // 检查时间冲突
        if (hasTimeConflict(day, start, end, name)) {
            QMessageBox::warning(this, "警告", "课程时间冲突!");
            return;
        }

        QString location = dialog.getLocation();

        QString details = name;
        if (!location.isEmpty()) {
            details += "\n地点: " + location;
        }

        QString st=Coursetime[start].left(5);
        QString en=Coursetime[end].right(5);
        currentuser.addcourse(Course(name,QTime::fromString(st, "HH:mm"),QTime::fromString(en, "HH:mm"),week[day],location));
        datamanager.saveUser(currentuser);

        for (int i = start; i <= end; ++i) {
            QTableWidgetItem *item = scheduleTable->item(i, day);
            item->setText(details);
            item->setData(Qt::UserRole, QVariant::fromValue<QString>(details));
        }

    }
}

void CourseSchedule::showCourseDetails(int row, int column) {
    QTableWidgetItem *item = scheduleTable->item(row, column);
    if (item && !item->text().isEmpty()) {
        QMessageBox msgBox;
        msgBox.setWindowTitle("信息:");
        msgBox.setText(item->data(Qt::UserRole).toString());
        //msgBox.setMinimumSize(300, 200);  // 设置详情窗口最小尺寸
        msgBox.setFixedSize(400, 300);
        msgBox.exec();
    }
}

void CourseSchedule::showContextMenu(const QPoint &pos) {
    QTableWidgetItem *item = scheduleTable->itemAt(pos);
    if (item && !item->text().isEmpty()) {
        selectedRow = scheduleTable->rowAt(pos.y());
        selectedCol = scheduleTable->columnAt(pos.x());
        contextMenu->exec(scheduleTable->viewport()->mapToGlobal(pos));
    }
}

void CourseSchedule::deleteCourse() {
    QString courseName = scheduleTable->item(selectedRow, selectedCol)->text().split('\n')[0];

    if (QMessageBox::question(this, "确认删除",
                              QString("确定要删除课程 '%1' 吗?").arg(courseName),
                              QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        currentuser.removecoursebyname(courseName);
        // 清除所有相同名称的课程
        for (int row = 0; row < 12; ++row) {
            QTableWidgetItem *item = scheduleTable->item(row, selectedCol);
            if (item && item->text().startsWith(courseName)) {
                item->setText("");
                item->setData(Qt::UserRole, QVariant());
            }
        }
        datamanager.saveUser(currentuser);
    }
}
