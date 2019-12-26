#pragma once
#include "EuroDiffusionManager.h"
#include <iostream>
#include <string>
#include <set>
#include <algorithm>
#include <functional>

void EuroDiffusionManager::ConstructCitiesLinks()
{
	for (const auto cityIterator : m_CitiesHash)
	{
		int x = cityIterator.second->GetPosX();
		int y = cityIterator.second->GetPosY();

		std::vector<City*>* neighbours = new std::vector<City *>();
		if (CheckLeftNeighbour(x, y))
			neighbours->push_back(m_CitiesHash[(x - 1) * 10 + y]);
		if (CheckRightNeighbour(x, y))
			neighbours->push_back(m_CitiesHash[(x + 1) * 10 + y]);
		if (CheckBottomNeighbour(x, y))
			neighbours->push_back(m_CitiesHash[x * 10 + y - 1]);
		if (CheckTopNeighbour(x, y))
			neighbours->push_back(m_CitiesHash[x * 10 + y + 1]);

		m_CitiesLinks[cityIterator.second] = neighbours;

	}
}

bool EuroDiffusionManager::CheckCountriesAccessibility()
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
			m_CitiesHash[city->GetPosX() * 10 + city->GetPosY()] = city;
			matrix[x][y] = hash;
		}
	}
}

bool EuroDiffusionManager::ValidateInputCoordinates()
{
	for (const auto& countryIterator : m_CountriesHash)
	{
		const Country* country = countryIterator.second;
		if ((country->m_RightY < m_minPos || country->m_RightY > m_maxPos) ||
			(country->m_RightX < m_minPos || country->m_RightX > m_maxPos) ||
			(country->m_LeftY < m_minPos || country->m_LeftY > m_maxPos) ||
			(country->m_LeftX < m_minPos || country->m_LeftX > m_maxPos) ||
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
	} while (!CheckCitiesFinish(day));
}

bool EuroDiffusionManager::CheckLeftNeighbour(int posX, int posY)
{
	return posX > m_minPos && matrix[posX - 1][posY];
}

bool EuroDiffusionManager::CheckRightNeighbour(int posX, int posY)
{
	return posX < m_maxPos && matrix[posX + 1][posY];
}

bool EuroDiffusionManager::CheckTopNeighbour(int posX, int posY)
{
	return posY < m_maxPos && matrix[posX][posY + 1];
}

bool EuroDiffusionManager::CheckBottomNeighbour(int posX, int posY)
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
		cityIterator.second->SetCoinsToPay();
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

bool EuroDiffusionManager::CheckCitiesFinish(int finishDay)
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

