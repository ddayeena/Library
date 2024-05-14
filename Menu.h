#pragma once
#include "Library.h"
class Menu {
public:
    static void ShowReaders(Library& l);
    static void ShowEvents(Library& l);

    static void AddReader(sql::Connection* con, Library& l);
    static void AddWorker(sql::Connection* con, Library& l);
    static void AddBook(sql::Connection* con, Library& l);
    static void AddEvent(sql::Connection* con, Library& l);
    static void AddLeaderToEvent(sql::Connection* con, Library& l);

    static void DeleteReader(sql::Connection* con, Library& l);
    static void DeleteBook(sql::Connection* con, Library& l);
    static void DeleteWorker(sql::Connection* con, Library& l);
    static void DeleteEvent(sql::Connection* con, Library& l);
    static void DeleteLeaderFromEvent(sql::Connection* con, Library& l);

    static void EditReader(sql::Connection* con, Library& l);
    static void EditBook(sql::Connection* con, Library& l);
    static void EditWorker(sql::Connection* con, Library& l);
    static void EditEvent(sql::Connection* con, Library& l);

    static void LendBook(sql::Connection* con, Library& l);
    static void TakeBook(sql::Connection* con, Library& l);
};