#pragma once
#include "Menu.h"
class LibrarianMenu{
    static void Show(Library& l);
    static void Add(sql::Connection* con,Library& l);
    static void Delete(sql::Connection* con,Library& l);
    static void Edit(sql::Connection* con,Library& l);
    static void Search(Library& l);
public:
    static void MainMenu(sql::Connection* con, Library& l, Worker& worker);
};