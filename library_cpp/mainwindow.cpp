#include "mainwindow.h"
#include "../user.h"
#include "../book.h"

sqlite3* db;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Library");

    // Create buttons
    registerButton = new QPushButton("Register", this);
    registerButton->setGeometry(50, 50, 100, 50);  // Adjust position and size as needed

    loginButton = new QPushButton("Login", this);
    loginButton->setGeometry(200, 50, 100, 50);  // Adjust position and size as needed


    connect(registerButton, &QPushButton::clicked, this, &MainWindow::onRegisterButtonClicked);
    connect(loginButton, &QPushButton::clicked, this, &MainWindow::onLoginButtonClicked);
}

MainWindow::~MainWindow()
{
    // Cleanup if necessary
}

void MainWindow::onRegisterButtonClicked()
{


}

void MainWindow::onLoginButtonClicked()
{

    qDebug() << "Login button clicked!";
}
