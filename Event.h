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
	//������������
	Event();
	Event(string name, int day, int month, int hours, int minutes, int event_id = 0);

	//��� ��������� ����������
	void ShowInfo();
	void ShowLeaders();
	static void ShowTop();
	static void ShowBottom();
	//��� ��������� ������
	void Create();
	//��� ����������� ������
	void Edit(sql::Connection* con);

	//������
	void SetName(string name) { this->name = name; }
	void SetDay(int day) { this->day = day; }
	void SetMonth(int month) { this->month = month; }
	void SetHours(int hours) { this->hours = hours; }
	void SetMinutes(int minutes) { this->minutes = minutes; }

	//������
	int GetID() const { return event_id; }
	string GetName() const { return name; }
	int GetDay() const { return day; }
	int GetMonth() const { return month; }
	int GetHours() const { return hours; }
	int GetMinutes() const { return minutes; }
	int GetNumberLeaders() const { return leaders.size(); }

	//���� ���������� �� ������
	void AddLeaderToEvent(Worker& worker);
	//������� ���������� � ������
	void RemoveLeaderFromEvent(sql::Connection* con, Worker& worker);

	//�������� �� ������ ��������� ��� ������� �� ������
	bool CheckLeaderID(int worker_id) const;

	//��� ���������� ����� � ��
	static Event ReadFromDB(sql::Connection* con, int event_id);
	//��� ������ ����� � ��
	void UpdateInDB(sql::Connection* con);
	//��� ��������� ����� � ��
	void WriteToDB(sql::Connection* con);
	//��� ��������� ����� � ��
	void DeleteInDB(sql::Connection* con);
	//��� ������ ����� � �� ��� ����������, �� � ��������� �� ����� 
	void WriteLeadersToDB(sql::Connection* con, int worker_id);
	//��� ��������� ����� � �� ����������, �� � ��������� �� �����
	void DeleteLeadersInDB(sql::Connection* con, int worker_id);
};