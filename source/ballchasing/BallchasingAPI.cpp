#include "pch.h"
#include <thread>
#include "../json/json.hpp"
using json = nlohmann::json;
#include "BallchasingAPI.h"
#include "APIDataClasses.h"
#include <chrono>

BallchasingAPI::BallchasingAPI(std::shared_ptr<CVarManagerWrapper> cvar, std::shared_ptr<GameWrapper> gw): cli("ballchasing.com"), cvar_(cvar), gw_(gw)
{
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
	replayGroupResult.clear();
	gw_->Toast("Ballchasing log", "Fetching your most recent replays");
	std::thread t([this]() {
		std::string url = "/api/replays?uploader=me";
		

		auto res = cli.Get(url.c_str(), GetAuthHeaders());
		if (res && res->status == 200)
		{
			json j = json::parse(res->body);
			try {
				std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
				auto getReplays = j.get<GetReplaysResponse>();
				std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
				auto dt = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
				//cvar_->log("Replay list parsed in : " + std::to_string(dt) + "[micro seconds]");
				onReplayGroupChange(getReplays);
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

GetReplayResponseData BallchasingAPI::GetTemporaryOverviewData(std::string id)
{
	for (auto& d : replayGroupResult)
	{
		if (d.id == id) {
			return d;
		}
	}
	return GetReplayResponseData();
}

void BallchasingAPI::onReplayGroupChange(GetReplaysResponse res)
{
	replayGroupResult = res.replays;
}

void BallchasingAPI::OnGetReplayGroups(GetReplayGroupsResponseData res)
{
	replayGroupsList = res.list;
}

GetReplayResponseData BallchasingAPI::GetCachedDetail(std::string id)
{
	auto it = detailsCache_.find(id);
	if (it != detailsCache_.end())
	{
		return it->second;
	}
	else {
		cvar_->log("Fetching the detailed object");
		// Add a empty detail object to stop this from executing many more requests
		detailsCache_[id] = GetTemporaryOverviewData(id);
		// Start to fetch the details.
		GetReplayDetails(id);

	}
	return GetReplayResponseData();
}

void BallchasingAPI::GetToplevelGroups()
{
	std::thread t([this]() {
		std::string url = "/api/groups?creator=me";

		auto res = cli.Get(url.c_str(), GetAuthHeaders());
		if (res && res->status == 200)
		{
			json j = json::parse(res->body);
			try {
				std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
				auto groupList = j.get<GetReplayGroupsResponseData>();
				//cvar_->log("Replay list: " + groupList.list[0].name);
				std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
				auto dt = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
				//cvar_->log("Replay list parsed in : " + std::to_string(dt) + "[micro seconds]");
				OnGetReplayGroups(groupList);
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

void BallchasingAPI::GetReplayGroupMatches(std::string id)
{
	replayGroupResult.clear();
	std::thread t([this, id]() {
		std::string url = "/api/replays?group=" + id;

		auto res = cli.Get(url.c_str(), GetAuthHeaders());
		if (res && res->status == 200)
		{
			//cvar_->log("Replay list first 2k res->body: " + res->body.substr(0, 2000));
			json j = json::parse(res->body);
			try {
				std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
				auto getReplays = j.get<GetReplaysResponse>();
				std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
				auto dt = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
				//cvar_->log("Replay list parsed in : " + std::to_string(dt) + "[micro seconds]");
				onReplayGroupChange(getReplays);
			}
			catch (const std::exception& e) {
				gw_->Toast("Ballchasing log", "ERROR! Check console for details");
				//gw_->Toast("Ballchasing parse error", e.what(), "default", 10);
				cvar_->log(e.what());
			}
		}
		else {
			gw_->Toast("Ballchasing log", "ERROR! Check console for details");
			cvar_->log("GetReplayDetails result was null");
		}
	});
	t.detach();
}

void BallchasingAPI::GetGroupStats()
{
	std::thread t([this]() {
		std::string url = "/api/groups/21-2-04-2020-tf01r9mjhz";

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
					auto replayDetails = j.get<GetReplayResponseData>();
					std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
					auto dt = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
					//cvar_->log("Replay details parsed in : " + std::to_string(dt) + "[micro seconds]");
					OnReplayDetails(replayDetails);
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

void BallchasingAPI::OnReplayDetails(GetReplayResponseData details)
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
	auto it = detailsCache_.find(id);
	if (it != detailsCache_.end())
	{
		detailsCache_[id] = details;
	}
	else {
		detailsCache_.emplace(id, details);
	}
}


httplib::Headers BallchasingAPI::GetAuthHeaders()
{
	std::string authKey = cvar_->getCvar("cl_autoreplayupload_ballchasing_authkey").getStringValue();
	return httplib::Headers { { "Authorization", authKey } };
}
