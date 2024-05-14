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
	//������������
	Reader();
	Reader(string name, string surname, string phoneNumber, int reader_id = 0); 
	
	//��� ��������� �������� ����������
	void ShowInfo();
	static void ShowTop();
	static void ShowBottom();
	//��� ��������� ��������� ����
	void ShowReadBooks();
	//��� ��������� ��������� ����
	void ShowBorrowedBooks();

	//����� ����� � ��������
	void BorrowBook(sql::Connection* con,Library& library, Book& book);
	//��������� ����� � ��������
	void ReturnBook(sql::Connection* con,Library& library, Book& book);
	
	//��������� ����������
	void Create();
	//����������� ����������
	void Edit(sql::Connection* con);

	//������
	void SetName(string name) { this->name = name; }
	void SetSurname(string surname) { this->surname = surname; }
	void SetPhoneNumber(string phoneNumber) { this->phoneNumber = phoneNumber; }

	//������
	int GetID() const { return reader_id; }
	string GetName() const { return name; }
	string GetSurname() const { return surname; }
	string GetPhoneNumber() const { return phoneNumber; }
	int GetBorrowedBooks() const { return currentBorrowed; }
	int GetReadBooks() const { return readBooks.size(); }

	//�������� ���������� ��������� ������ ��������
	bool CheckPhoneNumber() const;


	//��� ���������� ����� � ��
	static Reader ReadFromDB(sql::Connection* con, int reader_id);
	//��� ������ ����� � ��
	void WriteToDB(sql::Connection* con);
	//��� ��������� ����� � ��
	void UpdateInDB(sql::Connection* con);
	//��� ��������� ����� � ��
	void DeleteInDB(sql::Connection* con);

	//��� ������ ���������� ���� � ��
	void WriteReadBookToDB(sql::Connection* con, int book_id);
	//��� ������ ��������� ���� � ��
	void WriteBorrowedBookToDB(sql::Connection* con,int book_id);
	//��� ��������� ��������� ���� � ��
	void DeleteBorrowedBookInDB(sql::Connection* con, int book_id);
};