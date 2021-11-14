#include "pch.h"
#include "Ballchasinglog.h"


BAKKESMOD_PLUGIN(Ballchasinglog, "Ballchasinglog", plugin_version, PLUGINTYPE_FREEPLAY)

std::shared_ptr<CVarManagerWrapper> _globalCvarManager;

void Ballchasinglog::onLoad()
{
	_globalCvarManager = cvarManager;
	api_ = std::make_shared<BallchasingAPI>(cvarManager, gameWrapper);
	cvarManager->registerNotifier("ballchasing_ping", std::bind(&BallchasingAPI::Ping, api_), "test", 0);
	cvarManager->registerNotifier("ballchasing_groups", std::bind(&BallchasingAPI::GetTopLevelGroups, api_), "test", 0);
}

void Ballchasinglog::onUnload()
{
}