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

	void Ping();

	void GetLastMatches();
	std::vector<GetReplayResponseData> lastMatchesResult;

	GetReplayResponseData GetCachedDetail(std::string id);



private:
	std::map<std::string, GetReplayResponseData> detailsCache_;
	std::shared_ptr<CVarManagerWrapper> cvar_;
	std::shared_ptr<GameWrapper> gw_;
	httplib::SSLClient cli;

	GetReplayResponseData GetTemporaryOverviewData(std::string id);
	void OnLastMatches(GetReplaysResponse res);
	void OnReplayDetails(GetReplayResponseData details);
	void GetReplayDetails(std::string id);
	httplib::Headers GetAuthHeaders();
};

