#pragma once
#include "City.h"
#include <vector>
#include <map>

class EuroDiffusionManager
{
public:
	EuroDiffusionManager() {};

public:
	void ConstructCitiesLinks();
	bool CheckCountriesAccessibility();
	void AddCountry(Country* country);
	bool ValidateInputCoordinates();
	void ShowResults();
	void PrepareToStart();
	void StartDiffusion();

private:
	bool CheckLeftNeighbour(int posX, int posY);
	bool CheckRightNeighbour(int posX, int posY);
	bool CheckTopNeighbour(int posX, int posY);
	bool CheckBottomNeighbour(int posX, int posY);
	void FindCountryNeighbours(int countryNumber, std::vector<int>& accessibleCountries, std::vector<int>& currentCountryNeighbours);
	void SetInitialBalances();
	void PrepareNewDay();
	void FinishDay();
	bool CheckCitiesFinish(int finishDay);
	void DoTransaction(City* from, City* to, Country* country);
	void ProcessDay();

private:
	std::map<int, Country*> m_CountriesHash;
	std::map<int, City*> m_CitiesHash;
	std::map<City*, std::vector<City*>*> m_CitiesLinks;
	int matrix [10][10];
	const int m_minPos = 0;
	const int m_maxPos = 9;

};