#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QCheckBox>
#include <Qsettings>

class LoginDialog : public QDialog {
    Q_OBJECT
private:
    QLineEdit* usernameEdit;
    QLineEdit* passwordEdit;
    QCheckBox *rememberCheck;
    QPushButton *loginBtn;
    QPushButton *registerBtn;

    void loadSettings();
    void saveSettings();

public:
    explicit LoginDialog(QWidget* parent = nullptr);
    QString getUsername() const;
    QString getPassword() const;

signals:
    void registerRequested();
};

#endif // LOGINDIALOG_H
