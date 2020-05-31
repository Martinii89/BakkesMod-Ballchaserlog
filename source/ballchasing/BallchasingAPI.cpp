#include "pch.h"
#include <thread>
#include "../json/json.hpp"
using json = nlohmann::json;
#include "BallchasingAPI.h"
#include "APIDataClasses.h"
#include <queue>

BallchasingAPI::BallchasingAPI(std::shared_ptr<CVarManagerWrapper> cvar, std::shared_ptr<GameWrapper> gw): cli("ballchasing.com"), cvar_(cvar), gw_(gw)
{
	auto latestGroup = GroupData();
	latestGroup.id = "LATEST";
	latestGroup.name = "Your latest replays";
	latestGroup.subGroupsRequested = RequestState::SUCCESS;
	topLevelGroups.push_back(latestGroup.id);
	groupCache_[latestGroup.id] = latestGroup;
}

void BallchasingAPI::Ping()
{
	gw_->Toast("Ballchasing log", "pinging the Ballchasing API. Check console for results");
	std::thread t([this]() {
		std::string url = "/api/";
		
		cli.set_follow_location(true);
		auto res = cli.Get(url.c_str(), GetAuthHeaders());
		if (res && res->status == 200)
		{
			cvar_->log(res->body);
			cvar_->log(std::to_string(res->status));
		}
		else {
			cvar_->log("res was null");
		}
		});
	t.detach();
}

void BallchasingAPI::GetLastMatches()
{
	gw_->Toast("Ballchasing log", "Fetching your most recent replays");
	std::thread t([this]() {
		std::string url = "/api/replays?uploader=me";
		
		auto res = cli.Get(url.c_str(), GetAuthHeaders());
		if (res && res->status == 200)
		{
			json j = json::parse(res->body);
			try {
				auto getReplays = j.get<GetReplaysResponse>();
				OnGotReplayList(getReplays.replays, "LATEST");
				//OnGroupListChanged(getReplays);
			}
			catch (const std::exception & e) {
				gw_->Toast("Ballchasing log", "ERROR! Check console for details");
				cvar_->log(e.what());
			}
		}
		else {
			gw_->Toast("Ballchasing log", "ERROR! Check console for details");
			cvar_->log("GetLastMatches result was null");
		}
		});
	t.detach();
}

ReplayData BallchasingAPI::GetTemporaryOverviewData(std::string replayID, std::string groupID)
{
	auto pGroup = GetCachedGroup(groupID);
	if (pGroup) {
		for (auto& d : pGroup->groupReplays)
		{
			if (d.id == replayID) {
				return d;
			}
		}
	}
	return ReplayData();
}


void BallchasingAPI::OnGotReplayList(std::vector<ReplayData> replayList, std::string groupId)
{
	bool foundGroup;
	auto group = FindGroupById(groupId, foundGroup);
	if (!foundGroup) {
		cvar_->log("Failed to find the group to attach these replays to..( " + groupId + ")");
		return;
	}
	group->groupReplays = replayList;
}

void BallchasingAPI::OnGotGroupStats(GroupData data)
{
	bool found;
	auto pCachedGroup = FindGroupById(data.id, found);
	if (found) {
		pCachedGroup->status = data.status;
		pCachedGroup->players = data.players;
	}
}

GroupData* BallchasingAPI::FindGroupById(std::string groupId, bool& found)
{
	auto groupFinder = groupCache_.find(groupId);
	if (groupFinder != groupCache_.end()) {
		found = true;
		return &groupFinder->second;
	}
	found = false;
	return nullptr;
}

void BallchasingAPI::OnGetReplayGroupsSuccess(GetReplayGroupsResponseData res, std::string parentGroupID)
{
	if (parentGroupID.empty()) {
		// TODO: 
		// Update them rather than replace them if they already exist 
		// so we don't loose the subgroups and replay lists 
		if (topLevelGroups.size() > 1) {
			topLevelGroups.erase(topLevelGroups.begin() + 1, topLevelGroups.end());
		}
		for (auto group : res.list) {
			topLevelGroups.push_back(group.id);
			groupCache_[group.id] = group;
		}
	}
	else {
		bool found;
		auto parent = FindGroupById(parentGroupID, found);
		if (found) {
			parent->subgroups.clear();
			for (auto sub : res.list) {
				parent->subgroups.push_back(sub.id);
				groupCache_[sub.id] = sub;
			}
			parent->subGroupsRequested = RequestState::SUCCESS;
		}
	}
}

ReplayData BallchasingAPI::GetCachedReplayDetail(std::string replayID, std::string groupID)
{
	auto it = replayDetailsCache_.find(replayID);
	if (it != replayDetailsCache_.end())
	{
		return it->second;
	}
	else {
		// Add a empty detail object to stop this from executing many more requests
		replayDetailsCache_[replayID] = GetTemporaryOverviewData(replayID, groupID);
		// Start to fetch the details.
		GetReplayDetails(replayID);

	}
	return ReplayData();
}

GroupData* BallchasingAPI::GetCachedGroup(std::string id)
{
	bool found;
	auto group = FindGroupById(id, found);
	if (found) {
		return group;
	}
	cvar_->log("GetCachedGroup didn't find a cached gorup");
	return group;
}

void BallchasingAPI::GetTopLevelGroups()
{
	std::thread t([this]() {
		std::string url = "/api/groups?creator=me";

		auto res = cli.Get(url.c_str(), GetAuthHeaders());
		if (res && res->status == 200)
		{
			json j = json::parse(res->body);
			try {
				auto groupList = j.get<GetReplayGroupsResponseData>();
				OnGetReplayGroupsSuccess(groupList);
			}
			catch (const std::exception & e) {
				OnError("Check console for details");
				cvar_->log(e.what());
			}
		}
		else {
			OnError("Check console for details");
			cvar_->log("GetToplevelGroups result was null");
		}
		});
	t.detach();
}

void BallchasingAPI::GetReplaysForGroup(std::string id)
{
	std::thread t([this, id]() {
		std::string url = "/api/replays?group=" + id;

		auto res = cli.Get(url.c_str(), GetAuthHeaders());
		if (res && res->status == 200)
		{
			json j = json::parse(res->body);
			try {
				auto getReplays = j.get<GetReplaysResponse>();
				//OnGroupListChanged(getReplays);
				OnGotReplayList(getReplays.replays, id);
			}
			catch (const std::exception& e) {
				OnError("Check console for details");
				cvar_->log(e.what());
			}
		}
		else {
			OnError("Check console for details");
			cvar_->log("GetReplaysForGroup result was null");
		}
	});
	t.detach();
}

void BallchasingAPI::GetGroupStats(std::string id)
{
	std::thread t([this, id]() {
		std::string url = "/api/groups/" + id;
		auto res = cli.Get(url.c_str(), GetAuthHeaders());
		if (res && res->status == 200)
		{
			json j = json::parse(res->body);

			try {
				auto groupStats = j.get <GroupData>();
				OnGotGroupStats(groupStats);
			}
			catch (const std::exception & e) {
				OnError("Check console for details");
				cvar_->log(e.what());
			}
		}
		else {
			OnError("Check console for details");
			cvar_->log("GetGroupStats result was null");
		}
		});
	t.detach();
}

void BallchasingAPI::GetSubGroups(std::string groupID)
{
	std::thread t([this, groupID]() {
		std::string url = "/api/groups?group=" + groupID;
		auto res = cli.Get(url.c_str(), GetAuthHeaders());
		if (res && res->status == 200)
		{
			json j = json::parse(res->body);

			try {
				auto groupList = j.get<GetReplayGroupsResponseData>();
				OnGetReplayGroupsSuccess(groupList, groupID);
			}
			catch (const std::exception & e) {
				OnError("Check console for details");
				cvar_->log(e.what());
			}
		}
		else {
			OnError("Check console for details");
			
			cvar_->log("GetSubGroups result was null");
		}
		});
	t.detach();
}


// Call with empty GroupID to remove from group
void BallchasingAPI::AddReplayToGroup(std::string replayID, std::string groupID)
{
	std::thread t([this, replayID, groupID]() {
		std::string url = "/api/replays/" + replayID;
		json j;
		j["group"] = groupID;
		std::string body = j.dump();
		auto res = cli.Patch(url.c_str(), GetAuthHeaders(), body, "application/json");
		if (res && res->status == 204)
		{
			if (groupID == "") {
				OnOk("Replay removed from group");
			}
			else {
				OnOk("Replay added to group");
			}
		}
		else {
			OnError("Check console for details");
			cvar_->log("GetReplayDetails result was null");
		}
		});
	t.detach();
}

void BallchasingAPI::AssignReplays(std::string groupId, std::vector<std::string> addReplays, std::vector<std::string> removeReplays)
{
	std::thread t([this, groupId, addReplays, removeReplays]() {
		std::string url = "/api/groups/" + groupId;
		json j;
		j["assign_replays"] = addReplays;
		j["unassign_replays"] = removeReplays;
		std::string body = j.dump();
		auto res = cli.Patch(url.c_str(), GetAuthHeaders(), body, "application/json");
		if (res && res->status == 204)
		{
			OnOk("Group updated. Please give the server some processing time before you request the group");
		}
		else {
			OnError("Check console for details");
			cvar_->log("AssignReplays result was null");
		}
		});
	t.detach();
}

void BallchasingAPI::CreateGroup(std::string groupName, std::string parentGroupId)
{
	std::thread t([this, groupName, parentGroupId]() {
		bool subGroup = false;
		std::string url = "/api/groups";
		json j;
		j["name"] = groupName;
		if (!parentGroupId.empty()) {
			j["parent"] = parentGroupId;
			subGroup = true;
		}
		j["player_identification"] = "by-id";
		j["team_identification"] = "by-distinct-players";
		std::string body = j.dump();
		auto res = cli.Post(url.c_str(), GetAuthHeaders(), body, "application/json");
		if (res && res->status == 201)
		{
			OnOk("Group created. Refreshing grups");
			if (subGroup) {
				gw_->SetTimeout([this, parentGroupId](GameWrapper* gw) {GetSubGroups(parentGroupId); }, 1.0);
			}
			else {
				gw_->SetTimeout([this](GameWrapper* gw) {GetTopLevelGroups(); }, 1.0);
			}
		}
		else {
			OnError("Check console for details");
			cvar_->log("CreateGroup result was null");
		}
		});
	t.detach();
}

void BallchasingAPI::OnError(std::string message)
{
	gw_->Toast("Ballchasing log", "Error: " + message, "default", 3.5, ToastType_Error);
}

void BallchasingAPI::OnOk(std::string message)
{
	gw_->Toast("Ballchasing log", message, "default", 3.5, ToastType_OK);
}

void BallchasingAPI::GetReplayDetails(std::string id)
{
	
	std::thread t([this, id]() {
		std::string url = "/api/replays/" + id;

		auto res = cli.Get(url.c_str(), GetAuthHeaders());
		if (res && res->status == 200)
		{
			json j = json::parse(res->body);
			std::string status = j["status"].get<std::string>();

			try {
				auto replayDetails = j.get<ReplayData>();
				OnReplayDetailsSuccess(replayDetails);
			}
			catch (const std::exception & e) {
				OnError("Check console for details");
				cvar_->log(e.what());
			}	
		}
		else {
			OnError("Check console for details");
			cvar_->log("GetReplayDetails result was null");
		}
		});
	t.detach();
}

void BallchasingAPI::OnReplayDetailsSuccess(ReplayData details)
{
	// Fix some inconsistent stuff for the api.
	for (auto& p : details.blue.players){
		p.score = p.stats.core.score;
		details.blue.goals += p.stats.core.goals;
	}
	if (details.blue.name.empty()) details.blue.name = "Blue";
	for (auto& p : details.orange.players) {
		p.score = p.stats.core.score;
		details.orange.goals += p.stats.core.goals;
	}
	if (details.orange.name.empty()) details.orange.name = "Orange";

	auto id = details.id;
	auto it = replayDetailsCache_.find(id);
	if (it != replayDetailsCache_.end())
	{
		replayDetailsCache_[id] = details;
	}
	else {
		replayDetailsCache_.emplace(id, details);
	}
}


httplib::Headers BallchasingAPI::GetAuthHeaders()
{
	std::string authKey = cvar_->getCvar("cl_autoreplayupload_ballchasing_authkey").getStringValue();
	return httplib::Headers { { "Authorization", authKey } };
}
