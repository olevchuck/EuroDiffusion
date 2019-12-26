#pragma once
#include "Country.h"
#include <map>

class City
{
public:
	City(int posX, int posY, Country* parentCountry) :
		m_posX(posX), m_posY(posY), m_ParentCountry(parentCountry) {}

	int GetPosX() { return m_posX; }
	int GetPosY() { return m_posY; }
	Country* GetParentCountry() { return m_ParentCountry; }

public:
	void AcceptCoins(Country* fromWho, int amount);
	int WithdrawCoins(Country* fromWho);
	void RefreshCoinsFromDifferentCountriesToSend();
	void RefreshCurrentBalances();
	void ClearIncomingBalances();
	bool IsFinished();
	void SetInitialBalances(std::map<int, Country*>& countryHash);

private:
	int m_posX;
	int m_posY;
	int m_CoinsAmount = 1000000;
	const int m_NeededCoinsAmountToSend = 1000;
	Country* m_ParentCountry;
	std::map<Country*, int> m_CurrentCoinsAmountFromDifferentCountries;
	std::map<Country*, int> m_IncomingCoinsAmountFromDifferentCountries;
	std::map<Country*, int> m_CoinsFromDifferentCountriesToSend;

};