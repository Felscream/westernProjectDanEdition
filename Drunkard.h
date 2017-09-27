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
	//if the drunkness of Dan exceeds his limit then enter in drunk state and tell jokes
	int                   m_iDrunkness;

	//Bool used to know if Bob is in the saloon or not. Is set when receive a telegram from Bob when he enter or exit the saloon
	bool                  bobInSaloon;

	//List of jokes that Dan say when he is in drunk state
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

		//create Dan
		m_pStateMachine->SetCurrentState(Creation::Instance());

		m_pStateMachine->SetGlobalState(DrunkardGlobalState::Instance());

		/* NOTE, A GLOBAL STATE HAS NOT BEEN IMPLEMENTED FOR THE MINER */
	}

	~Drunkard() { delete m_pStateMachine; }

	//this must be implemented
	void Update();

	//so must this
	virtual bool  HandleMessage(const Telegram& msg);


	StateMachine<Drunkard>* GetFSM()const { return m_pStateMachine; };



	//-------------------------------------------------------------accessors

	bool          isKO()const;
	int			  getKO() { return m_iKO; }

	bool          isDrunk()const;
	//return a joke
	char*		  getJoke(int i) { return jokes[i]; }
	//increase the drunkness by 1
	void          DrinkAWhiskey() { m_iDrunkness += 1; }
	//when KO, sleep increase the hp if not over the limit and decrease the drunkness if > 0
	void		  Sleep() { 
		this->recoverKO();
		if (m_iDrunkness > 0) {
			m_iDrunkness -= 1;
		}
	}

	bool		  isSleeping();
	int			  getDrunkness() { return m_iDrunkness; }
	//indicate if bob is actually in the saloon
	bool		  bobIsInTheSaloon() { return bobInSaloon; }
	//when receiving a telegram from bob that indicate if he's coming in the saloon or exiting change the bool to the opposte
	void		  setBobInSaloon() { bobInSaloon = !bobInSaloon; }
};



#endif
