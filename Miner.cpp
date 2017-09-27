#include "Miner.h"

bool Miner::HandleMessage(const Telegram& msg)
{
  return m_pStateMachine->HandleMessage(msg);
}

//The update function of the miner : increment the thirst of Bob and update his status
void Miner::Update()
{
	for (int i = 0; i < 10; i++) {
		m_iThirst += 1;
		m_pStateMachine->Update();
	}

}


//Add a golden nugget to Bob and if by error the gold carried is < to 0, set it to 0
void Miner::AddToGoldCarried(const int val)
{
  m_iGoldCarried += val;

  if (m_iGoldCarried < 0) m_iGoldCarried = 0;
}

//add the gold carried by Bob to the bank and if by error his savings are < 0, set it to 0
void Miner::AddToWealth(const int val)
{
  m_iMoneyInBank += static_cast<float>(val);

  if (m_iMoneyInBank < 0) m_iMoneyInBank = 0;
}

//look if the miner is thirsty enough to go to the saloon
bool Miner::Thirsty()const
{
  if (m_iThirst >= ThirstLevel){return true;}

  return false;
}

//look if the tiredness of Bob exceeds his threshold 
bool Miner::Fatigued()const
{
  if (m_iFatigue > TirednessThreshold)
  {
    return true;
  }

  return false;
}

//look when in the saloon if Bob had drank enough to return to the mine
bool Miner::Hydrated()const {
	if (m_iThirst > 0) {
		return false;
	}
	return true;
}
