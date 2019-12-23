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
	for (int y = country->GetLeftY(); y <= country->GetRightY(); y++) {
		for (int x = country->GetLeftX(); x <= country->GetRightX(); x++) {
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
		if ((countryIterator.second->GetRightY() < m_minPos || countryIterator.second->GetRightY() > m_maxPos) ||
			(countryIterator.second->GetRightX() < m_minPos || countryIterator.second->GetRightX() > m_maxPos) ||
			(countryIterator.second->GetLeftY() < m_minPos || countryIterator.second->GetLeftY() > m_maxPos) ||
			(countryIterator.second->GetLeftX() < m_minPos || countryIterator.second->GetLeftX() > m_maxPos) ||
			countryIterator.second->GetLeftX() > countryIterator.second->GetRightX() || 
			countryIterator.second->GetLeftY() > countryIterator.second->GetRightY())

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
		if(elem1.second->GetFinishDay() < elem2.second->GetFinishDay())
			return true;
		if (elem1.second->GetFinishDay() == elem2.second->GetFinishDay() && elem1.second->GetName() < elem2.second->GetName())
			return true;
		return false;

	};

	// Declaring a set that will store the pairs using above comparision logic
	std::set<std::pair<int, Country*>, Comparator> setOfWords(
		m_CountriesHash.begin(), m_CountriesHash.end(), compFunctor);


	for (const auto& countryIt : setOfWords)
	{
		std::cout << countryIt.second->GetName() << "   " << countryIt.second->GetFinishDay() << "\n";
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
	for (int i = 0; i <= currentCountry->GetRightY() - currentCountry->GetLeftY(); i++) {
		if (currentCountry->GetLeftX() != m_minPos) 
		{
			int country = matrix[currentCountry->GetLeftX() - 1][currentCountry->GetLeftY() + i];
			if (country && !(std::find(accessibleCountries.begin(), accessibleCountries.end(), country) != accessibleCountries.end()) &&
				!(std::find(currentCountryNeighbours.begin(), currentCountryNeighbours.end(), country) != currentCountryNeighbours.end()))
			{
				currentCountryNeighbours.push_back(country);
			}
		}
		if (currentCountry->GetRightX() != m_maxPos) {
			int country = matrix[currentCountry->GetRightX() + 1][i + currentCountry->GetLeftY() + i];
			if (country && !(std::find(accessibleCountries.begin(), accessibleCountries.end(), country) != accessibleCountries.end()) &&
				!(std::find(currentCountryNeighbours.begin(), currentCountryNeighbours.end(), country) != currentCountryNeighbours.end()))
			{
				currentCountryNeighbours.push_back(country);
			}
		}
	}

	for (int i = currentCountry->GetLeftX(); i <= currentCountry->GetRightX() - currentCountry->GetLeftX(); i++) {
		if (currentCountry->GetLeftY() != m_minPos) {
			int country = matrix[currentCountry->GetLeftX() + i][currentCountry->GetLeftY() - 1];
			if (country && !(std::find(accessibleCountries.begin(), accessibleCountries.end(), country) != accessibleCountries.end()) &&
				!(std::find(currentCountryNeighbours.begin(), currentCountryNeighbours.end(), country) != currentCountryNeighbours.end()))
			{
				currentCountryNeighbours.push_back(country);
			}
		}
		if (currentCountry->GetRightY() != m_maxPos) {
			int country = matrix[currentCountry->GetLeftX() + i][currentCountry->GetRightY() + 1];
			if (country && !(std::find(accessibleCountries.begin(), accessibleCountries.end(), country) != accessibleCountries.end()) &&
				!(std::find(currentCountryNeighbours.begin(), currentCountryNeighbours.end(), country) != currentCountryNeighbours.end()))
			{
				currentCountryNeighbours.push_back(country);
			}
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
		if (countryIterator.second->GetFinishDay() < 0) {
			countryIterator.second->SetFinishDay(finishDay);
		}
	}
	bool allCitiesDone = true;
	for (auto& cityIterator : m_CitiesHash)
	{
		City* city = cityIterator.second;
		if (!city->IsFinished()) {
			allCitiesDone = false;
			city->GetParentCountry()->SetFinishDay(-1);
		}
	}
	return allCitiesDone;
}

void EuroDiffusionManager::DoTransaction(City* from, City* to, Country* country)
{
	to->AcceptCoins(country, from->WithdrawalCoins(country));
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

