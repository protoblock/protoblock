//
//  Source.cpp
//  fantasybit
//
//  Created by Jay Berg on 3/22/14.
//
//

#include "Source.h"

namespace fantasybit
{

decltype(Source::EventProjectionResult) Source::EventProjectionResult{};

decltype(Source::WeeklySchedule) Source::WeeklySchedule
{ 
	{ "1", { { "SEA", "201410130" }, { "GB", "201410130" }, { "ATL", "201410102" }, { "NO", "201410102" } } },
	{ "2", { { "SEA", "201410130" }, { "GB", "201410130" }, { "ATL", "201410102" }, { "NO", "201410102" } } },
	{ "3", { { "SEA", "201410130" }, { "GB", "201410130" }, { "ATL", "201410102" }, { "NO", "201410102" } } },
	{ "4", { { "SEA", "201410130" }, { "GB", "201410130" }, { "ATL", "201410102" }, { "NO", "201410102" } } },
	{ "5", { { "SEA", "201410130" }, { "GB", "201410130" }, { "ATL", "201410102" }, { "NO", "201410102" } } }
};


decltype(Source::PlayerTeam) Source::PlayerTeam{};
decltype(Source::TeamPlayers) Source::TeamPlayers{};
decltype(Source::TeamIds) Source::TeamIds{};

decltype(FantasyProjections::Projections) FantasyProjections::Projections{};

}
