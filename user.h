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
    User create_user(std::string password,std::string username,sqlite3 *db);
    void   save_user(sqlite3* db);
    void   delete_user(sqlite3* db);
    void   update_user(sqlite3* db);
    std::vector<Book> load_borrowed_books(sqlite3* db);
    User   login(sqlite3 *db);
    void change_password(std::string new_password,sqlite3 *db);

    bool   check_if_username_already_exists(sqlite3* db);
};


void create_user_db_not_exists(sqlite3* db);

#endif
