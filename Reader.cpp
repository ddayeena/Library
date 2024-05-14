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

		//������� ��� ��� ������
		stmt = con->prepareStatement("Select * FROM reader WHERE reader_id = ?;");
		stmt->setInt(1, reader_id);
		res = stmt->executeQuery();

		if (res->next()) {
			string name = res->getString("name");
			string surname = res->getString("surname");
			string phoneNumber = res->getString("phoneNumber");
			reader = Reader(name, surname, phoneNumber, reader_id);

			//������� ��� ��� �������� �����
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

			//������� ��� ��� ������� �����
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
		//������ ��� ��� ������ � ��
		stmt = con->prepareStatement("INSERT INTO reader (name, surname, phoneNumber) VALUES (?, ?, ?)");
		stmt->setString(1, name);
		stmt->setString(2, surname);
		stmt->setString(3, phoneNumber);
		stmt->executeUpdate();
		delete stmt;

		//������ id ������, ����� ���� ����� ��������, ��� ���������� ������������ ��������
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

		//������� ��� ������ � ��
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
		//����������, �� �����, �� ���� � ������ ��� ������� � ��������
		for (int i = 0; i < currentBorrowed; i++) {
			borrowedBooks[i].SetAvailability(true);
			borrowedBooks[i].UpdateInDB(con);
		}

		// ������� ������ ��� ������� ����� � ��, �� ���������� �� ����� ������
		stmt = con->prepareStatement("DELETE FROM borrowedbook WHERE reader_id = ?;");
		stmt->setInt(1, reader_id);
		stmt->executeUpdate();
		delete stmt;

		// ������� ������ ��� �������� ����� � ��, �� ���������� �� ����� ������
		stmt = con->prepareStatement("DELETE FROM readbook WHERE reader_id = ?;");
		stmt->setInt(1, reader_id);
		stmt->executeUpdate();
		delete stmt;

		// ������� ����� ��� ������ ������ � ��
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
		// ������� ������ ��� �������� ����� � ��, ���� ����� ������� �� � ��������
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
		//������ �������� ����� ������ � ��
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
		//������ ������� ����� ������ � ��
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
	//���� �� ��������� ���� ��������� ����, ��� ������ �� ����� ������
	if (currentBorrowed + 1< maxBorrowed) {
		//�������� ����� � ��������
		library.LendBook(con, book);
		++currentBorrowed;
		WriteBorrowedBookToDB(con, book.GetID());
		//������� ���������� ��� ������
		UpdateInDB(con);
	}
	//���� ��������� ���, ����������� ��� �� �������
	else cout << "\nLimit reached...Reader must return books so he can borrow new ones...\n";
}

void Reader::ReturnBook(sql::Connection* con,Library& library, Book& book) {
	//������ �� ����� � �����������
	for (int i = 0; i < currentBorrowed; i++) {
		if (borrowedBooks[i].GetID() == book.GetID()) {
			library.TakeBook(con, book);

			//��������� ����� � ���������
			DeleteBorrowedBookInDB(con, book.GetID());
			currentBorrowed--;

			//������ ����� �� ����������
			WriteReadBookToDB(con, book.GetID());
			readBooks.push_back(book);
				
			//��������� ��� ������ � ��� �����
			UpdateInDB(con);
			cout << "\n===>Book was returned to library with ID: " << book.GetID() << "<===" << endl;
			return;
		}
	}
	//���� ��� ���� ������� �� ������
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
	//�� ��������� ����� ������
	if (phoneNumber.length() != 10) {
		cout << "\n\tPhone number must contain 10 numbers and not contain letters...Try again: ";
		return false;
	}

	//�� ������ ����� �����
	for (char digit : phoneNumber)
		if (!isdigit(digit)) {
			cout << "\n\tPhone number must contain 10 numbers and not contain letters...Try again: ";
			return false;
		}

	return true;
}