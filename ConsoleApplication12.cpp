// ConsoleApplication12.cpp: определяет точку входа для консольного приложения.
//
#include <fstream>
#include <string>
#include <iostream>
#include "EuroDiffusionManager.h"


int main()
{
	std::ifstream idn_file;
	idn_file.open("test.txt");
	int numberOfCase = 1;
	while (idn_file.good())
	{
		EuroDiffusionManager* manager = new EuroDiffusionManager();
		int count;
		idn_file >> count;
		if (count)
		{
			std::cout << "Case number " << numberOfCase++ << "\n";
			for (int i = 0; i < count; i++)
			{
				std::string name;
				int leftX, leftY, rightX, rightY;
				idn_file >> name;
				idn_file >> leftX;
				idn_file >> leftY;
				idn_file >> rightX;
				idn_file >> rightY;

				Country* country = new Country(name, leftX, leftY, rightX, rightY);
				manager->AddCountry(country);

			}
			if (manager->AreInputCoordinatesValid())
			{
				if (manager->AreCountriesAccessible())
				{
					manager->PrepareToStart();
					manager->StartDiffusion();
					manager->ShowResults();
				}
				else
				{
					printf("Some countries are isolated\n");
				}
			}
			else
			{
				printf("wrong input\n");
			}
			std::cout << "\n";
		}
		manager->FreeMemory();
	}


	int a;
	std::cin >> a;

    return 0;
}

