#include "DeadlineWidget.h"
#include <QFormLayout>
#include <QMessageBox>
#include <QMouseEvent>
#include <QDebug>
#include <QStyle>
#include <algorithm>

//////// AddDDLDialog ////////

AddDDLDialog::AddDDLDialog(QWidget* parent) : QDialog(parent) {
    //currentuser_add=user;
    setWindowTitle("添加DDL");
    resize(400, 300);

    QFormLayout* layout = new QFormLayout(this);
    layout->setSpacing(15);

    nameEdit = new QLineEdit;
    nameEdit->setMinimumWidth(250);
    layout->addRow("名称:", nameEdit);

    QDateTime now = QDateTime::currentDateTime();
    QDateTime endOfDay = QDateTime(now.date(), QTime(23, 59));
    timeEdit = new QDateTimeEdit(endOfDay);
    timeEdit->setDisplayFormat("yyyy-MM-dd HH:mm");
    timeEdit->setCalendarPopup(true);
    timeEdit->setMinimumWidth(250);
    layout->addRow("截止时间:", timeEdit);

    typeCombo = new QComboBox;
    typeCombo->addItems({"任务", "论文", "考试"});
    typeCombo->setMinimumWidth(250);
    layout->addRow("类型:", typeCombo);

    cycleWidget = new QWidget;
    QHBoxLayout* cycleLayout = new QHBoxLayout(cycleWidget);
    cycleLayout->setSpacing(5);
    QStringList days = {"周一", "周二", "周三", "周四", "周五", "周六", "周日"};
    for (const QString& day : days) {
        QCheckBox* box = new QCheckBox(day);
        weekBoxes.append(box);
        cycleLayout->addWidget(box);
    }
    layout->addRow("周期:", cycleWidget);


    QPushButton* okBtn = new QPushButton("确定");
    QPushButton* cancelBtn = new QPushButton("取消");
    QHBoxLayout* btnLayout = new QHBoxLayout;
    btnLayout->addWidget(okBtn);
    btnLayout->addWidget(cancelBtn);
    layout->addRow(btnLayout);

    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
    connect(okBtn, &QPushButton::clicked, this, [this]() {
        if (nameEdit->text().trimmed().isEmpty()) {
            QMessageBox::warning(this, "错误", "请输入DDL名称！");
            return;
        }
        accept();
    });
    //
}

DDLItem AddDDLDialog::getItem() const {
    return {
        nameEdit->text().trimmed(),
        timeEdit->dateTime(),
        typeCombo->currentText(),
        encodeCycle(weekBoxes)
    };
}

void AddDDLDialog::setItem(const DDLItem& item) {
    nameEdit->setText(item.name);
    timeEdit->setDateTime(item.deadline);
    int typeIndex = typeCombo->findText(item.type);
    if (typeIndex >= 0) typeCombo->setCurrentIndex(typeIndex);

    QStringList cycleDays = item.cycle.split(",", Qt::SkipEmptyParts);
    for (QCheckBox* box : weekBoxes)
        box->setChecked(cycleDays.contains(box->text()));
}

QString AddDDLDialog::encodeCycle(const QList<QCheckBox*>& boxes) {
    QStringList days;
    for (QCheckBox* box : boxes)
        if (box->isChecked())
            days << box->text();
    if (days.isEmpty()) return "NoCycle";
    if (days.size() == 7) return "Daily";
    return days.join(",");
}

QString AddDDLDialog::displayCycle(const QString& cycle) {
    if (cycle == "NoCycle") return "/";
    if (cycle == "Daily") return "每天";
    return cycle;
}

//////// DDLWidget ////////

DDLWidget::DDLWidget(const DDLItem& item, QWidget* parent)
    : QWidget(parent), item(item)
{
    //currentuser_ddl=user;
    QVBoxLayout* layout = new QVBoxLayout(this);
    infoLabel = new QLabel;
    layout->addWidget(infoLabel);

    updateUI();
    updateBackground();

    setAutoFillBackground(true);
    setAttribute(Qt::WA_StyledBackground, true);
}

void DDLWidget::updateUI() {
    qint64 secs = QDateTime::currentDateTime().secsTo(item.deadline);
    QString remainStr;
    if (secs <= 0){
        remainStr = "已到期";
    }
    else {
        int days = secs / (3600 * 24);
        int hours = (secs / 3600) % 24;
        int mins = (secs / 60) % 60;
        remainStr = QString("%1天%2时%3分").arg(days).arg(hours).arg(mins);
    }

    QString dayOfWeek = item.deadline.toString("dddd");
    QString cycleDisplay = AddDDLDialog::displayCycle(item.cycle);

    QFont font;
    font.setBold(true);
    infoLabel->setFont(font);
    infoLabel->setText(
        QString("名称: %1\n类型: %2\n周期: %3\n截止: %4\n剩余: %5")
            .arg(item.name)
            .arg(item.type)
            .arg(cycleDisplay)
            .arg(item.deadline.toString("yyyy-MM-dd HH:mm"))
            .arg(remainStr)
        );
}

void DDLWidget::updateBackground() {
    qint64 secsLeft = QDateTime::currentDateTime().secsTo(item.deadline);
    QColor bg;
    if (secsLeft > 3600 * 24) bg = QColor(144, 238, 144);
    else if (secsLeft > 3600) bg = QColor(255, 255, 102);
    else bg = QColor(255, 102, 102);
    QPalette pal = palette();
    pal.setColor(QPalette::Window, bg);
    pal.setColor(QPalette::WindowText, Qt::black);
    setPalette(pal);
}

void DDLWidget::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        emit ddlEditRequested();
    } else if (event->button() == Qt::RightButton) {
        if (QMessageBox::question(this, "删除", "确定要删除该DDL？") == QMessageBox::Yes) {
            emit ddlDeleted();

        }
    }
}

void DDLWidget::setItem(const DDLItem& newItem) {
    item = newItem;
    updateUI();
    updateBackground();
    emit ddlUpdated(newItem);
}

//////// DeadlineWidget ////////

DeadlineWidget::DeadlineWidget(User& user,QWidget *parent) : QWidget(parent), currentuser_tot(user) {
    mainLayout = new QHBoxLayout(this);
    //currentuser_tot=user;

    // 左侧列表
    ddlListWidget = new QWidget;
    ddlLayout = new QVBoxLayout(ddlListWidget);
    ddlLayout->setAlignment(Qt::AlignTop);

    QScrollArea* scroll = new QScrollArea;
    scroll->setWidgetResizable(true);
    scroll->setWidget(ddlListWidget);

    QVBoxLayout* leftLayout = new QVBoxLayout;
    searchBox = new QLineEdit;
    searchBox->setPlaceholderText("搜索DDL名称...");
    connect(searchBox, &QLineEdit::textChanged, this, &DeadlineWidget::searchDDL);
    leftLayout->addWidget(searchBox);
    leftLayout->addWidget(scroll);

    QWidget* leftPanel = new QWidget;
    leftPanel->setLayout(leftLayout);
    mainLayout->addWidget(leftPanel, 4);

    // 右侧按钮
    QWidget* buttonPanel = new QWidget;
    buttonLayout = new QVBoxLayout(buttonPanel);
    buttonLayout->setAlignment(Qt::AlignTop);

    createButtons();
    mainLayout->addWidget(buttonPanel, 1);

    for (int i=0;i<currentuser_tot.tasks.length();i++) {
        Task task=currentuser_tot.tasks[i];
        if(task.title!=""){
            // 转换 TaskType 为字符串
            QString typeStr;
            switch (task.type) {
            case Assignment: typeStr = "任务"; break;
            case Paper:     typeStr = "论文"; break;
            case Exam:      typeStr = "考试"; break;
            }

            // 转换 QList<int> cycle 为 QString（如 "周一,周三"）
            QStringList cycleDays;
            for (int dayIndex : task.cycle) {
                cycleDays << QString("周%1").arg(QString::fromUtf8("一二三四五六日")[dayIndex]);
            }
            QString cycle = cycleDays.isEmpty() ? "NoCycle" : cycleDays.join(",");

            if(task.cycle.size()==7){
                cycle="Daily";
            }

            // 添加到 allDDLs
            allDDLs.append({
                task.title,      // 名称
                task.deadline,    // 截止时间
                typeStr,          // 类型（"任务"/"论文"/"考试"）
                cycle             // 周期（如 "周一,周三"）
            });
        }
        else{
            break;
        }
    }


    // 默认显示总览
    filterDDL("总览");
}

QVector<DDLItem> DeadlineWidget::sortDDLs(const QVector<DDLItem>& ddlList) const {
    QVector<DDLItem> sortedList = ddlList;

    // 使用 std::sort 进行排序
    std::sort(sortedList.begin(), sortedList.end(), [](const DDLItem& a, const DDLItem& b) {
        qint64 secsA = QDateTime::currentDateTime().secsTo(a.deadline);
        qint64 secsB = QDateTime::currentDateTime().secsTo(b.deadline);

        // 已到期的排在最后
        if (secsA <= 0 && secsB <= 0) return false; // 两个都已到期，保持原顺序
        if (secsA <= 0) return false; // a已到期，a应该排在后面
        if (secsB <= 0) return true;  // b已到期，a应该排在前面

        // 其他情况按剩余时间从小到大排序
        return secsA < secsB;
    });

    return sortedList;
}

void DeadlineWidget::createButtons() {
    QPushButton* addBtn = new QPushButton("添加DDL");
    addBtn->setFixedSize(130, 40);
    QFont font;
    font.setBold(true);
    addBtn->setFont(font);
    connect(addBtn, &QPushButton::clicked, this, &DeadlineWidget::showAddDDLDialog);
    buttonLayout->addWidget(addBtn);
    buttonLayout->addSpacing(10);

    overviewBtn = createFilterButton("总览");
    taskBtn = createFilterButton("任务");
    paperBtn = createFilterButton("论文");
    examBtn = createFilterButton("考试");

    buttonLayout->addWidget(overviewBtn);
    buttonLayout->addSpacing(10);
    buttonLayout->addWidget(taskBtn);
    buttonLayout->addSpacing(10);
    buttonLayout->addWidget(paperBtn);
    buttonLayout->addSpacing(10);
    buttonLayout->addWidget(examBtn);
    buttonLayout->addSpacing(10);

    overviewBtn->setStyleSheet("background-color: #4CAF50; color: white;");
}

QPushButton* DeadlineWidget::createFilterButton(const QString& text) {
    QPushButton* btn = new QPushButton(text);
    btn->setFixedSize(130, 40);
    QFont font;
    font.setBold(true);
    btn->setFont(font);
    connect(btn, &QPushButton::clicked, this, [this, text]() {
        currentFilter = text;
        filterDDL(text);
        updateButtonStyles();
    });
    return btn;
}

void DeadlineWidget::updateButtonStyles() {
    overviewBtn->setStyleSheet("");
    taskBtn->setStyleSheet("");
    paperBtn->setStyleSheet("");
    examBtn->setStyleSheet("");

    if (currentFilter == "总览") {
        overviewBtn->setStyleSheet("background-color: #4CAF50; color: white;");
    } else if (currentFilter == "任务") {
        taskBtn->setStyleSheet("background-color: #4CAF50; color: white;");
    } else if (currentFilter == "论文") {
        paperBtn->setStyleSheet("background-color: #4CAF50; color: white;");
    } else if (currentFilter == "考试") {
        examBtn->setStyleSheet("background-color: #4CAF50; color: white;");
    }
}

void DeadlineWidget::showAddDDLDialog() {
    AddDDLDialog dialog(this);
    while (true) {
        if (dialog.exec() != QDialog::Accepted)
            return;

        DDLItem item = dialog.getItem();
        QDateTime now = QDateTime::currentDateTime();

        if (item.name.isEmpty()) {
            QMessageBox::warning(this, "错误", "DDL名称不能为空！");
            continue;
        }

        bool nameExists = std::any_of(allDDLs.begin(), allDDLs.end(),
                                      [&item](const DDLItem& existing) {
                                          return existing.name == item.name;
                                      });

        if (nameExists) {
            QMessageBox::warning(this, "错误", "该DDL名称已存在，请使用其他名称！");
            continue;
        }

        if (item.deadline <= now) {
            QMessageBox::warning(this, "错误", "截止时间必须晚于当前时间！");
            continue;
        }

        addDDL(item);
        break;
    }
}

void DeadlineWidget::addDDL(const DDLItem& item) {


    TaskType type;
    if (item.type == "任务")      type = Assignment;
    else if (item.type == "论文") type = Paper;
    else                          type = Exam;

    // 转换周期字符串（如 "周一,周三"）为 QList<int>
    QList<int> cycle;
    QStringList days = item.cycle.split(",", Qt::SkipEmptyParts);
    for (const QString& day : days) {
        if (day == QString::fromUtf8("周一")) cycle << 0;
        else if (day == QString::fromUtf8("周二")) cycle << 1;
        else if (day == QString::fromUtf8("周三")) cycle << 2;
        else if (day == QString::fromUtf8("周四")) cycle << 3;
        else if (day == QString::fromUtf8("周五")) cycle << 4;
        else if (day == QString::fromUtf8("周六")) cycle << 5;
        else if (day == QString::fromUtf8("周日")) cycle << 6;
        else if (day == "Daily")cycle<<0<<1<<2<<3<<4<<5<<6;
    }
    qDebug()<<cycle;

    // 添加到 User::tasks
    Task newTask(item.name, item.deadline, 0, type, cycle, 1, 5);
    currentuser_tot.addtask(newTask);
    for (Task t: currentuser_tot.tasks) {
        qDebug() << t.title << Qt::endl;
    }
    qDebug() << '----' << Qt::endl;
    DataManager dm;
    dm.saveUser(currentuser_tot);

    allDDLs.append(item);
    filterDDL(currentFilter);
}

void DeadlineWidget::filterDDL(const QString& type) {
    QVector<DDLItem> filtered;
    for (const DDLItem& item : allDDLs) {
        if (type == "总览" || item.type == type)
            filtered.append(item);
    }
    refreshDDLList(filtered);
}

void DeadlineWidget::searchDDL(const QString& keyword) {
    QVector<DDLItem> filtered;
    for (const DDLItem& item : allDDLs) {
        bool typeMatch = (currentFilter == "总览" || item.type == currentFilter);
        bool nameMatch = item.name.contains(keyword, Qt::CaseInsensitive);
        if (typeMatch && nameMatch) {
            filtered.append(item);
        }
    }
    refreshDDLList(filtered);
}

void DeadlineWidget::refreshDDLList(QVector<DDLItem> list) {
    // 先对列表进行排序
    list = sortDDLs(list);

    // 清除现有布局
    QLayoutItem* child;
    while ((child = ddlLayout->takeAt(0))) {
        if (child->widget()) {
            child->widget()->deleteLater();
        }
        delete child;
    }

    // 添加新的DDL项
    for (const DDLItem& item : list) {
        DDLWidget* w = new DDLWidget(item);
        connect(w, &DDLWidget::ddlUpdated, this, &DeadlineWidget::onDDLUpdated);
        connect(w, &DDLWidget::ddlDeleted, this, [this, w]() {
            onDDLDeleted(w);
        });
        connect(w, &DDLWidget::ddlEditRequested, this, [this, w]() {
            onDDLEditRequested(w);
        });
        ddlLayout->addWidget(w);
    }
}

void DeadlineWidget::onDDLUpdated(const DDLItem& newItem) {
    for (int i = 0; i < allDDLs.size(); ++i) {
        if (allDDLs[i].name == newItem.name && allDDLs[i].type == newItem.type) {
            allDDLs[i] = newItem;
            break;
        }
    }

    for (Task& task : currentuser_tot.tasks) {
        if (task.title == newItem.name) {
            // 更新 Task 属性（类似 addDDL 的转换逻辑）
            if (newItem.type == "任务")      task.type = Assignment;
            else if (newItem.type == "论文") task.type = Paper;
            else                            task.type = Exam;

            task.deadline = newItem.deadline;

            QList<int> cycle;
            QStringList days = newItem.cycle.split(",", Qt::SkipEmptyParts);
            for (const QString& day : days) {
                if (day == QString::fromUtf8("周一")) cycle << 0;
                else if (day == QString::fromUtf8("周二")) cycle << 1;
                else if (day == QString::fromUtf8("周三")) cycle << 2;
                else if (day == QString::fromUtf8("周四")) cycle << 3;
                else if (day == QString::fromUtf8("周五")) cycle << 4;
                else if (day == QString::fromUtf8("周六")) cycle << 5;
                else if (day == QString::fromUtf8("周日")) cycle << 6;
                else if (day == "Daily")cycle<<0<<1<<2<<3<<4<<5<<6;
            }

            task.cycle = cycle;
            break;
        }
    }

    // // 3. 保存到文件/数据库
    DataManager dm;
    dm.saveUser(currentuser_tot);

    filterDDL(currentFilter);
}

void DeadlineWidget::onDDLDeleted(DDLWidget* widget) {
    if (!widget) return;

    DDLItem item = widget->getItem();
    allDDLs.erase(std::remove_if(allDDLs.begin(), allDDLs.end(),
                                 [&item](const DDLItem& i) {
                                     return i.name == item.name && i.type == item.type;
                                 }), allDDLs.end());
    qDebug()<<item.name;
    int i = -1;
    for (int j = 0; j < currentuser_tot.tasks.size(); ++j) {
        if (currentuser_tot.tasks[j].title == item.name) {
            i = j;
            break;
        }
    }
    if(i!=-1 && currentuser_tot.tasks.length()>=2){
        currentuser_tot.tasks.removeAt(i);
    }
    else{
        currentuser_tot.tasks.clear();
    }
    DataManager dm;
    dm.saveUser(currentuser_tot);

    filterDDL(currentFilter);
}

void DeadlineWidget::onDDLEditRequested(DDLWidget* widget) {
    if (!widget) return;

    AddDDLDialog dialog(this);
    dialog.setItem(widget->getItem());

    while (true) {
        if (dialog.exec() != QDialog::Accepted)
            return;

        DDLItem newItem = dialog.getItem();
        QDateTime now = QDateTime::currentDateTime();

        if (newItem.name.isEmpty()) {
            QMessageBox::warning(this, "错误", "DDL名称不能为空！");
            continue;
        }

        bool nameExists = false;
        for (const DDLItem& existing : allDDLs) {
            if (existing.name == newItem.name &&
                !(existing.name == widget->getItem().name && existing.type == widget->getItem().type)) {
                nameExists = true;
                break;
            }
        }

        if (nameExists) {
            QMessageBox::warning(this, "错误", "该DDL名称已存在，请使用其他名称！");
            continue;
        }

        if (newItem.deadline <= now) {
            QMessageBox::warning(this, "错误", "截止时间必须晚于当前时间！");
            continue;
        }

        // 更新数据
        for (int i = 0; i < allDDLs.size(); ++i) {
            if (allDDLs[i].name == widget->getItem().name &&
                allDDLs[i].type == widget->getItem().type) {
                allDDLs[i] = newItem;
                break;
            }
        }

        widget->setItem(newItem);
        filterDDL(currentFilter);
        break;
    }
}
