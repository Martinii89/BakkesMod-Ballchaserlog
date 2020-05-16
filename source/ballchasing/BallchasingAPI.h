#pragma once
#include <string>
#include <vector>
#include <memory>
#include "bakkesmod/wrappers/cvarmanagerwrapper.h"
#include "../http/httplib.h"
#include "APIDataClasses.h"

class BallchasingAPI
{
public:
	BallchasingAPI(std::shared_ptr<CVarManagerWrapper> cvar, std::shared_ptr<GameWrapper> gw_);

	//std::vector<ReplayData> replayList;
	//std::vector<GroupData> topLevelGroups;
	std::vector<std::string> topLevelGroups;
	
	// API calls
	void Ping();
	void GetLastMatches();
	void GetReplayDetails(std::string id);

	// Group stuff
	void GetTopLevelGroups();
	void GetReplaysForGroup(std::string id);
	void GetGroupStats(std::string id);
	void GetSubGroups(std::string groupID);

	// Get cached results
	ReplayData GetCachedReplayDetail(std::string replayID, std::string groupID);
	GroupData* GetCachedGroup(std::string id);

private:
	std::map<std::string, ReplayData> replayDetailsCache_;
	std::map<std::string, GroupData> groupCache_;
	std::shared_ptr<CVarManagerWrapper> cvar_;
	std::shared_ptr<GameWrapper> gw_;
	httplib::SSLClient cli;
	
	ReplayData GetTemporaryOverviewData(std::string replayID, std::string groupID);

	httplib::Headers GetAuthHeaders();
	// API response callbacks 
	void OnGetReplayGroupsSuccess(GetReplayGroupsResponseData res, std::string parentGroup = "");
	void OnReplayDetailsSuccess(ReplayData details);

	void OnGotReplayList(std::vector<ReplayData> replayList, std::string groupId);
	void OnGotGroupStats(GroupData data);

	GroupData* FindGroupById(std::string groupId, bool& found);

};

