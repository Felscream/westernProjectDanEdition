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
#include "misc/Utils.h"


//above this value Dan is drunk
const int DrunknessLevel = 5;



class Drunkard : public BaseGameEntity
{
private:

	//an instance of the state machine class
	StateMachine<Drunkard>*  m_pStateMachine;

	location_type   m_Location;
	//the higher the value, the thirstier the miner
	int                   m_iDrunkness;

	//the higher the value, the more tired the miner
	//int                   m_iKO;

	bool                  bobInSaloon;

	char*					jokes[5];

public:

	Drunkard(int id) :m_Location(saloon),
		m_iDrunkness(0),
		bobInSaloon(false),
		BaseGameEntity(id)

	{
		MaxHP = 5;
		m_iKO = MaxHP;

		jokes[0] = "I'm not an alcoholic. Alcoholics need a drink, but I already have one.";
		jokes[1] = "Alcohol is my worst enemy, but the bible says to love your enemy";
		jokes[2] = "I'm not alcoholic, I only drink twice a year. When it's my birthday, and when it's not my birthday.";
		jokes[3] = "My doctor told me to watch my drinking, so now I drink in front of a mirror.";
		jokes[4] = "A man's got to believe in something. I believe I'll have another drink.";

		//set up state machine
		m_pStateMachine = new StateMachine<Drunkard>(this);

		m_pStateMachine->SetCurrentState(QuenchThirstDan::Instance());

		m_pStateMachine->SetGlobalState(DrunkardGlobalState::Instance());

		/* NOTE, A GLOBAL STATE HAS NOT BEEN IMPLEMENTED FOR THE MINER */
	}

	~Drunkard() { delete m_pStateMachine; }

	//this must be implemented
	void Update(int loop);

	//so must this
	virtual bool  HandleMessage(const Telegram& msg);


	StateMachine<Drunkard>* GetFSM()const { return m_pStateMachine; };



	//-------------------------------------------------------------accessors
	//location_type Location()const { return m_Location; }

	bool          isKO()const;
	//void          DecreaseKO() { m_iKO -= 1; if (m_iKO < 0) { m_iKO = 0; } }
	int			  getKO() { return m_iKO; }

	bool          isDrunk()const;
	char*		  getJoke(int i) { return jokes[i]; }
	void          DrinkAWhiskey() { m_iDrunkness += 1; }
	void		  Sleep() { 
		this->recoverKO();
		m_iDrunkness -= 1; }

	bool		  isSleeping();
	int			  getDrunkness() { return m_iDrunkness; }
	bool		  bobIsInTheSaloon() { return bobInSaloon; }
	void		  setBobInSaloon() { bobInSaloon = !bobInSaloon; }
	bool		  getBobInSaloon() { return bobInSaloon; }
};



#endif
