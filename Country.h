#pragma once
#include <vector>

class Country
{
public:
	Country(std::string name, int leftX, int leftY, int rightX, int rightY) :
		m_CountryName(name), m_LeftX(leftX), m_LeftY(leftY), m_RightX(rightX), m_RightY(rightY) {}

	int GetFinishDay(){ return m_FinishDay; }
	void SetFinishDay(int finishDay) { m_FinishDay = finishDay; }
	int GetLeftX() { return m_LeftX; }
	int GetLeftY() { return m_LeftY; }
	int GetRightX() { return m_RightX; }
	int GetRightY() { return m_RightY; }

	void SetRightX(int x) { m_RightX = x; }
	void SetRightY(int y) { m_RightY = y; }
	void SetLeftX(int x) { m_LeftX = x; }
	void SetLeftY(int y) { m_LeftY = y; }
	std::string GetName() { return m_CountryName; }
private:
	int m_FinishDay = 0;
	int m_LeftX;
	int m_LeftY;
	int m_RightX;
	int m_RightY;
	std::string m_CountryName;
};