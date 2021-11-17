#pragma once
#include <memory>
#include <string>
#include <vector>
#include "APIDataClasses.h"
#include "ApiRequestStructs.h"
#include "bakkesmod/wrappers/cvarmanagerwrapper.h"

class GameWrapper;

class BallchasingAPI
{
public:
	BallchasingAPI(std::shared_ptr<CVarManagerWrapper> cvar, std::shared_ptr<GameWrapper> gw);
	static std::string GetApiPath(std::string_view sub_path);

	std::vector<std::string> topLevelGroups;

	// API calls
	void Ping() const;
	void GetLastMatches() const;
	void GetReplayDetails(std::string_view id) const;

	// Group stuff
	void GetGroups(const GetGroupsParms& params, const std::function<void(json& j,const GroupList& groups)>& on_success) const;
	void GetGroupStats(std::string_view id) const;
	void GetReplaysForGroup(std::string_view id) const;

	void GetTopLevelGroups();
	void GetSubGroups(std::string_view group_id) const;
	void AddReplayToGroup(std::string_view replay_id, std::string_view group_id) const;
	void AssignReplays(std::string_view group_id, const std::vector<std::string>& addReplays,
	                   const std::vector<std::string>
	                   & removeReplays) const;
	void CreateGroup(const std::string& groupName, const std::string& parentGroupId = "");
	void DeleteGroup(const std::string& groupID) const;

	void OnError(const std::string& message) const;
	void OnOk(std::string message) const;

	// Get cached results
	ReplayData GetCachedReplayDetail(std::string_view replay_id, std::string_view group_id);
	GroupList::GroupData* GetCachedGroup(std::string_view id);

private:
	std::map<std::string, ReplayData, std::less<>> replay_details_cache_;
	std::map<std::string, GroupList::GroupData, std::less<>> group_cache_;
	std::shared_ptr<CVarManagerWrapper> cvar_;
	std::shared_ptr<GameWrapper> gw_;

	ReplayData GetTemporaryOverviewData(std::string_view replay_id, std::string_view group_id);

	[[nodiscard]] std::map<std::string, std::string> GetAuthHeaders() const;
	// API response callbacks 
	void OnGetReplayGroupsSuccess(const GroupList& res, std::string_view parent_group = "");
	void OnReplayDetailsSuccess(ReplayData details);

	void OnGotReplayList(std::vector<ReplayData> replay_list, std::string_view group_id);
	void OnGotGroupStats(const GroupData& data);

	GroupList::GroupData* FindGroupById(std::string_view group_id, bool& found);

	inline static constexpr std::string_view api_root = "ballchasing.com";
	[[nodiscard]] CurlRequest GetRequestBase(std::string_view path, std::string_view http_verb) const;

	void DefaultOnError(int code, const std::string& msg) const;
	[[nodiscard]] bool WriteJsonToDebugFile(const json& j, const std::string& endpoint_name) const;

	template <typename JsonParsedType>
	[[nodiscard]] bool WriteJsonAndParsedToDebugFile(const json& j, const JsonParsedType& parsed,
	                                                 const std::string& endpoint_name) const;

	template <typename JsonType>
	struct JsonRequest
	{
		CurlRequest req;
		const std::function<void(json&, JsonType&)> on_success;
		const std::function<void(int, const std::string&)> on_error = nullptr;
		int expected_http_code = 200;
	};

	template <typename JsonType>
	void RequestJson(const JsonRequest<JsonType>& req) const;
};

template <typename JsonParsedType>
bool BallchasingAPI::WriteJsonAndParsedToDebugFile(const json& j, const JsonParsedType& parsed,
                                                   const std::string& endpoint_name) const
{
	const auto path = gw_->GetDataFolder() / "ballchasinglog" / fmt::format("{}.json", endpoint_name);;
	const auto path_parsed = gw_->GetDataFolder() / "ballchasinglog" / fmt::format("{}_parsed.json", endpoint_name);
	create_directories(path.parent_path());
	try
	{
		if (auto out = std::ofstream(path_parsed))
		{
			json j = parsed;
			out << j.dump(4);
		}
	}
	catch (...)
	{
		return false;
	}
	if (auto out = std::ofstream(path))
	{
		out << j.dump(4);
		LOG("Written json for {} to file", endpoint_name);
		return true;
	}
	return false;
}

template <typename JsonType>
void BallchasingAPI::RequestJson(const JsonRequest<JsonType>& req) const
{
	HttpWrapper::SendCurlRequest(req.req, [this, req](int http_code, const std::string& res)
	{
		auto on_error = req.on_error
			                ? req.on_error
			                : [this](const int code, const std::string msg)
			                {
				                DefaultOnError(code, msg);
			                };
		if (http_code != req.expected_http_code)
		{
			on_error(http_code, fmt::format("Wrong http response code (got {} expected {}", http_code, req.expected_http_code));
			return;
		}
		try
		{
			json j = json::parse(res.begin(), res.end());
			auto parsed = j.get<JsonType>();
			req.on_success(j, parsed);
			return;
		}
		catch (std::exception& e)
		{
			on_error(http_code, e.what());
		}
		catch (...)
		{
			on_error(http_code, "Unknown exception");
		}
	});
}
