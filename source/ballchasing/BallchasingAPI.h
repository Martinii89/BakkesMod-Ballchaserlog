#pragma once
#include <memory>
#include <string>
#include <vector>
#include "APIDataClasses.h"
#include "bakkesmod/wrappers/cvarmanagerwrapper.h"

class GameWrapper;

class BallchasingAPI
{
public:
	BallchasingAPI(std::shared_ptr<CVarManagerWrapper> cvar, std::shared_ptr<GameWrapper> gw);
	static std::string GetApiPath(std::string_view sub_path);

	std::vector<std::string> topLevelGroups;
	
	// API calls
	void Ping();
	void GetLastMatches();
	void GetReplayDetails(const std::string& id);

	// Group stuff
	void GetTopLevelGroups();
	void GetReplaysForGroup(const std::string& id);
	void GetGroupStats(const std::string& id);
	void GetSubGroups(const std::string& groupID);
	void AddReplayToGroup(const std::string& replayID, const std::string& groupID);
	void AssignReplays(const std::string& groupId, const std::vector<std::string>& addReplays, const std::vector<std::string>
	                   & removeReplays);
	void CreateGroup(const std::string& groupName, const std::string& parentGroupId = "");
	void DeleteGroup(const std::string& groupID);

	void OnError(const std::string& message);
	void OnOk(std::string message);

	// Get cached results
	ReplayData GetCachedReplayDetail(const std::string& replayID, std::string groupID);
	GroupData* GetCachedGroup(std::string id);

private:
	std::map<std::string, ReplayData> replay_details_cache_;
	std::map<std::string, GroupData> group_cache_;
	std::shared_ptr<CVarManagerWrapper> cvar_;
	std::shared_ptr<GameWrapper> gw_;
	
	ReplayData GetTemporaryOverviewData(const std::string& replay_id, std::string group_id);

	std::map<std::string, std::string> GetAuthHeaders();
	// API response callbacks 
	void OnGetReplayGroupsSuccess(const GetReplayGroupsResponseData& res, const std::string& parent_group = "");
	void OnReplayDetailsSuccess(ReplayData details);

	void OnGotReplayList(std::vector<ReplayData> replay_list, const std::string& group_id);
	void OnGotGroupStats(const GroupData& data);

	GroupData* FindGroupById(const std::string& group_id, bool& found);

	inline static constexpr std::string_view api_root = "ballchasing.com";
	CurlRequest GetRequestBase(std::string_view path, std::string_view http_verb);


};
