#include "MinersWife.h"

bool MinersWife::HandleMessage(const Telegram& msg)
{
  return m_pStateMachine->HandleMessage(msg);
}


void MinersWife::Update(int loop)
{
	for (int i = 0; i < loop; i++) {
		m_pStateMachine->Update();
	}

}