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

#include "messaging/Telegram.h"

//At this value, game entities pass out
const int KOThreshold = 0;

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
  //m_iKO is Dans's HP
	int m_iKO;

  //MaxHP is the max HP that Dan can have
	int MaxHP;

public:

  BaseGameEntity(int id)
  {
    SetID(id);
  }
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

  //Check if the entity is full HP
  bool needToRecoverFromKO() {
	  if (m_iKO < this->MaxHP) {
		  return true;
	  }
	  return false;
  }

  //Check if the entity is KO (0HP)
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

  //return HP of the entity
  int getKO() {
	  return this->m_iKO;
  }
};



#endif


