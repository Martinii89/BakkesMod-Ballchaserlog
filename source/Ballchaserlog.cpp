#include "pch.h"
#include "Ballchaserlog.h"


BAKKESMOD_PLUGIN(Ballchaserlog, "Examine Ballchasing stats without leave the game", plugin_version, PLUGINTYPE_FREEPLAY)


void Ballchaserlog::onLoad()
{
	api = std::make_shared<BallchasingAPI>(cvarManager, gameWrapper);
	cvarManager->registerNotifier("ballchasing_ping", std::bind(&BallchasingAPI::Ping, api), "test", 0);
	//cvarManager->registerNotifier("mymatches", std::bind(&BallchasingAPI::GetLastMatches, api), "test", 0);
	//cvarManager->registerNotifier("replay_detail", std::bind(&BallchasingAPI::GetReplayDetails, api), "test", 0);
}

void Ballchaserlog::onUnload()
{
}