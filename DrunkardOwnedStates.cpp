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
	//if the miner is not already located at the goldmine, he must
	//change location to the gold mine
	//cout << "\n " << GetNameOfEntity(pDrunkard->ID()) << ": DrunkardGlobalState - Enter()";
}

void DrunkardGlobalState::Execute(Drunkard* drunkard)
{

}

void DrunkardGlobalState::Exit(Drunkard* pDrunkard)
{
	//if the miner is not already located at the goldmine, he must
	//change location to the gold mine
	//cout << "\n " << GetNameOfEntity(pDrunkard->ID()) << ": DrunkardGlobalState - Exit()";
}

bool DrunkardGlobalState::OnMessage(Drunkard* drunkard, const Telegram& msg)
{
	
	switch (msg.Msg)
	{
		case Msg_ImInTheSaloon:
		{
			//cout << "\nMessage handled by " << GetNameOfEntity(drunkard->ID()) << " at time: " << Clock->GetCurrentTime();

			

			//cout << "\n" << GetNameOfEntity(drunkard->ID()) << ": Bob is in the saloon";

			drunkard->setBobInSaloon();
			//cout << "\n  bob is : " << drunkard->getBobInSaloon();
			return true;
			break;
		}

		case Msg_ImLeavingTheSaloon:
		{
			//cout << "\nMessage handled by " << GetNameOfEntity(drunkard->ID()) << " at time: "<< Clock->GetCurrentTime();

			

			//cout << "\n" << GetNameOfEntity(drunkard->ID()) << ": Bob is leaving";

			drunkard->setBobInSaloon();
			//cout << "\n  bob is : " << drunkard->getBobInSaloon();
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
	//if the miner is not already located at the goldmine, he must
	//change location to the gold mine
	//cout << "\n " << GetNameOfEntity(pDrunkard->ID()) << ": Creation - Enter()";
}


void Creation::Execute(Drunkard* pDrunkard)
{
	//Now the miner is at the goldmine he digs for gold until he
	//is carrying in excess of MaxNuggets. If he gets thirsty during
	//his digging he packs up work for a while and changes state to
	//gp to the saloon for a whiskey.
	//cout << "\n " << GetNameOfEntity(pDrunkard->ID()) << ": Creation - Execute()";
	pDrunkard->GetFSM()->ChangeState(QuenchThirstDan::Instance());
}

void Creation::Exit(Drunkard* pDrunkard)
{
	//Now the miner is at the goldmine he digs for gold until he
	//is carrying in excess of MaxNuggets. If he gets thirsty during
	//his digging he packs up work for a while and changes state to
	//gp to the saloon for a whiskey.
	//cout << "\n " << GetNameOfEntity(pDrunkard->ID()) << ": Drunkard - Exit()";
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
	//if the miner is not already located at the goldmine, he must
	//change location to the gold mine

	//cout << "\n" << GetNameOfEntity(pDrunkard->ID()) << ": Time to drink som' good ol' whiskey";
	pDrunkard->sharedPrint(GetNameOfEntity(pDrunkard->ID()), (string) "Time to drink som' good ol' whiskey", FOREGROUND_BLUE | FOREGROUND_INTENSITY);
}


void QuenchThirstDan::Execute(Drunkard* pDrunkard)
{
	//Now the miner is at the goldmine he digs for gold until he
	//is carrying in excess of MaxNuggets. If he gets thirsty during
	//his digging he packs up work for a while and changes state to
	//gp to the saloon for a whiskey.
	pDrunkard->DrinkAWhiskey();
	pDrunkard->sharedPrint(GetNameOfEntity(pDrunkard->ID()), (string) "Drinking...", FOREGROUND_BLUE | FOREGROUND_INTENSITY);

	if (pDrunkard->isDrunk()) {
		pDrunkard->GetFSM()->ChangeState(TellingStories::Instance());
	}
}

void QuenchThirstDan::Exit(Drunkard* pDrunkard)
{
	//Now the miner is at the goldmine he digs for gold until he
	//is carrying in excess of MaxNuggets. If he gets thirsty during
	//his digging he packs up work for a while and changes state to
	//gp to the saloon for a whiskey.
	//cout << "\n " << GetNameOfEntity(pDrunkard->ID()) << ": Drunkard - Exit()";
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
		//Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY, //time delay
		//	pDrunkard->ID(),        //ID of sender
		//	ent_Miner_Bob,            //ID of recipient
		//	Msg_DanIsKO,   //the message
		//	NO_ADDITIONAL_INFO);
		pDrunkard->GetFSM()->ChangeState(SleepAndSoberUpDan::Instance());
		return;
	}

	//cout << "\n" << GetNameOfEntity(pDrunkard->ID()) << ": " << "Dan : " << pDrunkard->getKO() << " HP";
	
	double hitType = RandFloat();
	if (hitType <= 0.3) {
		pDrunkard->sharedPrintTelegram(pDrunkard->ID(), ent_Miner_Bob, Msg_DanHitsBobBruise);
		//Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY, //time delay
		//	pDrunkard->ID(),        //ID of sender
		//	ent_Miner_Bob,            //ID of recipient
		//	Msg_DanHitsBobBruise,   //the message
		//	NO_ADDITIONAL_INFO);
		pDrunkard->sharedPrint(GetNameOfEntity(pDrunkard->ID()), (string) "Hits Bob for 1", FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	}
	else if(hitType > 0.3 && hitType < 0.8){
		pDrunkard->sharedPrintTelegram(pDrunkard->ID(), ent_Miner_Bob, Msg_DanHitsBob);
		//Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY, //time delay
		//	pDrunkard->ID(),        //ID of sender
		//	ent_Miner_Bob,            //ID of recipient
		//	Msg_DanHitsBob,   //the message
		//	NO_ADDITIONAL_INFO);
		pDrunkard->sharedPrint(GetNameOfEntity(pDrunkard->ID()), (string) "Hits Bob for 2", FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	}
	else {
		pDrunkard->sharedPrintTelegram(pDrunkard->ID(), ent_Miner_Bob, Msg_DanHitsBobCritical);
		//Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY, //time delay
		//	pDrunkard->ID(),        //ID of sender
		//	ent_Miner_Bob,            //ID of recipient
		//	Msg_DanHitsBobCritical,   //the message
		//	NO_ADDITIONAL_INFO);
		pDrunkard->sharedPrint(GetNameOfEntity(pDrunkard->ID()), (string) "Hits Bob for 4", FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	}

}

void FightWithBob::Exit(Drunkard* pDrunkard)
{
	//Now the miner is at the goldmine he digs for gold until he
	//is carrying in excess of MaxNuggets. If he gets thirsty during
	//his digging he packs up work for a while and changes state to
	//gp to the saloon for a whiskey.
	//cout << "\n " << GetNameOfEntity(pDrunkard->ID()) << ": FightWithBob - Exit()";
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
	//if the miner is not already located at the goldmine, he must
	//change location to the gold mine
	pDrunkard->sharedPrint(GetNameOfEntity(pDrunkard->ID()), (string) "Sleep time i guess... THUMP ... ZZZZZZZZZZZZ", FOREGROUND_BLUE | FOREGROUND_INTENSITY);
}





void SleepAndSoberUpDan::Execute(Drunkard* pDrunkard)
{
	//Now the miner is at the goldmine he digs for gold until he
	//is carrying in excess of MaxNuggets. If he gets thirsty during
	//his digging he packs up work for a while and changes state to
	//gp to the saloon for a whiskey.
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
	//Now the miner is at the goldmine he digs for gold until he
	//is carrying in excess of MaxNuggets. If he gets thirsty during
	//his digging he packs up work for a while and changes state to
	//gp to the saloon for a whiskey.
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
	//if the miner is not already located at the goldmine, he must
	//change location to the gold mine
	//cout << "\n " << GetNameOfEntity(pDrunkard->ID()) << ": Telling stories enter";
}





void TellingStories::Execute(Drunkard* pDrunkard)
{
	//Now the miner is at the goldmine he digs for gold until he
	//is carrying in excess of MaxNuggets. If he gets thirsty during
	//his digging he packs up work for a while and changes state to
	//gp to the saloon for a whiskey.
	
	//cout << "\n " << GetNameOfEntity(pDrunkard->ID()) << ": Telling stories execute";
	if (pDrunkard->bobIsInTheSaloon()) {
		//cout << "\n" << GetNameOfEntity(pDrunkard->ID()) << ": sending a message to bob";
		pDrunkard->sharedPrint(GetNameOfEntity(pDrunkard->ID()), (string) "Oh Bob come on, hips; i will kick your a**", FOREGROUND_BLUE | FOREGROUND_INTENSITY);
		//start fight with Bob
		pDrunkard->sharedPrintTelegram(pDrunkard->ID(), ent_Miner_Bob, Msg_Fight);
		//Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY, //time delay
		//	pDrunkard->ID(),        //ID of sender
		//	ent_Miner_Bob,            //ID of recipient
		//	Msg_Fight,   //the message
		//	NO_ADDITIONAL_INFO);
		
		pDrunkard->GetFSM()->ChangeState(FightWithBob::Instance());

		

	}
	else {
		pDrunkard->DecreaseKO(pDrunkard->bruise);
		int randJoke = rand() % 5;

		pDrunkard->sharedPrint(GetNameOfEntity(pDrunkard->ID()), pDrunkard->getJoke(randJoke), FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	}

	if (pDrunkard->isKO()) {
		pDrunkard->GetFSM()->ChangeState(SleepAndSoberUpDan::Instance());
	}

}



void TellingStories::Exit(Drunkard* pDrunkard)
{
	//Now the miner is at the goldmine he digs for gold until he
	//is carrying in excess of MaxNuggets. If he gets thirsty during
	//his digging he packs up work for a while and changes state to
	//gp to the saloon for a whiskey.
	//cout << "\n " << GetNameOfEntity(pDrunkard->ID()) << ": Telling stories exit";
}



bool TellingStories::OnMessage(Drunkard* pDrunkard, const Telegram& msg) {
	return false;
}