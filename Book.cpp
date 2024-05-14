#include "Book.h"
Book::Book() :book_id(0), author(""), name(""), year(0), genre(Genre::NoGenre), location(Location::NoLocation), isAvailable(0){}
Book::Book(string author, string name, int year, Genre genre, Location location, bool isAvailable,int book_id)
	:author(author),name(name),year(year),genre(genre),location(location),isAvailable(isAvailable),book_id(book_id){}

void Book::Edit(sql::Connection* con){
	int Choice;
	while(true){
		cout << "\n=========";
		cout << "\n=>EDIT<=\n";
		cout << "=========\n";
		cout<<"[1] - Author\n[2] - Name\n[3] - Year\n[4] - Genre\n[5] - Location\n[6] - Availabilty\n[7] - Back\n";
		cout << "=>Enter option: ";
		Choice = ConsolIO::GetValue(cin);
		switch (Choice) {
		case 1:
			cout << "=>Enter author: ";
			getline(cin >> ws, this->author);
			break;

		case 2:
			cout << "=>Enter name: ";
			getline(cin ,this->name);
			break;

		case 3: {
			cout << "=>Enter year: ";
			while (true) {
				this->year = ConsolIO::GetValue(cin);
				if (year < 0) cout << "\n\tYear can't be less than 0...Try again: ";
				else break;
			}
			break;
		}
		case 4: {
			string genre;
			for (int i = 0; i < int(Genre::NoGenre); ++i) {
				cout << "\n\t" << GenreToString(Genre(i));
			}
			cout << "\n=>Enter genre: ";
			while (true) {
				getline(cin , genre);
				if (stringToGenre(genre) == NoGenre) cout << "\n\tInvalid input...Try again: ";
				else break;
			}
			this->genre = stringToGenre(genre);
			break;
		}

		case 5: {
			string location;
			for (int i = 0; i < int(Location::NoLocation); i++) {
				cout << "\n\t" << LocationToString(Location(i));
			}
			cout << "\n=>Enter location: ";
			while (true) {
				getline(cin , location);
				if (stringToLocation(location) == NoLocation) cout << "\n\tInvalid input...Try again: ";
				else break;
			}
			this->location = stringToLocation(location);
			break;
		}

		case 6:
			cout << "=>Is book available y/n: ";
			char isAvailable;
			cin >> isAvailable;
			this->isAvailable = (isAvailable == 'y');

			break;

		case 7:
			UpdateInDB(con);
			cout << "\n===>The data was changed successfully<===\n";
			return;

		default:cout << "\n\tInvalid number...Try again: \n"; 
		}
	}

}

void Book::Create() {
	cout << "=>Enter name: ";
	getline(cin >> ws, name);

	cout << "=>Enter author: ";
	getline(cin, author);

	cout << "=>Enter year: ";
	while (true) {
		this->year = ConsolIO::GetValue(cin);
		if (year < 0) cout << "\n\tYear can't be less than 0...Try again: ";
		else break;
	}

	string genre;
	for (int i = 0; i < int(Genre::NoGenre); ++i) {
		cout << "\n\t" << GenreToString(Genre(i));
	}
	cout << "\n=>Enter  genre: ";
	while (true) {
		getline(cin, genre);
		if (stringToGenre(genre) == NoGenre) cout << "\n\tInvalid input...Try again: ";
		else break;
	}
	this->genre = stringToGenre(genre);

	string location;
	for (int i = 0; i < int(Location::NoLocation); i++) {
		cout << "\n\t" << LocationToString(Location(i));
	}
	cout << "\n=>Enter location: ";
	while (true) {
		getline(cin, location);
		if (stringToLocation(location) == NoLocation) cout << "\n\tInvalid input...Try again: ";
		else break;
	}
	this->location = stringToLocation(location);

	isAvailable = true;
}

void Book::ShowInfo(){
	cout << "|| " << setw(4) << book_id << " || " << 
		setw(16) << author << " || " << 
		setw(26) << name << " || " <<
		setw(6) << year << " || " << 
		setw(12) << GenreToString(genre) << " || " <<
		setw(24) << LocationToString(location) << " || " << 
		setw(9) << boolalpha << isAvailable << " ||" << endl;
}

void Book::ShowTop() {
	cout << "===============================================================================================================================" << endl;
	cout << "||                                                       =>BOOK<=                                                            ||" << endl;
	cout << "===============================================================================================================================" << endl;
	cout << "||  ID  ||      Author      ||            Name            ||  Year  ||    Genre     ||         Location         || Available ||" << endl;
	cout << "===============================================================================================================================" << endl;
}

void Book::ShowBottom() {
	cout << "===============================================================================================================================" << endl;
}

Book Book::ReadFromDB(sql::Connection* con,int book_id){
	Book book;
	try {
		sql::PreparedStatement* stmt;
		sql::ResultSet* res;
		//зчитує дані про книгу
		stmt = con->prepareStatement("Select * FROM book WHERE book_id = ?;");
		stmt->setInt(1,book_id);
		res = stmt->executeQuery();

		if (res->next()) {
			string author = res->getString("author");
			string name =  res->getString("name");
			int year =  res->getInt("year");
			string genre =  res->getString("genre");
			string location =  res->getString("location");
			bool isAvailable = res->getInt("isAvailable");
			book = Book( author,name, year, stringToGenre(genre), stringToLocation(location), isAvailable, book_id);
		}
		delete stmt;
		delete res;
	}
	catch (sql::SQLException& e) {
		cout << "# ERR: SQLException in " << __FILE__<<endl;
		cout << "# ERR: " << e.what();
	}
	return book;
}

void Book::WriteToDB(sql::Connection* con){
	try {
		sql::PreparedStatement* stmt;
		//записує дані про книгу
		stmt = con->prepareStatement("INSERT INTO book (author, name, year, genre, location, isAvailable) VALUES (?, ?, ?, ?, ?, ?)");
		stmt->setString(1, author);
		stmt->setString(2, name);
		stmt->setInt(3, year);
		stmt->setString(4, GenreToString(genre));
		stmt->setString(5, LocationToString(location));
		stmt->setInt(6, isAvailable);
		stmt->executeUpdate();
		delete stmt;

		//вибирає id книги,якого було щойно записано, щоб інформація відображалась коректно
		sql::ResultSet* res = con->createStatement()->executeQuery("SELECT LAST_INSERT_ID();");
		if (res->next()) 	this->book_id = res->getInt(1);
		delete res;
	}
	catch (sql::SQLException& e) {
		cout << "# ERR: SQLException in " << __FILE__<<endl;
		cout << "# ERR: " << e.what();
	}
}

void Book::UpdateInDB(sql::Connection* con){
	try {
		sql::PreparedStatement* stmt;
		//оновлює дані про книгу
		stmt = con->prepareStatement("UPDATE book SET author = ?, name = ?, year = ? , genre = ?, location = ?, isAvailable = ? WHERE book_id = ?");
		stmt->setString(1, author);
		stmt->setString(2, name);
		stmt->setInt(3, year);
		stmt->setString(4, GenreToString(genre));
		stmt->setString(5, LocationToString(location));
		stmt->setInt(6, isAvailable);
		stmt->setInt(7, book_id);
		stmt->executeUpdate();

		delete stmt;
	}
	catch (sql::SQLException& e) {
		cout << "# ERR: SQLException in " << __FILE__ << endl;
		cout << "# ERR: " << e.what();
	}
}

void Book::DeleteInDB(sql::Connection* con){
	try {
		sql::PreparedStatement* stmt;
		//видаляє книгу з таблиці borrowedbook
		stmt = con->prepareStatement("DELETE FROM borrowedbook WHERE book_id = ?;");
		stmt->setInt(1, book_id);
		stmt->executeUpdate();
		delete stmt;

		stmt = con->prepareStatement("DELETE FROM readbook WHERE book_id = ?;");
		stmt->setInt(1, book_id);
		stmt->executeUpdate();
		delete stmt;

		stmt = con->prepareStatement("DELETE FROM book WHERE book_id = ?;");
		stmt->setInt(1, book_id);
		stmt->executeUpdate();
		delete stmt;
	}
	catch (sql::SQLException& e) {
		cout << "# ERR: SQLException in " << __FILE__ << endl;
		cout << "# ERR: " << e.what();
	}
}

string Book::GenreToString(Genre genre){
	switch (genre) {
	case Classic:		return "Classic";
	case Dystopia:		return "Dystopia";
	case Mystery: 		return "Mystery";
	case Horror:  		return "Horror";
	case Romance: 		return "Romance";
	case Science_fiction:return "Science fiction";
	case Fantasy: 		return "Fantasy";
	case Thriller:		return "Thriller";
	case Historical_fiction:return "Historical fiction";
	case Biography:		return "Biography";
	case Poetry:		return "Poetry";
	case Drama: 		return "Drama";
	case Comedy:		return "Comedy";
	default: return "Unknown genre";
	}
}
Book::Genre Book::stringToGenre(string genre) {
	if (genre == "Classic") {
		return Classic;
	}
	else if (genre == "Dystopia") {
		return Dystopia;
	}
	else if (genre == "Mystery") {
		return Mystery;
	}
	else if (genre == "Horror") {
		return Horror;
	}
	else if (genre == "Romance") {
		return Romance;
	}
	else if (genre == "Science fiction") {
		return Science_fiction;
	}
	else if (genre == "Fantasy") {
		return Fantasy;
	}
	else if (genre == "Thriller") {
		return Thriller;
	}
	else if (genre == "Historical fiction") {
		return Historical_fiction;
	}
	else if (genre == "Biography") {
		return Biography;
	}
	else if (genre == "Poetry") {
		return Poetry;
	}
	else if (genre == "Drama") {
		return Drama;
	}
	else if (genre == "Comedy") {
		return Comedy;
	}
	else return NoGenre;
}

string Book::LocationToString(Location location) {
	switch (location) {
	case Fiction:				return "Fiction";
	case Non_fiction:			return "Non-fiction";
	case Childrens_literature:	return "Children's literature";
	case Magazines:				return "Magazines";
	case Historical_publications:return "Historical publications";
	case Encyclopedias:			return "Encyclopedias";
	case Rare_editions:			return "Rare editions";
	case Educational_materials:	return "Educational materials";
	default:					return "Unknown location";
	}
}
Book::Location Book::stringToLocation(string location) {
	if (location == "Fiction") {
		return Fiction;
	}
	else if (location == "Non-fiction") {
		return Non_fiction;
	}
	else if (location == "Children's literature") {
		return Childrens_literature;
	}
	else if (location == "Magazines") {
		return Magazines;
	}
	else if (location == "Historical publications") {
		return Historical_publications;
	}
	else if (location == "Encyclopedias") {
		return Encyclopedias;
	}
	else if (location == "Rare editions") {
		return Rare_editions;
	}
	else if (location == "Educational materials") {
		return Educational_materials;
	}
	else return NoLocation;
}