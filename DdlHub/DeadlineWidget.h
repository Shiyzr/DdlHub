#ifndef DEADLINEWIDGET_H
#define DEADLINEWIDGET_H

#include <QWidget>
#include <QDateTime>
#include <QList>
#include <QVector>
#include <QString>
#include <QCheckBox>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QDateTimeEdit>
#include <QDialog>
#include "ItemWidget.h"

struct DDLItem {
    QString name;
    QDateTime deadline;
    QString type;
    QString cycle;
};

class AddDDLDialog : public QDialog {
    Q_OBJECT
public:
    AddDDLDialog(QWidget* parent = nullptr);
    //AddDDLDialog(User&user,QWidget* parent = nullptr);
    DDLItem getItem() const;
    void setItem(const DDLItem& item);
    static QString encodeCycle(const QList<QCheckBox*>& boxes);
    static QString displayCycle(const QString& cycle);
    //User currentuser_add;

private:
    QLineEdit* nameEdit;
    QDateTimeEdit* timeEdit;
    QComboBox* typeCombo;
    QWidget* cycleWidget;
    QList<QCheckBox*> weekBoxes;
};

class DDLWidget : public QWidget {
    Q_OBJECT
public:
    DDLWidget(const DDLItem& item, QWidget* parent = nullptr);
    //DDLWidget(User&user,const DDLItem& item, QWidget* parent = nullptr);
    DDLItem getItem() const { return item; }
    void setItem(const DDLItem& newItem);
    //User currentuser_ddl;

signals:
    void ddlUpdated(const DDLItem& newItem);
    void ddlDeleted();
    void ddlEditRequested();

protected:
    void mousePressEvent(QMouseEvent* event) override;

private:
    void updateUI();
    void updateBackground();

    DDLItem item;
    QLabel* infoLabel;
};

class DeadlineWidget : public QWidget {
    Q_OBJECT
public:
    DeadlineWidget(User& user,QWidget *parent = nullptr);
    User& currentuser_tot;
    QVector<DDLItem> allDDLs;
    void refreshDDLList(QVector<DDLItem> list);

private slots:
    void showAddDDLDialog();
    void onDDLUpdated(const DDLItem& newItem);
    void onDDLDeleted(DDLWidget* widget);
    void onDDLEditRequested(DDLWidget* widget);
    void filterDDL(const QString& type);
    void searchDDL(const QString& keyword);

private:
    void createButtons();
    QPushButton* createFilterButton(const QString& text);
    void addDDL(const DDLItem& item);
    void updateButtonStyles();
    QVector<DDLItem> sortDDLs(const QVector<DDLItem>& ddlList) const;

    QVBoxLayout* ddlLayout;
    QWidget* ddlListWidget;
    QHBoxLayout* mainLayout;
    QVBoxLayout* buttonLayout;
    QLineEdit* searchBox;
    QString currentFilter = "总览";
    QPushButton* overviewBtn;
    QPushButton* taskBtn;
    QPushButton* paperBtn;
    QPushButton* examBtn;
};

#endif
