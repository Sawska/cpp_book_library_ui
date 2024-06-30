#include "loginform.h"
#include <QDebug>

LoginForm::LoginForm(User *user, sqlite3 *db, QWidget *parent)
    : QWidget(parent), m_user(user), m_db(db)
{
    setWindowTitle("Login");

    QVBoxLayout *layout = new QVBoxLayout(this);

    QLabel *usernameLabel = new QLabel("Username:", this);
    usernameEdit = new QLineEdit(this);

    QLabel *passwordLabel = new QLabel("Password:", this);
    passwordEdit = new QLineEdit(this);
    passwordEdit->setEchoMode(QLineEdit::Password);

    loginButton = new QPushButton("Login", this);

    layout->addWidget(usernameLabel);
    layout->addWidget(usernameEdit);
    layout->addWidget(passwordLabel);
    layout->addWidget(passwordEdit);
    layout->addWidget(loginButton);

    connect(loginButton, &QPushButton::clicked, this, &LoginForm::onLoginClicked);

    setLayout(layout);
}

void LoginForm::onLoginClicked()
{



}
