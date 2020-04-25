#include "pch.h"
#include "Ballchaserlog.h"


// Do ImGui rendering here
void Ballchaserlog::Render()
{
	if (!ImGui::Begin(menuTitle_.c_str(), &isWindowOpen_, ImGuiWindowFlags_None))
	{
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}

	ImGui::Columns(2);

	static std::string detailID = "";
	ImGui::BeginChild("ReplayList");
	for (auto& replay : api->lastMatchesResult)
	{
		if (ImGui::Selectable(replay.replay_title.c_str(), replay.id == detailID))
		{
			cvarManager->log("selected detail");
			detailID = replay.id;
		}
	}
	ImGui::EndChild();
	ImGui::NextColumn();
	if (!detailID.empty())
	{
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
	return "Ballchaserlog";
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

void DrawTeamCoreStats(Team& team)
{
	int cols = ImGui::GetColumnsCount();
	ImGui::Columns(9);
	ImGui::Text(""); ImGui::NextColumn();
	ImGui::Text("SCORE"); ImGui::NextColumn();
	ImGui::Text("SHOTS"); ImGui::NextColumn();
	ImGui::Text("GOALS"); ImGui::NextColumn();
	ImGui::Text("SHOOT%"); ImGui::NextColumn();
	ImGui::Text("ASSISTS"); ImGui::NextColumn();
	ImGui::Text("SAVES"); ImGui::NextColumn();
	ImGui::Text("DEMOS"); ImGui::NextColumn();
	ImGui::Text("DEMOS TAKEN"); ImGui::NextColumn();
	ImGui::Separator();

	for (auto& p : team.players) {
		ImGui::Text("%s", p.name); ImGui::NextColumn();
		ImGui::Text("%i", p.score); ImGui::NextColumn();
		ImGui::Text("%i", p.stats.core.shots); ImGui::NextColumn();
		ImGui::Text("%i", p.stats.core.goals); ImGui::NextColumn();
		ImGui::Text("%.1f", p.stats.core.shooting_percentage); ImGui::NextColumn();
		ImGui::Text("%i", p.stats.core.assists); ImGui::NextColumn();
		ImGui::Text("%i", p.stats.core.saves); ImGui::NextColumn();
		ImGui::Text("%i", p.stats.demo.inflicted); ImGui::NextColumn();
		ImGui::Text("%i", p.stats.demo.taken); ImGui::NextColumn();
	}

	ImGui::Columns(cols);
}

void Ballchaserlog::RenderReplayDetail(GetReplayResponseData* detail)
{
	if (ImGui::BeginTabBar("#"))
	{
		if (ImGui::BeginTabItem("Overview"))
		{
			ImGui::BeginChild("OverviewTab");
			int cols = ImGui::GetColumnsCount();
			ImGui::Columns(7);
			DrawTeamOverviewStats(detail->blue);
			ImGui::Separator();
			DrawTeamOverviewStats(detail->orange);
			ImGui::Columns(cols);

			ImGui::EndChild();
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Core"))
		{
			ImGui::BeginChild("CoreTab");
			DrawTeamCoreStats(detail->blue);
			ImGui::Separator();
			DrawTeamCoreStats(detail->orange);

			ImGui::EndChild();
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Boost"))
		{
			ImGui::BeginChild("BoostTab");
			ImGui::EndChild();
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Movement"))
		{
			ImGui::BeginChild("MovementTab");
			ImGui::EndChild();
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Positioning"))
		{
			ImGui::BeginChild("PositioningTab");
			ImGui::EndChild();
			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}
}

