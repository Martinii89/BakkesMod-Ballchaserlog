#include "pch.h"
#include "Ballchasinglog.h"

using namespace std::placeholders;

BAKKESMOD_PLUGIN(Ballchasinglog, "Ballchasinglog", plugin_version, PLUGINTYPE_FREEPLAY)


void Ballchasinglog::onLoad()
{
	api = std::make_shared<BallchasingAPI>(cvarManager, gameWrapper);
	cvarManager->registerNotifier("ballchasing_ping", std::bind(&BallchasingAPI::Ping, api), "test", 0);
	cvarManager->registerNotifier("ballchasing_groups", std::bind(&BallchasingAPI::GetTopLevelGroups, api), "test", 0);
	//cvarManager->registerNotifier("mymatches", std::bind(&BallchasingAPI::GetLastMatches, api), "test", 0);
	//cvarManager->registerNotifier("replay_detail", std::bind(&BallchasingAPI::GetReplayDetails, api), "test", 0);
}

void Ballchasinglog::onUnload()
{
}