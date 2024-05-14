#pragma once
#include "Book.h"
#include "Event.h"
#include "Reader.h"
#include "Worker.h"
#include "connectmysql.h"
#include <vector>
#include <algorithm>
class Library{
	vector<Book> books;
	vector<Reader> readers;
	vector<Worker> workers;
	vector<Event> events;

	//��� ������ ��'���� �� ID
	template<typename T>
	void SearchItemByID(vector<T>& items);
	//��� ������ ��'���� �� ������
	template<typename T>
	void SearchItemByName(vector<T>& items);

	template<typename T>
	void Search(vector<T>& items);
public:
	//�����������
	Library();
	//��� ��������� ����������
	void ShowBooks();
	void ShowReaders();
	void ShowWorkers();
	void ShowEvents();
	void ShowBorrowedBooks(int reader_id);
	void ShowReadBooks(int reader_id);
	void ShowLeaders(int event_id);

	//����� ��'����
	void SearchEvent() { Search(events); }
	void SearchBook() { Search(books); }
	void SearchReader() { Search(readers); }
	void SearchWorker() { Search(workers); }

	//���� ����� ��'��� � ��������
	void AddBook(Book& book);
	void AddReader(Reader& reader);
	void AddWorker(Worker& worker);
	void AddEvent(Event& event);
	void AddLeaderToEvent(sql::Connection* con, int event_id, Worker& worker);

	//������� ��'��� � ��������
	void RemoveBook(sql::Connection* con,Book& book);
	void RemoveReader(sql::Connection* con, Reader& reader);
	void RemoveWorker(sql::Connection* con, Worker& worker);
	void RemoveEvent(sql::Connection* con, Event& event);
	void RemoveLeaderFromEvent(sql::Connection* con, int event_id, Worker& worker);

	//������� ����� �� ������ ����� � ��������
	void TakeBook(sql::Connection* con,Book& book);
	//������ ����� �������
	void LendBook(sql::Connection* con, Book& book);

	//�������� �� ID �� ������ ��'��� � � ������
	bool CheckBookID(int book_id);
	bool CheckReaderID(int reader_id);
	bool CheckWorkerID(int worker_id);
	bool CheckEventID(int event_id);

	//���� ��� ��'���� � ��
	void ReadBooksFromDB(sql::Connection* con);
	void ReadWorkersFromDB(sql::Connection* con);
	void ReadReadersFromDB(sql::Connection* con);
	void ReadEventsFromDB(sql::Connection* con);

	//��� ��������� ����� ��'���� � ��
	void UpdateReaders(sql::Connection* con);
	void UpdateWorkers(sql::Connection* con);
	void UpdateEvents(sql::Connection* con);
	void UpdateBooks(sql::Connection* con);
};

template<typename T>
inline void Library::SearchItemByID( vector<T>& items) {
	cout << "\n=>Enter ID: ";
	int id = ConsolIO::GetValue(cin);
	system("cls");
	//���� ��'���
	auto it = find_if(items.begin(), items.end(), [id](T& obj) {return (obj.GetID() == id);	});

	//�������� ��'���
	if (it != items.end()) {
		T::ShowTop();
		(*it).ShowInfo();
		T::ShowBottom();
	}
	else cout << "\n===>NOT FOUND<===\n";
}

template<typename T>
inline void Library::SearchItemByName(vector<T>& items) {
	cout << "\n=>Enter name: ";
	string name;
	getline(cin >> ws, name);
	system("cls");
	
	//����� ��'���� �� ���� ����
	bool IsFound = false;
	for (auto item : items) {
		if (item.GetName() == name){
			if (!IsFound) item.ShowTop();
			IsFound = true;
			item.ShowInfo();
		}
	}
	if (!IsFound) cout << "\n===>NOT FOUND<===\n";
	else items[0].ShowBottom();
}

template<typename T>
inline void Library::Search(vector<T>& items) {
	int Choice;
	while (true) {
		cout << "\n\nSearch by:\n[1] - ID\n[2] - Name\n[3] - Back\n";
		cout << "=>Enter option: ";
		Choice = ConsolIO::GetValue(cin);
		switch (Choice) {
			case 1:	SearchItemByID(items);		break;
			case 2: SearchItemByName(items);	break;
			case 3:break;
			default:cout << "\n\tInvalid input...Try again: ";
		}
		if (Choice == 3) break;
	}
}