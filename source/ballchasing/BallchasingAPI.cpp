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
	//cli.set_follow_location(true);
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

GroupData* BallchasingAPI::FindGroupById(std::string groupId, bool& found)
{
	auto groupFinder = groupCache_.find(groupId);
	if (groupFinder != groupCache_.end()) {
		found = true;
		return &groupFinder->second;
	}
	found = false;
	//cvar_->log("Failed to find group by ID: " + groupId);
	//static auto dummyData = GroupData();
	return nullptr;
}

void BallchasingAPI::OnGetReplayGroupsSuccess(GetReplayGroupsResponseData res, std::string parentGroupID)
{
	cvar_->log("OnGetReplayGroupsSuccess");
	if (parentGroupID.empty()) {
		// TODO: 
		// Update them rather than replace them if they already exist 
		// so we don't loose the subgroups and replay lists 
		if (topLevelGroups.size() > 1) {
			topLevelGroups.erase(topLevelGroups.begin() + 1, topLevelGroups.end());
		}
		for (auto group : res.list) {
			cvar_->log("Adding toplevel group: " + group.id);
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
		cvar_->log("Fetching the detailed object");
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
				gw_->Toast("Ballchasing log", "ERROR! Check console for details");
				cvar_->log(e.what());
			}
		}
		else {
			gw_->Toast("Ballchasing log", "ERROR! Check console for details");
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
				gw_->Toast("Ballchasing log", "ERROR! Check console for details");
				cvar_->log(e.what());
			}
		}
		else {
			gw_->Toast("Ballchasing log", "ERROR! Check console for details");
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
				auto groupList = j.get <GroupData>();
				cvar_->log("got grup stats");
			}
			catch (const std::exception & e) {
				gw_->Toast("Ballchasing log", "ERROR! Check console for details");
				cvar_->log(e.what());
			}
		}
		else {
			gw_->Toast("Ballchasing log", "ERROR! Check console for details");
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
				cvar_->log("Got subgroups");
				OnGetReplayGroupsSuccess(groupList, groupID);
			}
			catch (const std::exception & e) {
				gw_->Toast("Ballchasing log", "ERROR! Check console for details");
				cvar_->log(e.what());
			}
		}
		else {
			gw_->Toast("Ballchasing log", "ERROR! Check console for details");
			cvar_->log("GetSubGroups result was null");
		}
		});
	t.detach();
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
			if (status == "ok") {

				try {
					std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
					auto replayDetails = j.get<ReplayData>();
					std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
					auto dt = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
					//cvar_->log("Replay details parsed in : " + std::to_string(dt) + "[micro seconds]");
					OnReplayDetailsSuccess(replayDetails);
				}
				catch (const std::exception & e) {
					gw_->Toast("Ballchasing log", "ERROR! Check console for details");
					//gw_->Toast("Ballchasing parse error", e.what(), "default", 10);
					cvar_->log(e.what());
				}	
			}
			else if (status == "pending")
			{
				//retry
				
				GetReplayDetails(id);
			}
			else {
				//Failed replay. fail silenty for now
			}
		}
		else {
			gw_->Toast("Ballchasing log", "ERROR! Check console for details");
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
