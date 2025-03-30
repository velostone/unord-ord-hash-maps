#include <iostream>
#include <string>
#include "maps.h"

using namespace maps;

void print_menu(const std::vector<std::string> menu)
{
	for (size_t i = 0; i < menu.size(); ++i)
		std::cout << menu[i] << std::endl;
	std::cout << "> ";
}

const std::vector<std::string> general_menu{ "0. Exit", "1. Create unordered map", "2. Create ordered map", "3. Create hash map" };
const std::vector<std::string> map_menu{ "0. Exit", "1. Insert elem", "2. Erase elem", "3. Find elem", "4. Print map", "5. Back" };
UnorderedTable<int> Unord;
OrderedTable<int> Ord;
HashTable<int> Hash;

int main()
{
	size_t k = 0;
	do
	{
		system("cls");
		print_menu(general_menu);
		std::cin >> k;
		switch (k)
		{
		case 0: break;
		case 1:
		{
			do
			{
				system("cls");
				print_menu(map_menu);
				std::cin >> k;
				switch (k)
				{
				case 0: break;
				case 1:
				{
					std::cout << "Print pair key & value: " << std::endl << "> ";
					size_t key;
					int value;
					std::cin >> key >> value;
					if (Unord.insert(key, value)) std::cout << "Done!" << std::endl;
					else std::cout << "WARNING! This key already exists!" << std::endl;
					system("pause");
					break;
				}
				case 2:
				{
					std::cout << "Print key: " << std::endl << "> ";
					size_t key;
					std::cin >> key;
					if (Unord.erase(key)) std::cout << "Done!" << std::endl;
					else std::cout << "WARNING! This key was not found!" << std::endl;
					system("pause");
					break;
				}
				case 3:
				{
					std::cout << "Print key: ";
					size_t key;
					std::cin >> key;
					if (Unord.find(key).has_value()) std::cout << Unord.find(key).value().first << " " << Unord.find(key).value().second << std::endl;
					else std::cout << "WARNING! This key was not found!" << std::endl;
					system("pause");
					break;
				}
				case 4:
				{
					std::cout << Unord;
					system("pause");
					break;
				}
				case 5: break;
				}
			} while (k != 0 && k != 5);
			break;
		}
		case 2:
		{
			do
			{
				system("cls");
				print_menu(map_menu);
				std::cin >> k;
				switch (k)
				{
				case 0: break;
				case 1:
				{
					std::cout << "Print pair key & value: " << std::endl << "> ";
					size_t key;
					int value;
					std::cin >> key >> value;
					if (Ord.insert(key, value)) std::cout << "Done!" << std::endl;
					else std::cout << "WARNING! This key already exists!" << std::endl;
					system("pause");
					break;
				}
				case 2:
				{
					std::cout << "Print key: " << std::endl << "> ";
					size_t key;
					std::cin >> key;
					if (Ord.erase(key)) std::cout << "Done!" << std::endl;
					else std::cout << "WARNING! This key was not found!" << std::endl;
					system("pause");
					break;
				}
				case 3:
				{
					std::cout << "Print key: ";
					size_t key;
					std::cin >> key;
					if (Ord.find(key).has_value()) std::cout << Ord.find(key).value().first << " " << Ord.find(key).value().second << std::endl;
					else std::cout << "WARNING! This key was not found!" << std::endl;
					system("pause");
					break;
				}
				case 4:
				{
					std::cout << Ord;
					system("pause");
					break;
				}
				case 5: break;
				}
			} while (k != 0 && k != 5);
			break;
		}
		case 3:
		{
			do
			{
				system("cls");
				print_menu(map_menu);
				std::cin >> k;
				switch (k)
				{
				case 0: break;
				case 1:
				{
					std::cout << "Print pair key & value: " << std::endl << "> ";
					size_t key;
					int value;
					std::cin >> key >> value;
					if (Hash.insert(key, value)) std::cout << "Done!" << std::endl;
					else std::cout << "WARNING! This key already exists!" << std::endl;
					system("pause");
					break;
				}
				case 2:
				{
					std::cout << "Print key: " << std::endl << "> ";
					size_t key;
					std::cin >> key;
					if (Hash.erase(key)) std::cout << "Done!" << std::endl;
					else std::cout << "WARNING! This key was not found!" << std::endl;
					system("pause");
					break;
				}
				case 3:
				{
					std::cout << "Print key: ";
					size_t key;
					std::cin >> key;
					if (Hash.find(key).has_value()) std::cout << Hash.find(key).value().first << " " << Hash.find(key).value().second << std::endl;
					else std::cout << "WARNING! This key was not found!" << std::endl;
					system("pause");
					break;
				}
				case 4:
				{
					std::cout << Hash;
					system("pause");
					break;
				}
				case 5: break;
				}
			} while (k != 0 && k != 5);
			break;
		}
		}
	} while (k != 0);
}