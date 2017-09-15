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
	cout << "\n DrunkardGlobalState - Enter()";
}

void DrunkardGlobalState::Execute(Drunkard* drunkard)
{
	cout << "EXECUTE GLOBAL STATE";
}

void DrunkardGlobalState::Exit(Drunkard* pDrunkard)
{
	//if the miner is not already located at the goldmine, he must
	//change location to the gold mine
	cout << "\n DrunkardGlobalState - Exit()";
}

bool DrunkardGlobalState::OnMessage(Drunkard* drunkard, const Telegram& msg)
{
	SetTextColor(BACKGROUND_BLUE | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_BLUE);
	cout << "\n Message received";
	switch (msg.Msg)
	{
	case Msg_ImInTheSaloon:
	{
		cout << "\nMessage handled by " << GetNameOfEntity(drunkard->ID()) << " at time: "
			<< Clock->GetCurrentTime();

		SetTextColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);

		cout << "\n" << GetNameOfEntity(drunkard->ID()) <<
			": Bob is in the saloon";

		drunkard->setBobInSaloon();
		cout << " \n "<<drunkard->getBobInSaloon();
	}
	case Msg_ImLeavingTheSaloon:
	{
		cout << "\nMessage handled by " << GetNameOfEntity(drunkard->ID()) << " at time: "
			<< Clock->GetCurrentTime();

		SetTextColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);

		cout << "\n" << GetNameOfEntity(drunkard->ID()) <<
			": Bob is leaving";

		drunkard->setBobInSaloon();
	}

	return true;

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
	cout <<"\n Creation - Enter()";
}


void Creation::Execute(Drunkard* pDrunkard)
{
	//Now the miner is at the goldmine he digs for gold until he
	//is carrying in excess of MaxNuggets. If he gets thirsty during
	//his digging he packs up work for a while and changes state to
	//gp to the saloon for a whiskey.
	cout << "\n"<< "Creation - Execute()";
	pDrunkard->GetFSM()->ChangeState(QuenchThirstDan::Instance());
}

void Creation::Exit(Drunkard* pDrunkard)
{
	//Now the miner is at the goldmine he digs for gold until he
	//is carrying in excess of MaxNuggets. If he gets thirsty during
	//his digging he packs up work for a while and changes state to
	//gp to the saloon for a whiskey.
	cout << "\n" "Drunkard - Exit()";
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
	cout << "\n Time to drink som' good ol' whiskey";
}


void QuenchThirstDan::Execute(Drunkard* pDrunkard)
{
	//Now the miner is at the goldmine he digs for gold until he
	//is carrying in excess of MaxNuggets. If he gets thirsty during
	//his digging he packs up work for a while and changes state to
	//gp to the saloon for a whiskey.
	pDrunkard->DrinkAWhiskey();
	cout << "\n Drunkard - drinking()";

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
	cout << "\n" "Drunkard - Exit()";
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
	//if the miner is not already located at the goldmine, he must
	//change location to the gold mine
	cout << "\n Oh Bob come on, hips; i will kick your a**";
}


void FightWithBob::Execute(Drunkard* pDrunkard)
{
	//Now the miner is at the goldmine he digs for gold until he
	//is carrying in excess of MaxNuggets. If he gets thirsty during
	//his digging he packs up work for a while and changes state to
	//gp to the saloon for a whiskey.
	pDrunkard->DecreaseKO();
	cout << "\n OH Woa why there are two Bob, hips, i think i drunk too much alcohol...";

	if (pDrunkard->isKO()) {
		pDrunkard->GetFSM()->ChangeState(SleepAndSoberUpDan::Instance());
	}
}

void FightWithBob::Exit(Drunkard* pDrunkard)
{
	//Now the miner is at the goldmine he digs for gold until he
	//is carrying in excess of MaxNuggets. If he gets thirsty during
	//his digging he packs up work for a while and changes state to
	//gp to the saloon for a whiskey.
	cout << "\n" "FightWithBob - Exit()";
}

bool FightWithBob::OnMessage(Drunkard* pDrukard, const Telegram& msg) {
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
	cout << "\n Sleep time i guess... THUMP ... ZZZZZZZZZZZZ";
}





void SleepAndSoberUpDan::Execute(Drunkard* pDrunkard)
{
	//Now the miner is at the goldmine he digs for gold until he
	//is carrying in excess of MaxNuggets. If he gets thirsty during
	//his digging he packs up work for a while and changes state to
	//gp to the saloon for a whiskey.
	pDrunkard->Sleeping();
	cout << "\n Drunkard - sleeping(), drunkness: " << pDrunkard->getDrunkness();

	if (!pDrunkard->isSleeping()) {
		pDrunkard->GetFSM()->ChangeState(QuenchThirstDan::Instance());
	}

}



void SleepAndSoberUpDan::Exit(Drunkard* pDrunkard)
{
	//Now the miner is at the goldmine he digs for gold until he
	//is carrying in excess of MaxNuggets. If he gets thirsty during
	//his digging he packs up work for a while and changes state to
	//gp to the saloon for a whiskey.
	cout << "\n" "What a great sleep i had!!";
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
	cout << "\n Telling stories enter";
}





void TellingStories::Execute(Drunkard* pDrunkard)
{
	//Now the miner is at the goldmine he digs for gold until he
	//is carrying in excess of MaxNuggets. If he gets thirsty during
	//his digging he packs up work for a while and changes state to
	//gp to the saloon for a whiskey.
	pDrunkard->DecreaseKO();
	cout << "\n Telling stories execute";

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
	cout << "\n" "Telling stories exit";
}



bool TellingStories::OnMessage(Drunkard* pDrunkard, const Telegram& msg) {
	return false;
}