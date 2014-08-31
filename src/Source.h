//
//  Source.h
//  fantasybit
//
//  Created by Jay Berg on 3/22/14.
//
//

#ifndef __fantasybit__Source__
#define __fantasybit__Source__

#include <unordered_map>
#include "fbutils.h"
#include "FantasyName.h"
#include <map> 

namespace fantasybit
{

class Source
{
    Uid event;
	alias_t agent;
	std::unordered_map<Uid, Int> results{}; //player,points
	std::unordered_map<Uid, std::unordered_map<alias_t,Int>> projections{};

public:
	Source(const Uid &uid) : event(uid) {}
    Uid eventId() const { return event; }
 
	//projections
	decltype(projections) & getProjections() { return projections; }
	void addProjection(const alias_t &alias, const Uid &player, Int proj) 
	{
		projections[player][alias] = proj;
	}

	//results
	decltype(results) & getResults() { return results; }
	void addResult(const Uid &player, Int res) {  results[player] = res; }

	//agent
	void setDataAgent(const alias_t &agent) { this->agent = agent; }
	std::string dataAgent() const { return agent; }

	static std::unordered_map<Uid, Source> EventProjectionResult;
	static std::map < std::string, std::map<std::string, Uid>> WeeklySchedule;
};



}
#endif /* defined(__fantasybit__Source__) */


//auto np = Commissioner::getName(alias);
//if (np == nullptr) return;

//todo check for duplicate projections