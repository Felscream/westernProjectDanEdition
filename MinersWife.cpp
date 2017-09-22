#include "MinersWife.h"

bool MinersWife::HandleMessage(const Telegram& msg)
{
  return m_pStateMachine->HandleMessage(msg);
}


void MinersWife::Update()
{
	for (int i = 0; i < 5; i++) {
		SetTextColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		m_pStateMachine->Update();
	}
}