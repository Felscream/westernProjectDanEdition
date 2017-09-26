#include "DrunkardOwnedStates.h"
#include "fsm/State.h"
#include "Drunkard.h"
#include "Locations.h"
#include "messaging/Telegram.h"
#include "MessageDispatcher.h"
#include "MessageTypes.h"
#include "Time/CrudeTimer.h"
#include "EntityNames.h"

#include <iostream>
using std::cout;


#ifdef TEXTOUTPUT
#include <fstream>
extern std::ofstream os;
#define cout os
#endif

//Dan global state
DrunkardGlobalState* DrunkardGlobalState::Instance()
{
	static DrunkardGlobalState instance;

	return &instance;
}

void DrunkardGlobalState::Enter(Drunkard* pDrunkard)
{
	
}

void DrunkardGlobalState::Execute(Drunkard* drunkard)
{

}

void DrunkardGlobalState::Exit(Drunkard* pDrunkard)
{
	
}

bool DrunkardGlobalState::OnMessage(Drunkard* drunkard, const Telegram& msg)
{
	
	switch (msg.Msg)
	{
		case Msg_ImInTheSaloon:
		{
			drunkard->setBobInSaloon();
			return true;
		}

		case Msg_ImLeavingTheSaloon:
		{
			drunkard->setBobInSaloon();
			return true;
		}

		

	}

	return false;
}

// ---------------------------


QuenchThirstDan* QuenchThirstDan::Instance()
{
	static QuenchThirstDan instance;

	return &instance;
}

void QuenchThirstDan::Enter(Drunkard* pDrunkard)
{
	pDrunkard->sharedPrint(GetNameOfEntity(pDrunkard->ID()), (string) "Time to drink som' good ol' whiskey", FOREGROUND_BLUE | FOREGROUND_INTENSITY);
}


void QuenchThirstDan::Execute(Drunkard* pDrunkard)
{
	pDrunkard->DrinkAWhiskey();
	pDrunkard->sharedPrint(GetNameOfEntity(pDrunkard->ID()), (string) "Drinking...", FOREGROUND_BLUE | FOREGROUND_INTENSITY);

	if (pDrunkard->isDrunk()) {
		pDrunkard->GetFSM()->ChangeState(TellingStories::Instance());
	}
}

void QuenchThirstDan::Exit(Drunkard* pDrunkard)
{
	pDrunkard->sharedPrint(GetNameOfEntity(pDrunkard->ID()), (string) "Feeling a bit tipsy... HIC ...", FOREGROUND_BLUE | FOREGROUND_INTENSITY);
}

bool QuenchThirstDan::OnMessage(Drunkard* pDrukard, const Telegram& msg) {
	return false;
}


FightWithBob* FightWithBob::Instance()
{
	static FightWithBob instance;

	return &instance;
}

void FightWithBob::Enter(Drunkard* pDrunkard)
{
	pDrunkard->sharedPrint(GetNameOfEntity(pDrunkard->ID()), (string) "Fighting -> " + to_string(pDrunkard->getKO()) + " HP", FOREGROUND_BLUE | FOREGROUND_INTENSITY);
}


void FightWithBob::Execute(Drunkard* pDrunkard)
{
	
	if (pDrunkard->isKO()) {
		
		pDrunkard->sharedPrint(GetNameOfEntity(pDrunkard->ID()), (string) "Dam' he got me", FOREGROUND_BLUE | FOREGROUND_INTENSITY);
		pDrunkard->sharedPrintTelegram(pDrunkard->ID(), ent_Miner_Bob, Msg_DanIsKO);
		pDrunkard->GetFSM()->ChangeState(SleepAndSoberUpDan::Instance());
		return;
	}
	
	double hitType = RandFloat();
	if (hitType <= 0.3) {
		pDrunkard->sharedPrintTelegram(pDrunkard->ID(), ent_Miner_Bob, Msg_DanHitsBobBruise);
		pDrunkard->sharedPrint(GetNameOfEntity(pDrunkard->ID()), (string) "Hits Bob for 1", FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	}
	else if(hitType > 0.3 && hitType < 0.8){
		pDrunkard->sharedPrintTelegram(pDrunkard->ID(), ent_Miner_Bob, Msg_DanHitsBob);
		pDrunkard->sharedPrint(GetNameOfEntity(pDrunkard->ID()), (string) "Hits Bob for 2", FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	}
	else {
		pDrunkard->sharedPrintTelegram(pDrunkard->ID(), ent_Miner_Bob, Msg_DanHitsBobCritical);
		pDrunkard->sharedPrint(GetNameOfEntity(pDrunkard->ID()), (string) "Hits Bob for 4", FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	}

}

void FightWithBob::Exit(Drunkard* pDrunkard)
{
	pDrunkard->sharedPrint(GetNameOfEntity(pDrunkard->ID()), (string) "I've got wine on my nos'", FOREGROUND_BLUE | FOREGROUND_INTENSITY);
}

bool FightWithBob::OnMessage(Drunkard* pDrunkard, const Telegram& msg) {
	switch (msg.Msg) {
		case Msg_BobHitsDanBruise:
			pDrunkard->DecreaseKO(pDrunkard->bruise);
			pDrunkard->checkKO();
			return true;

		case Msg_BobHitsDan:
			pDrunkard->DecreaseKO(pDrunkard->hit);
			pDrunkard->checkKO();
			return true;

		case Msg_BobIsKO:
			pDrunkard->GetFSM()->RevertToPreviousState();
			return true;
	}
	return false;
}

SleepAndSoberUpDan* SleepAndSoberUpDan::Instance()
{
	static SleepAndSoberUpDan instance;
	return &instance;
}



void SleepAndSoberUpDan::Enter(Drunkard* pDrunkard)

{
	pDrunkard->sharedPrint(GetNameOfEntity(pDrunkard->ID()), (string) "Sleep time i guess... THUMP ... ZZZZZZZZZZZZ", FOREGROUND_BLUE | FOREGROUND_INTENSITY);
}





void SleepAndSoberUpDan::Execute(Drunkard* pDrunkard)
{
	if (!pDrunkard->needToRecoverFromKO()) {
		pDrunkard->GetFSM()->ChangeState(QuenchThirstDan::Instance());
	}
	else {
		pDrunkard->Sleep();
		pDrunkard->sharedPrint(GetNameOfEntity(pDrunkard->ID()), (string) "Sleeping... -> drunkness : " + to_string(pDrunkard->getDrunkness()), FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	}
}



void SleepAndSoberUpDan::Exit(Drunkard* pDrunkard)
{
	pDrunkard->sharedPrint(GetNameOfEntity(pDrunkard->ID()), (string) " What a great sleep i had!!", FOREGROUND_BLUE | FOREGROUND_INTENSITY);
}



bool SleepAndSoberUpDan::OnMessage(Drunkard* pDrunkard, const Telegram& msg) {
	return false;
}

TellingStories* TellingStories::Instance()
{
	static TellingStories instance;
	return &instance;
}



void TellingStories::Enter(Drunkard* pDrunkard)

{
	pDrunkard->sharedPrint(GetNameOfEntity(pDrunkard->ID()), (string) "Hey, wanna hear a joke ?", FOREGROUND_BLUE | FOREGROUND_INTENSITY);
}





void TellingStories::Execute(Drunkard* pDrunkard)
{
	if (pDrunkard->isKO()) {
		pDrunkard->GetFSM()->ChangeState(SleepAndSoberUpDan::Instance());
	}
	if (pDrunkard->bobIsInTheSaloon()) {
		pDrunkard->sharedPrint(GetNameOfEntity(pDrunkard->ID()), (string) "Oh Bob come on, hips; i will kick your a**", FOREGROUND_BLUE | FOREGROUND_INTENSITY);
		pDrunkard->sharedPrintTelegram(pDrunkard->ID(), ent_Miner_Bob, Msg_Fight);
		pDrunkard->GetFSM()->ChangeState(FightWithBob::Instance());
	}
	else {
		pDrunkard->DecreaseKO(pDrunkard->bruise);
		int randJoke = rand() % 5;
		pDrunkard->sharedPrint(GetNameOfEntity(pDrunkard->ID()), pDrunkard->getJoke(randJoke), FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	}
}



void TellingStories::Exit(Drunkard* pDrunkard)
{
}



bool TellingStories::OnMessage(Drunkard* pDrunkard, const Telegram& msg) {
	return false;
}