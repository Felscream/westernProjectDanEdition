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
	//nothing happen here
}

void DrunkardGlobalState::Execute(Drunkard* drunkard)
{

}

void DrunkardGlobalState::Exit(Drunkard* pDrunkard)
{
	//nothing happen here
}

bool DrunkardGlobalState::OnMessage(Drunkard* drunkard, const Telegram& msg)
{
	
	switch (msg.Msg)
	{
		case Msg_ImInTheSaloon:
		{
			drunkard->setBobInSaloon();
			return true;
			break;
		}

		case Msg_ImLeavingTheSaloon:
		{
			drunkard->setBobInSaloon();
			return true;
			break;
		}

		

	}//end switch

	return false;
}

// ---------------------------
Creation* Creation::Instance()
{
static Creation instance;

return &instance;
}

void Creation::Enter(Drunkard* pDrunkard)
{
	//nothing happen here
}


void Creation::Execute(Drunkard* pDrunkard)
{
	//initial state at drinking state
	pDrunkard->GetFSM()->ChangeState(QuenchThirstDan::Instance());
}

void Creation::Exit(Drunkard* pDrunkard)
{
	//nothing happen here
}

bool Creation::OnMessage(Drunkard* pDrukard, const Telegram& msg) {
	return false;
}

QuenchThirstDan* QuenchThirstDan::Instance()
{
	static QuenchThirstDan instance;

	return &instance;
}

void QuenchThirstDan::Enter(Drunkard* pDrunkard)
{
	//Dan drink 

	pDrunkard->sharedPrint(GetNameOfEntity(pDrunkard->ID()), (string) "Time to drink som' good ol' whiskey", FOREGROUND_BLUE | FOREGROUND_INTENSITY);
}


void QuenchThirstDan::Execute(Drunkard* pDrunkard)
{
	//Dan drink until he pass in drunk state
	pDrunkard->DrinkAWhiskey();
	pDrunkard->sharedPrint(GetNameOfEntity(pDrunkard->ID()), (string) "Drinking...", FOREGROUND_BLUE | FOREGROUND_INTENSITY);

	if (pDrunkard->isDrunk()) {
		pDrunkard->GetFSM()->ChangeState(TellingStories::Instance());
	}
}

void QuenchThirstDan::Exit(Drunkard* pDrunkard)
{
	//nothing happen here
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
	//if Dan is KO by fighting Bob, he display that he is KO, send a telegram to Bob to indicate that he is KO and then enter in resting state
	if (pDrunkard->isKO()) {
		
		pDrunkard->sharedPrint(GetNameOfEntity(pDrunkard->ID()), (string) "Dam' he got me", FOREGROUND_BLUE | FOREGROUND_INTENSITY);
		pDrunkard->sharedPrintTelegram(pDrunkard->ID(), ent_Miner_Bob, Msg_DanIsKO);
		pDrunkard->GetFSM()->ChangeState(SleepAndSoberUpDan::Instance());
		return;
	}

	//determine by a random which hit is used : lesser 30%, normal 50% or critical 20%
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
	//nothing happen here
}

//receive the fight telegram from bob and decrease the hp bar when hit or indicate when bob is KO
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
	//display that Dan is KO and go sleep
	pDrunkard->sharedPrint(GetNameOfEntity(pDrunkard->ID()), (string) "Sleep time i guess... THUMP ... ZZZZZZZZZZZZ", FOREGROUND_BLUE | FOREGROUND_INTENSITY);
}





void SleepAndSoberUpDan::Execute(Drunkard* pDrunkard)
{
	//if the agent hp bar is full, he go drinking else it means that he is still resting  
	if (!pDrunkard->needToRecoverFromKO()) {
		pDrunkard->GetFSM()->ChangeState(QuenchThirstDan::Instance());
	}
	else {
		pDrunkard->Sleep();
		//display the drunkness level
		pDrunkard->sharedPrint(GetNameOfEntity(pDrunkard->ID()), (string) "Sleeping... -> drunkness : " + to_string(pDrunkard->getDrunkness()), FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	}
	

	

}



void SleepAndSoberUpDan::Exit(Drunkard* pDrunkard)
{
	//Dan is rested anough
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
	//nothing happen here
}





void TellingStories::Execute(Drunkard* pDrunkard)
{
	//drunk state of Dan
	//if bob is not in the saloon, Dan tell jokes (1 joke decrease his hp bar by 1 hp until he pass out at 0 hp)
	//if bob is in the saloon, a fight begin between Bob and Dan
	
	if (pDrunkard->bobIsInTheSaloon()) {
		pDrunkard->sharedPrint(GetNameOfEntity(pDrunkard->ID()), (string) "Oh Bob come on, hips; i will kick your a**", FOREGROUND_BLUE | FOREGROUND_INTENSITY);
		//start fight with Bob
		pDrunkard->sharedPrintTelegram(pDrunkard->ID(), ent_Miner_Bob, Msg_Fight);		
		pDrunkard->GetFSM()->ChangeState(FightWithBob::Instance());

		

	}
	else {
		pDrunkard->DecreaseKO(pDrunkard->bruise);
		//tell a random joke from the 5 available jokes
		int randJoke = rand() % 5;

		pDrunkard->sharedPrint(GetNameOfEntity(pDrunkard->ID()), pDrunkard->getJoke(randJoke), FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	}

	if (pDrunkard->isKO()) {
		pDrunkard->GetFSM()->ChangeState(SleepAndSoberUpDan::Instance());
	}

}



void TellingStories::Exit(Drunkard* pDrunkard)
{
	//nothing happen here
}



bool TellingStories::OnMessage(Drunkard* pDrunkard, const Telegram& msg) {
	return false;
}