#include "LoginDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QLabel>
#include <QCheckBox>
#include <Qsettings>

LoginDialog::LoginDialog(QWidget *parent):QDialog(parent){
    setWindowTitle("用户登录");
    setMinimumSize(200,200);

    QLabel *userLabel = new QLabel("用户名：");
    usernameEdit = new QLineEdit;
    QHBoxLayout *userLayout = new QHBoxLayout;
    userLayout->addWidget(userLabel);
    userLayout->addWidget(usernameEdit);

    QLabel *passLabel = new QLabel("密码：   ");
    passwordEdit = new QLineEdit;
    passwordEdit->setEchoMode(QLineEdit::Password);
    QHBoxLayout *passLayout = new QHBoxLayout;
    passLayout->addWidget(passLabel);
    passLayout->addWidget(passwordEdit);

    rememberCheck = new QCheckBox("记住账号");
    QFont smallFont = rememberCheck->font();
    smallFont.setPointSize(smallFont.pointSize() - 1);
    rememberCheck->setFont(smallFont);

    loginBtn =  new QPushButton("登录");
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(rememberCheck);
    buttonLayout->addStretch();
    buttonLayout->addWidget(loginBtn);

    registerBtn = new QPushButton("注册");
    QHBoxLayout *registerLayout = new QHBoxLayout;
    registerLayout->addStretch();
    registerLayout->addWidget(registerBtn);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(userLayout);
    mainLayout->addLayout(passLayout);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addLayout(registerLayout);

    setLayout(mainLayout);
    connect(loginBtn, &QPushButton::clicked, this, [this](){
        saveSettings();
        accept();
    });

    connect(registerBtn, &QPushButton::clicked, this, [this]() {
        emit registerRequested();
    });

    loadSettings();
}

QString LoginDialog::getUsername() const { return usernameEdit->text(); }
QString LoginDialog::getPassword() const { return passwordEdit->text(); }

void LoginDialog::loadSettings() {
    QSettings settings("Project", "Entry");
    QString savedUsername = settings.value("username").toString();
    QString savedPassname = settings.value("password").toString();
    bool remember = settings.value("remember", false).toBool();

    if (remember && !savedUsername.isEmpty()) {
        usernameEdit->setText(savedUsername);
        passwordEdit->setText(savedPassname);
        rememberCheck->setChecked(true);
    }
}

void LoginDialog::saveSettings() {
    QSettings settings("Project", "Entry");

    if (rememberCheck->isChecked()) {
        settings.setValue("username", usernameEdit->text());
        settings.setValue("password", passwordEdit->text());
        settings.setValue("remember", true);
    } else {
        settings.remove("username");
        settings.remove("password");
        settings.setValue("remember", false);
    }
}


