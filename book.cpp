#include "book.h"
#include <iostream>
#include <vector>
#include "user.h"
#include <cryptopp/sha.h>
#include <cryptopp/hex.h>
#include <cryptopp/filters.h>





bool openDatabase(const std::string& dbName, sqlite3** db) {
    return sqlite3_open(dbName.c_str(), db) == SQLITE_OK;
}

void closeDatabase(sqlite3* db) {
    sqlite3_close(db);
}

std::vector<Book> load_books(sqlite3* db) {
    std::vector<Book> books;

    std::string sql = "SELECT name, author, date, borrowed_by_id FROM books;";
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

void if_db_not_exists(sqlite3* db) {
    std::string sql = 
    "CREATE TABLE IF NOT EXISTS books ("
    "id INTEGER PRIMARY KEY, "
    "name TEXT NOT NULL, "
    "date TEXT, "
    "author TEXT, "
    "borrowed_by_id INTEGER, "
    "FOREIGN KEY (borrowed_by_id) REFERENCES users(id)"
    ");";


    char* errMsg = nullptr;

    int rc = sqlite3_exec(db,sql.c_str(),nullptr,nullptr,&errMsg);

    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    } else {
        std::cout << "Table created successfully or already exists." << std::endl;
    }
}

void Book::display() {
    std::cout << "serial number " << id << name << " by " << author << ", Published on: " << date << std::endl;
}

void Book::save(sqlite3* db) {
    std::string sql = "INSERT INTO books (name, author, date, borrowed_by_id) VALUES (?, ?, ?, ?);";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, author.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, date.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 4, borrowed_by);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "Error inserting Book: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_finalize(stmt);
}


Book Book::create_book(sqlite3* db)
{
    Book book;

    std::string original_name;
    std::string original_author;
    std::string original_date;

    std::cout << "Enter book name: ";
    std::getline(std::cin, original_name);

    book.name = original_name;

    std::cout << "Enter author: ";
    std::getline(std::cin, original_author);

    book.author = original_author;

    std::cout << "Enter date: ";
    std::getline(std::cin, original_date);

    book.date = original_date;

    book.borrowed_by = -1;

    
    book.save(db);

    
    book.id = sqlite3_last_insert_rowid(db);

    return book;
}


void Book::update_book(sqlite3* db) {
    std::string sql = "UPDATE books SET name = ?, author = ?, date = ?, borrowed_by_id = ? WHERE id = ?;";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare update SQL: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, author.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, date.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 4, borrowed_by);
    sqlite3_bind_int(stmt, 5, id);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::cerr << "Failed to update book: " << sqlite3_errmsg(db) << std::endl;
    } else {
        std::cout << "Book updated successfully." << std::endl;
    }

    sqlite3_finalize(stmt);
}


void Book::check_if_book_is_available(sqlite3* db) {
    std::cout << "Checking if book is available. Borrowed by ID: " << borrowed_by << std::endl;

    if (borrowed_by == -1) {
        std::cout << "Book is available to borrow" << std::endl;
    } else {
        std::string user_name_sql = "SELECT id, username FROM users WHERE id = ?";
        sqlite3_stmt* user_stmt;
        int rc = sqlite3_prepare_v2(db, user_name_sql.c_str(), -1, &user_stmt, nullptr);
        if (rc != SQLITE_OK) {
            std::cerr << "Failed to prepare SQL: " << sqlite3_errmsg(db) << std::endl;
            return;
        }

        sqlite3_bind_int(user_stmt, 1, borrowed_by);

        rc = sqlite3_step(user_stmt);
        if (rc == SQLITE_ROW) {
            int user_id = sqlite3_column_int(user_stmt, 0);
            const unsigned char* username_text = sqlite3_column_text(user_stmt, 1);
            std::string username = (username_text ? reinterpret_cast<const char*>(username_text) : "unknown");

            // Debugging information
            std::cout << "Debug: user_id = " << user_id << std::endl;
            std::cout << "Debug: username_text = " << (username_text ? reinterpret_cast<const char*>(username_text) : "null") << std::endl;
            std::cout << "Book is borrowed by user ID: " << user_id << " Username: " << username << std::endl;
        } else {
            std::cout << "Book is borrowed by unknown user" << std::endl;
        }

        sqlite3_finalize(user_stmt);
    }
}








void Book::borrow_book(sqlite3 *db, int borow_id) {
    if (borrowed_by != -1) {
        std::cout << "Can't borrow book, book is already borrowed" << std::endl;
        return;
    }

    borrowed_by = borow_id;
    std::cout << "Borrowing book. Borrowed by user ID: " << borrowed_by << std::endl;

    update_book(db);
}



void Book::unborrow_book(sqlite3 *db)
{
     borrowed_by = -1;

    update_book(db);
}

void Book::change_date(sqlite3 *db)
{
    std::string new_date;
    std::cout << "Enter new date: ";
    std::cin >> new_date;
    date = new_date;

    update_book(db);
}

void Book::change_author(sqlite3 *db)
{
    std::string new_author;
    std::cout << "Enter new author: ";
    std::cin.ignore(); 
    std::getline(std::cin, new_author); 
    author = new_author;

    update_book(db);
}



void Book::change_name_for_a_book(sqlite3 *db)
{
    std::string new_title;
    std::cout << "Enter new book title: ";
    std::cin.ignore(); 
    std::getline(std::cin, new_title); 
    name = new_title;

    update_book(db);
}


void delete_book(sqlite3* db, int id) {
    std::string sql = "DELETE FROM books WHERE id = ?;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, id);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
}
