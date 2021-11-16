#include "pch.h"
#include <thread>
#include <utility>
#include "../json/json.hpp"

#include <fstream>
using json = nlohmann::json;
#include "BallchasingAPI.h"
#include "APIDataClasses.h"

BallchasingAPI::BallchasingAPI(std::shared_ptr<CVarManagerWrapper> cvar, std::shared_ptr<GameWrapper> gw) :
	cvar_(std::move(cvar)), gw_(std::move(gw))
{
	auto latestGroup = GroupData();
	latestGroup.id = "LATEST";
	latestGroup.name = "Your latest replays";
	latestGroup.subGroupsRequested = RequestState::SUCCESS;
	topLevelGroups.push_back(latestGroup.id);
	group_cache_[latestGroup.id] = latestGroup;
}

std::string BallchasingAPI::GetApiPath(std::string_view sub_path)
{
	return fmt::format("{}/{}", api_root, sub_path);
}

CurlRequest BallchasingAPI::GetRequestBase(const std::string_view path, const std::string_view http_verb)
{
	CurlRequest req;
	req.url = GetApiPath(path);
	req.headers = GetAuthHeaders();
	req.verb = http_verb;
	return req;
}

void BallchasingAPI::DefaultOnError(int code, const std::string& msg) const
{
	gw_->Toast("Ballchasing log", "ERROR! Check console for details");
	cvar_->log(msg);
}

bool BallchasingAPI::WriteJsonToDebugFile(const json& j, const std::string& endpoint_name) const
{
	const auto path = gw_->GetDataFolder() / "ballchasinglog" / (endpoint_name + ".json");
	create_directories(path.parent_path());
	if (auto out = std::ofstream(path))
	{
		out << j.dump(4);
		LOG("Written json for {} to file", endpoint_name);
		return true;
	}
	return false;
}

void BallchasingAPI::Ping()
{
	gw_->Toast("Ballchasing log", "pinging the Ballchasing API. Check console for results");

	const CurlRequestDoneStringReturn on_done = [this](int http_code, const std::string& res)
	{
		LOG("Ping: {}\n{}", http_code, res);
	};
	const CurlRequest req = GetRequestBase("api/", "GET");
	HttpWrapper::SendCurlRequest(req, on_done);
}

void BallchasingAPI::GetLastMatches()
{
	gw_->Toast("Ballchasing log", "Fetching your most recent replays");

	auto on_done = [this](const json& j, ReplayList& data)
	{
		LOG("Count: {}, list length: {}", data.count, data.list.size());
		if (!WriteJsonAndParsedToDebugFile(j, data, "last_replays"))
		{
			DefaultOnError(0, "Failed writing debug file for last_replays");
		}
		//OnGotReplayList(data.list, "LATEST");
	};
	const JsonRequest<ReplayList> json_req{
		GetRequestBase("api/replays?uploader=me", "GET"),
	std::move(on_done) };
	RequestJson(json_req);
}

void BallchasingAPI::GetReplayDetails(const std::string& id)
{
	auto on_done = [this, id](const json& j, const ReplayData& data)
	{
		if (!WriteJsonAndParsedToDebugFile(j, data, fmt::format("replay_details_{}", id)))
		{
			DefaultOnError(0, fmt::format("Failed writing debug file for replay_details_{}", id));
		}
		OnReplayDetailsSuccess(data);
	};
	const JsonRequest<ReplayData> json_request
	{
		 GetRequestBase("api/replays/" + id, "GET"),
		on_done
	};
	RequestJson(json_request);
}

void BallchasingAPI::GetTopLevelGroups()
{
	auto on_done = [this](const json& j, const GetReplayGroupsResponseData& data)
	{
		if (!WriteJsonAndParsedToDebugFile(j, data, "top_groups"))
		{
			DefaultOnError(0, "Failed writing debug file for top_groups");
		}
		OnGetReplayGroupsSuccess(data);
	};
	const JsonRequest<GetReplayGroupsResponseData> json_req
	{
		GetRequestBase("api/groups?creator=me", "GET"),
		on_done
	};
	RequestJson(json_req);
}

void BallchasingAPI::GetSubGroups(const std::string& groupID)
{
	auto on_done = [this, groupID](const json& j, const GetReplayGroupsResponseData& data)
	{
		if (!WriteJsonAndParsedToDebugFile(j, data, fmt::format("group_subgroups_{}", groupID)))
		{
			DefaultOnError(0, fmt::format("Failed writing debug file for group_subgroups_{}", groupID));
		}
		OnGetReplayGroupsSuccess(data, groupID);
	};
	const JsonRequest<GetReplayGroupsResponseData> json_request
	{
		 GetRequestBase("api/groups?group=" + groupID, "GET"),
		on_done
	};
	RequestJson(json_request);
}

void BallchasingAPI::GetReplaysForGroup(const std::string& id)
{
	auto on_done2 = [this, id](const json& j, const GetReplaysResponse& data)
	{
		if (!WriteJsonAndParsedToDebugFile(j, data, fmt::format("group_replays_{}", id)))
		{
			DefaultOnError(0, fmt::format("Failed writing debug file for group_replays_{}", id));
		}
		OnGotReplayList(data.list, id);
	};
	const JsonRequest<GetReplaysResponse> json_req
	{
		GetRequestBase("api/replays?group=" + id, "GET"),
		on_done2
	};
	RequestJson(json_req);
}

void BallchasingAPI::GetGroupStats(const std::string& id)
{
	auto on_done = [this, id](const json& j, const GroupData& data)
	{
		if (!WriteJsonAndParsedToDebugFile(j, data, fmt::format("group_stats_{}", id)))
		{
			DefaultOnError(0, fmt::format("Failed writing debug file for group_stats_{}", id));
		}
		OnGotGroupStats(data);
	};
	const JsonRequest<GroupData> json_request
	{
		GetRequestBase("api/groups/" + id, "GET"),
		on_done
	};
	RequestJson(json_request);
}

ReplayData BallchasingAPI::GetTemporaryOverviewData(const std::string& replay_id, std::string group_id)
{
	if (const auto groupPtr = GetCachedGroup(std::move(group_id)))
	{
		for (auto& d : groupPtr->groupReplays)
		{
			if (d.id == replay_id)
			{
				return d;
			}
		}
	}
	return ReplayData();
}


void BallchasingAPI::OnGotReplayList(std::vector<ReplayData> replay_list, const std::string& group_id)
{
	bool foundGroup;
	auto group = FindGroupById(group_id, foundGroup);
	if (!foundGroup)
	{
		cvar_->log("Failed to find the group to attach these replays to..( " + group_id + ")");
		return;
	}
	group->groupReplays = std::move(replay_list);
}

void BallchasingAPI::OnGotGroupStats(const GroupData& data)
{
	bool found;
	auto pCachedGroup = FindGroupById(data.id, found);
	if (found)
	{
		pCachedGroup->status = data.status;
		pCachedGroup->players = data.players;
	}
}

GroupData* BallchasingAPI::FindGroupById(const std::string& group_id, bool& found)
{
	auto groupFinder = group_cache_.find(group_id);
	if (groupFinder != group_cache_.end())
	{
		found = true;
		return &groupFinder->second;
	}
	found = false;
	return nullptr;
}

void BallchasingAPI::OnGetReplayGroupsSuccess(const GetReplayGroupsResponseData& res, const std::string& parent_group)
{
	if (parent_group.empty())
	{
		// TODO: 
		// Update them rather than replace them if they already exist 
		// so we don't loose the subgroups and replay lists 
		if (topLevelGroups.size() > 1)
		{
			topLevelGroups.erase(topLevelGroups.begin() + 1, topLevelGroups.end());
		}
		for (auto group : res.list)
		{
			topLevelGroups.push_back(group.id);
			group_cache_[group.id] = group;
		}
	}
	else
	{
		bool found;
		auto parent = FindGroupById(parent_group, found);
		if (found)
		{
			parent->subgroups.clear();
			for (auto sub : res.list)
			{
				parent->subgroups.push_back(sub.id);
				group_cache_[sub.id] = sub;
			}
			parent->subGroupsRequested = RequestState::SUCCESS;
		}
	}
}

ReplayData BallchasingAPI::GetCachedReplayDetail(const std::string& replayID, std::string groupID)
{
	auto it = replay_details_cache_.find(replayID);
	if (it != replay_details_cache_.end())
	{
		return it->second;
	}
	else
	{
		// Add a empty detail object to stop this from executing many more requests
		replay_details_cache_[replayID] = GetTemporaryOverviewData(replayID, std::move(groupID));
		// Start to fetch the details.
		GetReplayDetails(replayID);
	}
	return ReplayData();
}

GroupData* BallchasingAPI::GetCachedGroup(std::string id)
{
	bool found;
	auto group = FindGroupById(std::move(id), found);
	if (found)
	{
		return group;
	}
	cvar_->log("GetCachedGroup didn't find a cached gorup");
	return group;
}

// Call with empty GroupID to remove from group
void BallchasingAPI::AddReplayToGroup(const std::string& replayID, const std::string& groupID)
{
	const CurlRequestDoneStringReturn on_done = [this, groupID](int http_code, const std::string& res)
	{
		if (http_code == 204)
		{
			if (groupID.empty())
			{
				OnOk("Replay removed from group");
			}
			else
			{
				OnOk("Replay added to group");
			}
		}
		else
		{
			OnError("Check console for details");
			cvar_->log("GetReplayDetails result was null");
		}
	};

	json j;
	j["group"] = groupID;

	CurlRequest req = GetRequestBase("/api/replays/" + replayID, "PATCH");
	req.body = j.dump();
	HttpWrapper::SendCurlJsonRequest(req, on_done);
}

void BallchasingAPI::AssignReplays(const std::string& groupId, const std::vector<std::string>& addReplays,
	const std::vector<std::string>
	& removeReplays)
{
	const CurlRequestDoneStringReturn on_done = [this](int http_code, const std::string& res)
	{
		if (http_code == 204)
		{
			OnOk("Group updated. Please give the server some processing time before you request the group");
		}
		else
		{
			OnError("Check console for details");
			cvar_->log("AssignReplays result was null");
		}
	};

	json j;
	j["assign_replays"] = addReplays;
	j["unassign_replays"] = removeReplays;

	CurlRequest req = GetRequestBase("api/groups/" + groupId, "PATCH");
	req.body = j.dump();
	HttpWrapper::SendCurlJsonRequest(req, on_done);
}

void BallchasingAPI::CreateGroup(const std::string& groupName, const std::string& parentGroupId)
{
	bool sub_group = !parentGroupId.empty();
	const CurlRequestDoneStringReturn on_done = [this, sub_group, parentGroupId](int http_code, const std::string& res)
	{
		if (http_code == 201)
		{
			OnOk("Group created. Refreshing grups");
			if (sub_group)
			{
				gw_->SetTimeout([this, parentGroupId](GameWrapper* gw) { GetSubGroups(parentGroupId); }, 1.0);
			}
			else
			{
				gw_->SetTimeout([this](GameWrapper* gw) { GetTopLevelGroups(); }, 1.0);
			}
		}
		else
		{
			OnError("Check console for details");
			cvar_->log("CreateGroup result was null");
		}
	};

	json j;
	j["name"] = groupName;
	if (!parentGroupId.empty())
	{
		j["parent"] = parentGroupId;
	}
	j["player_identification"] = "by-id";
	j["team_identification"] = "by-distinct-players";

	CurlRequest req = GetRequestBase("api/groups", "POST");
	req.body = j.dump();
	HttpWrapper::SendCurlJsonRequest(req, on_done);
}

void BallchasingAPI::DeleteGroup(const std::string& groupID)
{
	const CurlRequestDoneStringReturn on_done = [this](int http_code, const std::string& res)
	{
		if (http_code == 204)
		{
			OnOk("Group deleted");
		}
		else
		{
			OnError("Check console for details");
			cvar_->log("DeleteGroup result was null");
		}
	};

	const CurlRequest req = GetRequestBase("api/groups/" + groupID, "DELETE");
	HttpWrapper::SendCurlJsonRequest(req, on_done);
}

void BallchasingAPI::OnError(const std::string& message)
{
	gw_->Toast("Ballchasing log", "Error: " + message, "default", 3.5, ToastType_Error);
}

void BallchasingAPI::OnOk(std::string message)
{
	gw_->Toast("Ballchasing log", std::move(message), "default", 3.5, ToastType_OK);
}



void BallchasingAPI::OnReplayDetailsSuccess(ReplayData details)
{
	// Fix some inconsistent stuff for the api.
	for (auto& p : details.blue.players)
	{
		p.score = p.game_average.core.score;
		details.blue.goals += p.game_average.core.goals;
	}
	if (details.blue.name.empty()) details.blue.name = "Blue";
	for (auto& p : details.orange.players)
	{
		p.score = p.game_average.core.score;
		details.orange.goals += p.game_average.core.goals;
	}
	if (details.orange.name.empty()) details.orange.name = "Orange";

	auto id = details.id;
	auto it = replay_details_cache_.find(id);
	if (it != replay_details_cache_.end())
	{
		replay_details_cache_[id] = details;
	}
	else
	{
		replay_details_cache_.emplace(id, details);
	}
}


std::map<std::string, std::string> BallchasingAPI::GetAuthHeaders()
{
	std::string authKey = cvar_->getCvar("cl_autoreplayupload_ballchasing_authkey").getStringValue();
	return { {"Authorization", authKey} };
}
