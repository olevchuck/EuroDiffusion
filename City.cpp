#pragma once
#include "City.h"

void City::AcceptCoins(Country* fromWho, int amount)
{
	m_IncomingCoinsAmountFromDifferentCountries[fromWho] += amount;
}

int City::WithdrawalCoins(Country* fromWho)
{
	m_CurrentCoinsAmountFromDifferentCountries[fromWho] -= m_CoinsFromDifferentCountriesToSend[fromWho];

	return m_CoinsFromDifferentCountriesToSend[fromWho];
}

void City::SetCoinsToPay()
{
	for (const auto& country : m_CurrentCoinsAmountFromDifferentCountries)
	{
		m_CoinsFromDifferentCountriesToSend[country.first] = country.second / 1000;
	}
}

void City::RefreshCurrentBalances()
{
	for (auto& country : m_CurrentCoinsAmountFromDifferentCountries)
	{
		country.second += m_IncomingCoinsAmountFromDifferentCountries[country.first];
	}
}

void City::ClearIncomingBalances()
{
	for (auto& country : m_IncomingCoinsAmountFromDifferentCountries)
	{
		country.second = 0;
	}
}

bool City::IsFinished()
{
	for (auto& country : m_CurrentCoinsAmountFromDifferentCountries)
	{
		if (country.second == 0)
		{
			return false;
		}
	}

	return true;
}

void City::SetInitialBalances(std::map<int, Country*>& countryHash)
{
	for (const auto& countryIterator : countryHash) {
		Country* country = countryIterator.second;
		if (m_ParentCountry == country) 
		{
			m_CurrentCoinsAmountFromDifferentCountries[country] = m_CoinsAmount;
		}
		else {
			m_CurrentCoinsAmountFromDifferentCountries[country] = 0;
		}

		m_CoinsFromDifferentCountriesToSend[country] = 0;
		m_IncomingCoinsAmountFromDifferentCountries[country] = 0;
	}
}

