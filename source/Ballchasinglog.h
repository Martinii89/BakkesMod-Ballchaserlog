#pragma once

#include "bakkesmod/plugin/bakkesmodplugin.h"
#include "bakkesmod/plugin/pluginwindow.h"
#include "ballchasing/BallchasingAPI.h"
#include "GUISettings.h"

constexpr auto plugin_version = "1.0";

class Ballchasinglog: public BakkesMod::Plugin::BakkesModPlugin, public BakkesMod::Plugin::PluginWindow
{
	//Boilerplate
	virtual void onLoad();
	virtual void onUnload();

	std::shared_ptr<BallchasingAPI> api;

	// Inherited via PluginWindow
	

	bool isWindowOpen_ = false;
	bool isMinimized_ = false;
	std::string menuTitle_ = "Ballchasing.com - Log";
	GUISettings guiSettings;

	void RenderGroup(GroupData* group, GroupData** selectedGroup);

	virtual void Render() override;
	virtual std::string GetMenuName() override;
	virtual std::string GetMenuTitle() override;
	virtual void SetImGuiContext(uintptr_t ctx) override;
	virtual bool ShouldBlockInput() override;
	virtual bool IsActiveOverlay() override;
	virtual void OnOpen() override;
	virtual void OnClose() override;

	void OnReplayGroupChange(std::string link);
	void RenderReplayDetail(ReplayData* detail);
	void RenderGroupDetail(GroupData* group);

	void ContextMenu(std::vector<TableColumn>& columnData);
	bool ContextMenu(ReplayData* detail, GroupData* parentGroup);
	void RenderTableTab(std::string name, TableSettings& settings, ReplayData* detail, bool blueHeader = true, bool orangeHeader = true );
	void RenderGroupTab(std::string name, TableSettings& settings, GroupData* detail);
	void RenderTeamTable(Team& t, TableSettings& settings, bool drawHeader = true);
	void RenderPlayerRow(BaseStatPlayer& p, TableSettings& settings);
	
	
};

