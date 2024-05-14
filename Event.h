#pragma once
#include <iostream>
#include "Worker.h"
#include "ConsolIO.h"
#include <set>
#include <iomanip>
using namespace std;
class Event{
	int event_id;
	string name;
	int day;
	int month;
	int hours;
	int minutes;

	set<Worker> leaders;
public:
	//конструктори
	Event();
	Event(string name, int day, int month, int hours, int minutes, int event_id = 0);

	//для перегляду інформації
	void ShowInfo();
	void ShowLeaders();
	static void ShowTop();
	static void ShowBottom();
	//для створення заходу
	void Create();
	//для редагування заходу
	void Edit(sql::Connection* con);

	//сетери
	void SetName(string name) { this->name = name; }
	void SetDay(int day) { this->day = day; }
	void SetMonth(int month) { this->month = month; }
	void SetHours(int hours) { this->hours = hours; }
	void SetMinutes(int minutes) { this->minutes = minutes; }

	//гетери
	int GetID() const { return event_id; }
	string GetName() const { return name; }
	int GetDay() const { return day; }
	int GetMonth() const { return month; }
	int GetHours() const { return hours; }
	int GetMinutes() const { return minutes; }
	int GetNumberLeaders() const { return leaders.size(); }

	//додає працівника до заходу
	void AddLeaderToEvent(Worker& worker);
	//видаляє працівника з заходу
	void RemoveLeaderFromEvent(sql::Connection* con, Worker& worker);

	//перевіряє чи деякий працівник вже доданий до заходу
	bool CheckLeaderID(int worker_id) const;

	//для зчитування даних з бд
	static Event ReadFromDB(sql::Connection* con, int event_id);
	//для запису даних в бд
	void UpdateInDB(sql::Connection* con);
	//для оновлення даних в бд
	void WriteToDB(sql::Connection* con);
	//для видалення даних з бд
	void DeleteInDB(sql::Connection* con);
	//для запису даних в бд про працівників, які є головними на заході 
	void WriteLeadersToDB(sql::Connection* con, int worker_id);
	//для видалення даних з бд працівників, які є головними на заході
	void DeleteLeadersInDB(sql::Connection* con, int worker_id);
};