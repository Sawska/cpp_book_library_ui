#ifndef USER_H
#define USER_H

#include <string>
#include <vector>
#include <sqlite3.h>
#include "book.h" 

struct User {
    int id;
    std::string username;
    std::string password;
    std::vector<Book> books;

    User create_user(sqlite3* db);
    void save_user(sqlite3* db);
    void delete_user(sqlite3* db);
    void update_user(sqlite3* db);
    void change_password(sqlite3* db);
    void change_username(sqlite3* db);
    std::vector<Book> load_borrowed_books(sqlite3* db);
    void login(sqlite3 *db);

    bool check_if_username_already_exists(sqlite3* db);
};


void create_user_db_not_exists(sqlite3* db);

#endif
