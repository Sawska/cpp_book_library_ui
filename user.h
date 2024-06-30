#ifndef USER_H
#define USER_H

#include <string>
#include <vector>
#include <sqlite3.h>
#include "book.h" 

struct User {
    static int id;
    static std::string username;
    static std::string password;
    static std::vector<Book> books;

    User static create_user(sqlite3* db);
    void static  save_user(sqlite3* db);
    void static  delete_user(sqlite3* db);
    void static  update_user(sqlite3* db);
    void static  change_password(sqlite3* db);
    void static  change_username(sqlite3* db);
    std::vector<Book> load_borrowed_books(sqlite3* db);
    User static  login(sqlite3 *db);

    bool static  check_if_username_already_exists(sqlite3* db);
};


void create_user_db_not_exists(sqlite3* db);

#endif
