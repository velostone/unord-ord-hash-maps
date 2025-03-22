#include <iostream>
#include "maps.h"

using namespace maps;

int main()
{
	HashTable<int> M;
	std::cout << M.insert(13, 4) << std::endl;
	std::cout << M.insert(20, 5) << std::endl;
	std::cout << M.erase(20) << std::endl;
	std::cout << M.erase(13) << std::endl;
	
}