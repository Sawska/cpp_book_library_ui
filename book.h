#ifndef BOOK_H
#define BOOK_H

#include <string>
#include <sqlite3.h>
#include <vector>

struct Book {
     int id;
    std::string name;
     std::string author;
     int borrowed_by;
     std::string date;
    static void display();
    static void save(sqlite3* db);
    static void update_book(sqlite3* db);
    static void check_if_book_is_available(sqlite3* db);
    static void borrow_book(sqlite3* db,int borow_id);
    static void unborrow_book(sqlite3* db);
    static void change_name_for_a_book(sqlite3* db);
    static void change_date(sqlite3* db);
    static void change_author(sqlite3* db);
    static Book create_book(sqlite3* db);
};

void if_db_not_exists(sqlite3* db);
void delete_book(sqlite3* db,int id);
bool openDatabase(const std::string& dbName, sqlite3** db);
void closeDatabase(sqlite3* db);
std::vector<Book> load_books(sqlite3* db);

#endif 