#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <sqlite3.h>
#include "loginform.h"
#include "registerform.h"
#include "addbookform.h"
#include "booklistwidget.h"
#include "useroptionswidget.h"
#include "../user.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onRegisterButtonClicked();
    void onLoginButtonClicked();
    void onAddBookButtonClicked();
    void onViewBooksButtonClicked();

private:
    QPushButton *registerButton;
    QPushButton *loginButton;
    QPushButton *addBookButton;
    QPushButton *viewBooksButton;
    QLabel *infoLabel;

    LoginForm *loginForm;
    RegisterForm *registerForm;
    AddBookForm *addBookForm;
    BookListWidget *bookListWidget;
    UserOptionsWidget *userOptionsWidget;

    User *user;
    sqlite3 *db;
};

#endif // MAINWINDOW_H
