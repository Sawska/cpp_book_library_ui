#include "addbookform.h"
#include <QDebug>

AddBookForm::AddBookForm(User* user,sqlite3 *db, QWidget *parent)
    : QWidget(parent), m_db(db)
{
    setWindowTitle("Add Book");

    QVBoxLayout *layout = new QVBoxLayout(this);

    QLabel *nameLabel = new QLabel("Book Name:", this);
    nameEdit = new QLineEdit(this);

    QLabel *authorLabel = new QLabel("Author:", this);
    authorEdit = new QLineEdit(this);

    QLabel *borrowedByLabel = new QLabel("Borrowed By (ID):", this);
    borrowedByEdit = new QLineEdit(this);

    QLabel *dateLabel = new QLabel("Date:", this);
    dateEdit = new QLineEdit(this);

    addBookButton = new QPushButton("Add Book", this);

    layout->addWidget(nameLabel);
    layout->addWidget(nameEdit);
    layout->addWidget(authorLabel);
    layout->addWidget(authorEdit);
    layout->addWidget(borrowedByLabel);
    layout->addWidget(borrowedByEdit);
    layout->addWidget(dateLabel);
    layout->addWidget(dateEdit);
    layout->addWidget(addBookButton);

    connect(addBookButton, &QPushButton::clicked, this, &AddBookForm::onAddBookClicked);

    setLayout(layout);
}

void AddBookForm::onAddBookClicked()
{
    // Retrieve book details from input fields
    Book newBook;
    newBook.name = nameEdit->text().toStdString();
    newBook.author = authorEdit->text().toStdString();
    newBook.borrowed_by = borrowedByEdit->text().toInt();
    newBook.date = dateEdit->text().toStdString();

    // Insert the new book into the database
    QString query = QString("INSERT INTO books (name, author, borrowed_by, date) VALUES ('%1', '%2', %3, '%4')")
                        .arg(newBook.name.c_str())
                        .arg(newBook.author.c_str())
                        .arg(newBook.borrowed_by)
                        .arg(newBook.date.c_str());

    char *errMsg = nullptr;
    int rc = sqlite3_exec(m_db, query.toStdString().c_str(), nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        qCritical() << "SQL error:" << errMsg;
        sqlite3_free(errMsg);
    } else {
        qDebug() << "Book added successfully!";
    }

    // Clear the input fields
    nameEdit->clear();
    authorEdit->clear();
    borrowedByEdit->clear();
    dateEdit->clear();
}
