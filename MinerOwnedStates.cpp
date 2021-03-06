#include "MinerOwnedStates.h"
#include "fsm/State.h"
#include "Miner.h"
#include "Locations.h"
#include "messaging/Telegram.h"
#include "MessageDispatcher.h"
#include "MessageTypes.h"
#include "Time/CrudeTimer.h"
#include "EntityNames.h"
#include "misc/Utils.h"

#include <iostream>
using std::cout;


#ifdef TEXTOUTPUT
#include <fstream>
extern std::ofstream os;
#define cout os
#endif


//------------------------------------------------------------------------methods for EnterMineAndDigForNugget
EnterMineAndDigForNugget* EnterMineAndDigForNugget::Instance()
{
  static EnterMineAndDigForNugget instance;

  return &instance;
}


void EnterMineAndDigForNugget::Enter(Miner* pMiner)
{
  //if the miner is not already located at the goldmine, he must
  //change location to the gold mine
	SetTextColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
  if (pMiner->Location() != goldmine)
  {
    cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "Walkin' to the goldmine";

    pMiner->ChangeLocation(goldmine);
  }
}


void EnterMineAndDigForNugget::Execute(Miner* pMiner)
{  
  //Now the miner is at the goldmine he digs for gold until he
  //is carrying in excess of MaxNuggets. If he gets thirsty during
  //his digging he packs up work for a while and changes state to
  //gp to the saloon for a whiskey.
  pMiner->AddToGoldCarried(1);

  pMiner->IncreaseFatigue();

  cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "Pickin' up a nugget";

  //if enough gold mined, go and put it in the bank
  if (pMiner->PocketsFull())
  {
    pMiner->GetFSM()->ChangeState(VisitBankAndDepositGold::Instance());
  }

  if (pMiner->Thirsty())
  {
    pMiner->GetFSM()->ChangeState(QuenchThirst::Instance());
  }
}


void EnterMineAndDigForNugget::Exit(Miner* pMiner)
{
  cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " 
       << "Ah'm leavin' the goldmine with mah pockets full o' sweet gold";
}


bool EnterMineAndDigForNugget::OnMessage(Miner* pMiner, const Telegram& msg)
{
  //send msg to global message handler
  return false;
}

//------------------------------------------------------------------------methods for VisitBankAndDepositGold

VisitBankAndDepositGold* VisitBankAndDepositGold::Instance()
{
  static VisitBankAndDepositGold instance;

  return &instance;
}

void VisitBankAndDepositGold::Enter(Miner* pMiner)
{  
  //on entry the miner makes sure he is located at the bank
  if (pMiner->Location() != bank)
  {
    cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "Goin' to the bank. Yes siree";

    pMiner->ChangeLocation(bank);
  }
}


void VisitBankAndDepositGold::Execute(Miner* pMiner)
{
  //deposit the gold
  pMiner->AddToWealth(pMiner->GoldCarried());
    
  pMiner->SetGoldCarried(0);

  cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " 
       << "Depositing gold. Total savings now: "<< pMiner->Wealth();

  //wealthy enough to have a well earned rest?
  if (pMiner->Wealth() >= ComfortLevel)
  {
    cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " 
         << "WooHoo! Rich enough for now. Back home to mah li'lle lady";
      
    pMiner->GetFSM()->ChangeState(GoHomeAndSleepTilRested::Instance());      
  }

  //otherwise get more gold
  else 
  {
    pMiner->GetFSM()->ChangeState(EnterMineAndDigForNugget::Instance());
  }
}


void VisitBankAndDepositGold::Exit(Miner* pMiner)
{
  cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "Leavin' the bank";
}


bool VisitBankAndDepositGold::OnMessage(Miner* pMiner, const Telegram& msg)
{
  //send msg to global message handler
  return false;
}
//------------------------------------------------------------------------methods for GoHomeAndSleepTilRested

GoHomeAndSleepTilRested* GoHomeAndSleepTilRested::Instance()
{
  static GoHomeAndSleepTilRested instance;

  return &instance;
}

void GoHomeAndSleepTilRested::Enter(Miner* pMiner)
{
  if (pMiner->Location() != shack)
  {
    cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "Walkin' home";

    pMiner->ChangeLocation(shack); 

    //let the wife know I'm home
    Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY, //time delay
                              pMiner->ID(),        //ID of sender
                              ent_Elsa,            //ID of recipient
                              Msg_HiHoneyImHome,   //the message
                              NO_ADDITIONAL_INFO);    
  }
}

void GoHomeAndSleepTilRested::Execute(Miner* pMiner)
{ 
  //if miner is not fatigued start to dig for nuggets again.
	if (!pMiner->Fatigued() && !pMiner->needToRecoverFromKO())
  {
     cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " 
          << "All mah fatigue has drained away. Time to find more gold!";

     pMiner->GetFSM()->ChangeState(EnterMineAndDigForNugget::Instance());
  }

  else 
  {
    //sleep
    pMiner->DecreaseFatigue();

    cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "ZZZZ... ";
  } 
}

void GoHomeAndSleepTilRested::Exit(Miner* pMiner)
{ 
}


bool GoHomeAndSleepTilRested::OnMessage(Miner* pMiner, const Telegram& msg)
{

   switch(msg.Msg)
   {
   case Msg_StewReady:

     cout << "\nMessage handled by " << GetNameOfEntity(pMiner->ID()) 
     << " at time: " << Clock->GetCurrentTime();

     SetTextColor(FOREGROUND_RED|FOREGROUND_INTENSITY);

     cout << "\n" << GetNameOfEntity(pMiner->ID()) 
          << ": Okay Hun, ahm a comin'!";

     pMiner->GetFSM()->ChangeState(EatStew::Instance());
      
     return true;

   }//end switch

   return false; //send message to global message handler
}

//------------------------------------------------------------------------QuenchThirst

QuenchThirst* QuenchThirst::Instance()
{
  static QuenchThirst instance;

  return &instance;
}

void QuenchThirst::Enter(Miner* pMiner)
{
  if (pMiner->Location() != saloon)
  {    
    pMiner->ChangeLocation(saloon);

    cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "Boy, ah sure is thusty! Walking to the saloon";
  }

  Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY, //time delay
	  pMiner->ID(),        //ID of sender
	  ent_Dan,            //ID of recipient
	  Msg_ImInTheSaloon,   //the message
	  NO_ADDITIONAL_INFO);
}

void QuenchThirst::Execute(Miner* pMiner)
{
  pMiner->BuyAndDrinkAWhiskey();

  cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "That's mighty fine sippin' liquer";
  if (pMiner->Hydrated()) {
	  cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "Leaving the saloon, feelin' good";
	  pMiner->GetFSM()->ChangeState(EnterMineAndDigForNugget::Instance());
  }
}


void QuenchThirst::Exit(Miner* pMiner)
{ 
	SetTextColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
	Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY, //time delay
		pMiner->ID(),        //ID of sender
		ent_Dan,            //ID of recipient
		Msg_ImLeavingTheSaloon,   //the message
		NO_ADDITIONAL_INFO);
}


bool QuenchThirst::OnMessage(Miner* pMiner, const Telegram& msg)
{
  //If Dan is too drunk, Bob will fight with Dan
	switch (msg.Msg) {
		case Msg_Fight: {
			pMiner->GetFSM()->ChangeState(FightWithDan::Instance());
		}

		return true;
	}
	return false;
}

//------------------------------------------------------------------------EatStew

EatStew* EatStew::Instance()
{
  static EatStew instance;

  return &instance;
}


void EatStew::Enter(Miner* pMiner)
{
  cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "Smells Reaaal goood Elsa!";
}

void EatStew::Execute(Miner* pMiner)
{
  cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "Tastes real good too!";

  pMiner->GetFSM()->RevertToPreviousState();
}

void EatStew::Exit(Miner* pMiner)
{ 
  cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "Thankya li'lle lady. Ah better get back to whatever ah wuz doin'";
}


bool EatStew::OnMessage(Miner* pMiner, const Telegram& msg)
{
  //send msg to global message handler
  return false;
}



//------------------------------


FightWithDan* FightWithDan::Instance()
{
	static FightWithDan instance;

	return &instance;
}


void FightWithDan::Enter(Miner* pMiner)
{
  //Bob starts a fight with Dan
	SetTextColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
	cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": Gotta teach da' punk it's lesson!";
	cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": Fighting -> " << pMiner->getKO() << " HP";
}

void FightWithDan::Execute(Miner* pMiner)
{
	//Bob loses HP when he fights, so if he is KO he will return to home
  //He can hit Dan and deals 1 or 2HP damage to Dan, depends on his luck
	if (pMiner->isKO()) {
		pMiner->GetFSM()->ChangeState(GoHomeAndSleepTilRested::Instance());
		Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY, //time delay
			pMiner->ID(),        //ID of sender
			ent_Dan,            //ID of recipient
			Msg_BobIsKO,   //the message
			NO_ADDITIONAL_INFO);
		return;
	}

	//cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "Bob : " << pMiner->getKO()<<" HP";
	if (RandFloat() <= 0.6) {
		Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY, //time delay
			pMiner->ID(),        //ID of sender
			ent_Dan,            //ID of recipient
			Msg_BobHitsDanBruise,   //the message
			NO_ADDITIONAL_INFO);
		SetTextColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
		cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "Hits Dan for 1";
	}
	else {
		Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY, //time delay
			pMiner->ID(),        //ID of sender
			ent_Dan,            //ID of recipient
			Msg_BobHitsDan,   //the message
			NO_ADDITIONAL_INFO);
		SetTextColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
		cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "Hits Dan for 2";
	}
}

void FightWithDan::Exit(Miner* pMiner)
{
	SetTextColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
	cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "Dam it was tough";
}


bool FightWithDan::OnMessage(Miner* pMiner, const Telegram& msg)
{
  //Dan also hit Bob, he sends telegram so Bob can know how many HP he losed
	switch (msg.Msg) {
  	case Msg_DanHitsBobBruise:
  		pMiner->DecreaseKO(pMiner->bruise);
  		pMiner->checkKO();
  		return true;

  	case Msg_DanHitsBob:
  		pMiner->DecreaseKO(pMiner->hit);
  		pMiner->checkKO();
  		return true;

  	case Msg_DanHitsBobCritical:
  		pMiner->DecreaseKO(pMiner->critical);
  		pMiner->checkKO();
  		return true;
  	case Msg_DanIsKO:
  		pMiner->GetFSM()->ChangeState(QuenchThirst::Instance());
  		return true;
	}
	return false;
}


