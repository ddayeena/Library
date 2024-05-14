#include "Worker.h"
#include "Library.h"
Worker::Worker():name(""),surname(""),phoneNumber(""),position(""),address(""),salary(0),worker_id(0),login(""),password(""){}

Worker::Worker(string name, string surname, string phoneNumber, string position, string address,
	double salary, string login, string password, int worker_id):
	name(name), surname(surname), phoneNumber(phoneNumber), position(position), address(address), 
	salary(salary), worker_id(worker_id), login(login), password(password) {}

void Worker::ShowInfo(){
	cout << "|| " << setw(4) << worker_id << " || " <<
		setw(9) << name << " || " <<
		setw(9) << surname << " || " <<
		setw(10) << phoneNumber << " || " <<
		setw(14) << position << " || " <<
		setw(20) << address << " || " <<
		setw(10)  << salary << " ||" << endl;
}

void Worker::ShowTop() {
	cout << "==========================================================================================================" << endl;
	cout << "||                                              =>WORKER<=                                              ||" << endl;
	cout << "==========================================================================================================" << endl;
	cout << "||  ID  ||   Name    ||  Surname  ||Phone number||    Position    ||        Address       ||   Salary   ||" << endl;
	cout << "==========================================================================================================" << endl;
}

void Worker::ShowBottom() {
	cout << "==========================================================================================================" << endl;
}

void Worker::Create() {
	cout << "=>Enter name: ";
	name = ConsolIO::GetText(cin);

	cout << "=>Enter surname: ";
	surname = ConsolIO::GetText(cin);

	cout << "=>Enter phone number: ";
	while (true) {
		getline(cin>>ws, phoneNumber);
		if (CheckPhoneNumber()) break;
	}

	cout << "=>Enter position(Librarian, Administrator, Organizer will have access to system): ";
	getline(cin >> ws, position);
	if (position == "Librarian" || position == "Administrator" || position == "Organizer") {
		cout << "\n=>Enter login: ";
		while (true) {
			login = ConsolIO::GetText(cin);
			//якщо find нічого не знайде, то він поверне npos
			if (login.find(" ") == string::npos)break;
			else cout << "\n\tLogin must not contain spaces...Try again: ";
		}

		cout << "=>Enter password: ";
		while (true) {
			password = ConsolIO::GetText(cin);
			//якщо find нічого не знайде, то він поверне npos
			if (password.find(" ") == string::npos)break;
			else cout << "\n\tPassword must not contain spaces...Try again: ";
		}
	}

	cout << "\n=>Enter address: ";
	getline(cin>>ws, address);

	cout << "=>Enter salary: ";
	this->salary = ConsolIO::GetValue(cin);
}

void Worker::Edit(sql::Connection* con){
	int Choice;
	while(true){
		cout << "\n=========";
		cout << "\n=>EDIT<=\n";
		cout << "=========\n";
		cout<<"[1] - Name\n[2] - Surname\n[3] - Phone number\n[4] - Position\n[5] - Address\n[6] - Salary\n[7] - Back\n";
		cout << "=>Enter option: ";
		Choice = ConsolIO::GetValue(cin);
		switch (Choice) {
		case 1:
			cout << "=>Enter name: ";
			name = ConsolIO::GetText(cin);
			break;

		case 2:
			cout << "=>Enter surname: ";
			surname = ConsolIO::GetText(cin);
			break;

		case 3:
			cout << "=>Enter phone number: ";
			while (true) {
				getline(cin >> ws, phoneNumber);
				if (CheckPhoneNumber()) break;
			}
			break;

		case 4:
			cout << "=>Enter position(Librarian, Administrator, Organizer will have access to system): ";
			getline(cin >> ws, position);
			if (position == "Librarian" || position == "Administrator" || position == "Organizer") {
				cout << "\n=>Enter login: ";
				while (true) {
					login = ConsolIO::GetText(cin);
					//якщо find нічого не знайде, то він поверне npos
					if (login.find(" ") == string::npos)break;
					else cout << "\n\tLogin must not contain spaces...Try again: ";
				}

				cout << "=>Enter password: ";
				while (true) {
					password = ConsolIO::GetText(cin);
					//якщо find нічого не знайде, то він поверне npos
					if (password.find(" ") == string::npos)break;
					else cout << "\n\tPassword must not contain spaces...Try again: ";
				}
			}
			break;

		case 5:
			cout << "=>Enter address: ";
			getline(cin>>ws, address);
			break;

		case 6:
			cout << "=>Enter salary: ";
			this->salary = ConsolIO::GetValue(cin);
			break;

		case 7:
			UpdateInDB(con);
			cout << "\n===>The data was changed successfully<===\n";
			return;

		default: cout << "\n\tInvalid number...Try again: "; break;
		}
	} 
}

Worker Worker::ReadFromDB(sql::Connection* con, int worker_id){
	Worker worker;
	try {
		sql::PreparedStatement* stmt;
		sql::ResultSet* res;

		//зчитує дані про працівника
		stmt = con->prepareStatement("Select * FROM worker WHERE worker_id = ?;");
		stmt->setInt(1, worker_id);
		res = stmt->executeQuery();

		if (res->next()) {
			string name = res->getString("name");
			string surname = res->getString("surname");
			string phoneNumber = res->getString("phoneNumber");
			string position = res->getString("position");
			string address = res->getString("address");
			double salary = res->getDouble("salary");
			delete stmt;
			delete res;

			//зчитує логін і пароль працівника
			stmt = con->prepareStatement("SELECT user.password, user.login FROM user "
				"JOIN worker ON worker.role = user.user_id "
				"WHERE worker.worker_id = ?; ");
			stmt->setInt(1, worker_id);
			res = stmt->executeQuery();
			string login = "", password = "";
			if (res->next()) {
				password = res->getString("password");
				login = res->getString("login");
			}
			delete res;
			delete stmt;
			worker = Worker(name, surname, phoneNumber, position, address, salary,login, password, worker_id);
			return worker;
		}
		delete stmt;
		delete res;
	}
	catch (sql::SQLException& e) {
		cout << "# ERR: SQLException in " << __FILE__ << endl;
		cout << "# ERR: " << e.what();
	}
	return worker;
}

void Worker::WriteToDB(sql::Connection* con){
	try {
		sql::PreparedStatement* stmt;
		//записує дані про працівника в бд
		stmt = con->prepareStatement("INSERT INTO worker (name, surname, "
			"phoneNumber, position, address, salary) VALUES(? , ? , ? , ? , ? , ? )");
		stmt->setString(1, name);
		stmt->setString(2, surname);
		stmt->setString(3, phoneNumber);
		stmt->setString(4, position);
		stmt->setString(5, address);
		stmt->setDouble(6, salary);
		stmt->executeUpdate();
		delete stmt;
		
		//вибирає id працівника,якого було щойно записано, щоб інформація відображалась коректно
		sql::ResultSet* res = con->createStatement()->executeQuery("SELECT LAST_INSERT_ID();");
		if (res->next()) this->worker_id = res->getInt(1);
		delete res;
	}
	catch (sql::SQLException& e) {
		cout << "# ERR: SQLException in " << __FILE__ << endl;
		cout << "# ERR: " << e.what();
	}
}

void Worker::UpdateInDB(sql::Connection* con){
	try {
		sql::PreparedStatement* stmt;
		//оновлює дані працівника в бд
		stmt = con->prepareStatement("UPDATE worker SET name = ?, surname = ?, "
			"phoneNumber = ?, position = ?, address = ?, salary = ? WHERE worker_id = ?;");
		stmt->setString(1, name);
		stmt->setString(2, surname);
		stmt->setString(3, phoneNumber);
		stmt->setString(4, position);
		stmt->setString(5, address);
		stmt->setDouble(6, salary);
		stmt->setInt(7, worker_id);
		stmt->executeUpdate();
		delete stmt;
	}
	catch (sql::SQLException& e) {
		cout << "# ERR: SQLException in " << __FILE__ << endl;
		cout << "# ERR: " << e.what();
	}
}

void Worker::DeleteInDB(sql::Connection* con){
	try {
		sql::PreparedStatement* stmt;
		//видаляє працівника з таблиці eventleader
		stmt = con->prepareStatement("DELETE FROM eventleader WHERE worker_id = ?;");
		stmt->setInt(1, worker_id);
		stmt->executeUpdate();
		delete stmt;

		//видаляється сам працівник з бд
		stmt = con->prepareStatement("DELETE FROM worker WHERE worker_id = ?;");
		stmt->setInt(1,worker_id);
		stmt->executeUpdate();
		delete stmt;
	}
	catch (sql::SQLException& e) {
		cout << "# ERR: SQLException in " << __FILE__ << endl;
		cout << "# ERR: " << e.what();
	}
}

void Worker::WriteWithAccessToSystemToDB(sql::Connection* con){
	try {
		sql::PreparedStatement* stmt;
		//записує логін і пароль працівника в бд
		stmt = con->prepareStatement("INSERT INTO user (login,password,role) VALUES (?, ?, ?);");
		stmt->setString(1, login);
		stmt->setString(2, password);
		stmt->setString(3, position);
		stmt->executeUpdate();
		delete stmt;

		//зчитує id, що було щойно записане
		sql::ResultSet* res = con->createStatement()->executeQuery("SELECT LAST_INSERT_ID();");
		int role_id = 0;
		if (res->next())  role_id = res->getInt(1);

		//встановлює це id в посаду role працівника, щоб мати доступ до його логіна і пароля
		stmt = con->prepareStatement("UPDATE worker SET role = ?  WHERE worker_id = ?;");
		stmt->setInt(1, role_id);
		stmt->setInt(2, worker_id);
		stmt->executeUpdate();
		delete stmt;
		delete res;
	}
	catch (sql::SQLException& e) {
		cout << "# ERR: SQLException in " << __FILE__ << endl;
		cout << "# ERR: " << e.what();
	}
}

bool Worker::CheckPhoneNumber() const {
	//чи коректний розмір номера
	if (phoneNumber.length() != 10) {
		cout << "\n\tPhone number must contain 10 numbers and not contain letters...Try again: ";
		return false;
	}

	//чи містить номер букви
	for (char digit : phoneNumber)
		if (!isdigit(digit)) {
			cout << "\n\tPhone number must contain 10 numbers and not contain letters...Try again: ";
			return false;
		}

	return true;
}

Worker Worker::Login(sql::Connection* con) {
	while (true) {
		string login, password;
		cout << "\n==========";
		cout << "\n=>Log in<=\n";
		cout << "==========\n";
		cout << "=>Enter login: ";
		getline(cin >> ws, login);
		cout << "=>Enter password: ";
		getline(cin >> ws, password);
		try {
			sql::PreparedStatement* stmt;
			sql::ResultSet* res;
			//зчитує логін і пароль
			stmt = con->prepareStatement("Select * FROM user WHERE login = ? AND password = ?;");
			stmt->setString(1, login);
			stmt->setString(2, password);
			res = stmt->executeQuery();

			if (res->next()) {
				//якщо дані введено правильно, зчитує id працівника
				stmt = con->prepareStatement("SELECT worker.worker_id from worker "
					"JOIN user ON USER.user_id = worker.role "
					"WHERE USER.login = ? AND user.password = ?; ");
				stmt->setString(1, login);
				stmt->setString(2, password);
				res = stmt->executeQuery();

				if (res->next()) {
					//зчитує дані про самого працівника
					Worker worker = Worker::ReadFromDB(con, res->getInt("worker_id"));
					system("cls");
					cout << "\n===>Welcome back, " << worker.GetName() << "!<===\n";
					return worker;
				}
			}
			//якщо дані введено не правильно, то потрібно ввести їх заново
			else cout << "\n\tUser is NOT found...Try again\n";
			delete stmt;
			delete res;
		}
		catch (sql::SQLException& e) {
			cout << "# ERR: SQLException in " << __FILE__ << endl;
			cout << "# ERR: " << e.what();
		}
	}
}

void Worker::ChangePassword(sql::Connection* con){
	cout << "\n===================";
	cout << "\n=>CHANGE PASSWORD<=\n";
	cout << "===================\n";
	while (true) {
		cout << "=>Enter login: ";
		string login;
		getline(cin >> ws, login);
		//перевіряє, чи співпадає введений логін з власним логіном
		if (login != this->login) {
			cout << "\n\tIncorrect login...\n";
			continue;
		}
		//перевіряє, чи співпадає введений пароль з власним паролем
		cout << "=>Enter your old password: ";
		string oldPassword;
		getline(cin >> ws, oldPassword);
		if (oldPassword != this->password) {
			cout << "\n\tIncorrect password...\n";
			continue;
		}
		try {
			sql::PreparedStatement* stmt;

			cout << "=>Enter new password: ";
			string newPassword;
			newPassword = ConsolIO::GetText(cin);
			//оновлює дані
			stmt = con->prepareStatement("UPDATE user SET password = ? WHERE user_id = "
				"(SELECT role FROM worker WHERE worker_id = ?);");
			stmt->setString(1, newPassword);
			stmt->setInt(2, worker_id);
			stmt->executeUpdate();
			cout << "\n===>Password was changed succesfully<===\n";
			delete stmt;
		}
		catch (sql::SQLException& e) {
			cout << "# ERR: SQLException in " << __FILE__ << endl;
			cout << "# ERR: " << e.what();
		}
	}
}