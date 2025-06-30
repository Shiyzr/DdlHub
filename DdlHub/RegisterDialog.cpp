#include "registerdialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>

RegisterDialog::RegisterDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("注册");

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // 用户名
    QHBoxLayout *userLayout = new QHBoxLayout;
    userLayout->addWidget(new QLabel("用户名：   "));
    usernameEdit = new QLineEdit;
    userLayout->addWidget(usernameEdit);
    mainLayout->addLayout(userLayout);

    // 密码
    QHBoxLayout *pwdLayout = new QHBoxLayout;
    pwdLayout->addWidget(new QLabel("密码:         "));
    passwordEdit = new QLineEdit;
    passwordEdit->setEchoMode(QLineEdit::Password);
    pwdLayout->addWidget(passwordEdit);
    mainLayout->addLayout(pwdLayout);

    // 确认密码
    QHBoxLayout *confirmLayout = new QHBoxLayout;
    confirmLayout->addWidget(new QLabel("确认密码："));
    confirmEdit = new QLineEdit;
    confirmEdit->setEchoMode(QLineEdit::Password);
    confirmLayout->addWidget(confirmEdit);
    mainLayout->addLayout(confirmLayout);

    // 按钮
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    QPushButton *registerButton = new QPushButton("注册");
    QPushButton *cancelButton = new QPushButton("取消");
    buttonLayout->addWidget(registerButton);
    buttonLayout->addWidget(cancelButton);
    mainLayout->addLayout(buttonLayout);

    connect(registerButton, &QPushButton::clicked, this, &RegisterDialog::onRegisterClicked);
    connect(cancelButton, &QPushButton::clicked, this, &RegisterDialog::reject);
}

QString RegisterDialog::getUsername() const {
    return usernameEdit->text();
}

QString RegisterDialog::getPassword() const {
    return passwordEdit->text();
}

void RegisterDialog::onRegisterClicked() {
    QString username = usernameEdit->text().trimmed();
    QString password = passwordEdit->text();
    QString confirm = confirmEdit->text();

    if (username.isEmpty() || password.isEmpty() || confirm.isEmpty()) {
        QMessageBox::warning(this, "提示", "所有字段均不能为空！");
        return;
    }

    if (password != confirm) {
        QMessageBox::warning(this, "提示", "两次密码输入不一致！");
        return;
    }

    // 校验通过
    accept(); // 关闭对话框并返回 QDialog::Accepted
}
