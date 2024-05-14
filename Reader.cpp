#include "Reader.h"
#include "Library.h"
#include <iomanip>
Reader::Reader():name(""), surname(""), phoneNumber(""), reader_id(0) {}

Reader::Reader(string name, string surname, string phoneNumber, int reader_id):name(name),surname(surname),phoneNumber(phoneNumber),reader_id(reader_id){}

void Reader::ShowInfo(){
	cout << "|| " << setw(4) << reader_id << " || " <<
		setw(9) << name << " || " <<
		setw(9) << surname << " || " <<
		setw(10) << phoneNumber << " || " <<
		setw(12) << currentBorrowed << " || " <<
		setw(10) << readBooks.size() << " || " << endl;
}

void Reader::ShowTop() {
	cout << "================================================================================" << endl;
	cout << "||                                   =>READER<=                               ||" << endl;
	cout << "================================================================================" << endl;
	cout << "||  ID  ||   Name    ||  Surname  ||Phone number|| Borrow books || Read books ||" << endl;
	cout << "================================================================================" << endl;
}

void Reader::ShowBottom() {
	cout << "================================================================================" << endl;
}

void Reader::ShowReadBooks(){
	Book::ShowTop();
	for (int i = 0; i < readBooks.size(); i++) {
		readBooks[i].ShowInfo();
	}
	Book::ShowBottom();
}

void Reader::ShowBorrowedBooks(){
	Book::ShowTop();
	for (int i = 0; i < currentBorrowed; i++) {
		borrowedBooks[i].ShowInfo();
	}
	Book::ShowBottom();
}

Reader Reader::ReadFromDB(sql::Connection* con, int reader_id ){
	Reader reader;
	try {
		sql::PreparedStatement* stmt;
		sql::ResultSet* res;

		//зчитуємо дані про читача
		stmt = con->prepareStatement("Select * FROM reader WHERE reader_id = ?;");
		stmt->setInt(1, reader_id);
		res = stmt->executeQuery();

		if (res->next()) {
			string name = res->getString("name");
			string surname = res->getString("surname");
			string phoneNumber = res->getString("phoneNumber");
			reader = Reader(name, surname, phoneNumber, reader_id);

			//зчитуємо дані про прочитані книги
			stmt = con->prepareStatement("SELECT book.* FROM book "
				"JOIN ReadBook ON Book.book_id = ReadBook.book_id "
				"JOIN Reader ON ReadBook.reader_id = Reader.reader_id "
				"WHERE Reader.reader_id = ?; ");
			stmt->setInt(1, reader_id);
			res = stmt->executeQuery();
			while (res->next()) {
				int book_id = res->getInt(1);
				Book book = Book::ReadFromDB(con, book_id);
				reader.readBooks.push_back(book);
			}

			//зчитуємо дані про позичені книги
			stmt = con->prepareStatement("SELECT Book.* FROM Book "
				"JOIN borrowedbook ON Book.book_id = borrowedbook.book_id "
				"JOIN Reader ON borrowedbook.reader_id = Reader.reader_id "
				"WHERE Reader.reader_id = ?; ");
			stmt->setInt(1, reader_id);
			res = stmt->executeQuery();
			while (res->next()) {
				int book_id = res->getInt(1);
				Book book = Book::ReadFromDB(con, book_id);
				reader.borrowedBooks[reader.currentBorrowed] = book;
				reader.currentBorrowed++;
			}
		}
		delete stmt;
		delete res;
		return reader;
	}
	catch (sql::SQLException& e) {
		cout << "# ERR: SQLException in " << __FILE__ << endl;
		cout << "# ERR: " << e.what();
	}
	return reader;
}

void Reader::WriteToDB(sql::Connection* con){
	try {
		sql::PreparedStatement* stmt;
		//записує дані про читача в бд
		stmt = con->prepareStatement("INSERT INTO reader (name, surname, phoneNumber) VALUES (?, ?, ?)");
		stmt->setString(1, name);
		stmt->setString(2, surname);
		stmt->setString(3, phoneNumber);
		stmt->executeUpdate();
		delete stmt;

		//вибирає id читача, якого було щойно записано, щоб інформація відображалась коректно
		sql::ResultSet* res = con->createStatement()->executeQuery("SELECT LAST_INSERT_ID();");
		if (res->next())	this->reader_id = res->getInt(1); 
		delete res;
	}
	catch (sql::SQLException& e) {
		cout << "# ERR: SQLException in " << __FILE__ << endl;
		cout << "# ERR: " << e.what();
	}
}

void Reader::UpdateInDB(sql::Connection* con){
	try {
		sql::PreparedStatement* stmt;

		//оновлює дані читача в бд
		stmt = con->prepareStatement("UPDATE reader SET name = ?, surname = ?, phoneNumber = ?, readbooks = ?, borrowedbooks = ? WHERE reader_id = ?");
		stmt->setString(1, name);
		stmt->setString(2, surname);
		stmt->setString(3, phoneNumber);
		stmt->setInt(4, readBooks.size());
		stmt->setInt(5, currentBorrowed);
		stmt->setInt(6, reader_id);
		stmt->executeUpdate();
		delete stmt;
	}
	catch (sql::SQLException& e) {
		cout << "# ERR: SQLException in " << __FILE__ << endl;
		cout << "# ERR: " << e.what();
	}

}

void Reader::DeleteInDB(sql::Connection* con){
	try {
		sql::PreparedStatement* stmt;
		//встановлює, що книги, які були у читача вже доступні в бібліотеці
		for (int i = 0; i < currentBorrowed; i++) {
			borrowedBooks[i].SetAvailability(true);
			borrowedBooks[i].UpdateInDB(con);
		}

		// Видаляє записи про позичені книги з бд, які відносяться до цього читача
		stmt = con->prepareStatement("DELETE FROM borrowedbook WHERE reader_id = ?;");
		stmt->setInt(1, reader_id);
		stmt->executeUpdate();
		delete stmt;

		// Видаляє записи про прочитані книги з бд, які відносяться до цього читача
		stmt = con->prepareStatement("DELETE FROM readbook WHERE reader_id = ?;");
		stmt->setInt(1, reader_id);
		stmt->executeUpdate();
		delete stmt;

		// Видаляє запис про самого читача з бд
		stmt = con->prepareStatement("DELETE FROM reader WHERE reader_id = ?;");
		stmt->setInt(1, reader_id);
		stmt->executeUpdate();
		delete stmt;
	}
	catch (sql::SQLException& e) {
		cout << "# ERR: SQLException in " << __FILE__ << endl;
		cout << "# ERR: " << e.what();
	}
}

void Reader::DeleteBorrowedBookInDB(sql::Connection* con,int book_id) {
	try {
		sql::PreparedStatement* stmt;
		// Видаляє записи про позичену книги з бд, коли читач повертає її в бібліотеку
		stmt = con->prepareStatement("DELETE FROM borrowedbook WHERE book_id = ? AND reader_id = ?;");
		stmt->setInt(1, book_id);
		stmt->setInt(2, reader_id);
		stmt->executeUpdate();
		delete stmt;
	}
	catch (sql::SQLException& e) {
		cout << "# ERR: SQLException in " << __FILE__ << endl;
		cout << "# ERR: " << e.what();
	}
}

void Reader::WriteReadBookToDB(sql::Connection* con, int book_id){
	try {
		sql::PreparedStatement* stmt;
		//записує прочитані книги читача в бд
		stmt = con->prepareStatement("INSERT INTO readbook (reader_id, book_id) VALUES (?, ?)");
		stmt->setInt(1,reader_id );
		stmt->setInt(2, book_id);
		stmt->executeUpdate();
		delete stmt;
	}
	catch (sql::SQLException& e) {
		cout << "# ERR: SQLException in " << __FILE__ << endl;
		cout << "# ERR: " << e.what();
	}
}

void Reader::WriteBorrowedBookToDB(sql::Connection* con,int book_id){
	try {
		sql::PreparedStatement* stmt;
		//записує позичені книги читача в бд
		stmt = con->prepareStatement("INSERT INTO borrowedbook (reader_id, book_id) VALUES (?, ?)");
		stmt->setInt(1, reader_id);
		stmt->setInt(2, book_id);
		stmt->executeUpdate();
		delete stmt;
	}
	catch (sql::SQLException& e) {
		cout << "# ERR: SQLException in " << __FILE__ << endl;
		cout << "# ERR: " << e.what();
	}
}

void Reader::BorrowBook(sql::Connection* con,Library& library,Book& book){
	//якщо не досягнута ліміту позичених книг, тоді додаємо цю книгу читачу
	if (currentBorrowed + 1< maxBorrowed) {
		//позичаємо книгу в бібліотеки
		library.LendBook(con, book);
		++currentBorrowed;
		WriteBorrowedBookToDB(con, book.GetID());
		//оновлює інформацію про читача
		UpdateInDB(con);
	}
	//якщо досягнуто ліміт, повідомляємо про це читачеві
	else cout << "\nLimit reached...Reader must return books so he can borrow new ones...\n";
}

void Reader::ReturnBook(sql::Connection* con,Library& library, Book& book) {
	//шукаємо цю книгу в користувача
	for (int i = 0; i < currentBorrowed; i++) {
		if (borrowedBooks[i].GetID() == book.GetID()) {
			library.TakeBook(con, book);

			//видаляємо книгу з позичених
			DeleteBorrowedBookInDB(con, book.GetID());
			currentBorrowed--;

			//додаємо книгу до прочитаних
			WriteReadBookToDB(con, book.GetID());
			readBooks.push_back(book);
				
			//оновлюємо дані читача в базі даних
			UpdateInDB(con);
			cout << "\n===>Book was returned to library with ID: " << book.GetID() << "<===" << endl;
			return;
		}
	}
	//якщо дані були передані не коретні
	cout << "\nError....Are you sure you entered the right book?\n";
}

void Reader::Create(){
	cout << "=>Enter name: ";
	name = ConsolIO::GetText(cin);

	cout << "=>Enter surname: ";
	surname = ConsolIO::GetText(cin);

	cout << "=>Enter phone number: ";
	while (true) {
		getline(cin >> ws, phoneNumber);
		if (CheckPhoneNumber()) break;
	}
}

void Reader::Edit(sql::Connection* con){
	int Choice;
	while (true) {
		cout << "\n=========";
		cout << "\n=>EDIT<=\n";
		cout << "=========\n";
		cout<<"[1] - Name\n[2] - Surname\n[3] - Phone number\n[4] - Back\n";
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
			UpdateInDB(con);
			cout << "\n===>The data was changed successfully<===\n";
			return;

		default: cout << "\n\tInvalid number...Try again: "; break;
		}
	}
}

bool Reader::CheckPhoneNumber() const{
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