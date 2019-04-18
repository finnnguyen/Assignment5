// Finn Nguyen
// CPSC 301-01 
// Tues-Thurs: 10-12:50pm

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "person.h"
#include "book.h"
using namespace std;

void printMenu();
void readBooks(vector<Book *> & books);
int readPersons(vector<Person *> & cardholders);
void readRentals(vector<Book *> & books, vector<Person *> & cardholders);
Book * searchBook(vector<Book *> books, int id);
Person * searchPerson(vector<Person *> cardholders, int id);
Person * searchPerson(vector<Person *> cardholders, string firstName, string lastName);
void writePersons(vector<Person *> & cardholders);
void writeRentals(vector<Book *> & books);

int main()
{
	vector<Book *> books;
	vector<Person *> cardholders;
	int choice;
	int nextID;
	string firstName, lastName;
	int cardID;
	int bookID;
	Person * personPtr;
	Book * bookPtr;
	char confirm;
	int count;

	readBooks(books);
	nextID = readPersons(cardholders);
	readRentals(books, cardholders);

	do
	{
		printMenu();
		cin >> choice;
		
		switch (choice)
		{
		case 1:
			cout << "Please enter the card ID: ";
			cin >> cardID;

			personPtr = searchPerson(cardholders, cardID);

			if (personPtr == nullptr || !personPtr->isActive())
				cout << "Card ID not found" << endl;
			else
			{
				cout << "Cardholder: " << personPtr->fullName() << endl;
				
				cout << "Please enter the book ID: ";
				cin >> bookID;

				bookPtr = searchBook(books, bookID);

				if (bookPtr == nullptr)
					cout << "Book ID not found" << endl;
				else
				{
					if (bookPtr->getPersonPtr() != nullptr)
						cout << "Book already checked out" << endl;
					else
					{
						cout << "Title: " << bookPtr->getTitle() << endl;
						bookPtr->setPersonPtr(personPtr);
						cout << "Rental Completed" << endl;
					}
				}
			}
			break;
		case 2:
			cout << "Please enter the book ID to return: ";
			cin >> bookID;

			bookPtr = searchBook(books, bookID);

			if (bookPtr == nullptr)
				cout << "Book ID not found" << endl;
			else
			{
				cout << "Title: " << bookPtr->getTitle() << endl;

				if (bookPtr->getPersonPtr() == nullptr)
					cout << "Book is not checked out" << endl;
				else
				{
					bookPtr->setPersonPtr(nullptr);
					cout << "Return Completed" << endl;
				}
			}
			break;
		case 3:
			count = 0;

			for (size_t i = 0; i < books.size(); i++)
			{
				if (books[i]->getPersonPtr() == nullptr)
				{
					cout << "Book ID: " << books[i]->getId() << endl;
					cout << "Title: " << books[i]->getTitle() << endl;
					cout << "Author: " << books[i]->getAuthor() << endl;
					cout << "Category: " << books[i]->getCategory() << endl;
					cout << endl;
					count++;
				}
			}

			if (count == 0)
				cout << "No available books" << endl;
			break;
		case 4:
			count = 0;

			for (size_t i = 0; i < books.size(); i++)
			{
				personPtr = books[i]->getPersonPtr();

				if (personPtr != nullptr)
				{
					cout << "Book ID: " << books[i]->getId() << endl;
					cout << "Title: " << books[i]->getTitle() << endl;
					cout << "Author: " << books[i]->getAuthor() << endl;
					cout << "Cardholder: " << personPtr->fullName() << endl;
					cout << "Card ID: " << personPtr->getId() << endl;
					cout << endl;
					count++;
				}
			}

			if (count == 0)
				cout << "No outstanding rentals" << endl;
			break;
		case 5:
			cout << "Please enter the card ID: ";
			cin >> cardID;

			personPtr = searchPerson(cardholders, cardID);

			if (personPtr == nullptr || !personPtr->isActive())
				cout << "Card ID not found" << endl;
			else
			{
				cout << "Cardholder: " << personPtr->fullName() << endl;

				count = 0;

				for (size_t i = 0; i < books.size(); i++)
				{
					if (books[i]->getPersonPtr() == personPtr)
					{
						cout << endl;
						cout << "Book ID: " << books[i]->getId() << endl;
						cout << "Title: " << books[i]->getTitle() << endl;
						cout << "Author: " << books[i]->getAuthor() << endl;
						count++;
					}
				}

				if (count == 0)
					cout << "No books currently checked out" << endl;
			}
			break;
		case 6:
			cout << "Please enter the first name: ";
			cin >> firstName;
			cout << "Please enter the last name: ";
			cin >> lastName;

			personPtr = searchPerson(cardholders, firstName, lastName);

			if (personPtr != nullptr)
			{
				personPtr->setActive(true);
			}
			else
			{
				personPtr = new Person(nextID, true, firstName, lastName);
				cardholders.push_back(personPtr);

				nextID++;
			}

			cout << "Card ID " << personPtr->getId() << " active" << endl;
			cout << "Cardholder: " << personPtr->fullName() << endl;
			break;
		case 7:
			cout << "Please enter the card ID: ";
			cin >> cardID;
			
			personPtr = searchPerson(cardholders, cardID);

			if (personPtr == nullptr)
				cout << "Card ID not found" << endl;
			else
			{
				cout << "Cardholder: " << personPtr->fullName() << endl;

				if (!personPtr->isActive())
					cout << "Card ID is already inactive" << endl;
				else
				{
					cout << "Are you sure you want to deactivate card (y/n)? ";
					cin >> confirm;

					if (confirm == 'y' || confirm == 'Y')
					{
						for (size_t i = 0; i < books.size(); i++)
						{
							if (books[i]->getPersonPtr() == personPtr)
							{
								books[i]->setPersonPtr(nullptr);
							}
						}

						personPtr->setActive(false);
						cout << "Card ID deactivated" << endl;
					}
				}
			}
			break;
		case 8:
			writePersons(cardholders);
			writeRentals(books);
			break;
		default:
			cout << "Invalid entry" << endl;
			break;
		}

		cout << endl;
	} while (choice != 8);

	for (size_t i = 0; i < books.size(); i++)
		delete books[i];

	for (size_t i = 0; i < cardholders.size(); i++)
		delete cardholders[i];

	return 0;
}

void printMenu() {
	cout << "----------Library Book Rental System----------" << endl;
	cout << "1.  Book checkout" << endl;
	cout << "2.  Book return" << endl;
	cout << "3.  View all available books" << endl;
	cout << "4.  View all outstanding rentals" << endl;
	cout << "5.  View outstanding rentals for a cardholder" << endl;
	cout << "6.  Open new library card" << endl;
	cout << "7.  Close library card" << endl;
	cout << "8.  Exit system" << endl;
	cout << "Please enter a choice: ";
}

void readBooks(vector<Book *> & books) {
	ifstream fin;
	int bookID;
	string title, author, category;
	string line;

	fin.open("books.txt");

	if (!fin)
	{
		cout << "Error: could not open file 'books.txt'" << endl;
		return;
	}

	while (fin >> bookID)
	{
		getline(fin, line);
		getline(fin, title);
		getline(fin, author);
		getline(fin, category);

		books.push_back(new Book(bookID, title, author, category));
	}

	fin.close();
}

int readPersons(vector<Person *> & cardholders) {
	ifstream fin;
	int cardID;
	bool active;
	string firstName, lastName;
	int largestCardID = -1;

	fin.open("persons.txt");

	if (!fin)
	{
		cout << "Error: could not open file 'persons.txt'" << endl;
		return 0;
	}

	while (fin >> cardID >> active >> firstName >> lastName)
	{
		if (cardholders.size() == 0 || cardID > largestCardID)
			largestCardID = cardID;

		cardholders.push_back(new Person(cardID, active, firstName, lastName));
	}

	fin.close();

	return largestCardID + 1;
}

void readRentals(vector<Book *> & books, vector<Person *> & cardholders) {
	ifstream fin;
	int bookID, cardID;

	fin.open("rentals.txt");

	if (!fin)
	{
		cout << "Error: could not open file 'rentals.txt'" << endl;
		return;
	}

	while (fin >> bookID >> cardID)
	{
		Book * bookPtr = searchBook(books, bookID);
		Person * personPtr = searchPerson(cardholders, cardID);

		bookPtr->setPersonPtr(personPtr);
	}

	fin.close();
}

Book * searchBook(vector<Book *> books, int id) {
	for (size_t i = 0; i < books.size(); i++)
		if (books[i]->getId() == id)
			return books[i];

	return nullptr;
}

Person * searchPerson(vector<Person *> cardholders, int id) {
	for (size_t i = 0; i < cardholders.size(); i++)
		if (cardholders[i]->getId() == id)
			return cardholders[i];

	return nullptr;
}

Person * searchPerson(vector<Person *> cardholders, string firstName, string lastName) {
	for (size_t i = 0; i < cardholders.size(); i++)
		if (cardholders[i]->getFirstName() == firstName &&
			cardholders[i]->getLastName() == lastName)
			return cardholders[i];

	return nullptr;
}

void writePersons(vector<Person *> & cardholders) {
	ofstream fout;

	fout.open("persons.txt");

	if (!fout)
	{
		cout << "Error: could not open file 'persons.txt'" << endl;
		return;
	}

	for (size_t i = 0; i < cardholders.size(); i++)
	{
		Person *p = cardholders[i];

		fout << p->getId() << " " << p->isActive() << " " << p->fullName() << endl;
	}

	fout.close();
}

void writeRentals(vector<Book *> & books) {
	ofstream fout;

	fout.open("rentals.txt");

	if (!fout)
	{
		cout << "Error: could not open file 'rentals.txt'" << endl;
		return;
	}

	for (size_t i = 0; i < books.size(); i++)
	{
		Book *b = books[i];
		Person *p = b->getPersonPtr();

		if (p != nullptr)
		{
			fout << b->getId() << " " << p->getId() << endl;
		}
	}

	fout.close();
}
