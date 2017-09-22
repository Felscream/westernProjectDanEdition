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
	pMiner->sharedPrint(GetNameOfEntity(pMiner->ID()), (string) "Walkin' to the goldmine", FOREGROUND_RED | FOREGROUND_INTENSITY);
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
  pMiner->sharedPrint(GetNameOfEntity(pMiner->ID()), (string) "Pickin' up a nugget", FOREGROUND_RED | FOREGROUND_INTENSITY);

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
	pMiner->sharedPrint(GetNameOfEntity(pMiner->ID()), (string) "Ah'm leavin' the goldmine with mah pockets full o' sweet gold", FOREGROUND_RED | FOREGROUND_INTENSITY);
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
	pMiner->sharedPrint(GetNameOfEntity(pMiner->ID()), (string) "Goin' to the bank. Yes siree", FOREGROUND_RED | FOREGROUND_INTENSITY);

    pMiner->ChangeLocation(bank);
  }
}


void VisitBankAndDepositGold::Execute(Miner* pMiner)
{
  //deposit the gold
  pMiner->AddToWealth(pMiner->GoldCarried());
    
  pMiner->SetGoldCarried(0);
  pMiner->sharedPrint(GetNameOfEntity(pMiner->ID()), (string) "Depositing gold. Total savings now: "+to_string( pMiner->Wealth()), FOREGROUND_RED | FOREGROUND_INTENSITY);

  //wealthy enough to have a well earned rest?
  if (pMiner->Wealth() >= ComfortLevel)
  {
	pMiner->sharedPrint(GetNameOfEntity(pMiner->ID()), (string) "WooHoo! Rich enough for now. Back home to mah li'lle lady", FOREGROUND_RED | FOREGROUND_INTENSITY);
      
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
  pMiner->sharedPrint(GetNameOfEntity(pMiner->ID()), (string) "Leavin' the bank", FOREGROUND_RED | FOREGROUND_INTENSITY);
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
	  pMiner->sharedPrint(GetNameOfEntity(pMiner->ID()), (string) "Walkin' home", FOREGROUND_RED | FOREGROUND_INTENSITY);

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
		pMiner->sharedPrint(GetNameOfEntity(pMiner->ID()), (string) "All mah fatigue has drained away. Time to find more gold!", FOREGROUND_RED | FOREGROUND_INTENSITY);

     pMiner->GetFSM()->ChangeState(EnterMineAndDigForNugget::Instance());
  }

  else 
  {
    //sleep
    pMiner->DecreaseFatigue();

	pMiner->sharedPrint(GetNameOfEntity(pMiner->ID()), (string) "ZZZZ...", FOREGROUND_RED | FOREGROUND_INTENSITY);
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
	pMiner->sharedPrint(GetNameOfEntity(pMiner->ID()), (string) "Boy, ah sure is thusty! Walking to the saloon", FOREGROUND_RED | FOREGROUND_INTENSITY);
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
  pMiner->sharedPrint(GetNameOfEntity(pMiner->ID()), (string) "That's mighty fine sippin' liquer", FOREGROUND_RED | FOREGROUND_INTENSITY);
  if (pMiner->Hydrated()) {
	  pMiner->sharedPrint(GetNameOfEntity(pMiner->ID()), (string) "Leaving the saloon, feelin' good", FOREGROUND_RED | FOREGROUND_INTENSITY);
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
	pMiner->sharedPrint(GetNameOfEntity(pMiner->ID()), (string) "Smells Reaaal goood Elsa!", FOREGROUND_RED | FOREGROUND_INTENSITY);
}

void EatStew::Execute(Miner* pMiner)
{
	pMiner->sharedPrint(GetNameOfEntity(pMiner->ID()), (string) "Tastes real good too!", FOREGROUND_RED | FOREGROUND_INTENSITY);

  pMiner->GetFSM()->RevertToPreviousState();
}

void EatStew::Exit(Miner* pMiner)
{ 
	pMiner->sharedPrint(GetNameOfEntity(pMiner->ID()), (string) "Thankya li'lle lady. Ah better get back to whatever ah wuz doin", FOREGROUND_RED | FOREGROUND_INTENSITY);
}


bool EatStew::OnMessage(Miner* pMiner, const Telegram& msg)
{
  //send msg to global message handler
  return false;
}

FightWithDan* FightWithDan::Instance()
{
	static FightWithDan instance;

	return &instance;
}


void FightWithDan::Enter(Miner* pMiner)
{
	SetTextColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
	pMiner->sharedPrint(GetNameOfEntity(pMiner->ID()), (string) "Gotta teach da' punk it's lesson!", FOREGROUND_RED | FOREGROUND_INTENSITY);
	pMiner->sharedPrint(GetNameOfEntity(pMiner->ID()), (string) "Fighting -> "+to_string(pMiner->getKO())+" HP", FOREGROUND_RED | FOREGROUND_INTENSITY);

}

void FightWithDan::Execute(Miner* pMiner)
{
	
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
		pMiner->sharedPrint(GetNameOfEntity(pMiner->ID()), (string) "Hits Dan for 1", FOREGROUND_RED | FOREGROUND_INTENSITY);
	}
	else {
		Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY, //time delay
			pMiner->ID(),        //ID of sender
			ent_Dan,            //ID of recipient
			Msg_BobHitsDan,   //the message
			NO_ADDITIONAL_INFO);
		SetTextColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
		pMiner->sharedPrint(GetNameOfEntity(pMiner->ID()), (string) "Hits Dan for 2", FOREGROUND_RED | FOREGROUND_INTENSITY);
	}
}

void FightWithDan::Exit(Miner* pMiner)
{
	SetTextColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
	pMiner->sharedPrint(GetNameOfEntity(pMiner->ID()), (string) "Dam it was tough", FOREGROUND_RED | FOREGROUND_INTENSITY);
}


bool FightWithDan::OnMessage(Miner* pMiner, const Telegram& msg)
{
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


