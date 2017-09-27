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

//
//Dan Global State
//
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

//Thins function will permit to Dan to know if Bob is in the saloon or not
//Each time bob leaves or enter in the saloon, he will send a telegram to Dan
//Each time Dan change the value of bobInSaloon to true or false
bool DrunkardGlobalState::OnMessage(Drunkard* drunkard, const Telegram& msg)
{
	SetTextColor(BACKGROUND_BLUE | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_BLUE);
	switch (msg.Msg)
	{
		case Msg_ImInTheSaloon:
		{
			//cout << "\nMessage handled by " << GetNameOfEntity(drunkard->ID()) << " at time: " << Clock->GetCurrentTime();

			SetTextColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);

			//cout << "\n" << GetNameOfEntity(drunkard->ID()) << ": Bob is in the saloon";

			drunkard->setBobInSaloon();
			//cout << "\n  bob is : " << drunkard->getBobInSaloon();
			return true;
			break;
		}

		case Msg_ImLeavingTheSaloon:
		{
			//cout << "\nMessage handled by " << GetNameOfEntity(drunkard->ID()) << " at time: "<< Clock->GetCurrentTime();

			SetTextColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);

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


QuenchThirstDan* QuenchThirstDan::Instance()
{
	static QuenchThirstDan instance;

	return &instance;
}

void QuenchThirstDan::Enter(Drunkard* pDrunkard)
{
	//Dan is going to start to drink some alcohol
	cout << "\n" << GetNameOfEntity(pDrunkard->ID()) << ": Time to drink som' good ol' whiskey";
}


void QuenchThirstDan::Execute(Drunkard* pDrunkard)
{
	//Dan is drinking, his level of drunkness is increasing
	//If Dan is too drunk, we will change state and start telling stories
	pDrunkard->DrinkAWhiskey();
	cout << "\n" << GetNameOfEntity(pDrunkard->ID()) << ": Drinking...";

	if (pDrunkard->isDrunk()) {
		pDrunkard->GetFSM()->ChangeState(TellingStories::Instance());
	}
}

void QuenchThirstDan::Exit(Drunkard* pDrunkard)
{
	
}

bool QuenchThirstDan::OnMessage(Drunkard* pDrukard, const Telegram& msg) {
	return false;
}

//----------------------


TellingStories* TellingStories::Instance()
{
	static TellingStories instance;
	return &instance;
}

void TellingStories::Enter(Drunkard* pDrunkard)
{
	
}

void TellingStories::Execute(Drunkard* pDrunkard)
{
	//If Bob is in the saloon, Dan wants too fight Bob. He will send a telegram to Bob and change state
	//Else he continues to drink, as he already reachs his level max of drunkness, he starts to loose HP (1by round)
	//He also says some jokes until he has 0 HP then he will change state to sleep
	SetTextColor(BACKGROUND_BLUE | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_BLUE);
	//cout << "\n " << GetNameOfEntity(pDrunkard->ID()) << ": Telling stories execute";
	if (pDrunkard->getBobInSaloon()) {
		//cout << "\n" << GetNameOfEntity(pDrunkard->ID()) << ": sending a message to bob";
		SetTextColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);
		cout << "\n" << GetNameOfEntity(pDrunkard->ID()) << ": Oh Bob come on, hips; i will kick your a** ";
		//start fight with Bob
		Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY, //time delay
			pDrunkard->ID(),        //ID of sender
			ent_Miner_Bob,            //ID of recipient
			Msg_Fight,   //the message
			NO_ADDITIONAL_INFO);
		
		pDrunkard->GetFSM()->ChangeState(FightWithBob::Instance());

		

	}
	else {
		pDrunkard->DecreaseKO(pDrunkard->bruise);
		int randJoke = rand() % 5;

		SetTextColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);
		cout << "\n" << GetNameOfEntity(pDrunkard->ID()) << pDrunkard->getJoke(randJoke);
	}

	if (pDrunkard->isKO()) {
		pDrunkard->GetFSM()->ChangeState(SleepAndSoberUpDan::Instance());
	}

}

void TellingStories::Exit(Drunkard* pDrunkard)
{
	//cout << "\n " << GetNameOfEntity(pDrunkard->ID()) << ": Telling stories exit";
}

bool TellingStories::OnMessage(Drunkard* pDrunkard, const Telegram& msg) {
	return false;
}


//---------------------


FightWithBob* FightWithBob::Instance()
{
	static FightWithBob instance;

	return &instance;
}

void FightWithBob::Enter(Drunkard* pDrunkard)
{
	//Dan starts a fight with Bob
	SetTextColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	cout << "\n" << GetNameOfEntity(pDrunkard->ID()) << ": Fighting -> " << pDrunkard->getKO() << " HP";
}


void FightWithBob::Execute(Drunkard* pDrunkard)
{
	//Dan is loosing HP when he fights
	//If Dan is KO, we will send a telegram and change state to sleep
	if (pDrunkard->isKO()) {
		cout << "\n" << GetNameOfEntity(pDrunkard->ID()) << ": " << "Dam' he got me";
		Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY, //time delay
			pDrunkard->ID(),        //ID of sender
			ent_Miner_Bob,            //ID of recipient
			Msg_DanIsKO,   //the message
			NO_ADDITIONAL_INFO);
		pDrunkard->GetFSM()->ChangeState(SleepAndSoberUpDan::Instance());
		return;
	}

	//cout << "\n" << GetNameOfEntity(pDrunkard->ID()) << ": " << "Dan : " << pDrunkard->getKO() << " HP";
	

	//Dan can hit Bob and deals 1,2 or 4 HP damage, depends on his luck.
	double hitType = RandFloat();
	if (hitType <= 0.3) {
		Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY, //time delay
			pDrunkard->ID(),        //ID of sender
			ent_Miner_Bob,            //ID of recipient
			Msg_DanHitsBobBruise,   //the message
			NO_ADDITIONAL_INFO);
		SetTextColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);
		cout << "\n" << GetNameOfEntity(pDrunkard->ID()) << ": " << "Hits Bob for 1";
	}
	else if(hitType > 0.3 && hitType < 0.8){
		Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY, //time delay
			pDrunkard->ID(),        //ID of sender
			ent_Miner_Bob,            //ID of recipient
			Msg_DanHitsBob,   //the message
			NO_ADDITIONAL_INFO);
		SetTextColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);
		cout << "\n" << GetNameOfEntity(pDrunkard->ID()) << ": " << "Hits Bob for 2";
	}
	else {
		Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY, //time delay
			pDrunkard->ID(),        //ID of sender
			ent_Miner_Bob,            //ID of recipient
			Msg_DanHitsBobCritical,   //the message
			NO_ADDITIONAL_INFO);
		SetTextColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);
		cout << "\n" << GetNameOfEntity(pDrunkard->ID()) << ": " << "Hits Dan for 4";
	}

}

void FightWithBob::Exit(Drunkard* pDrunkard)
{
	
}

//Bob can hit Dan too, so to koww if dan is hit, bob will send a telegram to inform Dan how many HP he losed
bool FightWithBob::OnMessage(Drunkard* pDrunkard, const Telegram& msg) 
{
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


//----------------------


SleepAndSoberUpDan* SleepAndSoberUpDan::Instance()
{
	static SleepAndSoberUpDan instance;
	return &instance;
}

void SleepAndSoberUpDan::Enter(Drunkard* pDrunkard)

{
	//Dan sleep when he is KO (0HP)
	SetTextColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	cout << "\n" << GetNameOfEntity(pDrunkard->ID()) << ": Sleep time i guess... THUMP ... ZZZZZZZZZZZZ";
}

void SleepAndSoberUpDan::Execute(Drunkard* pDrunkard)
{
	//When Dan Sleeps he gains HP (1by round) and sober up
	if (!pDrunkard->needToRecoverFromKO()) {
		pDrunkard->GetFSM()->ChangeState(QuenchThirstDan::Instance());
	}
	else {
		pDrunkard->Sleep();
		cout << "\n" << GetNameOfEntity(pDrunkard->ID()) << ": Sleeping... -> drunkness : " << pDrunkard->getDrunkness();
	}
}

void SleepAndSoberUpDan::Exit(Drunkard* pDrunkard)
{
	//When his HP reachs MaxHP, he wkaes up
	cout << "\n" << GetNameOfEntity(pDrunkard->ID()) << ": What a great sleep i had!!";
}

bool SleepAndSoberUpDan::OnMessage(Drunkard* pDrunkard, const Telegram& msg) {
	return false;


}

