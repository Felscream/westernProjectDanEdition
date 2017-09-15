#ifndef MESSAGE_TYPES
#define MESSAGE_TYPES

#include <string>

enum message_type
{
	Msg_HiHoneyImHome,
	Msg_StewReady,
	Msg_ImInTheSaloon,
	Msg_ImLeavingTheSaloon,
	Msg_Fight,
	Msg_DanHitsBobBruise,
	Msg_DanHitsBob,
	Msg_DanHitsBobCritical,
	Msg_BobHitsDanBruise,
	Msg_BobHitsDan,
	Msg_BobIsKO,
	Msg_DanIsKO,
};


inline std::string MsgToStr(int msg)
{
  switch (msg)
  {
  case Msg_HiHoneyImHome:
    
    return "HiHoneyImHome"; 

  case Msg_StewReady:
    
    return "StewReady";

  case Msg_ImInTheSaloon:
	  return "ImInTheSaloon";

  case Msg_ImLeavingTheSaloon:
	  return "ImLeavingTheSaloon";

  case Msg_Fight:
	  return "Fight";

  case Msg_DanHitsBobBruise:
	  return "DanHitsBobBruise";

  case Msg_DanHitsBob:
	  return "DanHitsBob";

  case Msg_DanHitsBobCritical:
	  return "DanHitsBobCritical";

  case Msg_BobHitsDanBruise:
	  return "BobHitsDanBruise";

  case Msg_BobHitsDan:
	  return "BobHitsDan";

  case Msg_BobIsKO:
	  return "BobisKO";

  case Msg_DanIsKO:
	  return "DanisKO";

  default:
    return "Not recognized!";
  }
}

#endif