#include "pch.h"
#include "Ballchasinglog.h"


BAKKESMOD_PLUGIN(Ballchasinglog, "Examine Ballchasing stats without leave the game", plugin_version, PLUGINTYPE_FREEPLAY)


void Ballchasinglog::onLoad()
{
	api = std::make_shared<BallchasingAPI>(cvarManager, gameWrapper);
	cvarManager->registerNotifier("ballchasing_ping", std::bind(&BallchasingAPI::Ping, api), "test", 0);
	cvarManager->registerNotifier("ballchasing_groups", std::bind(&BallchasingAPI::GetToplevelGroups, api), "test", 0);
	cvarManager->registerNotifier("ballchasing_group_stats", std::bind(&BallchasingAPI::GetGroupStats, api), "test", 0);
	//cvarManager->registerNotifier("mymatches", std::bind(&BallchasingAPI::GetLastMatches, api), "test", 0);
	//cvarManager->registerNotifier("replay_detail", std::bind(&BallchasingAPI::GetReplayDetails, api), "test", 0);
}

void Ballchasinglog::onUnload()
{
}