#include "Drunkard.h"
#include <iostream>



bool Drunkard::HandleMessage(const Telegram& msg)
{
	return m_pStateMachine->HandleMessage(msg);
}

//update the turn of Dan to do the next action
void Drunkard::Update()
{
	for (int i = 0; i < 10; i++) {
		m_pStateMachine->Update();
	}
	
}

//check if Dan is drunk
bool Drunkard::isDrunk()const {

	if (m_iDrunkness >= DrunknessLevel)
		return true;
	return false;
}

//check if Dan is KO 
bool Drunkard::isKO()const {
	if (m_iKO == KOThreshold)
		return true;
	return false;
}
//Check if Dan has rested enough to continue drinking
bool Drunkard::isSleeping() {
	if (m_iKO >= 5)
		return false;
	return true;
}

