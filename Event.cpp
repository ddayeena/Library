#include "Event.h"
Event::Event():name(""),day(0),month(0),hours(0),minutes(0),event_id(0){}

Event::Event(string name, int day, int month, int hours, int minutes, int event_id):
	name(name),day(day),month(month),hours(hours),minutes(minutes),event_id(event_id){}

void Event::AddLeaderToEvent(Worker& worker){
	leaders.insert(worker);
}

void Event::RemoveLeaderFromEvent(sql::Connection* con,Worker& worker){
	DeleteLeadersInDB(con, worker.GetID());
	leaders.erase(worker);
	UpdateInDB(con);
}

void Event::ShowInfo(){
	cout << "|| " << setw(4) << event_id << " || " <<
		setw(70) << name << " || " <<
		setw(3) << day << " || " <<
		setw(5) << month << " || " <<
		setw(5) << hours << " || " <<
		setw(7) << minutes << " || " <<
		setw(7) << leaders.size() << " || " << endl;
}

void Event::ShowLeaders(){
	cout << "==========================================================================================================" << endl;
	cout << "||                                             =>LEADERS OF EVENT<=                                     ||" << endl;
	cout << "==========================================================================================================" << endl;
	cout << "||  ID  ||   Name    ||  Surname  ||Phone number||    Position    ||        Address       ||   Salary   ||" << endl;
	cout << "==========================================================================================================" << endl;
	for (auto element : leaders) {
		element.ShowInfo();
	}
	cout << "==========================================================================================================" << endl;
}

void Event::ShowTop() {
	cout << "===================================================================================================================================" << endl;
	cout << "||                                                       =>EVENT<=                                                               ||" << endl;
	cout << "===================================================================================================================================" << endl;
	cout << "||  ID  ||                                 Name                                   || Day || Month || Hours || Minutes || Leaders ||" << endl;
	cout << "===================================================================================================================================" << endl;
}

void Event::ShowBottom() {
	cout << "===================================================================================================================================" << endl;
}

void Event::Create(){
	cout << "=>Enter name: ";
	getline(cin >> ws, name);

	cout << "=>Enter month: ";
	while(true){
		this->month = ConsolIO::GetValue(cin);
		if (month > 12 || month < 1) cout << "\n\tMonth must be greater than 0 and less than 12...Try again: ";
		else break;
	}

	cout << "=>Enter day: ";
	while(true){
		this->day = ConsolIO::GetValue(cin);
		if (day <= 0) {
			cout << "\n\tDay must be greater than 0...Try again: ";
			continue;
		}
		bool IsValid = false;
		// ѕерев≥рка коректност≥ введенн€ дн€ залежно в≥д обраного м≥с€ц€
		switch (month) {
		case 4:	case 6:	case 9:	case 11:
			if (day <=30) IsValid = true;	break;
		case 2:
			if (day <=28) IsValid = true;	break;
		default:
			if (day <= 31) IsValid = true;	break;
		}
		if (!IsValid) cout << "\n\tNumber is too big...Try again: ";
		else break;
	}
	
	cout << "=>Enter hours: ";
	while(true){
		this->hours = ConsolIO::GetValue(cin);
		if (hours < 0 || hours> 23) cout << "\n\tHours must be greater than -1 and less than 24...Try again: ";
		else break;
	}

	cout << "=>Enter minutes: ";
	while(true){
		this->minutes = ConsolIO::GetValue(cin);
		if (minutes < 0 || minutes> 59) cout << "\n\tMinutes must be greater than -1 and less 60...Try again: ";
		else break;
	}
}

void Event::Edit(sql::Connection* con){
	int Choice;
	while(true){
		cout << "\n=========";
		cout << "\n=>EDIT<=\n";
		cout << "=========\n";
		cout << "[1] - Name\n[2] - Month\n[3] - Day\n[4] - Hours\n[5] - Minutes\n[6] - Back\n";
		cout << "=>Enter option: ";
		Choice = ConsolIO::GetValue(cin);
		switch (Choice) {
		case 1:
			cout << "=>Enter name: ";
			getline(cin >> ws, this->name);
			break;

		case 2:
			cout << "=>Enter month: ";
			while (true) {
				this->month = ConsolIO::GetValue(cin);
				if (month > 12 || month < 1) cout << "\n\tInvalid input...Try again: ";
				else break;
			}
			break;
		
		case 3: 
			cout << "=>Enter day: ";
			while (true) {
				this->day = ConsolIO::GetValue(cin);
				if (day <= 0) {
					cout << "\n\tDay must be greater than 0...Try again: ";
					continue;
				}
				bool IsValid = false;
				// ѕерев≥рка коректност≥ введенн€ дн€ залежно в≥д обраного м≥с€ц€
				switch (month) {
				case 4:	case 6:	case 9:	case 11:
					if (day <= 30) IsValid = true;	break;
				case 2:
					if (day <= 28) IsValid = true;	break;
				default:
					if (day <= 31) IsValid = true;	break;
				}
				if (!IsValid) cout << "\n\tNumber is too big...Try again: ";
				else break;
			}
			break;
		
		case 4: 
			cout << "=>Enter hours: ";
			while (true) {
				this->hours = ConsolIO::GetValue(cin);
				if (hours < 0 || hours> 23) cout << "\n\tHours must be greater than -1 and less than 24...Try again: ";
				else break;
			}
			break;

		case 5:
			cout << "=>Enter minutes: ";
			while (true) {
				this->minutes = ConsolIO::GetValue(cin);
				if (minutes < 0 || minutes> 59) cout << "\n\Minutes must be greater than -1 and less than 60...Try again: ";
				else break;
			}
			break;

		case 6:
			UpdateInDB(con);
			cout << "\n===>The data was changed successfully<===\n";
			return;

		default: cout << "\n\tInvalid number...Try again: "; break;
		}
	} 
}

Event Event::ReadFromDB(sql::Connection* con, int event_id) {
	Event event;
	try {
		sql::PreparedStatement* stmt;
		sql::ResultSet* res;

		//зчитуЇмо дан≥ про зах≥д
		stmt = con->prepareStatement("Select * FROM event WHERE event_id = ?;");
		stmt->setInt(1, event_id);
		res = stmt->executeQuery();

		if (res->next()) {
			string name = res->getString("name");
			int day = res->getInt("day");
			int month = res->getInt("month");
			int hours = res->getInt("hours");
			int minutes = res->getInt("minutes");

			event = Event(name, day, month, hours, minutes, event_id);
			delete stmt;
			delete res;

			//зчитуЇмо дан≥ про головних прац≥вник≥в заходу
			stmt = con->prepareStatement("SELECT Worker.* "
										"FROM Worker "
										"JOIN eventleader ON Worker.worker_id = eventleader.worker_id "
										"JOIN event ON eventleader.event_id = event.event_id "
										"WHERE Event.event_id = ?; ");
			stmt->setInt(1, event_id);
			res = stmt->executeQuery();
			while (res->next()) {
				int worker_id = res->getInt("worker_id");
				Worker worker = Worker::ReadFromDB(con, worker_id);
				event.leaders.insert(worker);
			}
			delete stmt;
			delete res;
			return event;
		}
	}
	catch (sql::SQLException& e) {
		cout << "# ERR: SQLException in " << __FILE__ << endl;
		cout << "# ERR: " << e.what();
	}
	return event;
}

void Event::UpdateInDB(sql::Connection* con){
	try {
		sql::PreparedStatement* stmt;

		//оновлюЇ дан≥ заходу
		stmt = con->prepareStatement("UPDATE event SET name = ?, day = ?, month = ?, hours = ?, minutes = ?, leaders = ? WHERE event_id = ?;");
		stmt->setString(1, name);
		stmt->setInt(2, day);
		stmt->setInt(3, month);
		stmt->setInt(4, hours);
		stmt->setInt(5, minutes);
		stmt->setInt(6, leaders.size());
		stmt->setInt(7, event_id);
		stmt->executeUpdate();
		delete stmt;
	}
	catch (sql::SQLException& e) {
		cout << "# ERR: SQLException in " << __FILE__ << endl;
		cout << "# ERR: " << e.what();
	}
}

void Event::WriteToDB(sql::Connection* con){
	try {
		sql::PreparedStatement* stmt;
		//записуЇ дан≥ про зах≥д в бд
		stmt = con->prepareStatement("INSERT INTO event (name, day, month, hours, minutes) VALUES (?, ?, ?, ?, ?)");

		stmt->setString(1, name);
		stmt->setInt(2, day);
		stmt->setInt(3, month);
		stmt->setInt(4, hours);
		stmt->setInt(5, minutes);
		stmt->executeUpdate();
		delete stmt;

		//вибираЇ id заходу, €кого було щойно записано, щоб ≥нформац≥€ в≥дображалась коректно
		sql::ResultSet* res = con->createStatement()->executeQuery("SELECT LAST_INSERT_ID();");
		if (res->next())	this->event_id = res->getInt(1);
		delete res;
	}
	catch (sql::SQLException& e) {
		cout << "# ERR: SQLException in " << __FILE__ << endl;
		cout << "# ERR: " << e.what();
	}
}

void Event::DeleteInDB(sql::Connection* con){
	try {
		sql::PreparedStatement* stmt;
		//видал€Їтьс€ ≥нформац≥€ про зах≥д з таблиц≥ eventleader
		stmt = con->prepareStatement("DELETE FROM eventleader WHERE event_id = ?;");
		stmt->setInt(1, event_id);
		stmt->executeUpdate();
		
		//видал€Їтьс€ зах≥д з бд
		stmt = con->prepareStatement("DELETE FROM event WHERE event_id = ?;");
		stmt->setInt(1, event_id);
		stmt->executeUpdate();
		delete stmt;
	}
	catch (sql::SQLException& e) {
		cout << "# ERR: SQLException in " << __FILE__ << endl;
		cout << "# ERR: " << e.what();
	}
}

void Event::WriteLeadersToDB(sql::Connection* con, int worker_id){
	try {
		sql::PreparedStatement* stmt;
		//записуЇ доданого прац≥вника до заходу в бд
		stmt = con->prepareStatement("INSERT INTO eventleader (event_id,worker_id) VALUES (?, ?);");
		stmt->setInt(1, event_id);
		stmt->setInt(2, worker_id);
		stmt->executeUpdate();

		delete stmt;	
	}
	catch (sql::SQLException& e) {
		cout << "# ERR: SQLException in " << __FILE__ << endl;
		cout << "# ERR: " << e.what();
	}
}

void Event::DeleteLeadersInDB(sql::Connection* con, int worker_id){
	try {
		sql::PreparedStatement* stmt;
		//видал€Ї прац≥вника ≥з заходу в бд
		stmt = con->prepareStatement("DELETE FROM eventleader WHERE event_id = ? AND worker_id = ?;");
		stmt->setInt(1, event_id);
		stmt->setInt(2, worker_id);
		stmt->executeUpdate();
		delete stmt;
	}
	catch (sql::SQLException& e) {
		cout << "# ERR: SQLException in " << __FILE__ << endl;
		cout << "# ERR: " << e.what();
	}
}

bool Event::CheckLeaderID(int worker_id) const{
	//перев≥р€Ї чи Ї певний прац≥вник доданий до заходу
	for (auto element : leaders) {
		if (element.GetID() == worker_id)	return true;
	}
	return false;
}