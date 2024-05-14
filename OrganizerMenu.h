#pragma once
#include "Menu.h"
class OrganizerMenu{
    static void Show(Library& l);
    static void Add(sql::Connection* con, Library& l);
    static void Delete(sql::Connection* con, Library& l);
public:
    static void MainMenu(sql::Connection* con, Library& l, Worker& worker);
};