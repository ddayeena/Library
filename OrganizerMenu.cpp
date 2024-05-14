#include "OrganizerMenu.h"

void OrganizerMenu::Show(Library& l) {
    system("cls");
    int Choice;
    while (true) {
        cout << "\n==============";
        cout << "\n=>   SHOW   <=\n";
        cout << "==============\n";
        cout << "[1] - Events\n[2] - Workers\n[3] - Back\n";
        cout << "=>Enter option: ";
        Choice = ConsolIO::GetValue(cin);
        system("cls");
        switch (Choice) {
            case 1:Menu::ShowEvents(l); break;
            case 2:l.ShowWorkers(); break;
            case 3:break;
            default:cout << "\n\tInvalid input... Try again:";
        }
        if (Choice == 3)break;
    }
}

void OrganizerMenu::Add(sql::Connection* con, Library& l) {
    system("cls");
    int Choice;
    while (true) {
        cout << "\n===============";
        cout << "\n=>    ADD    <=\n";
        cout << "===============\n";
        cout<<"[1] - Event\n[2] - Leader to event\n[3] - Back\n";
        cout << "=>Enter option: ";
        Choice = ConsolIO::GetValue(cin);
        system("cls");
        switch (Choice) {
            case 1: Menu::AddEvent(con, l);  break;
            case 2: Menu::AddLeaderToEvent(con, l); break;
            case 3: break;
            default:cout << "\n\tInvalid input...Try again: ";
        }
        if (Choice == 3)break;
    }
}

void OrganizerMenu::Delete(sql::Connection* con, Library& l){
    system("cls");
    int Choice;
    while (true) {
        cout << "\n================";
        cout << "\n=>   DELETE   <=\n";
        cout << "================\n";
        cout << "[1] - Event\n[2] - Leader from event\n[3] - Back\n";
        cout << "=>Enter option: ";
        Choice = ConsolIO::GetValue(cin);
        system("cls");
        switch (Choice) {
            case 1:Menu::DeleteEvent(con, l);   break;
            case 2:Menu::DeleteLeaderFromEvent(con, l); break;
            case 3:break;
            default: cout << "\n\tInvalid input...Try again: ";
        }
        if (Choice == 3)break;
    }
}

void OrganizerMenu::MainMenu(sql::Connection* con, Library& l, Worker& worker) {
    int Choice;
    do {
        cout << "\n=============";
        cout << "\n=>MAIN MENU<=\n";
        cout << "==============\n";
        cout << "\n[1] - Me\n[2] - Show\n[3] - Add\n[4] - Delete\n[5] - Edit event\n[6] - Search event\n[7] - Change password\n[8] - Log out\n[9] - Exit\n";
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
            Menu::EditEvent(con, l);
            break;
        case 6:
            system("cls");
            l.SearchEvent();
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
    } while (worker.GetPosition() == "Organizer");
}