#pragma once
#include "Book.h"
#include <vector>
const int maxBorrowed = 5;
class Library;
class Reader {
	int reader_id;
	string name;
	string surname;
	string phoneNumber;

	vector<Book> readBooks;
	int currentBorrowed = 0;
	Book borrowedBooks[maxBorrowed];
public:
	//конструктори
	Reader();
	Reader(string name, string surname, string phoneNumber, int reader_id = 0); 
	
	//для перегляду особистої інформації
	void ShowInfo();
	static void ShowTop();
	static void ShowBottom();
	//для перегляду прочитнах книг
	void ShowReadBooks();
	//для перегляду позичених книг
	void ShowBorrowedBooks();

	//взяти книгу в бібліотеці
	void BorrowBook(sql::Connection* con,Library& library, Book& book);
	//повернути книгу в бібліотеку
	void ReturnBook(sql::Connection* con,Library& library, Book& book);
	
	//створення працівника
	void Create();
	//редагування працівника
	void Edit(sql::Connection* con);

	//сетери
	void SetName(string name) { this->name = name; }
	void SetSurname(string surname) { this->surname = surname; }
	void SetPhoneNumber(string phoneNumber) { this->phoneNumber = phoneNumber; }

	//гетери
	int GetID() const { return reader_id; }
	string GetName() const { return name; }
	string GetSurname() const { return surname; }
	string GetPhoneNumber() const { return phoneNumber; }
	int GetBorrowedBooks() const { return currentBorrowed; }
	int GetReadBooks() const { return readBooks.size(); }

	//перевірка коректності введеного номера телефону
	bool CheckPhoneNumber() const;


	//для зчитування даних з бд
	static Reader ReadFromDB(sql::Connection* con, int reader_id);
	//для запису даних в бд
	void WriteToDB(sql::Connection* con);
	//для оновлення даних в бд
	void UpdateInDB(sql::Connection* con);
	//для видалення даних з бд
	void DeleteInDB(sql::Connection* con);

	//для запису прочитаних книг в бд
	void WriteReadBookToDB(sql::Connection* con, int book_id);
	//для запису позичених книг в бд
	void WriteBorrowedBookToDB(sql::Connection* con,int book_id);
	//для видалення позичених книг з бд
	void DeleteBorrowedBookInDB(sql::Connection* con, int book_id);
};