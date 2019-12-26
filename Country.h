#pragma once
#include <vector>

struct Country
{
	Country(const std::string& name, int leftX, int leftY, int rightX, int rightY) :
		m_CountryName(name), m_LeftX(leftX), m_LeftY(leftY), m_RightX(rightX), m_RightY(rightY) {}

	int m_FinishDay = 0;
	int m_LeftX;
	int m_LeftY;
	int m_RightX;
	int m_RightY;
	const std::string& m_CountryName;
};