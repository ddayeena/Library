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
	//конструктори
	Worker();
	Worker(string name, string surname, string phoneNumber, string position, string address, double salary,string login,string password, int worker_id = 0);
	//для перегляду інформації
	void ShowInfo();
	static void ShowTop();
	static void ShowBottom();

	//для створення працівника
	void Create();
	//для редагування працівника
	void Edit(sql::Connection* con);

	//сетери
	void SetName(string name) { this->name = name; }
	void SetSurname(string surname) { this->surname = surname; }
	void SetPhoneNumber(string phoneNumber) { this->phoneNumber = phoneNumber; }
	void SetPosition(string position) { this->position = position; }
	void SetAddress(string address) { this->address = address; }
	void SetSalary(double salary) { this->salary = salary; }

	//гетери
	int GetID() const { return worker_id; }
	string GetName() const { return name; }
	string GetSurname() const { return surname; }
	string GetPhoneNumber() const { return phoneNumber; }
	string GetPosition() const { return position; }
	string GetAddress() const { return address; }
	double GetSalary() const { return salary; }

	//перевірка коректності введеного номера телефону
	bool CheckPhoneNumber() const;

	//для входу в систему
	static Worker Login(sql::Connection* con);

	//для змінення паролю
	void ChangePassword(sql::Connection* con);

	//для порівняння працівників за worker_id
	bool operator<(const Worker& other) const { return this->worker_id < other.worker_id; }

	//для зчитування даних з бд
	static Worker ReadFromDB(sql::Connection* con, int worker_id);
	//для запису даних в бд
	void WriteToDB(sql::Connection* con);
	//для оновлення даних в бд
	void UpdateInDB(sql::Connection* con);
	//для видалення даних з бд
	void DeleteInDB(sql::Connection* con);
	//для запису логіна і пароля працівника в бд
	void WriteWithAccessToSystemToDB(sql::Connection* con);
};