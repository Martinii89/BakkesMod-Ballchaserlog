#include "pch.h"
#include "Ballchaserlog.h"
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

// Do ImGui rendering here
void Ballchaserlog::Render()
{
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
	if (!replayListCollapsed)
	{
		ImGui::Text("Your latest replays"); ImGui::SameLine(ImGui::GetColumnWidth(0) - 25);
		if (ImGui::ArrowButton(">>", ImGuiDir_Left)) {
			replayListCollapsed = true;
			uncollapseWidth = ImGui::GetColumnWidth(0);
			ImGui::SetColumnWidth(0, 20);
		}
	}
	else {
		if (ImGui::ArrowButton(">>", ImGuiDir_Right)) {
			replayListCollapsed = false;
			ImGui::SetColumnWidth(0, uncollapseWidth);
		}
	}
	ImGui::BeginChild("ReplayList");

	if (!replayListCollapsed){
		for (auto& replay : api->lastMatchesResult)
		{
			if (ImGui::Selectable(replay.replay_title.c_str(), replay.id == detailID))
			{
				cvarManager->log("selected detail");
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
		}
	}
	ImGui::EndChild();
	ImGui::NextColumn();
	if (!detailID.empty())
	{
		ImGui::Text("Righ click any tab to configure which stats is shown - drag to reorder");
		auto replayDetail = api->GetCachedDetail(detailID);
		RenderReplayDetail(&replayDetail);
	}


	ImGui::End();

	if (!isWindowOpen_)
	{
		cvarManager->executeCommand("togglemenu " + GetMenuName());
	}
}

// Name of the menu that is used to toggle the window.
std::string Ballchaserlog::GetMenuName()
{
	return "Ballchasing";
}

// Title to give the menu
std::string Ballchaserlog::GetMenuTitle()
{
	return menuTitle_;
}

// Don't call this yourself, BM will call this function with a pointer to the current ImGui context
void Ballchaserlog::SetImGuiContext(uintptr_t ctx)
{
	ImGui::SetCurrentContext(reinterpret_cast<ImGuiContext*>(ctx));
}

// Should events such as mouse clicks/key inputs be blocked so they won't reach the game
bool Ballchaserlog::ShouldBlockInput()
{
	return ImGui::GetIO().WantCaptureMouse || ImGui::GetIO().WantCaptureKeyboard;
}

// Return true if window should be interactive
bool Ballchaserlog::IsActiveOverlay()
{
	return true;
}

// Called when window is opened
void Ballchaserlog::OnOpen()
{
	isWindowOpen_ = true;
	api->GetLastMatches();
}

// Called when window is closed
void Ballchaserlog::OnClose()
{
	isWindowOpen_ = false;
}

void DrawTeamOverviewStats(Team& team)
{

	ImGui::Text("%i", team.goals); ImGui::NextColumn();
	ImGui::Text("%s", team.name); ImGui::NextColumn();
	ImGui::Text("SCORE"); ImGui::NextColumn();
	ImGui::Text("GOALS"); ImGui::NextColumn();
	ImGui::Text("ASSISTS"); ImGui::NextColumn();
	ImGui::Text("SAVES"); ImGui::NextColumn();
	ImGui::Text("SHOTS"); ImGui::NextColumn();
	ImGui::Separator();

	for (auto& p : team.players) {
		ImGui::Text(""); ImGui::NextColumn();
		ImGui::Text("%s", p.name); ImGui::NextColumn();
		ImGui::Text("%i", p.score); ImGui::NextColumn();
		ImGui::Text("%i", p.stats.core.goals); ImGui::NextColumn();
		ImGui::Text("%i", p.stats.core.assists); ImGui::NextColumn();
		ImGui::Text("%i", p.stats.core.saves); ImGui::NextColumn();
		ImGui::Text("%i", p.stats.core.shots); ImGui::NextColumn();
	}
}



void Ballchaserlog::RenderReplayDetail(GetReplayResponseData* detail)
{
	if (ImGui::BeginTabBar("#"))
	{
		RenderTableTab("Overview", guiSettings.overviewTableConfig, detail);
		RenderTableTab("Core", guiSettings.coreTableConfig, detail);
		RenderTableTab("Boost", guiSettings.boostTableConfig, detail);
		RenderTableTab("Movement", guiSettings.movementTableConfig, detail);
		RenderTableTab("Positioning", guiSettings.positioningTableConfig, detail);

		ImGui::EndTabBar();
	}

}


void Ballchaserlog::ContextMenu(std::vector<TableColumn>& columnData)
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
void Ballchaserlog::RenderTableTab(std::string name, TableSettings& settings, GetReplayResponseData* detail, bool bBlueHeader, bool bOrangeHeader)
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

void Ballchaserlog::RenderTeamTable(Team& team, TableSettings& settings, bool drawHeader)
{
	int cols = ImGui::GetColumnsCount();
	ImGui::Columns(settings.GetColumnCount());
	for (auto& col : settings.Columns) {
		col.RenderHeader(team);
	}
	ImGui::Separator();

	for (auto& p : team.players) {
		for (auto col : settings.Columns) {
			col.RenderCell(p);
		}
	}
	ImGui::Columns(cols);
}



