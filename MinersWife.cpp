#include "MinersWife.h"

bool MinersWife::HandleMessage(const Telegram& msg)
{
  return m_pStateMachine->HandleMessage(msg);
}


void MinersWife::Update()
{
	for (int i = 0; i < 10; i++) {
		m_pStateMachine->Update();
	}

}