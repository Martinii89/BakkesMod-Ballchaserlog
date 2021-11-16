#pragma once
#include <string>
#include <chrono>

struct GetGroupsParms
{
	enum class SortBy { created, name };

	enum class SortDir { asc, desc };

	std::string name;
	std::string creator;
	std::string parent_group;
	std::chrono::time_point<std::chrono::system_clock> created_before;
	std::chrono::time_point<std::chrono::system_clock> created_after;
	int count = 150;
	SortBy sort_by = SortBy::created;
	SortDir sort_dir = SortDir::desc;
};

struct CreateGroupParams
{
	enum class PlayerIdentification { ById, ByName };

	enum class TeamIdentification { ById, ByName };

	std::string name;
	std::string parent;
	PlayerIdentification player_identification;
	TeamIdentification team_identification;
};

NLOHMANN_JSON_SERIALIZE_ENUM(CreateGroupParams::PlayerIdentification,
                             {
                             {CreateGroupParams::PlayerIdentification::ById, "by-id"},
                             {CreateGroupParams::PlayerIdentification::ByName, "by-name"},
                             })

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_OPTIONAL(CreateGroupParams, name, parent, player_identification, team_identification);