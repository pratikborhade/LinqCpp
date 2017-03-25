// Linq-cpp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "IEnumerable.h"
#include <vector>
#include <iostream>
int main()
{
	std::vector<int> v{1,2, 3, 5, 6,7, 8, 10, 20};
	auto ienum = LINQ(v);

	struct tempStruct
	{
		int a;
		int b;
	};

	tempStruct temp;
	temp.a = 20;
	temp.b = 30;
	std::vector<tempStruct> v2;
	v2.push_back(temp);

	auto test2 = LINQ(v2).Select( &tempStruct::a);

	auto test = ienum.Where([](const int &a)->bool {return a > 2; }).Where([](const int &a)->bool { return a < 10; }).Select<int>([](const int &a) ->int {return a*a; });

	for (auto i = test; i != test.end(); ++i)
	{
		std::cout << *i << "\n";
	}
	std::cout << "Special Select\n\n";
	for (auto i = test2; i != test2.end(); ++i)
		std::cout << *i << "\n";

    return 0;
}

