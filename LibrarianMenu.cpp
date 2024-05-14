#include "LibrarianMenu.h"

void LibrarianMenu::Show(Library& l){
    system("cls");
    int Choice;
    while (true) {
        cout << "\n==============";
        cout << "\n=>   SHOW   <=\n";
        cout << "==============\n";
        cout << "[1] - Readers\n[2] - Books\n[3] - Events\n[4] - Back\n";
        cout << "=>Enter option: ";
        Choice = ConsolIO::GetValue(cin);
        system("cls");
        switch (Choice) {
            case 1: Menu::ShowReaders(l);    break;
            case 2: l.ShowBooks();      break;
            case 3: Menu::ShowEvents(l);     break;
            case 4: break;
            default:cout << "\n\tIvalid input...Try again: ";
        }
        if (Choice == 4) break;
    }
}

void LibrarianMenu::Add(sql::Connection* con, Library& l){
    system("cls");
    int Choice;
    while (true) {
        cout << "\n===============";
        cout << "\n=>    ADD    <=\n";
        cout << "===============\n";
        cout << "[1] - Reader\n[2] - Book\n[3]- Back\n";
        cout << "=>Enter option: ";
        Choice = ConsolIO::GetValue(cin);
        switch (Choice) {
            case 1:Menu::AddReader(con, l); break;
            case 2:Menu::AddBook(con, l);   break;
            case 3: break;
            default: cout << "\n\tInvalid input...Try again: ";
        }
        if (Choice == 3)break;
    }
}

void LibrarianMenu::Delete(sql::Connection* con, Library& l){
    system("cls");
    int Choice;
    while (true) {
        cout << "\n================";
        cout << "\n=>   DELETE   <=\n";
        cout << "================\n";
        cout << "[1] - Reader\n[2] - Book\n[3] - Back\n";
        cout << "=>Enter option: ";
        Choice = ConsolIO::GetValue(cin);
        switch (Choice) {
            case 1:Menu::DeleteReader(con, l);  break;
            case 2:Menu::DeleteBook(con, l);   break;
            case 3: break;
            default:cout << "\n\tInvalid input...Try again: ";
        }
        if (Choice == 3)break;
    }
}

void LibrarianMenu::Edit(sql::Connection* con, Library& l){
    system("cls");
    int Choice;
    while (true) {
        cout << "\n==============";
        cout << "\n=>   EDIT   <=\n";
        cout << "==============\n";
        cout<<"[1] - Readers\n[2] - Books\n[3] - Back\n";
        cout << "=>Enter option: ";
        Choice = ConsolIO::GetValue(cin);
        switch (Choice) {
            case 1:Menu::EditReader(con, l);    break;
            case 2:Menu::EditBook(con, l);      break;
            case 3:break;
            default:cout << "\n\tInvalid input...Try again: ";
        }
        if (Choice == 3)break;
    }
}

void LibrarianMenu::Search(Library& l){
    system("cls");
    int Choice;
    while (true) {
        cout << "\n============";
        cout << "\n=> SEARCH <=\n";
        cout << "============\n";
        cout << "[1] - Book\n[2] - Reader\n[3] - Event\n[4] - Back\n";
        cout << "=>Enter option: ";
        Choice = ConsolIO::GetValue(cin);
        switch (Choice) {
            case 1:l.SearchBook();     break;
            case 2:l.SearchReader();   break;
            case 3:l.SearchEvent();    break;
            case 4: break;
            default:cout << "\n\tInvalid input...Try again: ";
        }
        if (Choice == 4) break;
    }
}

void LibrarianMenu::MainMenu(sql::Connection* con, Library& l, Worker& worker) {
    int Choice;
    do {
        cout << "\n=============";
        cout << "\n=>MAIN MENU<=\n";
        cout << "=============\n";
        cout << "\n[1] - Me\n[2] - Show\n[3] - Add\n[4] - Delete\n[5] - Lend a book to a reader\n[6] - Take a book back from a reader\n[7] - Edit\n[8] - Search\n[9] - Change password\n[10] - Log out\n[11] - Exit\n";
        cout << "=>Enter option: ";
        Choice = ConsolIO::GetValue(cin);
        switch (Choice) {
        case 1:
            system("cls");
            cout << "==========================================================================================================" << endl;
            cout << "||                                             =>MY PERSONAL INFO<=                                     ||" << endl;
            cout << "==========================================================================================================" << endl;
            cout << "||  ID  ||   Name    ||  Surname  ||Phone number||    Position    ||        Address       ||   Salary   ||" << endl;
            cout << "==========================================================================================================" << endl;
            worker.ShowInfo();
            cout << "==========================================================================================================" << endl;
            break;
        case 2:
            Show(l);
            break;
        case 3:
            Add(con, l);
            break;
        case 4:
            Delete(con, l);
            break;
        case 5:
            Menu::LendBook(con, l);
            break;
        case 6:
            Menu::TakeBook(con, l);
            break;
        case 7:
            Edit(con, l);
            break;
        case 8:
            Search(l);
            break;
        case 9:
            system("cls");
            worker.ChangePassword(con);
            break;

        case 10:
            system("cls");
            worker = Worker::Login(con);
            break;

        case 11:  exit(0);
        default: cout << "\n\tInvalid input...\n";
        }
    } while (worker.GetPosition() == "Librarian");
}