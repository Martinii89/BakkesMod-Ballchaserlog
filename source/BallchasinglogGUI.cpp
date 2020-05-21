#include "pch.h"
#include "Ballchasinglog.h"
#include "IMGUI/imgui_internal.h"


std::string joinPlayers(Team t)
{
	std::ostringstream ss;
	bool first = true;
	for (const auto& p : t.players)
	{
		if (!first) {
			ss << ", ";
		}
		ss << p.name;
		first = false;
	}
	return ss.str();
}

void Ballchasinglog::RenderGroup(GroupData* group, GroupData** selectedGroup) {
	if (group == nullptr) return;
	static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
	ImGuiTreeNodeFlags node_flags = base_flags;
	if (group == *selectedGroup)
		node_flags |= ImGuiTreeNodeFlags_Selected;
	bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)group, node_flags, group->name.c_str());
	if (ImGui::IsItemClicked()) {
		*selectedGroup = group;
		OnReplayGroupChange(group->id);
		api->GetSubGroups(group->id);
		api->GetGroupStats(group->id);
	}
	if (ImGui::IsItemHovered()) {
		ImGui::BeginTooltip();
		ImGui::Text("Created: %s", group->created.c_str());
		ImGui::Text("%s", group->shared ? "Public" : "Private");
		ImGui::Text("Status: %s", group->status.c_str());
		ImGui::EndTooltip();
	}
	if (node_open)
	{
		for (auto subgroupID : group->subgroups) {
			auto pSubGroup = api->GetCachedGroup(subgroupID);
			RenderGroup(pSubGroup, selectedGroup);
		}
		ImGui::TreePop();
	}
}

// Do ImGui rendering here
void Ballchasinglog::Render()
{
	const std::string LATEST_REPLAYS = "LATEST";
	const std::string DEFAULT_GROUP_NAME = "Your latest replays";
	if (!ImGui::Begin(menuTitle_.c_str(), &isWindowOpen_, ImGuiWindowFlags_None))
	{
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}
	//ImGui::ShowDemoWindow();

	ImGui::Columns(2);
	static bool replayListCollapsed = false;
	static float uncollapseWidth = 0;
	static std::string detailID = "";
	static std::string selectedGroupID = LATEST_REPLAYS;
	//static std::string groupName = DEFAULT_GROUP_NAME;

	GroupData* selectedGroup = api->GetCachedGroup(selectedGroupID);

	if (!replayListCollapsed)
	{
		ImGui::Text("Your replay groups"); ImGui::SameLine(ImGui::GetColumnWidth(0) - 25);
		if (ImGui::ArrowButton(">>", ImGuiDir_Left)) {
			replayListCollapsed = true;
			uncollapseWidth = ImGui::GetColumnWidth(0);
			ImGui::SetColumnWidth(0, 30);
		}
		ImGui::BeginChild("ReplayGroupList", ImVec2(0, ImGui::GetWindowHeight() * 0.5f - 40), true);

		for (auto& groupID : api->topLevelGroups)
		{
			GroupData* group = api->GetCachedGroup(groupID);
			RenderGroup(group, &selectedGroup);

			if (selectedGroup != nullptr) {
				selectedGroupID = selectedGroup->id;
			}
		}
		ImGui::EndChild();
		std::string groupName = (selectedGroup != nullptr) ? selectedGroup->name : "";

		ImGui::Text(groupName.c_str());

		ImGui::BeginChild("ReplayList", ImVec2(0, ImGui::GetWindowHeight() * 0.5f - 40), true);
		if (selectedGroup != nullptr)
		{
			static ReplayData* rightClickedReplay = nullptr;
			for (auto& replay : selectedGroup->groupReplays)
			{
				std::string list_lbl = replay.replay_title + "##replaylist";
				if (ImGui::Selectable(list_lbl.c_str(), replay.id == detailID || &replay == rightClickedReplay))
				{
					detailID = replay.id;
				}
				if (ImGui::IsItemHovered()) {
					ImGui::BeginTooltip();
					std::string blueTeam = joinPlayers(replay.blue);
					std::string orangeTeam = joinPlayers(replay.orange);
					ImGui::Text("%i\t: %s", replay.blue.goals, blueTeam.c_str());
					ImGui::Text("%i\t: %s", replay.orange.goals, orangeTeam.c_str());
					ImGui::EndTooltip();
				}

				if (&replay == rightClickedReplay) { rightClickedReplay = nullptr; }

				if (ContextMenu(&replay, selectedGroup)) {
					rightClickedReplay = &replay;
				}
			}
		}
		ImGui::EndChild();
	}
	else {
		if (ImGui::ArrowButton(">>", ImGuiDir_Right)) {
			replayListCollapsed = false;
			ImGui::SetColumnWidth(0, uncollapseWidth);
		}
	}

	ImGui::NextColumn();
	if (ImGui::BeginTabBar("##DetailTabs"))
	{
		if (selectedGroup != nullptr && selectedGroup->id != "LATEST") {
			bool tabOpen = ImGui::BeginTabItem("Group Stats");

			if (tabOpen) {
				ImGui::BeginChild("GroupStats");
				//ImGui::Text("Render the group stats here!");
				RenderGroupDetail(selectedGroup);
				//for (auto& player : selectedGroup->players) {
				//	ImGui::Text("%s: WinPercent: %.0f%%", player.name, player.cumulative.win_percentage);
				//}
				ImGui::EndChild();
				ImGui::EndTabItem();
			}
		}

		if (!detailID.empty())
		{
			bool tabOpen = ImGui::BeginTabItem("Replay Stats");

			if (tabOpen) {
				ImGui::BeginChild("ReplayStats");

				ImGui::Text("Right click any tab to configure which stats is shown - drag to reorder");
				auto replayDetail = api->GetCachedReplayDetail(detailID, (selectedGroup != nullptr) ? selectedGroup->id : "");
				RenderReplayDetail(&replayDetail);
				ImGui::EndChild();
				ImGui::EndTabItem();
			}
		}
		ImGui::EndTabBar();
	}


	ImGui::End();

	if (!isWindowOpen_)
	{
		cvarManager->executeCommand("togglemenu " + GetMenuName());
	}
}

// Name of the menu that is used to toggle the window.
std::string Ballchasinglog::GetMenuName()
{
	return "Ballchasing";
}

// Title to give the menu
std::string Ballchasinglog::GetMenuTitle()
{
	return menuTitle_;
}

// Don't call this yourself, BM will call this function with a pointer to the current ImGui context
void Ballchasinglog::SetImGuiContext(uintptr_t ctx)
{
	ImGui::SetCurrentContext(reinterpret_cast<ImGuiContext*>(ctx));
}

// Should events such as mouse clicks/key inputs be blocked so they won't reach the game
bool Ballchasinglog::ShouldBlockInput()
{
	return ImGui::GetIO().WantCaptureMouse || ImGui::GetIO().WantCaptureKeyboard;
}

// Return true if window should be interactive
bool Ballchasinglog::IsActiveOverlay()
{
	return true;
}

// Called when window is opened
void Ballchasinglog::OnOpen()
{
	isWindowOpen_ = true;
	api->GetLastMatches();
	api->GetTopLevelGroups();
}

void Ballchasinglog::OnReplayGroupChange(std::string id)
{
	if (id == "LATEST") {
		api->GetLastMatches();
	}
	else {
		api->GetReplaysForGroup(id);
	}
}

// Called when window is closed
void Ballchasinglog::OnClose()
{
	isWindowOpen_ = false;
}


void Ballchasinglog::RenderReplayDetail(ReplayData* detail)
{
	if (ImGui::BeginTabBar("#"))
	{
		RenderTableTab("Overview", guiSettings.overviewTableConfig, detail);
		RenderTableTab("Core", guiSettings.coreTableConfig, detail);
		RenderTableTab("Boost", guiSettings.boostTableConfig, detail);
		RenderTableTab("Movement", guiSettings.movementTableConfig, detail);
		RenderTableTab("Positioning", guiSettings.positioningTableConfig, detail);
		RenderTableTab("Camera", guiSettings.cameraTableConfig, detail);

		ImGui::EndTabBar();
	}

}

void Ballchasinglog::RenderGroupDetail(GroupData* group)
{
	if (ImGui::BeginTabBar("#"))
	{
		RenderGroupTab("Overview", guiSettings.groupOverviewTableConfig, group);
		RenderGroupTab("Core (Avg)", guiSettings.coreTableConfig, group);
		RenderGroupTab("Boost (Avg)", guiSettings.boostTableConfig, group);
		RenderGroupTab("Movement (Avg)", guiSettings.movementTableConfig, group);
		RenderGroupTab("Positioning (Avg)", guiSettings.positioningTableConfig, group);

		ImGui::EndTabBar();
	}
}


void Ballchasinglog::ContextMenu(std::vector<TableColumn>& columnData)
{
	if (ImGui::BeginPopupContextItem())
	{
		static bool dragging = false;
		// No hiding of the first column (mainly just to be lazy and not worry about all cols beeing hidden)
		for (size_t i = 1; i < columnData.size(); i++)
		{
			auto& col = columnData[i];
			if (ImGui::Selectable(col.name.c_str(), &col.visible, ImGuiSelectableFlags_DontClosePopups))
			{
				if (dragging) col.visible = !col.visible;
				dragging = false;
			}
			if (ImGui::IsItemActive() && !ImGui::IsItemHovered())
			{
				dragging = true;
				int i_next = i + (ImGui::GetMouseDragDelta(0).y < 0.f ? -1 : 1);
				if (i_next >= 0 && i_next < columnData.size())
				{
					std::swap(columnData[i], columnData[i_next]);
					ImGui::ResetMouseDragDelta();
				}
			}
		}
		ImGui::EndPopup();
	}
}
bool Ballchasinglog::ContextMenu(ReplayData* detail, GroupData* parentGroup)
{
	if (ImGui::BeginPopupContextItem()) {

		if (ImGui::BeginMenu("Add to group")) {
			for (auto& groupID : api->topLevelGroups) {
				if (ImGui::Selectable(groupID.c_str())) {
					api->AddReplayToGroup(detail->id, groupID);
				}

			}
			ImGui::EndMenu();
		}
		if (ImGui::Selectable("Remove from group")) {
			api->AddReplayToGroup(detail->id, "");
		}

		ImGui::EndPopup();
		return true;
	}
	return false;
}
void Ballchasinglog::RenderTableTab(std::string name, TableSettings& settings, ReplayData* detail, bool bBlueHeader, bool bOrangeHeader)
{
	bool tabOpen = ImGui::BeginTabItem(name.c_str());
	ContextMenu(settings.Columns);
	if (tabOpen)
	{
		ImGui::BeginChild((name + "tab").c_str());
		RenderTeamTable(detail->blue, settings, bBlueHeader);
		ImGui::Separator();
		RenderTeamTable(detail->orange, settings, bOrangeHeader);

		ImGui::EndChild();
		ImGui::EndTabItem();
	}
}

void Ballchasinglog::RenderGroupTab(std::string name, TableSettings& settings, GroupData* detail)
{
	static Team dummyTeam;
	bool tabOpen = ImGui::BeginTabItem(name.c_str());
	ContextMenu(settings.Columns);
	if (tabOpen)
	{
		ImGui::BeginChild((name + "tab").c_str());
		int cols = ImGui::GetColumnsCount();
		ImGui::Columns(settings.GetColumnCount());
		for (auto& col : settings.Columns) {
			col.RenderHeader(dummyTeam);
		}
		ImGui::Separator();
		for (auto& p : detail->players) {
			RenderPlayerRow(p, settings);
		}
		//RenderTeamTable(detail->blue, settings, bBlueHeader);
		//ImGui::Separator();
		//RenderTeamTable(detail->orange, settings, bOrangeHeader);

		ImGui::EndChild();
		ImGui::EndTabItem();
	}
}

void Ballchasinglog::RenderTeamTable(Team& team, TableSettings& settings, bool drawHeader)
{
	int cols = ImGui::GetColumnsCount();
	ImGui::Columns(settings.GetColumnCount());
	if (drawHeader) {
		for (auto& col : settings.Columns) {
			col.RenderHeader(team);
		}
		ImGui::Separator();
	}

	
	for (auto& p : team.players) {
		RenderPlayerRow(p, settings);
		//for (auto col : settings.Columns) {
		//	col.RenderCell(p);
		//}
	}
	ImGui::Columns(cols);
}

void Ballchasinglog::RenderPlayerRow(BaseStatPlayer& p, TableSettings& settings)
{
	for (auto col : settings.Columns) {
		col.RenderCell(p);
	}
}




