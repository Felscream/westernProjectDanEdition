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
//Class used for receiving a telegram in all possible states
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

//Class for the Initialisation of Dan
class Creation : public State<Drunkard>
{
private:

	Creation() {}

	//copy ctor and assignment should be private
	Creation(const Creation&);
	Creation& operator=(const Creation&);

public:

	//this is a singleton
	static Creation* Instance();

	virtual void Enter(Drunkard* Drunkard);

	virtual void Execute(Drunkard* Drunkard);

	virtual void Exit(Drunkard* Drunkard);

	virtual bool OnMessage(Drunkard* agent, const Telegram& msg);

};

//Class for the Drinking state where d	n is going to drink every turn and increase his drunkness. 
//When the limit is attained, Dan become drunk and pass in TellingStories state 
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



//Class used for the fighting state between Bob and Dan
//This state is entered when dan is drunk and bob is in the bar
//the both hit the other turn by turn with a probability to hit with a lesser hit, normal hit or critical hit
//when dan win the fight, he continue to tell jokes until he pass out and enter in resting state
//when dan lose the fight, he enter directly in resting state 
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

//Class used for the resting state after Dan is KO
//Gradually sober up Dan until he is rested enough to continue to drink
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

//Class used for when Dan is drunk and start telling jokes in the saloon
//Telling jokes decrease the hp bar of Dan by 1 each time until he pass out or that Bob enter the bar and a fight begins
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