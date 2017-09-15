#ifndef MESSAGE_TYPES
#define MESSAGE_TYPES

#include <string>

enum message_type
{
	Msg_HiHoneyImHome,
	Msg_StewReady,
	Msg_ImInTheSaloon,
	Msg_ImLeavingTheSaloon,
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

  default:

    return "Not recognized!";
  }
}

#endif