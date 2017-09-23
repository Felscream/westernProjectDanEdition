<<<<<<< HEAD
#include "MinersWifeOwnedStates.h"
#include "MinerOwnedStates.h"
#include "MinersWife.h"
#include "Locations.h"
#include "Time/CrudeTimer.h"
#include "MessageDispatcher.h"
#include "MessageTypes.h"
#include "EntityNames.h"

#include <iostream>
using std::cout;

#ifdef TEXTOUTPUT
#include <fstream>
extern std::ofstream os;
#define cout os
#endif

//-----------------------------------------------------------------------Global state

WifesGlobalState* WifesGlobalState::Instance()
{
  static WifesGlobalState instance;

  return &instance;
}


void WifesGlobalState::Execute(MinersWife* wife)
{
  //1 in 10 chance of needing the bathroom (provided she is not already
  //in the bathroom)
  if ( (RandFloat() < 0.1) && 
       !wife->GetFSM()->isInState(*VisitBathroom::Instance()) )
  {
    wife->GetFSM()->ChangeState(VisitBathroom::Instance());
  }
}

bool WifesGlobalState::OnMessage(MinersWife* wife, const Telegram& msg)
{
  

  switch(msg.Msg)
  {
  case Msg_HiHoneyImHome:
   {
       /*cout << "\nMessage handled by " << GetNameOfEntity(wife->ID()) << " at time: " 
       << Clock->GetCurrentTime();*/

	 wife->sharedPrint(GetNameOfEntity(wife->ID()), (string) "Hi honey. Let me make you some of mah fine country stew", FOREGROUND_GREEN | FOREGROUND_INTENSITY);

     wife->GetFSM()->ChangeState(CookStew::Instance());
   }

   return true;

  }//end switch

  return false;
}

//-------------------------------------------------------------------------DoHouseWork

DoHouseWork* DoHouseWork::Instance()
{
  static DoHouseWork instance;

  return &instance;
}


void DoHouseWork::Enter(MinersWife* wife)
{
  wife->sharedPrint(GetNameOfEntity(wife->ID()), (string) "Time to do some more housework!", FOREGROUND_GREEN | FOREGROUND_INTENSITY);
}


void DoHouseWork::Execute(MinersWife* wife)
{
  switch(RandInt(0,2))
  {
  case 0:

	wife->sharedPrint(GetNameOfEntity(wife->ID()), (string) "Moppin' the floor", FOREGROUND_GREEN | FOREGROUND_INTENSITY);

    break;

  case 1:

	wife->sharedPrint(GetNameOfEntity(wife->ID()), (string) "Washin' the dishes", FOREGROUND_GREEN | FOREGROUND_INTENSITY);

    break;

  case 2:

	wife->sharedPrint(GetNameOfEntity(wife->ID()), (string) "Makin' the bed", FOREGROUND_GREEN | FOREGROUND_INTENSITY);

    break;
  }
}

void DoHouseWork::Exit(MinersWife* wife)
{
}

bool DoHouseWork::OnMessage(MinersWife* wife, const Telegram& msg)
{
  return false;
}

//------------------------------------------------------------------------VisitBathroom

VisitBathroom* VisitBathroom::Instance()
{
  static VisitBathroom instance;

  return &instance;
}


void VisitBathroom::Enter(MinersWife* wife)
{  
  wife->sharedPrint(GetNameOfEntity(wife->ID()), (string) "Walkin' to the can. Need to powda mah pretty li'lle nose", FOREGROUND_GREEN | FOREGROUND_INTENSITY);
}


void VisitBathroom::Execute(MinersWife* wife)
{
  wife->sharedPrint(GetNameOfEntity(wife->ID()), (string) "Ahhhhhh! Sweet relief!", FOREGROUND_GREEN | FOREGROUND_INTENSITY);

  wife->GetFSM()->RevertToPreviousState();
}

void VisitBathroom::Exit(MinersWife* wife)
{
  wife->sharedPrint(GetNameOfEntity(wife->ID()), (string) "Leavin' the Jon", FOREGROUND_GREEN | FOREGROUND_INTENSITY);
}


bool VisitBathroom::OnMessage(MinersWife* wife, const Telegram& msg)
{
  return false;
}


//------------------------------------------------------------------------CookStew

CookStew* CookStew::Instance()
{
  static CookStew instance;

  return &instance;
}


void CookStew::Enter(MinersWife* wife)
{
  //if not already cooking put the stew in the oven
  if (!wife->Cooking())
  {
	wife->sharedPrint(GetNameOfEntity(wife->ID()), (string) "Putting the stew in the oven", FOREGROUND_GREEN | FOREGROUND_INTENSITY);
  
    //send a delayed message myself so that I know when to take the stew
    //out of the oven
	wife->sharedPrintTelegram(wife->ID(), wife->ID(), Msg_StewReady);
    //Dispatch->DispatchMessage(1.5,                  //time delay
    //                          wife->ID(),           //sender ID
    //                          wife->ID(),           //receiver ID
    //                          Msg_StewReady,        //msg
    //                          NO_ADDITIONAL_INFO); 

    wife->SetCooking(true);
  }
}


void CookStew::Execute(MinersWife* wife)
{
  wife->sharedPrint(GetNameOfEntity(wife->ID()), (string) "Fussin' over food", FOREGROUND_GREEN | FOREGROUND_INTENSITY);
}

void CookStew::Exit(MinersWife* wife)
{
  wife->sharedPrint(GetNameOfEntity(wife->ID()), (string) "Puttin' the stew on the table", FOREGROUND_GREEN | FOREGROUND_INTENSITY);
}


bool CookStew::OnMessage(MinersWife* wife, const Telegram& msg)
{
 

  switch(msg.Msg)
  {
    case Msg_StewReady:
    {
      /*cout << "\nMessage received by " << GetNameOfEntity(wife->ID()) <<
           " at time: " << Clock->GetCurrentTime();*/
	  wife->sharedPrint(GetNameOfEntity(wife->ID()), (string) "StewReady! Lets eat", FOREGROUND_GREEN | FOREGROUND_INTENSITY);

      //let hubby know the stew is ready
	  wife->sharedPrintTelegram(wife->ID(), ent_Miner_Bob, Msg_StewReady);
      /*Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY,
                                wife->ID(),
                                ent_Miner_Bob,
                                Msg_StewReady,
                                NO_ADDITIONAL_INFO);
*/
      wife->SetCooking(false);

      wife->GetFSM()->ChangeState(DoHouseWork::Instance());               
    }

    return true;

  }//end switch

  return false;

}