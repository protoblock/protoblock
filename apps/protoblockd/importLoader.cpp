#include "importLoader.h"

namespace  pb {


#define IEITEM(Name) {"Name"},

decltype(ImportLoader::headers) ImportLoader::headers{
    IEITEM(ProtoblockPlayerID)
    IEITEM(Player_Name)
    IEITEM(Position)
    IEITEM(TeamID)
    IEITEM(MyProjection)
    IEITEM(PassTD)
    IEITEM(PassYd)
    IEITEM(RushTD)
    IEITEM(RushYd)
    IEITEM(RecTD)
    IEITEM(RecYd)
    IEITEM(Rec)
    IEITEM(Int)
    IEITEM(Fum)
    IEITEM(_2Pt)
    IEITEM(PAT)
    IEITEM(FG19)
    IEITEM(FG29)
    IEITEM(FG39)
    IEITEM(FG49)
    IEITEM(FG59)
    IEITEM(D_TD)
    IEITEM(Sack)
    IEITEM(TO)
    IEITEM(SFTY)
    IEITEM(D2pt)
    IEITEM(D1pt)
    IEITEM(PtsA)
};

}

