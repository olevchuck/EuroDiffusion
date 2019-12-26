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
	bool AreCountriesAccessible();
	void AddCountry(Country* country);
	bool AreInputCoordinatesValid();
	void ShowResults();
	void PrepareToStart();
	void StartDiffusion();

private:
	bool IsLeftNeighbourPresent(int posX, int posY);
	bool IsRightNeighbourPresent(int posX, int posY);
	bool IsTopNeighbourPresent(int posX, int posY);
	bool IsBottomNeighbourPresent(int posX, int posY);
	void FindCountryNeighbours(int countryNumber, std::vector<int>& accessibleCountries, std::vector<int>& currentCountryNeighbours);
	void SetInitialBalances();
	void PrepareNewDay();
	void FinishDay();
	bool AreCitiesFinished(int finishDay);
	void DoTransaction(City* from, City* to, Country* country);
	void ProcessDay();
	void AddCountryToTheNeighboursList(int country, std::vector<int>& accessibleCountries, std::vector<int>& currentCountryNeighbours);
	int GetHashForCountry(int x, int y) { return x * 10 + y; }
	bool IsValueBetween(int value, int left, int right);

private:
	std::map<int, Country*> m_CountriesHash;
	std::map<int, City*> m_CitiesHash;
	std::map<City*, std::vector<City*>*> m_CitiesLinks;
	int matrix [10][10];
	const int m_minPos = 0;
	const int m_maxPos = 9;

};