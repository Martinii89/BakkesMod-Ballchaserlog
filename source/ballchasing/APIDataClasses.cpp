#include "pch.h"
//#include "../json/json.hpp"
//#include "APIDataClasses.h"
//
//#define JOPTIONAL(var) if (j.find(#var) != j.end()){j.at(#var).get_to(p.var);}
//#define JOPTIONAL2(var, var2) if (j.find(#var) != j.end()){j.at(#var).get_to(p.var2);}
//#define J(var) j.at(#var).get_to(p.var);
//#define J2(var, var2) j.at(#var).get_to(p.var2);
//
//
//void from_json(const nlohmann::json & j, GetReplayGroupsResponseData & p) {
//	JOPTIONAL(next)
//	JOPTIONAL(list)
//}
//
//void from_json(const nlohmann::json& j, ReplayData& p) {
//	JOPTIONAL(replay_title)
//	JOPTIONAL2(title, replay_title) // done
//	JOPTIONAL(status)
//	JOPTIONAL(id)
//	JOPTIONAL(link)
//	JOPTIONAL(blue)
//	JOPTIONAL(orange)
//}
//
//void from_json(const nlohmann::json& j, GetReplaysResponse& p) {
//	JOPTIONAL(count)
//	JOPTIONAL2(list, replays) // done
//}
//
//
////void from_json(const nlohmann::json& j, Team::Player& p)
////{
////	JOPTIONAL(name)
////	JOPTIONAL(score)
////	JOPTIONAL(id)
////	JOPTIONAL(stats)
////	JOPTIONAL(camera)
////}
////
////void from_json(const nlohmann::json& j, Team::Player::Id& p)
////{
////	JOPTIONAL(id)
////	JOPTIONAL(platform)
////}
//
//void from_json(const nlohmann::json& j, GroupData& p)
//{
//	JOPTIONAL(id)
//	JOPTIONAL(link)
//	JOPTIONAL(name)
//	JOPTIONAL(created)
//	JOPTIONAL(status)
//	JOPTIONAL(shared)
//	JOPTIONAL(players)
//}
//
//void from_json(const nlohmann::json& j, GroupPlayer& p)
//{
//	JOPTIONAL(platform)
//	JOPTIONAL(id)
//	JOPTIONAL(name)
//	JOPTIONAL(team)
//	JOPTIONAL2(cumulative, cumulative_stats) //done
//	JOPTIONAL2(game_average, stats) // done
//}
//
//
//
//#undef J
//#undef JOPTIONAL
