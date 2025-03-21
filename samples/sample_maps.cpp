#include <iostream>
#include "maps.h"

using namespace maps;

int main()
{
	OrderedTable<int> M;
	std::cout << M.insert(5, 4);
	std::cout << M.insert(1, 4);
	std::cout << M.insert(5, 4);
	std::cout << M.insert(3, 4);
	std::cout << M.insert(3, 5);
	std::cout << M.insert(2, 4);
	std::cout << M.erase(8);
	std::cout << M.erase(5);
	std::cout << M[0].second;

}