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
	//конструктори
	Book();
	Book(string author, string name, int year, Genre genre, Location location, bool isAvailable,int book_id = 0);
	//для перегляду інформації
	void ShowInfo();
	static void ShowTop();
	static void ShowBottom();
	//для створення книги
	void Create();
	//для редагування книги
	void Edit(sql::Connection* con);

	//сетери
	void SetAuthor(string author) { this->author = author; }
	void SetName(string name) { this->name = name; }
	void SetYear(int year) { this->year = year; }
	void SetGenre(Genre genre) { this->genre = genre; }
	void SetLocation(Location location) { this->location = location; }
	void SetAvailability(bool isAvailable) { this->isAvailable = isAvailable; }
	
	//гетери
	int GetID() { return book_id; }
	string GetAuthor() { return author; }
	string GetName() { return name; }
	int GetYear() { return year; }
	bool GetAvailability() { return isAvailable; }

	static string GenreToString(Genre genre);
	static Genre stringToGenre(string genre);

	static string LocationToString(Location location);
	static Location stringToLocation(string location);

	//для зчитування даних з бд
	static Book ReadFromDB(sql::Connection* con, int book_id);
	//для запису даних в бд
	void WriteToDB(sql::Connection* con);
	//для оновлення даних в бд
	void UpdateInDB(sql::Connection* con);
	//для видалення даних з бд
	void DeleteInDB(sql::Connection* con);
};