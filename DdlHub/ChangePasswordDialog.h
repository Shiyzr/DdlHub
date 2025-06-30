#ifndef CHANGEPASSWORDDIALOG_H
#define CHANGEPASSWORDDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include "ItemWidget.h"

class ChangePasswordDialog : public QDialog
{
    Q_OBJECT

public:
    ChangePasswordDialog(User& _user, QWidget *parent = nullptr);
    QString getUsername() const;
    QString getPassword() const;
    User& user;
    void handleSave();

private:
    QLineEdit *usernameEdit;
    QLineEdit *passwordEdit;
    QPushButton *okButton;
    QPushButton *cancelButton;
};

#endif // CHANGEPASSWORDDIALOG_H
