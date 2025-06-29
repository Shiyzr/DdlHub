#include "mainwindow.h"
#include "LoginDialog.h"
#include "RegisterDialog.h"

#include <QApplication>
#include <QMessageBox>
#include <QDebug>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    LoginDialog loginDialog;
    DataManager dm;
    while (true) {
        LoginDialog loginDialog;

        // ✅ 连接“注册”信号
        QObject::connect(&loginDialog, &LoginDialog::registerRequested, [&]() {
            RegisterDialog regDialog;
            if (regDialog.exec() == QDialog::Accepted) {
                QString name = regDialog.getUsername();
                QString pwd = regDialog.getPassword();

                if (dm.loadUser(name).username != "") {
                    QMessageBox::warning(nullptr, "注册失败", "用户名已存在！");
                } else {
                    User newUser(name, pwd);
                    dm.saveUser(newUser);
                    QMessageBox::information(nullptr, "注册成功", "注册成功，请重新登录！");
                }
            }
        });

        if (loginDialog.exec() == QDialog::Accepted) {
            QString username = loginDialog.getUsername();
            QString password = loginDialog.getPassword();
            if (username == "") {
                QMessageBox::warning(nullptr, "登录失败", "用户名不能为空！");
                continue;
            }
            if (password == "") {
                QMessageBox::warning(nullptr, "登录失败", "密码不能为空！");
            }
            else{
                // 尝试读取用户数据并验证
                User user = dm.loadUser(username);
                if (user.username == "") {
                    QMessageBox::warning(nullptr, "登录失败", "用户不存在，请检查用户名！");
                }
                else if (user.verify(username, password)) {
                    // 登录成功，打开主窗口
                    MainWindow w(user);
                    w.show();
                    return a.exec(); // ⬅️ 进入 Qt 主事件循环
                } else {
                    QMessageBox::warning(nullptr, "登录失败", "密码错误，请重试！");
                }
            }
        } else {
            // 用户点击了“取消”
            break;
        }
    }
    return 0;
}
