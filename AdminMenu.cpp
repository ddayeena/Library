#include "AdminMenu.h"

void AdminMenu::Show(Library& l) {
    system("cls");
    int Choice;
    while (true) {
        cout << "\n==============";
        cout << "\n=>   SHOW   <=\n";
        cout << "==============\n";
        cout << "[1] - Readers\n[2] - Workers\n[3] - Books\n[4] - Events\n[5] - Back\n";
        cout << "=>Enter option: ";
        Choice = ConsolIO::GetValue(cin);
        system("cls");
        switch (Choice) {
            case 1: Menu::ShowReaders(l);   break;
            case 2: l.ShowWorkers();    break;
            case 3: l.ShowBooks();      break;
            case 4: Menu::ShowEvents(l);     break;
            case 5:break;
            default:cout << "\n\tIvalid input...Try again: ";
        }
        if (Choice == 5)break;
    }
}

void AdminMenu::Add(sql::Connection* con, Library& l) {
    system("cls");
    int Choice;
    while (true) {
        cout << "\n===============";
        cout << "\n=>    ADD    <=\n";
        cout << "===============\n";
        cout << "[1] - Reader\n[2] - Worker\n[3] - Book\n[4] - Event\n[5] - Leader to event\n[6] - Back\n";
        cout << "=>Enter option: ";
        Choice = ConsolIO::GetValue(cin);
        system("cls");
        switch (Choice) {
            case 1: Menu::AddReader(con, l);    break;
            case 2: Menu::AddWorker(con, l);    break;
            case 3: Menu::AddBook(con, l);      break;
            case 4: Menu::AddEvent(con, l);     break;
            case 5: Menu::AddLeaderToEvent(con, l); break;
            case 6: break;
            default: cout << "\nInvalid input...\n";
        }
        if (Choice == 6)break;
    }
}

void AdminMenu::Delete(sql::Connection* con, Library& l) {
    system("cls");
    int Choice;
    while (true) {
        cout << "\n================";
        cout << "\n=>   DELETE   <=\n";
        cout << "================\n";
        cout << "[1] - Reader\n[2] - Worker\n[3] - Book\n[4] - Event\n[5] - Leader from event\n[6] - Back\n";
        cout << "=>Enter option: ";
        Choice = ConsolIO::GetValue(cin);
        system("cls");
        switch (Choice) {
            case 1: Menu::DeleteReader(con, l);     break;
            case 2: Menu::DeleteWorker(con, l);     break;
            case 3: Menu::DeleteBook(con, l);       break;
            case 4: Menu::DeleteEvent(con, l);      break;
            case 5: Menu::DeleteLeaderFromEvent(con, l); break;
            case 6: break;
            default: cout << "\nInvalid input...\n";
        }
        if (Choice == 6)break;
    }
}

void AdminMenu::Edit(sql::Connection* con, Library& l) {
    system("cls");
    int Choice;
    while (true) {
        cout << "\n==============";
        cout << "\n=>   EDIT   <=\n";
        cout << "==============\n";
        cout << "[1] - Readers\n[2] - Workers\n[3] - Books\n[4] - Events\n[5] - Back\n";
        cout << "=>Enter option: ";
        Choice = ConsolIO::GetValue(cin);
        system("cls");
        switch (Choice) {
            case 1: Menu::EditReader(con, l);   break;
            case 2: Menu::EditWorker(con, l);   break;
            case 3: Menu::EditBook(con, l);     break;
            case 4: Menu::EditEvent(con, l);    break;
            case 5: break;
            default:cout << "\nInvalid input...\n";
        }
        if (Choice == 5)break;
    }
}

void AdminMenu::Search(Library& l) {
    system("cls");
    int Choice;
    while (true) {
        cout << "\n============";
        cout << "\n=> SEARCH <=\n";
        cout << "============\n";
        cout << "[1] - Book\n[2] - Reader\n[3] - Event\n[4] - Worker\n[5] - Back\n";
        cout << "=>Enter option: ";
        Choice = ConsolIO::GetValue(cin);
        system("cls");
        switch (Choice) {
            case 1:l.SearchBook(); break;
            case 2:l.SearchReader();   break;
            case 3:l.SearchEvent();    break;
            case 4:l.SearchWorker();     break;
            case 5: break;
            default:cout << "\n\tInvalid input...Try again: ";
        }
        if (Choice == 5) break;
    }
}

void AdminMenu::MainMenu(sql::Connection* con, Library& l, Worker& worker) {
    int Choice;
    do {
        cout << "\n=============";
        cout << "\n=>MAIN MENU<=\n";
        cout << "=============\n";
        cout << "\n[1] - Me\n[2] - Show\n[3] - Add\n[4] - Delete\n[5] - Edit\n[6] - Search\n[7] - Change password\n[8] - Log out\n[9] - Exit\n";
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
            Edit(con, l);
            break;
        case 6:
            Search(l);
            break;
        case 7:
            system("cls");
            worker.ChangePassword(con);
            break;
        case 8:
            system("cls");
            worker = Worker::Login(con);
            break;
        case 9: exit(0);
        default: cout << "\n\tInvalid input...\n";
        }
    } while (worker.GetPosition() == "Administrator");
}