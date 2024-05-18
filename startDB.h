#pragma once
#include "connectmysql.h"
void CreateTables(sql::Connection* con) {
        sql::Statement* stmt;
        stmt = con->createStatement();
        stmt->execute("CREATE TABLE IF NOT EXISTS book("
            "book_id INT AUTO_INCREMENT, "
            "author VARCHAR(50), "
            "name VARCHAR(50), "
            "year INT, "
            "genre VARCHAR(30),"
            "location VARCHAR(50), "
            "isAvailable BOOL, "
            "PRIMARY KEY(book_id));");

        stmt = con->createStatement();
        stmt->execute("CREATE TABLE IF NOT EXISTS worker("
            "worker_id INT AUTO_INCREMENT, "
            "name VARCHAR(20), "
            "surname VARCHAR(20), "
            "phoneNumber VARCHAR(10),"
            "position VARCHAR(30), "
            "address VARCHAR(50), "
            "salary DOUBLE,"
            "role INT, "
            "FOREIGN KEY(role) REFERENCES user(user_id), "
            "PRIMARY KEY(worker_id));");

        stmt = con->createStatement();
        stmt->execute("CREATE TABLE IF NOT EXISTS reader("
            "reader_id INT AUTO_INCREMENT, "
            "name VARCHAR(20), "
            "surname VARCHAR(20), "
            "readbooks INT,"
            "borrowedbooks INT,"
            "phoneNumber VARCHAR(10),"
            "PRIMARY KEY(reader_id));");

        stmt = con->createStatement();
        stmt->execute("CREATE TABLE IF NOT EXISTS readbook("
            "read_id INT AUTO_INCREMENT,"
            "reader_id INT,"
            "book_id INT,"
            "PRIMARY KEY(read_id),"
            "FOREIGN KEY(reader_id) REFERENCES Reader(reader_id),"
            "FOREIGN KEY(book_id) REFERENCES Book(book_id)); ");

        stmt = con->createStatement();
        stmt->execute("CREATE TABLE IF NOT EXISTS borrowedbook("
            "borrowed_id INT AUTO_INCREMENT,"
            "reader_id INT,"
            "book_id INT,"
            "PRIMARY KEY(borrowed_id),"
            "FOREIGN KEY(reader_id) REFERENCES Reader(reader_id),"
            "FOREIGN KEY(book_id) REFERENCES Book(book_id)); ");

        stmt = con->createStatement();
        stmt->execute("CREATE TABLE IF NOT EXISTS event("
            "event_id INT AUTO_INCREMENT, "
            "name VARCHAR(100),"
            "day INT, "
            "month INT, "
            "hours INT, "
            "minutes INT, "
            "leaders INT, "
            "PRIMARY KEY(event_id));");

        stmt = con->createStatement();
        stmt->execute("CREATE TABLE IF NOT EXISTS eventleader("
            "leader_id INT AUTO_INCREMENT,"
            "event_id INT,"
            "worker_id INT,"
            "PRIMARY KEY(leader_id),"
            "FOREIGN KEY(event_id) REFERENCES event(event_id),"
            "FOREIGN KEY(worker_id) REFERENCES worker(worker_id)); ");

        stmt = con->createStatement();
        stmt->execute("CREATE TABLE IF NOT EXISTS user("
                      "user_id INT AUTO_INCREMENT, "
                      "login VARCHAR(50), "
                      "password VARCHAR(10) NOT NULL,"
                      "role ENUM('Administrator', 'Librarian', 'Organizer'), "
                      "PRIMARY KEY(user_id));");
        
        sql::ResultSet* res = stmt->executeQuery("SELECT COUNT(*) AS count FROM user;");
        if (res->next() && res->getInt("count") == 0) {
            stmt->execute("INSERT INTO user (login, password, role) VALUES ('admin', 'admin', 'Administrator');");

            stmt = con->createStatement();
            stmt->execute("INSERT INTO worker (name, surname, phoneNumber, position, address, salary, role) VALUES "
                "('Admin', 'Admin', '0000000000', 'Administrator', 'No Street', 0, 1);");
        }
        delete res;
        delete stmt;
}
void ConnectDB(sql::Connection** con, sql::Driver** driver) {
    *driver = get_driver_instance();
    *con = (*driver)->connect("tcp://127.0.0.1:3306", "root", "gokau567");
    (*con)->setSchema("library");
}
