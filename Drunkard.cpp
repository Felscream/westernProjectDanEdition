#include "Drunkard.h"
#include <iostream>



bool Drunkard::HandleMessage(const Telegram& msg)
{
	return m_pStateMachine->HandleMessage(msg);
}


void Drunkard::Update()
{
	for (int i = 0; i < 5; i++) {
		SetTextColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);
		m_pStateMachine->Update();
	}
	
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

