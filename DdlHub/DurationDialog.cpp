#include "DurationDialog.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QSpinBox>
#include <QPushButton>
#include <QDialogButtonBox>

DurationDialog::DurationDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("设置时间");

    m_workSpinBox = new QSpinBox(this);
    m_workSpinBox->setRange(1, 180);
    m_workSpinBox->setValue(25);

    m_breakSpinBox = new QSpinBox(this);
    m_breakSpinBox->setRange(1, 60);
    m_breakSpinBox->setValue(5);

    auto *layout = new QVBoxLayout(this);
    layout->addWidget(new QLabel("工作时长（分钟）:", this));
    layout->addWidget(m_workSpinBox);
    layout->addWidget(new QLabel("休息时长（分钟）:", this));
    layout->addWidget(m_breakSpinBox);

    QDialogButtonBox *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
    layout->addWidget(buttons);
}

int DurationDialog::workMinutes() const {
    return m_workSpinBox->value();
}

int DurationDialog::breakMinutes() const {
    return m_breakSpinBox->value();
}
