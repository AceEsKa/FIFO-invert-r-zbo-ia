#pragma once
#include <iostream>
#include "queue.cpp"
#include "Vector.cpp"
class inventar
{
private:
	//commands used to intearct with the program trough user input
	enum UserCommands
	{
		Inventory = 'i', Buy = 'k', Sell = 'p', Load = 'n', Statisticks = 's', Export = 'w', EXIT = 'e',
		//represents unsuported command
		UNSAPORTED_COMMAND = 'x'
	};

	//item identyfiers
	enum eItemID
	{
		ItemOne = 1, ItemTwo, ItemThree,
		//represents invalid item
		INVALID_ID
	};

	//Types of transactions supported by the program.
	enum eTransaction
	{
		buying = 'K', selling = 'P'
	};
public:
	//defualt constructor
	inventar();
	//prints menu
	void printMenu();

	//gets users input
	void getInput();

	//ensures we got correct input from user, prevents input of different type than expected
	bool chceckInput();

	//handles the excecution of necceseary command
	void commandRouter(char c, std::string fileName = "inventar.txt");

	//shows stock of a given item
	void showItemInventory(queue<double>& item, eItemID itemID);

	//input from from console/terminal
	void buy(queue<double>& item, eItemID itemID);

	//handles buy command from file
	void buy(int amount, queue<double>& item, std::string s_price, eItemID itemID);

	//input from console / terminal
	void sell(queue<double>& item, eItemID itemID);

	//handles sell command from file
	void sell(size_t amount, queue<double>& item, eItemID itemID);

	//gets the number of digits of price or amount
	unsigned int getDigits(std::string line, unsigned int start = 4);

	//handles buy/sell input from file
	void performTransaction(queue<double>& item, std::string line, eItemID itemID);

	//loads our inventar from file
	void load(std::string fileName = "inventar.txt");

	//logs all of the transactions and info about them, both new and previous transactions that happend
	void logTransaction(int amount, double price, eItemID itemID, eTransaction c);

	//writes our log in to file
	void write(std::string fileName = "inventar.txt");

	//shows statistics of our seels of items 1,2,3
	void showStatistics();

	//terminates program
	void Exit() { exit(0); }

	//destructor
	~inventar() { }
private:

	//items 
	queue<double> m_item1;

	queue<double> m_item2;

	queue<double> m_item3;

	//holds info about given transaction sell/buy
	struct transactionInfo
	{
		eItemID  m_ItemID;
		eTransaction m_Transaction;
		double m_Price;
		size_t m_Amount;

		transactionInfo() {};

		//counstructs our struct from given set of data
		transactionInfo(size_t amount, double price, eItemID itemID, eTransaction c)
			: m_ItemID(itemID), m_Transaction(c), m_Price(price), m_Amount(amount) {}

		//checks if it holds the same data as given transaction if true it merges the amounts
		bool operator==(const transactionInfo& other)
		{
			if (this->m_ItemID == other.m_ItemID && this->m_Price == other.m_Price && this->m_Transaction == other.m_Transaction)
			{
				this->m_Amount += other.m_Amount;
				return true;
			}
			return false;
		}
	};

	//holds information about all of our transactions
	queue<transactionInfo> m_log;

	//hold info about the price of bought/sold units
	struct info
	{
		int m_units = 1;
		double m_Price = 0;

		info() {}

		//as we are most of the time just merging same units there is a need for constructor that would take only price 
		info(double price)
			:m_Price(price) {}

		//counstructs our struct from given set of data
		info(int amount, double price)
			: m_units(amount), m_Price(price) {}

		//checks if it holds the same data as given info if true it merges the amounts
		bool operator==(const info& other)
		{
			if (other.m_Price == this->m_Price)
			{
				m_units += other.m_units;
				return true;
			}
			return other.m_Price == this->m_Price;
		}
	};


	UserCommands mapCommand(char c);

	eItemID mapeItemID(int item);
};