#include "pch.h"
#include <thread>
#include <utility>
#include "../json/json.hpp"

#include <fstream>
using json = nlohmann::json;
#include "BallchasingAPI.h"
#include "APIDataClasses.h"

void hexchar(unsigned char c, unsigned char &hex1, unsigned char &hex2)
{
    hex1 = c / 16;
    hex2 = c % 16;
    hex1 += hex1 <= 9 ? '0' : 'a' - 10;
    hex2 += hex2 <= 9 ? '0' : 'a' - 10;
}

std::string urlencode(std::string_view s)
{
    const char *str = s.data();
    std::vector<char> v(s.size());
    v.clear();
    for (size_t i = 0, l = s.size(); i < l; i++)
    {
        char c = str[i];
        if ((c >= '0' && c <= '9') ||
            (c >= 'a' && c <= 'z') ||
            (c >= 'A' && c <= 'Z') ||
            c == '-' || c == '_' || c == '.' || c == '!' || c == '~' ||
            c == '*' || c == '\'' || c == '(' || c == ')')
        {
            v.push_back(c);
        }
        else if (c == ' ')
        {
            v.push_back('+');
        }
        else
        {
            v.push_back('%');
            unsigned char d1, d2;
            hexchar(c, d1, d2);
            v.push_back(d1);
            v.push_back(d2);
        }
    }

	return {v.cbegin(), v.cend()};
}

BallchasingAPI::BallchasingAPI(std::shared_ptr<CVarManagerWrapper> cvar, std::shared_ptr<GameWrapper> gw) :
	cvar_(std::move(cvar)), gw_(std::move(gw))
{
	auto latestGroup = GroupList::GroupData();
	latestGroup.id = "LATEST";
	latestGroup.name = "Your latest replays";
	//latestGroup.subGroupsRequested = RequestState::SUCCESS;
	topLevelGroups.push_back(latestGroup.id);
	group_cache_[latestGroup.id] = latestGroup;
}

std::string BallchasingAPI::GetApiPath(std::string_view sub_path)
{
	return fmt::format("{}/{}", api_root, sub_path);
}

CurlRequest BallchasingAPI::GetRequestBase(const std::string_view path, const std::string_view http_verb) const
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

void BallchasingAPI::Ping() const
{
	gw_->Toast("Ballchasing log", "pinging the Ballchasing API. Check console for results");

	const CurlRequestDoneStringReturn on_done = [this](int http_code, const std::string& res)
	{
		LOG("Ping: {}\n{}", http_code, res);
	};
	const CurlRequest req = GetRequestBase("api/", "GET");
	HttpWrapper::SendCurlRequest(req, on_done);
}

void BallchasingAPI::GetLastMatches() const
{
	gw_->Toast("Ballchasing log", "Fetching your most recent replays");

	auto on_done = [this](const json& j, ReplayList& data)
	{
		LOG("Count: {}, list length: {}", data.count, data.list.size());
		if (!WriteJsonAndParsedToDebugFile(j, data, "last_replays"))
		{
			DefaultOnError(0, "Failed writing debug file for last_replays");
		}
		GetReplayDetails(data.list[0].id);
		//OnGotReplayList(data.list, "LATEST");
	};
	const JsonRequest<ReplayList> json_req{
		GetRequestBase("api/replays?uploader=me", "GET"),
	std::move(on_done) };
	RequestJson(json_req);
}

void BallchasingAPI::GetReplayDetails(const std::string_view id) const
{
	auto on_done = [this, id](const json& j, const ReplayDetail& data)
	{
		if (!WriteJsonAndParsedToDebugFile(j, data, fmt::format("replay_details_{}", id)))
		{
			DefaultOnError(0, fmt::format("Failed writing debug file for replay_details_{}", id));
		}
		//OnReplayDetailsSuccess(data);
	};
	const JsonRequest<ReplayDetail> json_request
	{
		 GetRequestBase(fmt::format("api/replays/{}",  id), "GET"),
		on_done
	};
	RequestJson(json_request);
}

void BallchasingAPI::GetGroups(const GetGroupsParms& params,
	const std::function<void(json& j,const GroupList& groups)>& on_success) const
{
	std::vector<std::string> args;
	auto add_arg = [&args](std::string_view val, std::string_view param_name)mutable
	{
		if (!val.empty())
		{
			args.push_back(fmt::format("{}={}", param_name, val));
		}
	};
	add_arg(params.name, "name");
	add_arg(params.creator, "creator");
	add_arg(params.parent_group, "group");
	//todo: add created before and after
	add_arg(std::to_string(params.count), "count");
	//add_arg(params.sort_by == GetGroupsParms::SortBy::created ? "crated": "name", "sort-by");
	//add_arg(params.sort_dir == GetGroupsParms::SortDir::desc ? "desc": "asc", "sort-dir");
	const JsonRequest<GroupList> json_req
	{
		GetRequestBase("api/groups?"+ format("{}",fmt::join(args, "&")), "GET"),
		on_success
	};
	DEBUGLOG("Requesting: {}", json_req.req.url);
	RequestJson(json_req);
}

void BallchasingAPI::GetTopLevelGroups()
{
	auto on_done = [this](const json& j, const GroupList& data)
	{
		if (!WriteJsonAndParsedToDebugFile(j, data, "top_groups"))
		{
			DefaultOnError(0, "Failed writing debug file for top_groups");
		}
		OnGetReplayGroupsSuccess(data);
		if (!data.list.empty())
		{
			GetSubGroups(data.list[0].id);
		}
	};
	GetGroups({ .creator = "me" }, on_done);
}

void BallchasingAPI::GetSubGroups(const std::string_view group_id) const
{
	auto on_done = [this, group_id = std::string(group_id)](const json& j, const GroupList& data)
	{
		if (!WriteJsonAndParsedToDebugFile(j, data, fmt::format("group_subgroups_{}", group_id)))
		{
			DefaultOnError(0, fmt::format("Failed writing debug file for group_subgroups_{}", group_id));
		}
		if (!data.list.empty())
		{
			GetReplaysForGroup(data.list[0].id);
			GetGroupStats(data.list[0].id);
		}
		//OnGetReplayGroupsSuccess(data, groupID);
	};
	
	GetGroups({ .parent_group = group_id.data() }, on_done);
}

void BallchasingAPI::GetReplaysForGroup(const std::string_view id) const
{
	auto on_done = [this, id = std::string(id)](const json& j, const GetReplaysResponse& data)
	{
		if (!WriteJsonAndParsedToDebugFile(j, data, fmt::format("group_replays_{}", id)))
		{
			DefaultOnError(0, fmt::format("Failed writing debug file for group_replays_{}", id));
		}
		//OnGotReplayList(data.list, id);
	};
	const JsonRequest<GetReplaysResponse> json_req
	{
		GetRequestBase(fmt::format("api/replays?group={}", id), "GET"),
		on_done
	};
	RequestJson(json_req);
}

void BallchasingAPI::GetGroupStats(const std::string_view id) const
{
	LOG("[GetGroupStats]: {}", id);
	auto on_done = [this, id = std::string(id)](const json& j, const GroupDetail& data)
	{
		if (!WriteJsonAndParsedToDebugFile(j, data, fmt::format("group_stats_{}", id)))
		{
			DefaultOnError(0, fmt::format("Failed writing debug file for group_stats_{}", id));
		}
		//OnGotGroupStats(data);
	};
	const JsonRequest<GroupDetail> json_request
	{
		GetRequestBase(fmt::format("api/groups/{}", id), "GET"),
		on_done
	};
	LOG("[GetGroupStats]: Requesting: {}", json_request.req.url);
	RequestJson(json_request);
}

ReplayData BallchasingAPI::GetTemporaryOverviewData(const std::string_view replay_id, const std::string_view group_id)
{
	if (const auto groupPtr = GetCachedGroup(group_id))
	{
		//for (auto& d : groupPtr->groupReplays)
		//{
		//	if (d.id == replay_id)
		//	{
		//		return d;
		//	}
		//}
	}
	return {};
}


void BallchasingAPI::OnGotReplayList(std::vector<ReplayData> replay_list, std::string_view group_id)
{
	bool foundGroup;
	auto group = FindGroupById(group_id, foundGroup);
	if (!foundGroup)
	{
		LOG("Failed to find the group to attach these replays to..({})", group_id);
		return;
	}
	//group->groupReplays = std::move(replay_list);
}

void BallchasingAPI::OnGotGroupStats(const GroupData& data)
{
	bool found;
	auto pCachedGroup = FindGroupById(data.id, found);
	if (found)
	{
		//pCachedGroup->status = data.status;
		//pCachedGroup->players = data.players;
	}
}

GroupList::GroupData* BallchasingAPI::FindGroupById(const std::string_view group_id, bool& found)
{
	const auto groupFinder = group_cache_.find(group_id);
	if (groupFinder != group_cache_.end())
	{
		found = true;
		return &groupFinder->second;
	}
	found = false;
	return nullptr;
}

void BallchasingAPI::OnGetReplayGroupsSuccess(const GroupList& res, std::string_view parent_group)
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
		for (const auto& group : res.list)
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
			//parent->subgroups.clear();
			for (const auto& sub : res.list)
			{
				//parent->subgroups.push_back(sub.id);
				group_cache_[sub.id] = sub;
			}
			//parent->subGroupsRequested = RequestState::SUCCESS;
		}
	}
}

ReplayData BallchasingAPI::GetCachedReplayDetail(const std::string_view replay_id, const std::string_view group_id)
{
	auto it = replay_details_cache_.find(replay_id);
	if (it != replay_details_cache_.end())
	{
		return it->second;
	}
	else
	{
		// Add a empty detail object to stop this from executing many more requests
		replay_details_cache_[replay_id.data()] = GetTemporaryOverviewData(replay_id, group_id); 
		// Start to fetch the details.
		GetReplayDetails(replay_id);
	}
	return{};
}

GroupList::GroupData* BallchasingAPI::GetCachedGroup(const std::string_view id)
{
	bool found;
	const auto group = FindGroupById(id, found);
	if (found)
	{
		return group;
	}
	cvar_->log("GetCachedGroup didn't find a cached gorup");
	return group;
}

// Call with empty GroupID to remove from group
void BallchasingAPI::AddReplayToGroup(const std::string_view replay_id, const std::string_view group_id) const
{
	const CurlRequestDoneStringReturn on_done = [this, group_id](int http_code, const std::string& res)
	{
		if (http_code == 204)
		{
			if (group_id.empty())
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
	j["group"] = group_id;

	CurlRequest req = GetRequestBase(fmt::format("/api/replays/{}", replay_id), "PATCH");
	req.body = j.dump();
	HttpWrapper::SendCurlJsonRequest(req, on_done);
}

void BallchasingAPI::AssignReplays(const std::string_view group_id, const std::vector<std::string>& addReplays,
                                   const std::vector<std::string>
                                   & removeReplays) const
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

	CurlRequest req = GetRequestBase(fmt::format("api/groups/", group_id), "PATCH");
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

void BallchasingAPI::DeleteGroup(const std::string& groupID) const
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

void BallchasingAPI::OnError(const std::string& message) const
{
	gw_->Toast("Ballchasing log", "Error: " + message, "default", 3.5, ToastType_Error);
}

void BallchasingAPI::OnOk(std::string message) const
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


std::map<std::string, std::string> BallchasingAPI::GetAuthHeaders() const
{
	if (auto auth_key_cvar = cvar_->getCvar("cl_autoreplayupload_ballchasing_authkey"))
	{
		return { {"Authorization", auth_key_cvar.getStringValue()} };
	}
	return {};
	
}
