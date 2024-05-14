#include <iostream>
#include "Library.h"
#include "startDB.h"
#include "AdminMenu.h"
#include "OrganizerMenu.h"
#include "LibrarianMenu.h"
using namespace std;

int main() {
    system("color F0");
    try {
        sql::Driver* driver;
        sql::Connection* con;
        sql::Statement* stmt;
        sql::ResultSet* res;

        //з'єднання з базою даних
        ConnectDB(&con, &driver);
        CreateTables(con);

        Library l;
        
        //зчитування даних з бази даних
        l.ReadBooksFromDB(con);
        l.ReadEventsFromDB(con);
        l.ReadWorkersFromDB(con);
        l.ReadReadersFromDB(con);

        //вхід в систему
        Worker worker = Worker::Login(con);
        while(true){
            //librarian
            if (worker.GetPosition() == "Librarian")
                LibrarianMenu::MainMenu(con, l, worker);
          
            //administrator
            else if (worker.GetPosition() == "Administrator")
                AdminMenu::MainMenu(con, l, worker);
            
            //organizer
            else if (worker.GetPosition() == "Organizer") 
                OrganizerMenu::MainMenu(con, l, worker);
            
            else cout << "ERROR";
        } 
    }
    catch (sql::SQLException& e) {
        cout << "# ERR: SQLException in " << __FILE__;
        cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
        cout << "# ERR: " << e.what();
        cout << " (MySQL error code: " << e.getErrorCode();
        cout << ", SQLState: " << e.getSQLState() << " )" << endl;
    }
    return 0;
}