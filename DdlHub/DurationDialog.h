#ifndef DURATIONDIALOG_H
#define DURATIONDIALOG_H

#include <QDialog>

class QSpinBox;

class DurationDialog : public QDialog {
    Q_OBJECT
public:
    DurationDialog(QWidget *parent = nullptr);
    int workMinutes() const;
    int breakMinutes() const;

private:
    QSpinBox *m_workSpinBox;
    QSpinBox *m_breakSpinBox;
};

#endif // DURATIONDIALOG_H
