#include "ChangePasswordDialog.h"

ChangePasswordDialog::ChangePasswordDialog(User& _user, QWidget *parent)
    : QDialog(parent), user(_user)
{
    //user = _user;
    setWindowTitle("Profile");

    QLabel *usernameLabel = new QLabel("用户名:");
    usernameEdit = new QLineEdit(user.username);

    QLabel *passwordLabel = new QLabel("密码:");
    passwordEdit = new QLineEdit(user.password);

    okButton = new QPushButton("确认");
    cancelButton = new QPushButton("取消");

    connect(okButton, &QPushButton::clicked, this, &ChangePasswordDialog::handleSave);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(usernameLabel);
    layout->addWidget(usernameEdit);
    layout->addWidget(passwordLabel);
    layout->addWidget(passwordEdit);
    layout->addWidget(okButton);
    layout->addWidget(cancelButton);

    setLayout(layout);
}

QString ChangePasswordDialog::getUsername() const {
    return usernameEdit->text();
}

QString ChangePasswordDialog::getPassword() const {
    return passwordEdit->text();
}

void ChangePasswordDialog::handleSave() {
    user.username = getUsername();
    user.password = getPassword();
    qDebug() << user.username << ' ' << user.password << Qt::endl;
    DataManager dm;
    dm.saveUser(user);
    accept();
}
