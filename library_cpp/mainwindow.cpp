#include "mainwindow.h"
#include <QVBoxLayout>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    user(new User),
    db(nullptr),
    loginForm(nullptr),
    registerForm(nullptr),
    addBookForm(nullptr),
    bookListWidget(nullptr),
    userOptionsWidget(nullptr)
{
    setWindowTitle("Library");

    // Initialize the SQLite database connection
    int rc = sqlite3_open("my_db.db", &db);
    if (rc) {
        qCritical() << "Can't open database:" << sqlite3_errmsg(db);
    } else {
        qDebug() << "Opened database successfully";
    }

    loginForm = new LoginForm(user, db, this);
    registerForm = new RegisterForm(user, db, this);
    addBookForm = new AddBookForm(user,db);
    bookListWidget = new BookListWidget(db);
    userOptionsWidget = new UserOptionsWidget();

    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    infoLabel = new QLabel("Please login or register", this);
    layout->addWidget(infoLabel);

    registerButton = new QPushButton("Register", this);
    loginButton = new QPushButton("Login", this);
    addBookButton = new QPushButton("Add Book", this);
    viewBooksButton = new QPushButton("View Books", this);

    layout->addWidget(registerButton);
    layout->addWidget(loginButton);
    layout->addWidget(addBookButton);
    layout->addWidget(viewBooksButton);

    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);

    connect(registerButton, &QPushButton::clicked, this, &MainWindow::onRegisterButtonClicked);
    connect(loginButton, &QPushButton::clicked, this, &MainWindow::onLoginButtonClicked);
    connect(addBookButton, &QPushButton::clicked, this, &MainWindow::onAddBookButtonClicked);
    connect(viewBooksButton, &QPushButton::clicked, this, &MainWindow::onViewBooksButtonClicked);
}

MainWindow::~MainWindow()
{
    if (db) {
        sqlite3_close(db);
    }
    delete user;
}

void MainWindow::onRegisterButtonClicked()
{
    registerForm->show();
    qDebug() << "Register button clicked!";
}

void MainWindow::onLoginButtonClicked()
{
    loginForm->show();
    qDebug() << "Login button clicked!";
}

void MainWindow::onAddBookButtonClicked()
{
    addBookForm->show();
    qDebug() << "Add Book button clicked!";
}

void MainWindow::onViewBooksButtonClicked()
{
    bookListWidget->show();
    userOptionsWidget->show();
    qDebug() << "View Books button clicked!";
}
