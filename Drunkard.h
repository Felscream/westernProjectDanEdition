#ifndef DRUNKARD_H
#define DRUNKARD_H
//------------------------------------------------------------------------
//
//  Name:   Drunkard.h
//
//  Desc:   A class defining a drunk man.
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
	//Alcohol level of Dan
	int                   m_iDrunkness;

	//Dan knows if bob is in the saloon or not
	bool                  bobInSaloon;

	//Dan can tell jokes
	char*					jokes[5];

public:

	Drunkard(int id) :m_Location(saloon),
		//Level of drunkness start at 0
		m_iDrunkness(0),

		//At the begining bob is not in the saloon
		bobInSaloon(false),
		BaseGameEntity(id)

	{
		MaxHP = 5;

		//m_iKO represents Dan's HP, at 0 he will go to sleep
		m_iKO = MaxHP; //Dan start with 5Hp

		jokes[0] = ": I'm not an alcoholic. Alcoholics need a drink, but I already have one.";
		jokes[1] = ": Alcohol is my worst enemy, but the bible says to love your enemy";
		jokes[2] = ": I'm not alcoholic, I only drink twice a year. When it's my birthday, and when it's not my birthday.";
		jokes[3] = ": My doctor told me to watch my drinking, so now I drink in front of a mirror.";
		jokes[4] = ": A man's got to believe in something. I believe I'll have another drink.";

		//set up state machine
		m_pStateMachine = new StateMachine<Drunkard>(this);

		m_pStateMachine->SetCurrentState(QuenchThirstDan::Instance());

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

	//isKO permits to know if Dan is KO (0 HP)
	bool          isKO()const; 

	int			  getKO() { return m_iKO; }

	bool          isDrunk()const; 

	char*		  getJoke(int i) { return jokes[i]; }

	//When Dan drinks, he increases his level of drunkness
	void          DrinkAWhiskey() { m_iDrunkness += 1; }

	//When Dan sleeps, he win 1 HP per round and decreases his level of drunkness
	void		  Sleep() { 
		this->recoverKO(); //define in BasGameEntity.h
		m_iDrunkness -= 1; 
	}

	//To know if Dan is sleeping
	bool		  isSleeping();

	int			  getDrunkness() { return m_iDrunkness; }

	//Change th value of the boolean when Bob leaves or enters in the saloon
	void		  setBobInSaloon() { bobInSaloon = !bobInSaloon; }

	bool		  getBobInSaloon() { return bobInSaloon; }
};



#endif
