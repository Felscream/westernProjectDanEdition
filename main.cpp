#include <fstream>
#include <time.h>
#include <thread>
#include "Locations.h"
#include "Miner.h"
#include "MinersWife.h"
#include "Drunkard.h"
#include "EntityManager.h"
#include "MessageDispatcher.h"
#include "misc/ConsoleUtils.h"
#include "EntityNames.h"
#define LOOP 20
using namespace std;


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

	thread danThread(&Drunkard::Update,Dan,LOOP);
	thread bobThread(&Miner::Update,Bob, LOOP);
	thread elsaThread(&MinersWife::Update, Elsa, LOOP);
	
	danThread.join();
	cout << "\nDan a finit";
	bobThread.join();
	cout << "\nBob a finit";
	elsaThread.join();
	cout << "\nElsa a finit";


	//dispatch any delayed messages
	Dispatch->DispatchDelayedMessages();

  
  
  

  //tidy up
  delete Bob;
  delete Elsa;
  delete Dan;

  //wait for a keypress before exiting
  PressAnyKeyToContinue();


  return 0;
}






