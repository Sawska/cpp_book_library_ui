#ifndef ADDBOOKFORM_H
#define ADDBOOKFORM_H

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QLabel>
#include <sqlite3.h>
#include "../book.h"
#include "../user.h"

class AddBookForm : public QWidget
{
    Q_OBJECT

public:
    explicit AddBookForm(User* user,sqlite3 *db, QWidget *parent = nullptr);

private slots:
    void onAddBookClicked();

private:
    QLineEdit *nameEdit;
    QLineEdit *authorEdit;
    QLineEdit *borrowedByEdit;
    QLineEdit *dateEdit;
    QPushButton *addBookButton;

    User *user;
    sqlite3 *m_db;
};

#endif // ADDBOOKFORM_H
