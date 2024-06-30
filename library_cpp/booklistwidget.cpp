#include "booklistwidget.h"
#include <QHeaderView>
#include <QDebug>

BookListWidget::BookListWidget(sqlite3 *db, QWidget *parent)
    : QWidget(parent), m_db(db)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    table = new QTableWidget(this);
    table->setColumnCount(4);
    table->setHorizontalHeaderLabels({"ID", "Name", "Author", "Borrowed By"});
    table->horizontalHeader()->setStretchLastSection(true);

    layout->addWidget(table);
    setLayout(layout);

    loadBooks();
}

void BookListWidget::loadBooks()
{
    const char *sql = "SELECT id, name, author, borrowed_by FROM books";
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(m_db, sql, -1, &stmt, nullptr);

    if (rc != SQLITE_OK) {
        qCritical() << "Cannot prepare statement:" << sqlite3_errmsg(m_db);
        return;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int row = table->rowCount();
        table->insertRow(row);

        int id = sqlite3_column_int(stmt, 0);
        const char *name = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
        const char *author = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2));
        int borrowedBy = sqlite3_column_int(stmt, 3);

        table->setItem(row, 0, new QTableWidgetItem(QString::number(id)));
        table->setItem(row, 1, new QTableWidgetItem(name));
        table->setItem(row, 2, new QTableWidgetItem(author));
        table->setItem(row, 3, new QTableWidgetItem(QString::number(borrowedBy)));
    }

    sqlite3_finalize(stmt);
}
