#include <fstream>
#include <time.h>

#include "Locations.h"
#include "Miner.h"
#include "MinersWife.h"
#include "Drunkard.h"
#include "EntityManager.h"
#include "MessageDispatcher.h"
#include "misc/ConsoleUtils.h"
#include "EntityNames.h"


std::ofstream os;

int main()
{
//define this to send output to a text file (see locations.h)
#ifdef TEXTOUTPUT
  os.open("output.txt");
#endif

  //seed random number generator
  srand((unsigned) time(NULL));

  //create a miner
  Miner* Bob = new Miner(ent_Miner_Bob);

  //create his wife
  MinersWife* Elsa = new MinersWife(ent_Elsa);

  //Create Dan the drunkard
  Drunkard* Dan = new Drunkard(ent_Dan);

  //register them with the entity manager
  EntityMgr->RegisterEntity(Bob);
  EntityMgr->RegisterEntity(Elsa);
  EntityMgr->RegisterEntity(Dan);

  //run Bob and Elsa through a few Update calls
  for (int i=0; i<50; ++i)
  { 
	Dan->Update();
    Bob->Update();
    Elsa->Update();

    //dispatch any delayed messages
    Dispatch->DispatchDelayedMessages();

    Sleep(100);
  }

  //tidy up
  delete Bob;
  delete Elsa;
  delete Dan;

  //wait for a keypress before exiting
  PressAnyKeyToContinue();


  return 0;
}






