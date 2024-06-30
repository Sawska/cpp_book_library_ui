#include "registerform.h"
#include <QDebug>

RegisterForm::RegisterForm(User *user, sqlite3 *db, QWidget *parent)
    : QWidget(parent), m_user(user), m_db(db)
{
    setWindowTitle("Register");

    QVBoxLayout *layout = new QVBoxLayout(this);

    QLabel *usernameLabel = new QLabel("Username:", this);
    usernameEdit = new QLineEdit(this);

    QLabel *passwordLabel = new QLabel("Password:", this);
    passwordEdit = new QLineEdit(this);
    passwordEdit->setEchoMode(QLineEdit::Password);

    QLabel *confirmPasswordLabel = new QLabel("Confirm Password:", this);
    confirmPasswordEdit = new QLineEdit(this);
    confirmPasswordEdit->setEchoMode(QLineEdit::Password);

    registerButton = new QPushButton("Register", this);

    layout->addWidget(usernameLabel);
    layout->addWidget(usernameEdit);
    layout->addWidget(passwordLabel);
    layout->addWidget(confirmPasswordLabel);
    layout->addWidget(confirmPasswordEdit);
    layout->addWidget(registerButton);

    connect(registerButton, &QPushButton::clicked, this, &RegisterForm::onRegisterClicked);

    setLayout(layout);
}

void RegisterForm::onRegisterClicked()
{


}
