#pragma once
#include "connectmysql.h"
#include "Reader.h"
#include "ConsolIO.h"
#include <iomanip>
class Worker {
	int worker_id;
	string name;
	string surname;
	string phoneNumber;
	string position;
	string address;
	double salary;

	string login;
	string password;
public:
	//������������
	Worker();
	Worker(string name, string surname, string phoneNumber, string position, string address, double salary,string login,string password, int worker_id = 0);
	//��� ��������� ����������
	void ShowInfo();
	static void ShowTop();
	static void ShowBottom();

	//��� ��������� ����������
	void Create();
	//��� ����������� ����������
	void Edit(sql::Connection* con);

	//������
	void SetName(string name) { this->name = name; }
	void SetSurname(string surname) { this->surname = surname; }
	void SetPhoneNumber(string phoneNumber) { this->phoneNumber = phoneNumber; }
	void SetPosition(string position) { this->position = position; }
	void SetAddress(string address) { this->address = address; }
	void SetSalary(double salary) { this->salary = salary; }

	//������
	int GetID() const { return worker_id; }
	string GetName() const { return name; }
	string GetSurname() const { return surname; }
	string GetPhoneNumber() const { return phoneNumber; }
	string GetPosition() const { return position; }
	string GetAddress() const { return address; }
	double GetSalary() const { return salary; }

	//�������� ���������� ��������� ������ ��������
	bool CheckPhoneNumber() const;

	//��� ����� � �������
	static Worker Login(sql::Connection* con);

	//��� ������� ������
	void ChangePassword(sql::Connection* con);

	//��� ��������� ���������� �� worker_id
	bool operator<(const Worker& other) const { return this->worker_id < other.worker_id; }

	//��� ���������� ����� � ��
	static Worker ReadFromDB(sql::Connection* con, int worker_id);
	//��� ������ ����� � ��
	void WriteToDB(sql::Connection* con);
	//��� ��������� ����� � ��
	void UpdateInDB(sql::Connection* con);
	//��� ��������� ����� � ��
	void DeleteInDB(sql::Connection* con);
	//��� ������ ����� � ������ ���������� � ��
	void WriteWithAccessToSystemToDB(sql::Connection* con);
};