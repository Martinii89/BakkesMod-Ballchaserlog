#pragma once

#include "bakkesmod/plugin/bakkesmodplugin.h"
#include "bakkesmod/plugin/pluginwindow.h"
#include "ballchasing/BallchasingAPI.h"

constexpr auto plugin_version = "1.0";

class Ballchaserlog: public BakkesMod::Plugin::BakkesModPlugin, public BakkesMod::Plugin::PluginWindow
{

	//Boilerplate
	virtual void onLoad();
	virtual void onUnload();

	std::shared_ptr<BallchasingAPI> api;

	// Inherited via PluginWindow
	

	bool isWindowOpen_ = false;
	bool isMinimized_ = false;
	std::string menuTitle_ = "Ballchasing.com - Log";

	virtual void Render() override;
	virtual std::string GetMenuName() override;
	virtual std::string GetMenuTitle() override;
	virtual void SetImGuiContext(uintptr_t ctx) override;
	virtual bool ShouldBlockInput() override;
	virtual bool IsActiveOverlay() override;
	virtual void OnOpen() override;
	virtual void OnClose() override;

	void RenderReplayDetail(GetReplayResponseData* detail);
	
	
};

