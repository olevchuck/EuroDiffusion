#pragma once
#include "EuroDiffusionManager.h"
#include <iostream>
#include <string>
#include <set>
#include <algorithm>
#include <functional>

#define m_minPos 0
#define m_maxPos 9

void EuroDiffusionManager::ConstructCitiesLinks()
{
	for (const auto cityIterator : m_CitiesHash)
	{
		int x = cityIterator.second->GetPosX();
		int y = cityIterator.second->GetPosY();

		std::vector<City*>* neighbours = new std::vector<City *>();
		if (IsLeftNeighbourPresent(x, y))
			neighbours->push_back(m_CitiesHash[GetHashForCountry(x-1, y)]);
		if (IsRightNeighbourPresent(x, y))
			neighbours->push_back(m_CitiesHash[GetHashForCountry(x + 1, y)]);
		if (IsBottomNeighbourPresent(x, y))
			neighbours->push_back(m_CitiesHash[GetHashForCountry(x, y - 1)]);
		if (IsTopNeighbourPresent(x, y))
			neighbours->push_back(m_CitiesHash[GetHashForCountry(x, y + 1)]);

		m_CitiesLinks[cityIterator.second] = neighbours;

	}
}

bool EuroDiffusionManager::AreCountriesAccessible()
{
	std::vector<int> accessibleCountries;
	std::vector<int> currentCountryNeighbours;

	int currentCountry = m_CountriesHash.begin()->first;

	while (accessibleCountries.size() < m_CountriesHash.size())
	{
		accessibleCountries.push_back(currentCountry);
		FindCountryNeighbours(currentCountry, accessibleCountries, currentCountryNeighbours);
		if (currentCountryNeighbours.size() != 0)
			currentCountry = currentCountryNeighbours[0];
		else
			break;
	}
	return accessibleCountries.size() == m_CountriesHash.size();
}

void EuroDiffusionManager::AddCountry(Country* country)
{
	int hash = m_CountriesHash.size() + 1;
	m_CountriesHash[hash] = country;
	for (int y = country->m_LeftY; y <= country->m_RightY; y++) {
		for (int x = country->m_LeftX; x <= country->m_RightX; x++) {
			City* city = new City(x, y, country);
			m_CitiesHash[GetHashForCountry(city->GetPosX(), city->GetPosY())] = city;
			matrix[x][y] = hash;
		}
	}
}

bool EuroDiffusionManager::AreInputCoordinatesValid()
{
	for (const auto& countryIterator : m_CountriesHash)
	{
		const Country* country = countryIterator.second;
		if (!IsValueBetween(country->m_RightY , m_minPos, m_maxPos) ||
			!IsValueBetween(country->m_RightX, m_minPos, m_maxPos) ||
			!IsValueBetween(country->m_LeftX, m_minPos, m_maxPos) ||
			!IsValueBetween(country->m_LeftY, m_minPos, m_maxPos) ||
			(country->m_LeftX > country->m_RightX) ||
			(country->m_LeftY > country->m_RightY))

			return false;
	}
	return true;
}

void EuroDiffusionManager::ShowResults()
{
	typedef std::function<bool(std::pair<int, Country*>, std::pair<int, Country*>)> Comparator;

	// Defining a lambda function to compare two pairs. It will compare two pairs using second field
	Comparator compFunctor =
		[](std::pair<int, Country*> elem1, std::pair<int, Country*> elem2)
	{
		if(elem1.second->m_FinishDay < elem2.second->m_FinishDay)
			return true;
		if (elem1.second->m_FinishDay == elem2.second->m_FinishDay && elem1.second->m_CountryName < elem2.second->m_CountryName)
			return true;
		return false;

	};

	// Declaring a set that will store the pairs using above comparision logic
	std::set<std::pair<int, Country*>, Comparator> setOfWords(
		m_CountriesHash.begin(), m_CountriesHash.end(), compFunctor);


	for (const auto& countryIt : setOfWords)
	{
		std::cout << countryIt.second->m_CountryName << "   " << countryIt.second->m_FinishDay << "\n";
	}
		
}

void EuroDiffusionManager::PrepareToStart()
{
	ConstructCitiesLinks();
	SetInitialBalances();
}

void EuroDiffusionManager::StartDiffusion()
{
	int day = 0;
	do 
	{
		day++;
		PrepareNewDay();
		ProcessDay();
		FinishDay();
	} while (!AreCitiesFinished(day));
}

void EuroDiffusionManager::FreeMemory()
{
	for (auto& countryIt : m_CountriesHash)
	{
		delete countryIt.second;
	}

	for (auto& cityIt : m_CitiesHash)
	{
		delete cityIt.second;
	}
}

bool EuroDiffusionManager::IsLeftNeighbourPresent(int posX, int posY)
{
	return posX > m_minPos && matrix[posX - 1][posY];
}

bool EuroDiffusionManager::IsRightNeighbourPresent(int posX, int posY)
{
	return posX < m_maxPos && matrix[posX + 1][posY];
}

bool EuroDiffusionManager::IsTopNeighbourPresent(int posX, int posY)
{
	return posY < m_maxPos && matrix[posX][posY + 1];
}

bool EuroDiffusionManager::IsBottomNeighbourPresent(int posX, int posY)
{
	return posY > m_minPos && matrix[posX][posY - 1];
}

void EuroDiffusionManager::FindCountryNeighbours(int countryNumber, std::vector<int>& accessibleCountries, std::vector<int>& currentCountryNeighbours)

{
	Country* currentCountry = m_CountriesHash[countryNumber];
	if (!currentCountry)
		return;
	for (int i = 0; i <= currentCountry->m_RightY - currentCountry->m_LeftY; i++) {
		if (currentCountry->m_LeftX != m_minPos) 
		{
			int country = matrix[currentCountry->m_LeftX - 1][currentCountry->m_LeftY + i];
			AddCountryToTheNeighboursList(country, accessibleCountries, currentCountryNeighbours);

		}
		if (currentCountry->m_RightX != m_maxPos) {
			int country = matrix[currentCountry->m_RightX + 1][i + currentCountry->m_LeftY + i];
			AddCountryToTheNeighboursList(country, accessibleCountries, currentCountryNeighbours);
		}
	}

	for (int i = currentCountry->m_LeftX; i <= currentCountry->m_RightX - currentCountry->m_LeftX; i++) {
		if (currentCountry->m_LeftY != m_minPos) {
			int country = matrix[currentCountry->m_LeftX + i][currentCountry->m_LeftY - 1];
			AddCountryToTheNeighboursList(country, accessibleCountries, currentCountryNeighbours);
		}
		if (currentCountry->m_RightY != m_maxPos) {
			int country = matrix[currentCountry->m_LeftX + i][currentCountry->m_RightY + 1];
			AddCountryToTheNeighboursList(country, accessibleCountries, currentCountryNeighbours);
		}
	}
}

void EuroDiffusionManager::SetInitialBalances()
{
	for (auto& cityIterator : m_CitiesHash)
	{
		cityIterator.second->SetInitialBalances(m_CountriesHash);
	}
}

void EuroDiffusionManager::PrepareNewDay()
{
	for (auto& cityIterator : m_CitiesHash)
	{
		cityIterator.second->RefreshCoinsFromDifferentCountriesToSend();
	}
}

void EuroDiffusionManager::FinishDay()
{
	for (auto& cityIterator : m_CitiesHash)
	{
		cityIterator.second->RefreshCurrentBalances();
		cityIterator.second->ClearIncomingBalances();
	}
}

bool EuroDiffusionManager::AreCitiesFinished(int finishDay)
{
	for (auto& countryIterator: m_CountriesHash) {
		Country* country = countryIterator.second;
		if (!country)
			break;
		if (country->m_FinishDay < 0) {
			country->m_FinishDay = finishDay;
		}
	}
	bool allCitiesDone = true;
	for (auto& cityIterator : m_CitiesHash)
	{
		City* city = cityIterator.second;
		if (!city->IsFinished()) {
			allCitiesDone = false;
			city->GetParentCountry()->m_FinishDay = -1;
		}
	}
	return allCitiesDone;
}

void EuroDiffusionManager::DoTransaction(City* from, City* to, Country* country)
{
	to->AcceptCoins(country, from->WithdrawCoins(country));
}

void EuroDiffusionManager::ProcessDay()
{
	for (auto& cityIt : m_CitiesLinks) 
	{
		City* city = cityIt.first;

		for (City* neighbour : *(m_CitiesLinks[city])) 
		{
			for (auto& countryIt : m_CountriesHash) {
				DoTransaction(city, neighbour, countryIt.second);
			}
		}
	}
}

void EuroDiffusionManager::AddCountryToTheNeighboursList(int country, std::vector<int>& accessibleCountries, std::vector<int>& currentCountryNeighbours)
{
	if (country && !(std::find(accessibleCountries.begin(), accessibleCountries.end(), country) != accessibleCountries.end()) &&
		!(std::find(currentCountryNeighbours.begin(), currentCountryNeighbours.end(), country) != currentCountryNeighbours.end()))
	{
		currentCountryNeighbours.push_back(country);
	}
}

bool EuroDiffusionManager::IsValueBetween(int value, int left, int right)
{
	return value <= right && value >= left;
}

