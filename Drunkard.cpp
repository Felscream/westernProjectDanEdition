#include "Drunkard.h"
#include <iostream>

bool Drunkard::HandleMessage(const Telegram& msg)
{
	return m_pStateMachine->HandleMessage(msg);
}


void Drunkard::Update()
{
	SetTextColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);

	//m_iThirst += 1;
	//cout << "Updating \n";

	m_pStateMachine->Update();
}

bool Drunkard::isDrunk()const {

	if (m_iDrunkness >= DrunknessLevel)
		return true;
	return false;
}

bool Drunkard::isKO()const {
	if (m_iKO == KOThreshold)
		return true;
	return false;
}

bool Drunkard::isSleeping() {
	if (m_iKO >= 5)
		return false;
	return true;
}

/*void Drunkard::AddToGoldCarried(const int val)
{
	m_iGoldCarried += val;

	if (m_iGoldCarried < 0) m_iGoldCarried = 0;
}

void Drunkard::AddToWealth(const int val)
{
	m_iMoneyInBank += val;

	if (m_iMoneyInBank < 0) m_iMoneyInBank = 0;
}

bool Drunkard::Thirsty()const
{
	if (m_iThirst >= ThirstLevel) { return true; }

	return false;
}

bool Drunkard::Fatigued()const
{
	if (m_iFatigue > TirednessThreshold)
	{
		return true;
	}

	return false;
}*/
