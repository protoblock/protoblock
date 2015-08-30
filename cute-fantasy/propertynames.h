#ifndef PROPERTYNAMES_H
#define PROPERTYNAMES_H

#include <QMetaType>
//modified work of http://stackoverflow.com/questions/21456262/enum-to-string-in-c11

//#define ENUMITEM(Name) \
//struct Name {\
//static constexpr const char* name = #Name;\
//};

#define ENUMITEM(Name) \
    struct Name {\
    static const char* name() { return #Name;} \
    };


namespace PropertyNames {

    ENUMITEM(Game_ID)
    ENUMITEM(Game_Time)
    ENUMITEM(Home)
    ENUMITEM(Away)
    ENUMITEM(Kickoff_Time)
    ENUMITEM(Fantasy_Name)
    ENUMITEM(Balance)
    ENUMITEM(Player_ID)
    ENUMITEM(Player_Name)
    ENUMITEM(Position)
    ENUMITEM(Team_ID)
    ENUMITEM(Game_Status)
    ENUMITEM(Player_Status)
    ENUMITEM(Player_Game_Status)
    ENUMITEM(Projection)
    ENUMITEM(ProjectionStatus)
    ENUMITEM(KnownProjection)
    ENUMITEM(Result)

    template<typename Name>
    static const char * naturalName() {
        //QString result= QString(Name::name()).replace('_',' ');
        //return result.toLatin1().constData();
        return Name::name();
    }
}

#include "Data.h"
Q_DECLARE_METATYPE(fantasybit::GameStatus_Status)
Q_DECLARE_METATYPE(fantasybit::PlayerStatus_Status)
Q_DECLARE_METATYPE(fantasybit::PlayerGameStatus)


#endif // PROPERTYNAMES_H
