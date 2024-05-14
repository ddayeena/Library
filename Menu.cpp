#include "Menu.h"

void Menu::ShowReaders(Library& l){
    int Choice;
    l.ShowReaders();
    while (true) {
        cout << "[1] - Show borrowed books\n[2] - Show read books\n[3] - Back\n";
        cout << "=>Enter option: ";
        Choice = ConsolIO::GetValue(cin);
        switch (Choice) {
        case 1: {
            cout << "\n=>Enter ID of reader you want to see borrowed books: ";
            int id = ConsolIO::GetValue(cin);
            l.ShowBorrowedBooks(id);
        } 
              break;
        case 2: {
            cout << "\n=>Enter ID of reader you want to see read books: ";
            int id = ConsolIO::GetValue(cin);
            l.ShowReadBooks(id);
        }
              break;
        case 3:break;
        default:cout << "\n\tIvalid input...Try again: ";
        }
        if (Choice == 3)break;
    }
}

void Menu::ShowEvents(Library& l){
    int Choice;
    l.ShowEvents();
    while (true) {
        cout << "[1] - Show leaders of event\n[2] - Back\n";
        cout << "=>Enter option: ";
        Choice = ConsolIO::GetValue(cin);
        switch (Choice) {
        case 1: {
            cout << "\n=>Enter ID of event you want to see leaders: ";
            int id = ConsolIO::GetValue(cin);
            l.ShowLeaders(id);
        }
              break;
        case 2:break;
        default:cout << "\n\tIvalid input...Try again: ";
        }
        if (Choice == 2)break;
    }
}

void Menu::AddReader(sql::Connection* con, Library& l) {
    system("cls");
    Reader reader;
    reader.Create();
    reader.WriteToDB(con);
    l.AddReader(reader);
    cout << "\n===>A reader has been added<===\n";
}

void Menu::AddWorker(sql::Connection* con, Library& l) {
    system("cls");
    Worker worker;
    worker.Create();
    worker.WriteToDB(con);
    //перевіряє чи посада працівника відповідає тій посаді, що може мати доступ до системи
    if (worker.GetPosition() == "Librarian" || worker.GetPosition() == "Administrator" || worker.GetPosition() == "Organizer")
        worker.WriteWithAccessToSystemToDB(con);

    l.AddWorker(worker);
    cout << "\n===>A worker has been added<===\n";
}

void Menu::AddBook(sql::Connection* con, Library& l) {
    system("cls");
    Book book;
    book.Create();
    book.WriteToDB(con);
    l.AddBook(book);
    cout << "\n===>A book has been added<===\n";
}

void Menu::AddEvent(sql::Connection* con, Library& l) {
    system("cls");
    Event event;
    event.Create();
    event.WriteToDB(con);
    l.AddEvent(event);
    cout << "\n===>A event has been added<===\n";
}

void Menu::AddLeaderToEvent(sql::Connection* con, Library& l) {
    system("cls");
    l.ShowEvents();
    cout << "\nWhich meeting to add a leader to? \n=>Enter id: ";
    int id;
    while (true) {
         id = ConsolIO::GetValue(cin);
        //перевіряє чи є такий об'єкт з таким id в бібліотеці
         if (!l.CheckEventID(id))   cout << "\n\tThere is no event with such an ID...Try again: ";
         else break;
    }

    Event event = Event::ReadFromDB(con, id);
    system("cls");
    l.ShowWorkers();
    cout << "\n=>Enter ID of worker: ";
    while (true) {
        id = ConsolIO::GetValue(cin);
        //перевіряє чи є такий об'єкт з таким id в бібліотеці
        if (!l.CheckWorkerID(id))   cout << "\n\tThere is no worker with such an ID...Try again: ";
        else break;
    }

    //перевіряє чи є такий об'єкт з таким id вже доданий до заходу
    if (event.CheckLeaderID(id)) {
        cout << "\nError...This worker is already the leader of this event\n";
        return;
    }

    Worker worker = Worker::ReadFromDB(con, id);
    l.AddLeaderToEvent(con, event.GetID(), worker);
    cout << "\n===>A leader has been added<===\n";
}

void Menu::DeleteReader(sql::Connection* con, Library& l) {
    system("cls");
    l.ShowReaders();
    cout << "\n=>Enter ID of reader: ";
    while (true) {
        int id = ConsolIO::GetValue(cin);
        //перевіряє чи є такий об'єкт з таким id в бібліотеці
        if (l.CheckReaderID(id)) {
            Reader reader = Reader::ReadFromDB(con, id);
            l.RemoveReader(con, reader);
            cout << "\n===>A reader has been deleted<===\n";
            break;
        }
        else cout << "\n\tThere is no reader with such an ID...Try again: ";
    }
}

void Menu::DeleteBook(sql::Connection* con, Library& l) {
    system("cls");
    l.ShowBooks();
    cout << "\n=>Enter ID of book: ";
    while (true) {
        int id = ConsolIO::GetValue(cin);
        //перевіряє чи є такий об'єкт з таким id в бібліотеці
        if (l.CheckBookID(id)) {
            Book book = Book::ReadFromDB(con, id);
            l.RemoveBook(con, book);
            cout << "\n===>A book has been deleted<===\n";
            break;
        }
        else cout << "\n\tThere is no book with such an ID...Try again: ";
    }
}

void Menu::DeleteWorker(sql::Connection* con, Library& l) {
    system("cls");
    l.ShowWorkers();
    cout << "\n=>Enter ID of worker: ";
    while (true) {
        int id = ConsolIO::GetValue(cin);
        //перевіряє чи є такий об'єкт з таким id в бібліотеці
        if (l.CheckWorkerID(id)) {
            Worker worker = Worker::ReadFromDB(con, id);
            l.RemoveWorker(con, worker);
            cout << "\n===>A worker has been deleted<===\n";
            break;
        }
        else cout << "\n\tThere is no worker with such an ID...Try again: ";
    }
}

void Menu::DeleteEvent(sql::Connection* con, Library& l) {
    system("cls");
    l.ShowEvents();
    cout << "\n=>Enter ID of event: ";
    while (true) {
        int id = ConsolIO::GetValue(cin);
        //перевіряє чи є такий об'єкт з таким id в бібліотеці
        if (l.CheckEventID(id)) {
            Event event = Event::ReadFromDB(con, id);
            l.RemoveEvent(con, event);
            cout << "\n===>A event has been deleted<===\n";
            break;
        }
        else cout << "\n\tThere is no event with such an ID...Try again: ";
    }
}

void Menu::DeleteLeaderFromEvent(sql::Connection* con, Library& l) {
    system("cls");
    l.ShowEvents();
    cout << "\n=>Enter ID of event: ";
    int id;
    while (true) {
        id = ConsolIO::GetValue(cin);
        //перевіряє чи є такий об'єкт з таким id в бібліотеці
        if (!l.CheckEventID(id))  cout << "\n\tThere is no event with such an ID...Try again: ";
        else break;
    }
    Event event = Event::ReadFromDB(con, id);
    if (event.GetNumberLeaders() == 0) {
        cout << "\nError...There are no leaders at this event yet...\n";
        return;
    }

    system("cls");
    event.ShowLeaders();
    cout << "\n=>Enter ID of leader you want to delete: ";
    while (true) {
        id = ConsolIO::GetValue(cin);
        //перевіряє чи є такий об'єкт з таким id доданий до заходу
        if (!event.CheckLeaderID(id))  cout << "\n\tThere is no worker with such an ID...Try again: ";
        else break;
    }

    Worker worker = Worker::ReadFromDB(con, id);
    l.RemoveLeaderFromEvent(con, event.GetID(), worker);
    cout << "\n===>A leader has been deleted<===\n";
}

void Menu::EditReader(sql::Connection* con, Library& l) {
    system("cls");
    l.ShowReaders();
    cout << "\n=>Enter ID of reader: ";
    while (true) {
        int id = ConsolIO::GetValue(cin);
        //перевіряє чи є такий об'єкт з таким id в бібліотеці
        if (l.CheckReaderID(id)) {
            system("cls");
            Reader reader = Reader::ReadFromDB(con, id);
            Reader::ShowTop();
            reader.ShowInfo();
            Reader::ShowBottom();
            reader.Edit(con);
            l.UpdateReaders(con);
            break;
        }
        else cout << "\n\tThere is no reader with such an ID...Try again: ";
    }
}

void Menu::EditBook(sql::Connection* con, Library& l) {
    system("cls");
    l.ShowBooks();
    cout << "\n=>Enter ID of book: ";
    while (true) {
        int id = ConsolIO::GetValue(cin);
        //перевіряє чи є такий об'єкт з таким id в бібліотеці
        if (l.CheckBookID(id)) {
            system("cls");
            Book book = Book::ReadFromDB(con, id);
            Book::ShowTop();
            book.ShowInfo();
            Book::ShowBottom();
            book.Edit(con);
            l.UpdateBooks(con);
            break;
        }
        else cout << "\n\tThere is no book with such an ID...Try again: ";
    }
}

void Menu::EditWorker(sql::Connection* con, Library& l) {
    system("cls");
    l.ShowWorkers();
    cout << "\n=>Enter ID of worker: ";
    while (true) {
        int id = ConsolIO::GetValue(cin);
        //перевіряє чи є такий об'єкт з таким id в бібліотеці
        if (l.CheckWorkerID(id)) {
            system("cls");
            Worker worker = Worker::ReadFromDB(con, id);
            Worker::ShowTop();
            worker.ShowInfo();
            Worker::ShowBottom();
            worker.Edit(con);

            //перевіряє чи посада працівника відповідає тій посаді, що може мати доступ до систем
            if (worker.GetPosition() == "Librarian" || worker.GetPosition() == "Administrator" || worker.GetPosition() == "Organizer")
                worker.WriteWithAccessToSystemToDB(con);
            l.UpdateWorkers(con);
            break;
        }
        else cout << "\n\tThere is no worker with such an ID...Try again: ";
    }
}

void Menu::EditEvent(sql::Connection* con, Library& l) {
    system("cls");
    l.ShowEvents();
    cout << "\n=>Enter ID of event: ";
    while (true) {
        int id = ConsolIO::GetValue(cin);
        //перевіряє чи є такий об'єкт з таким id в бібліотеці
        if (l.CheckEventID(id)) {
            system("cls");
            Event event = Event::ReadFromDB(con, id);
            Event::ShowTop();
            event.ShowInfo();
            Event::ShowBottom();
            event.Edit(con);
            l.UpdateEvents(con);
            break;
        }
        else cout << "\n\tThere is no event with such an ID...Try again: ";
    }
}

void Menu::LendBook(sql::Connection* con, Library& l) {
    system("cls");
    l.ShowBooks();
    cout << "\nWhich book a reader wants to take?\n=>Enter ID: ";
    int id;
    while (true) {
        id = ConsolIO::GetValue(cin);
        //перевіряє чи є такий об'єкт з таким id в бібліотеці
        if (!l.CheckBookID(id))  cout << "\n\tThere is no book with such an ID...Try again: ";
        else break;
    }

    Book book = Book::ReadFromDB(con, id);
    //перевіряє чи книга доступна 
    if (!book.GetAvailability()) {
        cout << "\nError...This book is not available at this moment...\n";
        return;
    }

    system("cls");
    l.ShowReaders();
    cout << "\n=>Enter ID of reader: "; 
    while (true) {
        id = ConsolIO::GetValue(cin);
        //перевіряє чи є такий об'єкт з таким id в бібліотеці
        if (l.CheckReaderID(id)) {
            Reader reader = Reader::ReadFromDB(con, id);
            reader.BorrowBook(con, l, book);
            l.UpdateReaders(con);
            break;
        }
        else cout << "\n\tThere is no reader with such an ID...Try again: ";
    }
}

void Menu::TakeBook(sql::Connection* con, Library& l) {
        system("cls");
        l.ShowBooks();
        cout << "\nWhich book a reader wants to return?\n=>Enter ID: ";
        int id; 
        while (true) {
            id = ConsolIO::GetValue(cin);
            //перевіряє чи є такий об'єкт з таким id в бібліотеці
            if (!l.CheckBookID(id)) cout << "\n\tThere is no book with such an ID...Try again: ";
            else break;
        }

        Book book = Book::ReadFromDB(con, id);
        //перевіряє чи доступна книга
        if (book.GetAvailability()) {
            cout << "\nError...The book was not taken...\n";
            return;
        }
        
        system("cls");
        l.ShowReaders();
        cout << "\n=>Enter ID of reader: \n";
        while (true) {
            id = ConsolIO::GetValue(cin);
            //перевіряє чи є такий об'єкт з таким id в бібліотеці
            if (l.CheckReaderID(id)) {
                Reader reader = Reader::ReadFromDB(con, id);
                reader.ReturnBook(con, l, book);
                l.UpdateReaders(con);
                break;
            }
            else cout << "\n\tThere is no reader with such an ID...Try again: ";
        }
}