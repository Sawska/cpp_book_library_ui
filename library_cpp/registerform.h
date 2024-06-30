#ifndef REGISTERFORM_H
#define REGISTERFORM_H

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QLabel>
#include <sqlite3.h>
#include "../user.h"

class RegisterForm : public QWidget
{
    Q_OBJECT

public:
    explicit RegisterForm(User *user, sqlite3 *db, QWidget *parent = nullptr);

private slots:
    void onRegisterClicked();

private:
    QLineEdit *usernameEdit;
    QLineEdit *passwordEdit;
    QLineEdit *confirmPasswordEdit;
    QPushButton *registerButton;

    User *m_user;
    sqlite3 *m_db;
};

#endif // REGISTERFORM_H
