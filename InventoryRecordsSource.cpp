/*
Thomas Matz
CIS 1202.201
Binary Files - Records
10/14/2022
*/

#include <fstream>// step 1 for working with files
#include <iomanip>
#include <cstring>
#include <iostream>
#include <string>

using namespace std;

//Function prototypes
int Show_Menu();
void Create_File(fstream&);
void Display_File(fstream&);
void Display_Record(fstream&, int);
void Modify_Record(fstream&);

const int NAME_SIZE = 40;
struct Product_S
{
	long number;
	char name[NAME_SIZE];
	double price;
	int quantity;
};

const int ONE = 1;
int main()
{
	int choice = 0;
	int recordNum;
	//for this project the file name is given as "inventory.dat"
	//Step 2 for working with files

	//the file object is defined next, and opened at the same command
	//(Step 3 and 4 for working with files
	fstream binaryFile("inventory.dat", ios::in | ios::out | ios::trunc | ios::binary);

	//the file will be processed in all of the functions (Step 5 for working with files)
	Create_File(binaryFile);

	cout << setprecision(2) << fixed;

	while (choice < 5)
	{
		choice = Show_Menu();
		switch (choice)
		{
		case 1:
			//call the display file function
			Display_File(binaryFile);
			break;
		case 2:
			//call the display record function
			cout << "\nEnter the record number.> ";
			cin >> recordNum;
			Display_Record(binaryFile, recordNum - 1);//records begin at 0
			break;
		case 3:
			//call the modify record function
			Modify_Record(binaryFile);
			break;
		case 4:
			choice++;
			break;
		default:
			break;
		}
	}
	binaryFile.close();//the 6th and final step when working with files
}

void Modify_Record(fstream& fileObjectPar)
{
	int recordNum;
	Product_S item;
	cout << "\nEnter the record number to modify.> ";
	cin >> recordNum;

	cout << "\nEnter the new item number: ";
	cin >> item.number;

	cout << "\nEnter the new name.> ";
	cin.ignore();
	cin.get(item.name, NAME_SIZE);
	cin.clear();
	cin.ignore(numeric_limits<streamsize>::max(), '\n');

	cout << "\nEnter the new price.> ";
	cin >> item.price;

	cout << "\nEnter the new quantity.> ";
	cin >> item.quantity;

	fileObjectPar.clear();
	
	recordNum--; // the records begin at 0
	//next seek to the record
	fileObjectPar.seekp(recordNum * sizeof(item), ios::beg);
	//next write to the record
	fileObjectPar.write(reinterpret_cast<char*>(&item), sizeof(item));
}
void Display_Record(fstream& fileObjectPar, int recordNumPar)
{
	Product_S item;
	fileObjectPar.clear();// clear the EOF flag
	fileObjectPar.seekg(recordNumPar * sizeof(item), ios::beg);// move to the 
			// record that was passed down to this function
		//Next read in the entire structure at this record.
	fileObjectPar.read(reinterpret_cast<char*>(&item), sizeof(item));
	//
	cout << "\nRecord Number: " << recordNumPar + 1;
	cout << "\n   Item Number: " << item.number;
	cout << "\n   Item Name: " << item.name;
	cout << "\n   Item Price: $ " << item.price;
	cout << "\n   Item Quantity: " << item.quantity;
}

void Display_File(fstream& fileObjectPar)
{
	int recordNum = 0;
	int positionInFile;
	Product_S item;
	fileObjectPar.clear();// clear the EOF flag
	fileObjectPar.seekg(recordNum, ios::beg);// seek to the first record

	//tellg function gives the position in bytes of the read operation
	//the size of Product_S is 64 bytes in this program
	positionInFile = (fileObjectPar.tellg() / sizeof(item)) + ONE;
	//read in the record (structure) from the binary file
	fileObjectPar.read(reinterpret_cast<char*>(&item), sizeof(item));
	while (!fileObjectPar.eof())
	{
		cout << "\nRecord Number: " << positionInFile;
		cout << "\n   Item Number: " << item.number;
		cout << "\n   Item Name: " << item.name;
		cout << "\n   Item Price: $ " << item.price;
		cout << "\n   Item Quantity: " << item.quantity;
		positionInFile = (fileObjectPar.tellg() / sizeof(item));//update after outputting the record

		//read in the next record (structure) from the binary file
		fileObjectPar.read(reinterpret_cast<char*>(&item), sizeof(item));
	}
}

void Create_File(fstream& fileObjectPar)
{
	//this function populates the structure from customer input
	Product_S item;
	cout << "\nEnter the item number, or 0 to quit.> ";
	cin >> item.number;
	while (item.number > 0)
	{
		cout << "\nEnter the item name.> ";
		cin.ignore();
		cin.get(item.name, NAME_SIZE);
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');

		cout << "\nEnter the item price.> ";
		cin >> item.price;

		cout << "\nEnter the item quantity.> ";
		cin >> item.quantity;

		//write the structure in one command to the binary file
		fileObjectPar.write(reinterpret_cast<char*>(&item), sizeof(item));

		cout << "\nEnter another item number, or 0 to quit.> ";
		cin >> item.number;
	}
}

int Show_Menu()
{
	//this function displays the menu, gets the user input, and validates it
	bool badChoice = true;
	int selectedOption;
	
	while (badChoice)
	{
		cout << "\n1. Display the entire inventory.";
		cout << "\n2. Display a particular product.";
		cout << "\n3. Modify a product.";
		cout << "\n4. Exit the program.";
		cout << "\nEnter your selection> ";

		cin >> selectedOption;

		if (selectedOption < 0 || selectedOption > 5)
			cout << "\n " << selectedOption << " is not an option, try again.";
		else
			badChoice = false;
	}
	return selectedOption;
}