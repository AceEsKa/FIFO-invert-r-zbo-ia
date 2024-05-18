#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <regex>
#include <iomanip>
#include "inventar.h"

void inventar::printMenu()
{
	std::cout << "<i>nventar ..... vypis inventar pre cislo polozky" << '\n';
	std::cout << "<k>up .......... nakup X kusov pre cislo polozky" << '\n';
	std::cout << "<p>redaj ....... predaj X kusov pre cislo polozky" << '\n';
	std::cout << "<n>acitaj ...... nacitaj inventar zo suboru" << '\n';
	std::cout << "<s>tatistika ... vyis statistiku zbozia" << '\n';
	std::cout << "<w>rite ........ zapis inventar do suboru" << '\n';
	std::cout << "<e>exit ........ ukoncit program" << '\n' << '\n';
}

void inventar::getInput()
{
	printMenu();

	std::string userInput;
	do
	{
		std::cout << "zadaj svoju volbu: ";
		getline(std::cin,userInput);

		std::string substring = userInput.substr(1, userInput.length());

		if (substring.length() > 5)
		{
			commandRouter(userInput[0], substring);
		}
		else
		{
			commandRouter(userInput[0]);
		}


		// Clear the input buffer otherwise doesnt prompt user input and throws exception 
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	} while (true);

}

void inventar::logTransaction(int amount, double price, eItemID itemID, eTransaction c)
{
	m_log.emplaceBack(amount, price, itemID, c);
}

//handles inccorect input
bool inventar::chceckInput()
{
	if (std::cin.fail())
	{
		//clears the error state
		std::cin.clear();

		//discards invalid input
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		std::cerr << "error nespravny input!" << '\n';
		return false;
	}

	return true;
}

//handles routing/calling of appropriate functions according to user command
void inventar::commandRouter(char c,std::string fileName)
{
	int i_input;
	switch (c)
	{
	case Inventory:
		std::cout << "Pre ktoru polozku?: ";

		std::cin >> i_input;

		if (!chceckInput()) {	 return; }

		switch (i_input)
		{
		case ItemOne:
			showItemInventory(m_item1, ItemOne);
			break;
		case ItemTwo:
			showItemInventory(m_item2, ItemTwo);
			break;
		case ItemThree:
			showItemInventory(m_item3, ItemThree);
			break;
		default:
			std::cout << "neexistujuci item" << '\n';
			return;
		}
		break;
	case Buy:
		std::cout << "Zadaj polozku, pocet ks, cenu:" <<'\n';
		std::cin >> i_input;

		if (!chceckInput()) { return; }
		switch (i_input)
		{
		case eItemID::ItemOne:
			buy(m_item1, ItemOne);
			break;
		case eItemID::ItemTwo:
			buy(m_item2, ItemTwo);
			break;
		case eItemID::ItemThree:
			buy(m_item3, ItemThree);
			break;
		default:
			return;
		}
		break;
	case Sell:
		std::cout << "Zadaj polozku, pocet ks na predaj:" << '\n';
		std::cin >> i_input;
		if (!chceckInput()) { return; }

		switch (i_input)
		{
		case eItemID::ItemOne:
			sell(m_item1, ItemOne);
			break;
		case eItemID::ItemTwo:
			sell (m_item2, ItemTwo);
			break;
		case eItemID::ItemThree:
			sell (m_item3, ItemThree);
			break;
		default:
			return;
		}
		break;
	case Load:
		load(fileName);
		break;
	case Statisticks:
		showStatistics();
		break;
	case Export:
		write(fileName);
		break;
	case EXIT:
		Exit();
		break;
	default:
		std::cout << "Router error! Unkown command" << '\n';
		break;
	}
}

void inventar::showItemInventory(queue<double>& item,eItemID itemID)
{
	if (item.empty()) 
	{
		std::cout << '\n' << "Item " << itemID;
		std::cout << "0 units" << '\n';
		return;
	}

	std::cout << '\n' << "Item " << itemID;

	Vector<info> unitsPrices;

	for (queue<double>::const_Iterator it = item.begin(); it != item.end(); ++it)
	{
		unitsPrices.PushBackUnique(info(*it));
	}

	for (unsigned int i = 0; i < unitsPrices.size(); ++i)
	{
		std::cout << '\n';
		std::cout << unitsPrices[i].m_units << " units - " << std::fixed << std::setprecision(2) << unitsPrices[i].m_Price << " EUR";
	}

	std::cout << '\n';
}

//input from from console/terminal
void inventar::buy(queue<double>& item, eItemID itemID)
{
	int amount;
	std::cin >> amount;

	if (!chceckInput()) { return; }

	double d_price;
	std::cin >> d_price;

	if (!chceckInput()) { return; }

	logTransaction(amount, d_price,itemID, eTransaction::buying);

	for (int i = 0; i < amount; ++i)
	{
		item.push_back(d_price);
	}

	std::cout << '\n' << "Nakupena polozka " << itemID << " - " << amount << " ks za cenu ";
	if (d_price - int(d_price) == 0)
	{
		std::cout << int(d_price) << " EUR/ks." << '\n';
		return;
	}
	std::cout << std::fixed << std::setprecision(2) << d_price << " EUR/ks." << '\n';
}

//input from file
void inventar::buy(int amount, queue<double>& item,std::string s_price, eItemID itemID)
{
	double d_price = std::stod(s_price);

	logTransaction(amount, d_price, itemID, eTransaction::buying);

	for (int i = 0; i < amount; ++i)
	{
		//no need for emplace back for primitive types, negliable difference
		item.push_back(d_price);
	}
}

//input from console/terminal
void inventar::sell(queue<double>& item, eItemID itemID)
{
	unsigned int amount;
	std::cin >> amount;

	if (!chceckInput()) { return; }
	int s = item.size();
	if (amount <= item.size())
	{
		sell(amount,item, itemID);
		std::cout << "Predalo sa " << std::fixed << std::setprecision(2) << amount << " ks polozky " << itemID << "." << '\n';
		return;
	}
	std::cout << "Transaction terminated not enough stock" << '\n';
}

//input from file
void inventar::sell(size_t amount, queue<double>& item, eItemID itemID)
{
	Vector<info> unitsPrices;

	if (amount <= item.size())
	{
		for (int i = 0; i < amount; ++i)
		{
			//see if if unique works with emplaceBack ?
			unitsPrices.PushBackUnique(info(item.front()));
			item.pop_front();
		}


		//log.reserve(unitsPrices.size());
		for (unsigned int i = 0; i < unitsPrices.size(); ++i)
		{

			logTransaction(unitsPrices[i].m_units, unitsPrices[i].m_Price, itemID, eTransaction::selling);
		}
		return;
	}
	std::cout << "Transaction terminated not enough stock" << '\n';
}

unsigned int inventar::getDigits(std::string line,unsigned int start)
{
	unsigned int it;
	for (it = start; it < line.length(); ++it)
	{
		if (!isdigit(line[it]))
		{
			break;
		}
	}

	return it - start;
}


void inventar::performTransaction(queue<double>& item,std::string line, eItemID itemID)
{
	if (line[2] != eTransaction::buying && line[2] != eTransaction::selling) { return; }
		
	unsigned int digits = getDigits(line);
	int amount = stoi(line.substr(4,digits));

	if (line[2] == eTransaction::buying)
	{
		buy(amount,item, line.substr(digits+4,line.length()-digits-4),itemID);
		return;
	}
	sell(amount, item,itemID);
}

void inventar::load(std::string fileName)
{
	std::ifstream inv;
	std::string line;
	inv.open(fileName);
	bool b = inv.is_open();

	if (inv.is_open())
	{
		std::regex pattern(R"(\d+\s+\w\s+\d+\s+\d+\.\d+)");

		while (getline(inv, line))
		{
			if (std::regex_match(line, pattern))
			{
				int i = line[0]-48;
				switch (i)
				{
				case eItemID::ItemOne:
					performTransaction(m_item1, line, eItemID::ItemOne);
					break;
				case eItemID::ItemTwo:
					performTransaction(m_item2, line, eItemID::ItemTwo);
					break;
				case eItemID::ItemThree:
					performTransaction(m_item3, line, eItemID::ItemThree);
					break;
				default:
					std::cout << "incorrect item" << '\n';
					break;
				}
			}
			else
			{
				std::cout << "invalidfile format" << '\n';
			}
		}
		
		inv.close();
		std::cout << "Nacitanie inventara z uvedeneho suboru prebehlo v poriadku." << '\n';
		return;
	}
	std::cout << "Chyba pri otvarani suboru." << '\n';
	getInput();
}


void inventar::write(std::string fileName)
{
	std::ofstream inv;
	inv.open(fileName);

	if (inv.is_open())
	{
		for (queue<transactionInfo>::const_Iterator it = m_log.begin(); it != m_log.end(); ++it)
		{
			inv << it->m_ItemID << " " << char(it->m_Transaction) << " " << it->m_Amount << " "
				<< std::fixed << std::setprecision(2) << it->m_Price << '\n';
		}

		inv.close();
		std::cout << '\n' << "Inventar bol zapisany do uvedeneho suboru." << '\n';

		return;
	}
}

void inventar::showStatistics()
{
	if (m_log.empty()) 
	{
		std::cout << '\n' << "Item " << 1 << '\n';
		std::cout << "0 units" << '\n';
		std::cout << "Sumar - " << 0.00 << " EUR" << '\n';
		std::cout << "Item " << 2 << '\n';
		std::cout << "0 units" << '\n';
		std::cout << "Sumar - " << 0.00 << " EUR" << '\n';
		std::cout << "Item " << 3 << '\n';
		std::cout << "0 units" << '\n';
		std::cout << "Sumar - " << 0.00 << " EUR" << '\n';
		return;
	}
	std::cout << '\n' << "Item " << 1 << '\n';

	Vector<transactionInfo> unitsPrices;

	for (queue<transactionInfo>::const_Iterator it = m_log.begin(); it != m_log.end(); ++it)
	{
		if (it->m_Transaction == eTransaction::selling)
		{
			unitsPrices.PushBackUnique(*it);
		}
	}

	double sum[3] = {0,0,0};

	Vector<info> Item_Two;
	Vector<info> Item_Three;

	for (unsigned int i = 0; i < unitsPrices.size(); ++i)
	{
		if (unitsPrices[i].m_ItemID == eItemID::ItemOne)
		{
			//item 1
			std::cout << unitsPrices[i].m_Amount << " units - " << std::fixed << std::setprecision(2) 
				<< unitsPrices[i].m_Price << " EUR" << '\n';

			sum[0] += unitsPrices[i].m_Price * unitsPrices[i].m_Amount;
		}
		else if (unitsPrices[i].m_ItemID == eItemID::ItemTwo)
		{
			Item_Two.EmplaceBack(unitsPrices[i].m_Amount, unitsPrices[i].m_Price);
			sum[1] += unitsPrices[i].m_Price * unitsPrices[i].m_Amount;
		}
		else
		{
			Item_Three.EmplaceBack(unitsPrices[i].m_Amount, unitsPrices[i].m_Price);
			sum[2] += unitsPrices[i].m_Price * unitsPrices[i].m_Amount;
		}
	}
	std::cout << "Sumar - " << sum[0] << " EUR" << '\n';

	//item 2
	std::cout << '\n' << "Item " << 2 << '\n';
	for (unsigned int i = 0; i < Item_Two.size(); ++i)
	{
		std::cout << Item_Two[i].m_units << " units - " << std::fixed << std::setprecision(2)
			<< Item_Two[i].m_Price << " EUR" << '\n';
	}
	std::cout << "Sumar - " << sum[1] << " EUR" << '\n';

	//item 3
	std::cout << '\n' << "Item " << 3 << '\n';
	for (unsigned int i = 0; i < Item_Two.size(); ++i)
	{
		std::cout << Item_Three[i].m_units << " units - " << std::fixed << std::setprecision(2)
			<< Item_Three[i].m_Price << " EUR" << '\n';
	}
	std::cout << "Sumar - " << sum[2] << " EUR" << '\n';
}

inventar::UserCommands inventar::mapCommand(char c)
{
	switch (c)
	{
	case 'i':
		return UserCommands::Inventory;
	case 'k':
		return UserCommands::Buy;
	case 'p':
		return UserCommands::Sell;
	case 'n':
		return UserCommands::Load;
	case 's':
		return UserCommands::Statisticks;
	case 'w':
		return UserCommands::Export;
	case 'e':
		return UserCommands::EXIT;
	default:
		return UserCommands::UNSAPORTED_COMMAND;
	}
}

inventar::eItemID inventar::mapeItemID(int item)
{
	switch (item)
	{
	case 1:
		return eItemID::ItemOne;
	case 2:
		return eItemID::ItemTwo;
	case 3:
		return eItemID::ItemThree;
	default:
		return eItemID::INVALID_ID;
	}
}

inventar::inventar() 
{
	getInput();
}

int main()
{
	inventar inv;

	return 0;
}
