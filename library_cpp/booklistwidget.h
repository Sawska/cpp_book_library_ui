#ifndef BOOKLISTWIDGET_H
#define BOOKLISTWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QVBoxLayout>
#include <sqlite3.h>

class BookListWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BookListWidget(sqlite3 *db, QWidget *parent = nullptr);

private:
    QTableWidget *table;
    sqlite3 *m_db;

    void loadBooks();
};

#endif // BOOKLISTWIDGET_H
