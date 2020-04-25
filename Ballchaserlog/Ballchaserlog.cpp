#include "pch.h"
#include "Ballchaserlog.h"


BAKKESMOD_PLUGIN(Ballchaserlog, "write a plugin description here", plugin_version, PLUGINTYPE_FREEPLAY)


void Ballchaserlog::onLoad()
{
	api = std::make_shared<BallchasingAPI>(cvarManager);
	cvarManager->log("Plugin loaded!");
	cvarManager->registerNotifier("ping", std::bind(&BallchasingAPI::Ping, api), "test", 0);
	cvarManager->registerNotifier("mymatches", std::bind(&BallchasingAPI::GetLastMatches, api), "test", 0);
	//cvarManager->registerNotifier("replay_detail", std::bind(&BallchasingAPI::GetReplayDetails, api), "test", 0);
}

void Ballchaserlog::onUnload()
{
}