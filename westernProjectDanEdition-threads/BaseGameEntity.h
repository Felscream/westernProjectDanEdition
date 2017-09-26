#ifndef ENTITY_H
#define ENTITY_H
//------------------------------------------------------------------------
//
//  Name:   BaseGameEntity.h
//
//  Desc:   Base class for a game object
//
//  Author: Mat Buckland 2002 (fup@ai-junkie.com)
//
//------------------------------------------------------------------------
#include <string>
#include <iostream>
#include <thread>
#include <mutex>
#include <shared_mutex>

#include "misc/ConsoleUtils.h"
#include "MessageDispatcher.h"
#include "MessageTypes.h"
using namespace std;

#include "messaging/Telegram.h"

//At this value, game entities pass out
const int KOThreshold = 0;

static std::shared_timed_mutex mu;

class BaseGameEntity
{

private:

  //every entity must have a unique identifying number
  int          m_ID;

  
	
  //this is the next valid ID. Each time a BaseGameEntity is instantiated
  //this value is updated
  static int  m_iNextValidID;

  //this must be called within the constructor to make sure the ID is set
  //correctly. It verifies that the value passed to the method is greater
  //or equal to the next valid ID, before setting the ID and incrementing
  //the next valid ID
  void SetID(int val);

protected:
	//the actual hp of an agent, when iKO == 0 or < 0 then the agent is considered KO and must rest
	int m_iKO;
	//the Hp max of an agent 
	int MaxHP;

public:

  BaseGameEntity(int id)
  {
    SetID(id);
  }
  //the 3 types of hit that Dan or Bob can do in a fight excpet that crit is reserved for Dan
  enum damageType :int { bruise = 1, hit = 2, critical = 4 };
  virtual ~BaseGameEntity(){}

  //all entities must implement an update function
  virtual void  Update()=0;

  //all entities can communicate using messages. They are sent
  //using the MessageDispatcher singleton class
  virtual bool  HandleMessage(const Telegram& msg)=0;

  int           ID()const{return m_ID;}  

  //Decrease HP lightly

  void DecreaseKO(damageType damage) {
	  this->m_iKO -= damage;
  }

  //Check if entity is down
  bool isKO() {
	if (m_iKO > 0) {
		return false;
	}
	return true;
  }

  //check if the IA still need healing when resting from KO state
  bool needToRecoverFromKO() {
	  if (m_iKO < this->MaxHP) {
		  return true;
	  }
	  return false;
  }

  //when the hp bar is < 0 after a hit set the hp bar to 0 to avoid too many resting turns
  void checkKO() {
	  if (this->m_iKO < 0) {
		  this->m_iKO = 0;
	  }
  }

  //Entity recovers from KO
  void recoverKO() {
	  this->m_iKO++;
	  if (m_iKO > this->MaxHP) {
		  m_iKO = MaxHP;
	  }
  }

  //return the tiredness of an agent
  int getKO() {
	  return this->m_iKO;
  }

  void printThreadId() {
	  auto th_id = this_thread::get_id();
	  cout << th_id << endl;
  }

  //shared print that the 3 IA are using by taking turns with a shared mutex to avoid text superposition. The color of the text is one of the parameters.
  void sharedPrint(string writer, string mes, WORD colors) {
	  std::unique_lock<std::shared_timed_mutex> lock(mu);
	  SetTextColor(colors);
	  cout << "\n" << writer << " : " << mes << endl;
  }

  //Shared print telegram to avoid that the telegrams are writed in the middle of other text by the use of a shared mutex
  void sharedPrintTelegram(int sender, int recipient, message_type message) {
	  std::shared_lock<std::shared_timed_mutex> lock(mu);
	  SetTextColor(BACKGROUND_RED | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	  Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY, //time delay
		  sender,        //ID of sender
		  recipient,            //ID of recipient
		  message,   //the message
		  NO_ADDITIONAL_INFO);
  }
};




#endif


