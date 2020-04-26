#include "pch.h"
#include <thread>
#include "../json/json.hpp"
using json = nlohmann::json;
#include "BallchasingAPI.h"
#include "APIDataClasses.h"

BallchasingAPI::BallchasingAPI(std::shared_ptr<CVarManagerWrapper> cvar): cli("ballchasing.com"), cvar_(cvar)
{
	//cli.set_follow_location(true);
}

void BallchasingAPI::Ping()
{
	cvar_->log("ping called");
	std::thread t([this]() {
		std::string url = "/api/";
		
		cli.set_follow_location(true);
		auto res = cli.Get(url.c_str(), GetAuthHeaders());
		if (res)
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
	std::thread t([this]() {
		std::string url = "/api/replays?uploader=me";

		auto res = cli.Get(url.c_str(), GetAuthHeaders());
		if (res && res->status == 200)
		{
			json j = json::parse(res->body);
			try {
				auto getReplays = j.get<GetReplaysResponse>();
				cvar_->log("Got replays: " + std::to_string(getReplays.count));
				cvar_->log("Got replaysList: " + std::to_string(getReplays.replays.size()));
				OnLastMatches(getReplays);
			}
			catch (const std::exception & e) {
				cvar_->log(e.what());
			}
		}
		else {
			cvar_->log("GetLastMatches result was null");
		}
		});
	t.detach();
}

GetReplayResponseData BallchasingAPI::GetTemporaryOverviewData(std::string id)
{
	for (auto& d : lastMatchesResult)
	{
		if (d.id == id) {
			return d;
		}
	}
	return GetReplayResponseData();
}

void BallchasingAPI::OnLastMatches(GetReplaysResponse res)
{
	lastMatchesResult = res.replays;
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
					auto replayDetails = j.get<GetReplayResponseData>();
					OnReplayDetails(replayDetails);
				}
				catch (const std::exception & e) {
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
