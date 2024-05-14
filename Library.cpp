#include "Library.h"
#include <iomanip>
Library::Library(){}

//----------------------------------------------BOOKS----------------------------------------
void Library::ShowBooks() {
	Book::ShowTop();
	for (int i = 0; i < books.size(); i++) {
		books[i].ShowInfo();
	}
	Book::ShowBottom();
}

void Library::AddBook(Book& book){
	books.push_back(book);
}

void Library::RemoveBook(sql::Connection* con,Book& book){
	book.DeleteInDB(con);
	UpdateBooks(con);
}

void Library::TakeBook(sql::Connection* con,Book& book){
	for (int i = 0; i < books.size(); i++) {
		if (books[i].GetID() == book.GetID()) {
			//робимо книгу доступною
			books[i].SetAvailability(true);
			//оновлюємо її значення в бд
			books[i].UpdateInDB(con);
		}
	}
}

void Library::LendBook(sql::Connection* con, Book& book) {
		//пошук нкиги
		for (int i = 0; i < books.size(); i++) {
			if (books[i].GetID() == book.GetID()) {
				if (books[i].GetAvailability()) {
					//робимо книгу не доступною
					books[i].SetAvailability(false);
					//оновлюємо її дані  вбд
					books[i].UpdateInDB(con);
					break;
				}
			}
		}
		cout << "\n===>Book was lent to reader with ID: " << book.GetID() << "<===" << endl;
}

void Library::UpdateBooks(sql::Connection* con) {
	books.clear();
	ReadBooksFromDB(con);
}

void Library::ShowBorrowedBooks(int reader_id) {
	for (auto reader : readers) {
		if (reader.GetID() == reader_id)
			reader.ShowBorrowedBooks();
	}
}

void Library::ShowReadBooks(int reader_id) {
	for (auto reader : readers) {
		if (reader.GetID() == reader_id)
			reader.ShowReadBooks();
	}
}

void Library::ShowLeaders(int event_id) {
	for (auto event : events) {
		if (event.GetID() == event_id)
			event.ShowLeaders();
	}
}

void Library::ReadBooksFromDB(sql::Connection* con) {
	try {
		sql::Statement* stmt;
		sql::ResultSet* res;
		stmt = con->createStatement();
		res = stmt->executeQuery("SELECT book_id FROM book;");
		while (res->next())
			books.push_back(Book::ReadFromDB(con, res->getInt("book_id")));
		delete stmt;
		delete res;
	}
	catch (sql::SQLException& e) {
		cout << "# ERR: SQLException in " << __FILE__ << endl;
		cout << "# ERR: " << e.what();
	}
}

//-------------------------------------------------READERS----------------------------------
void Library::ShowReaders(){
	Reader::ShowTop();
	for (int i = 0; i < readers.size(); i++) {
		readers[i].ShowInfo();
	}
	Reader::ShowBottom();
}

void Library::AddReader(Reader& reader){
	readers.push_back(reader);
}

void Library::RemoveReader(sql::Connection*con,Reader& reader){
	reader.DeleteInDB(con);
	UpdateReaders(con);
	UpdateBooks(con);
}

void Library::UpdateReaders(sql::Connection* con) {
	readers.clear();
	ReadReadersFromDB(con);
}

void Library::ReadReadersFromDB(sql::Connection* con) {
	try {
		sql::Statement* stmt;
		sql::ResultSet* res;
		stmt = con->createStatement();
		res = stmt->executeQuery("SELECT reader_id FROM reader;");
		while (res->next())
			readers.push_back(Reader::ReadFromDB(con, res->getInt("reader_id")));
		delete stmt;
		delete res;
	}
	catch (sql::SQLException& e) {
		cout << "# ERR: SQLException in " << __FILE__ << endl;
		cout << "# ERR: " << e.what();
	}
}

//-------------------------------------------------WORKERS-----------------------------------
void Library::ShowWorkers(){
	Worker::ShowTop();
	for (int i = 0; i < workers.size(); i++) {
		workers[i].ShowInfo();
	}
	Worker::ShowBottom();
}

void Library::AddWorker(Worker& worker){
	workers.push_back(worker);
}

void Library::RemoveWorker(sql::Connection* con,Worker& worker){
	worker.DeleteInDB(con);
	UpdateEvents(con);
	UpdateWorkers(con);
}

void Library::UpdateWorkers(sql::Connection* con) {
	workers.clear();
	ReadWorkersFromDB(con);
}

void Library::ReadWorkersFromDB(sql::Connection* con) {
	try {
		sql::Statement* stmt;
		sql::ResultSet* res;
		stmt = con->createStatement();
		res = stmt->executeQuery("SELECT worker_id FROM worker;");
		while (res->next())
			workers.push_back(Worker::ReadFromDB(con, res->getInt("worker_id")));
		delete stmt;
		delete res;
	}
	catch (sql::SQLException& e) {
		cout << "# ERR: SQLException in " << __FILE__ << endl;
		cout << "# ERR: " << e.what();
	}
}

//--------------------------------------------------EVENTS--------------------------------------
void Library::AddEvent(Event& event){
	events.push_back(event);
}

void Library::ShowEvents(){
	Event::ShowTop();
	for (int i = 0; i < events.size(); i++) {
		events[i].ShowInfo();
	}
	Event::ShowBottom();
}

void Library::RemoveEvent(sql::Connection* con,Event& event){
	event.DeleteInDB(con);
	UpdateEvents(con);
}

void Library::AddLeaderToEvent(sql::Connection* con,int event_id, Worker& worker){
	for (int i = 0; i < events.size(); i++) {
		if (events[i].GetID() == event_id) {
			events[i].AddLeaderToEvent(worker);
			events[i].UpdateInDB(con);
			events[i].WriteLeadersToDB(con, worker.GetID());
		}
	}
}

void Library::RemoveLeaderFromEvent(sql::Connection* con, int event_id, Worker& worker){
	for (int i = 0; i < events.size(); i++) {
		if (events[i].GetID() == event_id) {
			events[i].RemoveLeaderFromEvent(con,worker);
		}
	}
}

void Library::UpdateEvents(sql::Connection* con) {
	events.clear();
	ReadEventsFromDB(con);
}

void Library::ReadEventsFromDB(sql::Connection* con) {
	try {
		sql::Statement* stmt;
		sql::ResultSet* res;
		stmt = con->createStatement();
		res = stmt->executeQuery("SELECT event_id FROM event;");
		while (res->next())
			events.push_back(Event::ReadFromDB(con, res->getInt("event_id")));
		delete stmt;
		delete res;
	}
	catch (sql::SQLException& e) {
		cout << "# ERR: SQLException in " << __FILE__ << endl;
		cout << "# ERR: " << e.what();
	}
}

//-------------------------------------------- CHECKING ID -------------------------------------

bool Library::CheckBookID(int book_id){
	for (int i = 0; i < books.size(); i++) {
		if (books[i].GetID() == book_id)
			return true;
	}
	return false;
}

bool Library::CheckReaderID(int reader_id){
	for (int i = 0; i < readers.size(); i++) {
		if (readers[i].GetID() == reader_id)
			return true;
	}
	return false;
}

bool Library::CheckWorkerID(int worker_id){
	for (int i = 0; i < workers.size(); i++) {
		if (workers[i].GetID() == worker_id)
			return true;
	}
	return false;
}

bool Library::CheckEventID(int event_id){
	for (int i = 0; i < events.size(); i++) {
		if (events[i].GetID() == event_id)
			return true;
	}
	return false;
}