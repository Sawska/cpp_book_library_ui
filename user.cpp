#include "user.h"
#include <iostream>
#include <vector>
#include <cryptopp/sha.h>
#include <cryptopp/hex.h>
#include <cryptopp/filters.h>


std::string sha256(const std::string& password) {
    CryptoPP::SHA256 hash;
    std::string digest;

    CryptoPP::StringSource(password,true,
    new CryptoPP::HashFilter(hash,
    new CryptoPP::HexEncoder(
        new CryptoPP::StringSink(digest))));

    return digest;
}

void create_user_db_not_exists(sqlite3* db) {
    std::string sql =
    "CREATE TABLE IF NOT EXISTS users ("
    "id INTEGER PRIMARY KEY, "
    "username TEXT NOT NULL, "
    "password TEXT NOT NULL"
    ");";


    char* errMsg = nullptr;

    int rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);

    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    } else {
        std::cout << "Table created successfully or already exists." << std::endl;
    }
}


bool User::check_if_username_already_exists(sqlite3* db) {
    std::string sql = "SELECT username FROM users WHERE username = ?";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare SQL: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);

    bool exists = false;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        exists = true;
    }

    sqlite3_finalize(stmt);
    return exists;
}

void User::update_user(sqlite3 *db)
{
    std::string sql = "UPDATE users SET username = ?, password = ? WHERE id = ?;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 3, id);
    
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "Error updating user: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_finalize(stmt);
}

void User::change_password(std::string new_password,sqlite3 *db)
{
    password = sha256(new_password);
    update_user(db);
}






std::vector<Book> User::load_borrowed_books(sqlite3 *db)
{

    std::vector<Book> books = std::vector<Book>();

    std::string sql = "SELECT id, name, author, date, borrowed_by_id FROM books WHERE borrowed_by_id = ?";
    sqlite3_stmt* stmt;

        int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);

        if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare SQL: " << sqlite3_errmsg(db) << std::endl;
        return books;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Book book;
        book.name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        book.author = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        book.date = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        book.borrowed_by = sqlite3_column_int(stmt, 3);

        books.push_back(book);
    }

    sqlite3_finalize(stmt);
    

    return books;
}


User User::login(sqlite3* db) {
    
    std::vector<Book> books; 

    
    std::string sql =
    "SELECT "
    "    users.id, users.username, "
    "    books.id AS book_id, books.name AS book_name, "
    "    books.date AS book_date, books.author AS book_author "
    "FROM "
    "    users "
    "LEFT JOIN "
    "    books ON users.id = books.borrowed_by_id "
    "WHERE "
    "    users.username = ? AND users.password = ?";


    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);

    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare SQL statement: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);

    if (rc == SQLITE_ROW) {
        
        int userId = sqlite3_column_int(stmt, 0); 
        std::string fetchedUsername(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));

        std::cout << "Login successful! User ID: " << userId << ", Username: " << fetchedUsername << std::endl;

        do {
            int bookId = sqlite3_column_int(stmt, 2); // Fetch book ID as int
            std::string bookName(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3))); 
            std::string bookDate(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4))); 
            std::string bookAuthor(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5))); 

            Book book = { bookId, bookName, bookDate, userId,bookAuthor };
            books.push_back(book);
        } while (sqlite3_step(stmt) == SQLITE_ROW); 

        
        for (const auto& book : books) {
            std::cout << "Book ID: " << book.id << ", Name: " << book.name << ", Author: " << book.author << std::endl;
        }

    } else {
        std::cout << "Login failed. Invalid username or password." << std::endl;
    }

    sqlite3_finalize(stmt); 
}



User User::create_user(std::string password,std::string username,sqlite3 *db) {
    User user;
    

    user.password = sha256(password);
    user.save_user(db);

    user.id = sqlite3_last_insert_rowid(db);
    return user;
}
   

void User::save_user(sqlite3 *db) {
    std::string sql = "INSERT INTO users (username, password) VALUES (?, ?);";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "Error inserting user: " << sqlite3_errmsg(db) << std::endl;
    }
    sqlite3_finalize(stmt);
}




void User::delete_user(sqlite3 *db)
{
    std::string sql = "DELETE FROM users WHERE id = ?;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, id);
    
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "Error deleting user: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_finalize(stmt);
}





