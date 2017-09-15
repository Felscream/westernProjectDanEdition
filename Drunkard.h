#ifndef DRUNKARD_H
#define DRUNKARD_H
//------------------------------------------------------------------------
//
//  Name:   Miner.h
//
//  Desc:   A class defining a goldminer.
//
//  Author: Mat Buckland 2002 (fup@ai-junkie.com)
//
//------------------------------------------------------------------------
#include <string>
#include <cassert>
#include <iostream>

#include "BaseGameEntity.h"
#include "Locations.h"
#include "misc/ConsoleUtils.h"
#include "DrunkardOwnedStates.h"
#include "fsm/StateMachine.h"


//above this value Dan is drunk
const int DrunknessLevel = 5;
//above this value Dan passes out
const int KOThreshold = 0;



class Drunkard : public BaseGameEntity
{
private:

	//an instance of the state machine class
	StateMachine<Drunkard>*  m_pStateMachine;

	//the higher the value, the thirstier the miner
	int                   m_iDrunkness;

	//the higher the value, the more tired the miner
	int                   m_iKO;

public:

	Drunkard(int id) :m_iDrunkness(0),
		m_iKO(5),
		BaseGameEntity(id)

	{
		//set up state machine
		m_pStateMachine = new StateMachine<Drunkard>(this);

		m_pStateMachine->SetCurrentState(Creation::Instance());

		/* NOTE, A GLOBAL STATE HAS NOT BEEN IMPLEMENTED FOR THE MINER */
	}

	~Drunkard() { delete m_pStateMachine; }

	//this must be implemented
	void Update();

	//so must this
	virtual bool  HandleMessage(const Telegram& msg);


	StateMachine<Drunkard>* GetFSM()const { return m_pStateMachine; };



	//-------------------------------------------------------------accessors
	//location_type Location()const { return m_Location; }

	bool          isKO()const;
	void          DecreaseKO() { m_iKO -= 1; }
	int			  getKO() { return m_iKO; }

	bool          isDrunk()const;
	void          DrinkAWhiskey() { m_iDrunkness += 1; }
	void		  Sleeping() { m_iKO += 1; m_iDrunkness -= 1; }

	bool		  isSleeping();
	int			  getDrunkness() { return m_iDrunkness; }
};



#endif
