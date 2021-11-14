#pragma once

#include "bakkesmod/plugin/bakkesmodplugin.h"
#include "bakkesmod/plugin/pluginwindow.h"
#include "bakkesmod/plugin/PluginSettingsWindow.h"
#include "ballchasing/BallchasingAPI.h"
#include "GUISettings.h"


#include "version.h"
constexpr auto plugin_version = stringify(VERSION_MAJOR) "." stringify(VERSION_MINOR) "." stringify(VERSION_PATCH) "." stringify(VERSION_BUILD);

struct VM;

class Ballchasinglog: public BakkesMod::Plugin::BakkesModPlugin, public BakkesMod::Plugin::PluginWindow
{

#pragma region Boilerplate
	//Boilerplate
	void onLoad() override;
	void onUnload() override;

	

	bool isWindowOpen_ = false;
	bool isMinimized_ = false;
	std::string menu_title_ = "Ballchasing.com - Log (" + std::string(stringify(VERSION_MAJOR) "." stringify(VERSION_MINOR)) + ")";

	void Render() override;
	std::string GetMenuName() override;
	std::string GetMenuTitle() override;
	void SetImGuiContext(uintptr_t ctx) override;
	bool ShouldBlockInput() override;
	bool IsActiveOverlay() override;
	void OnOpen() override;
	void OnClose() override;
#pragma endregion

	std::shared_ptr<BallchasingAPI> api_;
	GUISettings gui_settings_;

	void RenderGroup(GroupData* group, GroupData** selectedGroup);
	bool RenderGroupSelector(GroupData* group, GroupData** selectedGroup);

	void OnReplayGroupChange(std::string link);
	void RenderReplayDetail(ReplayData* detail);
	void RenderGroupDetail(GroupData* group);
	void AssignReplayPopup(VM* vm);
	bool GroupSelector(std::string* group);
	void CreateGroupPopup();
	void ContextMenu(std::vector<TableColumn>& columnData);
	bool ContextMenu(ReplayData* detail, GroupData* parentGroup);
	void RenderTableTab(std::string name, TableSettings& settings, ReplayData* detail, bool blueHeader = true, bool orangeHeader = true );
	void RenderGroupTab(std::string name, TableSettings& settings, GroupData* detail);
	void RenderTeamTable(Team& t, TableSettings& settings, bool drawHeader = true);
	void RenderPlayerRow(BaseStatPlayer& p, TableSettings& settings);
};

