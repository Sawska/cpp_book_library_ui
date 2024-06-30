#ifndef LOGINFORM_H
#define LOGINFORM_H

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QLabel>
#include <sqlite3.h>
#include "../user.h"

class LoginForm : public QWidget
{
    Q_OBJECT

public:
    explicit LoginForm(User *user, sqlite3 *db, QWidget *parent = nullptr);

private slots:
    void onLoginClicked();

private:
    QLineEdit *usernameEdit;
    QLineEdit *passwordEdit;
    QPushButton *loginButton;

    User *m_user;
    sqlite3 *m_db;
};

#endif // LOGINFORM_H
