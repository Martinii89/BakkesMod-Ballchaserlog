#include "pch.h"
#include "Ballchasinglog.h"
#include "IMGUI/imgui_stdlib.h"

struct VM
{
	GroupData* selectedGroup;
	ReplayData* detailReplay;
	std::set<std::string> selectedReplays;
	std::string lastSelectedReplayID;

	void SetSelectedGroup(GroupData* group);
	void SetDetailReplay(ReplayData* replay);
	bool ToggleReplaySelected(ReplayData* start);
	bool RangeSelectReplays(ReplayData* end);
	void ClearReplaySelection();
	bool ShouldReplayBeHighlighed(ReplayData* replay);
	std::string GetSelectedGroupId();
	std::string GetDetailReplayId();
	std::string GetDetailReplayLink();
	std::string GetSelectedGroupLink();
};

std::string joinPlayers(Team t)
{
	std::ostringstream ss;
	bool first = true;
	for (const auto& p : t.players)
	{
		if (!first)
		{
			ss << ", ";
		}
		ss << p.name;
		first = false;
	}
	return ss.str();
}


// Name of the menu that is used to toggle the window.
std::string Ballchasinglog::GetMenuName()
{
	return "Ballchasinglog";
}

// Title to give the menu
std::string Ballchasinglog::GetMenuTitle()
{
	return menu_title_;
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
	api_->GetLastMatches();
	api_->GetTopLevelGroups();
}

// Called when window is closed
void Ballchasinglog::OnClose()
{
	isWindowOpen_ = false;
}

// Do ImGui rendering here
void Ballchasinglog::Render()
{
	static VM vm;
	if (vm.selectedGroup == nullptr)
	{
		vm.SetSelectedGroup(api_->GetCachedGroup("LATEST"));
	}
	//const std::string LATEST_REPLAYS = "LATEST";
	//const std::string DEFAULT_GROUP_NAME = "Your latest replays";
	ImGui::SetNextWindowSize({800, 400}, ImGuiCond_FirstUseEver);

	if (!ImGui::Begin(menu_title_.c_str(), &isWindowOpen_, ImGuiWindowFlags_None))
	{
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}

	ImGui::Columns(2);
	static bool replayListCollapsed = false;
	static float uncollapseWidth = 0;
	//static std::string detailID = "";
	//static std::string selectedGroupID = LATEST_REPLAYS;
	//static std::string groupName = DEFAULT_GROUP_NAME;

	GroupData* selectedGroup = api_->GetCachedGroup(vm.GetSelectedGroupId());

	if (!replayListCollapsed)
	{
		ImGui::Text("Your replay groups");

		ImGui::SameLine();
		if (ImGui::Button("Manage groups"))
		{
			ImGui::OpenPopup("CreateGroupPopup");
		}
		CreateGroupPopup();

		ImGui::SameLine();
		if (ImGui::Button("Copy url##copygroupURL"))
		{
			ImGui::SetClipboardText(vm.GetSelectedGroupLink().c_str());
			ImGui::SameLine();
			ImGui::Text("Copied");
		}


		ImGui::SameLine(ImGui::GetColumnWidth(0) - 25);
		if (ImGui::ArrowButton(">>", ImGuiDir_Left))
		{
			replayListCollapsed = true;
			uncollapseWidth = ImGui::GetColumnWidth(0);
			ImGui::SetColumnWidth(0, 30);
		}
		ImGui::BeginChild("ReplayGroupList", ImVec2(0, ImGui::GetWindowHeight() * 0.5f - 40), true);

		for (auto& groupID : api_->topLevelGroups)
		{
			GroupData* group = api_->GetCachedGroup(groupID);
			auto oldGroup = selectedGroup;
			RenderGroup(group, &selectedGroup);

			if (selectedGroup != oldGroup)
			{
				vm.SetSelectedGroup(selectedGroup);
			}
		}
		ImGui::EndChild();
		std::string groupName = (selectedGroup != nullptr) ? selectedGroup->name : "";

		ImGui::Text(groupName.c_str());

		ImGui::BeginChild("ReplayList", ImVec2(0, ImGui::GetWindowHeight() * 0.5f - 70), true);
		if (selectedGroup != nullptr)
		{
			for (size_t i = 0; i < selectedGroup->groupReplays.size(); i++)
			{
				auto& replay = selectedGroup->groupReplays[i];
				std::string list_lbl = replay.replay_title + "##replaylist";
				if (ImGui::Selectable(list_lbl.c_str(), vm.ShouldReplayBeHighlighed(&replay)))
				{
					auto io = ImGui::GetIO();
					if (io.KeyShift)
					{
						vm.RangeSelectReplays(&replay);
					}
					else if (io.KeyCtrl)
					{
						vm.ToggleReplaySelected(&replay);
					}
					else
					{
						vm.ClearReplaySelection();
						vm.SetDetailReplay(&replay);
					}
				}
				if (ImGui::IsItemHovered())
				{
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
		//if (ImGui::Button("Clear")) {
		//	vm.ClearReplaySelection();
		//}ImGui::SameLine();
		auto btn_lbl = vm.GetSelectedGroupId() == "LATEST" ? "Assign replays" : "Reassign replays";
		if (ImGui::Button(btn_lbl))
		{
			ImGui::OpenPopup("AssignReplaysPopup");
		}
		AssignReplayPopup(&vm);

		ImGui::SameLine();
		if (ImGui::Button("Copy url##copyreplayurl"))
		{
			ImGui::SetClipboardText(vm.GetDetailReplayLink().c_str());
			ImGui::SameLine();
			ImGui::Text("Copied");
		}
	}
	else
	{
		if (ImGui::ArrowButton(">>", ImGuiDir_Right))
		{
			replayListCollapsed = false;
			ImGui::SetColumnWidth(0, uncollapseWidth);
		}
	}

	ImGui::NextColumn();
	if (ImGui::BeginTabBar("##DetailTabs"))
	{
		if (selectedGroup != nullptr && selectedGroup->id != "LATEST")
		{
			bool tabOpen = ImGui::BeginTabItem("Group Stats");

			if (tabOpen)
			{
				ImGui::BeginChild("GroupStats");
				RenderGroupDetail(selectedGroup);
				ImGui::EndChild();
				ImGui::EndTabItem();
			}
		}

		std::string detailID = vm.GetDetailReplayId();

		if (!detailID.empty())
		{
			bool tabOpen = ImGui::BeginTabItem("Replay Stats");

			if (tabOpen)
			{
				ImGui::BeginChild("ReplayStats");

				ImGui::Text("Right click any tab to configure which stats is shown - drag to reorder");
				auto replayDetail = api_->GetCachedReplayDetail(
					detailID, (selectedGroup != nullptr) ? selectedGroup->id : "");
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

void Ballchasinglog::RenderGroup(GroupData* group, GroupData** selectedGroup)
{
	if (group == nullptr) return;
	static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick |
		ImGuiTreeNodeFlags_SpanAvailWidth;
	ImGuiTreeNodeFlags node_flags = base_flags;
	if (group == *selectedGroup)
		node_flags |= ImGuiTreeNodeFlags_Selected;
	bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)group, node_flags, group->name.c_str());
	if (ImGui::IsItemClicked())
	{
		*selectedGroup = group;
		OnReplayGroupChange(group->id);
		if (group->id != "LATEST")
		{
			api_->GetSubGroups(group->id);
			api_->GetGroupStats(group->id);
		}
	}
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::Text("Created: %s", group->created.c_str());
		ImGui::Text("%s", group->shared ? "Public" : "Private");
		ImGui::Text("Status: %s", group->status.c_str());
		ImGui::EndTooltip();
	}
	if (node_open)
	{
		for (auto subgroupID : group->subgroups)
		{
			auto pSubGroup = api_->GetCachedGroup(subgroupID);
			RenderGroup(pSubGroup, selectedGroup);
		}
		ImGui::TreePop();
	}
}

bool Ballchasinglog::RenderGroupSelector(GroupData* group, GroupData** selectedGroup)
{
	bool groupClicked = false;
	if (group == nullptr) return groupClicked;
	static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick |
		ImGuiTreeNodeFlags_SpanAvailWidth;
	ImGuiTreeNodeFlags node_flags = base_flags;
	if (group == *selectedGroup)
		node_flags |= ImGuiTreeNodeFlags_Selected;
	bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)group, node_flags, group->name.c_str());
	if (ImGui::IsItemClicked())
	{
		*selectedGroup = group;
		groupClicked = true;
		if (group->id != "LATEST")
		{
			api_->GetSubGroups(group->id);
		}
	}
	if (node_open)
	{
		for (auto subgroupID : group->subgroups)
		{
			auto pSubGroup = api_->GetCachedGroup(subgroupID);
			groupClicked |= RenderGroupSelector(pSubGroup, selectedGroup);
		}
		ImGui::TreePop();
	}
	return groupClicked;
}

void Ballchasinglog::OnReplayGroupChange(std::string id)
{
	if (id == "LATEST") {
		api_->GetLastMatches();
	}
	else {
		api_->GetReplaysForGroup(id);
	}
}

void Ballchasinglog::RenderReplayDetail(ReplayData* detail)
{
	if (ImGui::BeginTabBar("#"))
	{
		RenderTableTab("Overview", gui_settings_.overviewTableConfig, detail);
		RenderTableTab("Core", gui_settings_.coreTableConfig, detail);
		RenderTableTab("Boost", gui_settings_.boostTableConfig, detail);
		RenderTableTab("Movement", gui_settings_.movementTableConfig, detail);
		RenderTableTab("Positioning", gui_settings_.positioningTableConfig, detail);
		RenderTableTab("Camera", gui_settings_.cameraTableConfig, detail);

		ImGui::EndTabBar();
	}

}

void Ballchasinglog::RenderGroupDetail(GroupData* group)
{
	if (ImGui::BeginTabBar("#"))
	{
		RenderGroupTab("Overview", gui_settings_.groupOverviewTableConfig, group);
		RenderGroupTab("Core (Avg)", gui_settings_.coreTableConfig, group);
		RenderGroupTab("Boost (Avg)", gui_settings_.boostTableConfig, group);
		RenderGroupTab("Movement (Avg)", gui_settings_.movementTableConfig, group);
		RenderGroupTab("Positioning (Avg)", gui_settings_.positioningTableConfig, group);

		ImGui::EndTabBar();
	}
}

void Ballchasinglog::AssignReplayPopup(VM* vm)
{
	if (ImGui::BeginPopup("AssignReplaysPopup"))
	{
		if (vm->detailReplay == nullptr && vm->selectedReplays.size() == 0) {
			ImGui::Text("Select some replays first");
			ImGui::EndPopup();
			return;
		}
		static std::string selectedGroupId = "";
		ImGui::InputText("GroupID", &selectedGroupId);
		for (auto& groupID : api_->topLevelGroups) {
			if (groupID == "LATEST") continue;
			GroupData* group = api_->GetCachedGroup(groupID);
			GroupData* selectedGroup = nullptr;
			if (RenderGroupSelector(group, &selectedGroup)) {
				selectedGroupId = selectedGroup->id;
			}
		}
		if (ImGui::Button("Assign")) {
			api_->AssignReplays(selectedGroupId, std::vector<std::string>(vm->selectedReplays.begin(), vm->selectedReplays.end()), {});
			ImGui::CloseCurrentPopup();
		}ImGui::SameLine();
		auto selectedGroup = vm->GetSelectedGroupId();
		if (!selectedGroup.empty() && selectedGroup != "LATEST") {
			if (ImGui::Button("Remove")) {
				api_->AssignReplays(selectedGroup, {}, std::vector<std::string>(vm->selectedReplays.begin(), vm->selectedReplays.end()));
				ImGui::CloseCurrentPopup();
			}
		}

		ImGui::EndPopup();
	}
}

bool Ballchasinglog::GroupSelector(std::string* groupId) {
	bool clicked = false;
	for (auto& groupID : api_->topLevelGroups) {

		GroupData* group = api_->GetCachedGroup(groupID);
		GroupData* selectedGroup = nullptr;
		if (RenderGroupSelector(group, &selectedGroup)) {
			clicked = true;
			*groupId = selectedGroup->id;
		}
	}
	return clicked;
}


void Ballchasinglog::CreateGroupPopup()
{
	static bool deleteGroup = false;
	static bool deleteGroupSure = false;
	static std::string groupName = "";
	static std::string parentGroup = "";
	if (ImGui::BeginPopup("CreateGroupPopup")) {
		ImGui::Checkbox("Delete", &deleteGroup);
		if (!deleteGroup) {
			ImGui::InputText("Group name", &groupName);
			ImGui::InputText("Parent Group", &parentGroup);
			ImGui::Text("Select Parent group");
		}
		else {
			ImGui::InputText("Group to delete", &parentGroup);
			ImGui::Text("Select Group to delete");
		}
		ImGui::Separator();
		if (GroupSelector(&parentGroup)) {
			if (parentGroup == "LATEST") parentGroup = "";
		}
		ImGui::Separator();
		if (!deleteGroup) {
			if (ImGui::Button("Create")) {
				api_->CreateGroup(groupName, parentGroup);
				ImGui::CloseCurrentPopup();
			}
		}
		else {
			ImGui::Checkbox("You sure?", &deleteGroupSure); ImGui::SameLine();
			if (ImGui::Button("Delete##deletegroupbutton")) {
				if (deleteGroupSure) {
					api_->DeleteGroup(parentGroup);
					ImGui::CloseCurrentPopup();
				}
			}
		}


		ImGui::EndPopup();
	}
	else {
		deleteGroup = false;
		deleteGroupSure = false;
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
			for (auto& groupID : api_->topLevelGroups) {
				if (ImGui::Selectable(groupID.c_str())) {
					api_->AddReplayToGroup(detail->id, groupID);
				}

			}
			ImGui::EndMenu();
		}
		if (ImGui::Selectable("Remove from group")) {
			api_->AddReplayToGroup(detail->id, "");
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

void VM::SetSelectedGroup(GroupData* group)
{
	selectedGroup = group;
	detailReplay = nullptr;
	lastSelectedReplayID = "";
	selectedReplays.clear();
}

void VM::SetDetailReplay(ReplayData* replay)
{
	detailReplay = replay;
	lastSelectedReplayID = replay->id;
}

bool VM::ToggleReplaySelected(ReplayData* start)
{
	if (!start) return false;
	if (selectedReplays.erase(start->id) == 0) {
		selectedReplays.insert(start->id);
	}
	return true;
}

bool VM::RangeSelectReplays(ReplayData* end)
{
	if (!selectedGroup) {
		return false;
	}
	if (lastSelectedReplayID.empty()) {
		return false;
	}
	auto first = std::find_if(selectedGroup->groupReplays.begin(), selectedGroup->groupReplays.end(), [this](ReplayData a) { return a.id == this->lastSelectedReplayID; });
	auto stop = std::find_if(selectedGroup->groupReplays.begin(), selectedGroup->groupReplays.end(), [end](ReplayData a) { return a.id == end->id; });
	if (first == selectedGroup->groupReplays.end() || stop == selectedGroup->groupReplays.end()) {
		return false;
	}
	if (first > stop) {
		std::swap(first, stop);
	}
	std::for_each(first, stop+1, [this](ReplayData r) {this->selectedReplays.insert(r.id); });
	return true;
}

void VM::ClearReplaySelection()
{
	selectedReplays.clear();
}

bool VM::ShouldReplayBeHighlighed(ReplayData* replay)
{
	if (replay == detailReplay) {
		return true;
	}
	if (selectedReplays.find(replay->id) != selectedReplays.end()) {
		return true;
	}
	return false;
}

std::string VM::GetSelectedGroupId()
{
	if (selectedGroup) {
		return selectedGroup->id;
	}
	return "LATEST";
}

std::string VM::GetDetailReplayId()
{
	if (detailReplay == nullptr) return "";

	return detailReplay->id;
}

std::string VM::GetDetailReplayLink()
{
	if (detailReplay == nullptr) {
		return "No replay selected";
	}
	std::stringstream url;
	url << "https://ballchasing.com/replay/" << detailReplay->id;
	return url.str();
}

std::string VM::GetSelectedGroupLink()
{
	if (selectedGroup == nullptr) {
		return "No group selected";
	}
	std::stringstream url;
	url << "https://ballchasing.com/group/" << selectedGroup->id;
	return url.str();
}
