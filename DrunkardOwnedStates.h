#ifndef DRUNKARD_OWNED_STATES_H
#define DRUNKARD_OWNED_STATES_H
//------------------------------------------------------------------------
//
//  Name:   DrunkardOwnedStates.h
//
//  Desc:   All the states that can be assigned to the Drunkard class.
//          Note that a global state has not been implemented.
//
//  Author: Mat Buckland 2002 (fup@ai-junkie.com)
//
//------------------------------------------------------------------------
#include "fsm/State.h"


class Drunkard;
struct Telegram;




//------------------------------------------------------------------------
//
//  In this state the Drunkard will walk to a goldmine and pick up a nugget
//  of gold. If the Drunkard already has a nugget of gold he'll change state
//  to VisitBankAndDepositGold. If he gets thirsty he'll change state
//  to QuenchThirst
//------------------------------------------------------------------------
class DrunkardGlobalState : public State<Drunkard> {
private:

	DrunkardGlobalState() {}

	//copy ctor and assignment should be private
	DrunkardGlobalState(const DrunkardGlobalState&);
	DrunkardGlobalState& operator=(const DrunkardGlobalState&);

public:

	//this is a singleton
	static DrunkardGlobalState* Instance();

	virtual void Enter(Drunkard* Drunkard);

	virtual void Execute(Drunkard* Drunkard);

	virtual void Exit(Drunkard* Drunkard);

	virtual bool OnMessage(Drunkard* agent, const Telegram& msg);
};

class QuenchThirstDan : public State<Drunkard>{
private:

	QuenchThirstDan(){}
	QuenchThirstDan(const QuenchThirstDan&);
	QuenchThirstDan& operator=(const QuenchThirstDan&);

public:

	static QuenchThirstDan* Instance();

	virtual void Enter(Drunkard* Drunkard);

	virtual void Execute(Drunkard* Drunkard);

	virtual void Exit(Drunkard* Drunkard);

	virtual bool OnMessage(Drunkard* agent, const Telegram& msg);

};




class FightWithBob : public State<Drunkard> {
private:

	FightWithBob() {}
	FightWithBob(const FightWithBob&);
	FightWithBob& operator=(const FightWithBob&);

public:

	static FightWithBob* Instance();

	virtual void Enter(Drunkard* Drunkard);

	virtual void Execute(Drunkard* Drunkard);

	virtual void Exit(Drunkard* Drunkard);

	virtual bool OnMessage(Drunkard* agent, const Telegram& msg);

};

class SleepAndSoberUpDan : public State<Drunkard> {
private:

	SleepAndSoberUpDan() {}
	SleepAndSoberUpDan(const SleepAndSoberUpDan&);
	SleepAndSoberUpDan& operator=(const SleepAndSoberUpDan&);


public:

	//this is a singleton
	static SleepAndSoberUpDan* Instance();

	virtual void Enter(Drunkard* Drunkard);

	virtual void Execute(Drunkard* Drunkard);

	virtual void Exit(Drunkard* Drunkard);

	virtual bool OnMessage(Drunkard* agent, const Telegram& msg);

};


class TellingStories : public State<Drunkard> {
private:

	TellingStories() {}
	TellingStories(const TellingStories&);
	TellingStories& operator=(const TellingStories&);


public:

	//this is a singleton
	static TellingStories* Instance();

	virtual void Enter(Drunkard* Drunkard);

	virtual void Execute(Drunkard* Drunkard);

	virtual void Exit(Drunkard* Drunkard);

	virtual bool OnMessage(Drunkard* agent, const Telegram& msg);

};
#endif