#include "useroptionswidget.h"
#include <QDebug>

UserOptionsWidget::UserOptionsWidget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    updateUserButton = new QPushButton("Update User", this);
    deleteUserButton = new QPushButton("Delete User", this);
    logoutButton = new QPushButton("Log Out", this);

    layout->addWidget(updateUserButton);
    layout->addWidget(deleteUserButton);
    layout->addWidget(logoutButton);

    connect(updateUserButton, &QPushButton::clicked, this, &UserOptionsWidget::onUpdateUserClicked);
    connect(deleteUserButton, &QPushButton::clicked, this, &UserOptionsWidget::onDeleteUserClicked);
    connect(logoutButton, &QPushButton::clicked, this, &UserOptionsWidget::onLogoutClicked);

    setLayout(layout);
}

void UserOptionsWidget::onUpdateUserClicked()
{
    qDebug() << "Update User clicked!";
    // Add code to handle user update
}

void UserOptionsWidget::onDeleteUserClicked()
{
    qDebug() << "Delete User clicked!";
    // Add code to handle user deletion
}

void UserOptionsWidget::onLogoutClicked()
{
    qDebug() << "Log Out clicked!";
    // Add code to handle user logout
}
