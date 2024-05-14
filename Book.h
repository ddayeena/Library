#pragma once
#include <iostream>
#include "connectmysql.h"
#include "ConsolIO.h"
#include <iomanip>
using namespace std;
class Book{
public:	
	enum Genre {
		Classic, Dystopia, Mystery, Horror, Romance, Science_fiction, Fantasy,
		Thriller, Historical_fiction, Biography, Poetry, Drama, Comedy, NoGenre
	};
	enum Location {
		Fiction, Non_fiction, Childrens_literature, Magazines, Historical_publications,
		Encyclopedias, Rare_editions, Educational_materials, NoLocation
	};
private:
	int book_id;
	string author;
	string name;
	int year;
	Genre genre;
	Location location;
	bool isAvailable;
public:
	//������������
	Book();
	Book(string author, string name, int year, Genre genre, Location location, bool isAvailable,int book_id = 0);
	//��� ��������� ����������
	void ShowInfo();
	static void ShowTop();
	static void ShowBottom();
	//��� ��������� �����
	void Create();
	//��� ����������� �����
	void Edit(sql::Connection* con);

	//������
	void SetAuthor(string author) { this->author = author; }
	void SetName(string name) { this->name = name; }
	void SetYear(int year) { this->year = year; }
	void SetGenre(Genre genre) { this->genre = genre; }
	void SetLocation(Location location) { this->location = location; }
	void SetAvailability(bool isAvailable) { this->isAvailable = isAvailable; }
	
	//������
	int GetID() { return book_id; }
	string GetAuthor() { return author; }
	string GetName() { return name; }
	int GetYear() { return year; }
	bool GetAvailability() { return isAvailable; }

	static string GenreToString(Genre genre);
	static Genre stringToGenre(string genre);

	static string LocationToString(Location location);
	static Location stringToLocation(string location);

	//��� ���������� ����� � ��
	static Book ReadFromDB(sql::Connection* con, int book_id);
	//��� ������ ����� � ��
	void WriteToDB(sql::Connection* con);
	//��� ��������� ����� � ��
	void UpdateInDB(sql::Connection* con);
	//��� ��������� ����� � ��
	void DeleteInDB(sql::Connection* con);
};