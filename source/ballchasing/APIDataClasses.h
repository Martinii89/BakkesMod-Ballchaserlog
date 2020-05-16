#pragma once
#include <string>
#include <vector>
#include "../json/json.hpp"

enum class RequestState {
	UNKNOWN = 0,
	REQUESTED = 1,
	FAILED = 2,
	SUCCESS = 3
};

struct CoreStats {
	float shots = 0;
	float shots_against = 0;
	float goals = 0;
	float goals_against = 0;
	float saves = 0;
	float assists = 0;
	float score = 0;
	float mvp = 0;
	float shooting_percentage = 0;
};

struct BoostStats {
	float bpm = 0;
	float bcpm = 0;
	float avg_amount = 0;
	float amount_collected = 0;
	float amount_stolen = 0;
	float amount_collected_big = 0;
	float amount_stolen_big = 0;
	float amount_collected_small = 0;
	float amount_stolen_small = 0;
	float count_collected_big = 0;
	float count_stolen_big = 0;
	float count_collected_small = 0;
	float count_stolen_small = 0;
	float amount_overfill = 0;
	float amount_overfill_stolen = 0;
	float amount_used_while_supersonic = 0;
	float time_zero_boost = 0;
	float percent_zero_boost = 0;
	float time_full_boost = 0;
	float percent_full_boost = 0;
	float time_boost_0_25 = 0;
	float time_boost_25_50 = 0;
	float time_boost_50_75 = 0;
	float time_boost_75_100 = 0;
	float percent_boost_0_25 = 0;
	float percent_boost_25_50 = 0;
	float percent_boost_50_75 = 0;
	float percent_boost_75_100 = 0;
};

struct MovementStats {
	float avg_speed = 0;
	float total_distance = 0;
	float time_supersonic_speed = 0;
	float time_boost_speed = 0;
	float time_slow_speed = 0;
	float time_ground = 0;
	float time_low_air = 0;
	float time_high_air = 0;
	float time_powerslide = 0;
	float count_powerslide = 0;
	float avg_powerslide_duration = 0;
	float avg_speed_percentage = 0;
	float percent_slow_speed = 0;
	float percent_boost_speed = 0;
	float percent_supersonic_speed = 0;
	float percent_ground = 0;
	float percent_low_air = 0;
	float percent_high_air = 0;
};

struct PositioningStats {
	float avg_distance_to_ball = 0;
	float avg_distance_to_ball_possession = 0;
	float avg_distance_to_ball_no_possession = 0;
	float avg_distance_to_mates = 0;
	float time_defensive_third = 0;
	float time_neutral_third = 0;
	float time_offensive_third = 0;
	float time_defensive_half = 0;
	float time_offensive_half = 0;
	float time_behind_ball = 0;
	float time_infront_ball = 0;
	float time_most_back = 0;
	float time_most_forward = 0;
	float goals_against_while_last_defender = 0;
	float time_closest_to_ball = 0;
	float time_farthest_from_ball = 0;
	float percent_defensive_third = 0;
	float percent_offensive_third = 0;
	float percent_neutral_third = 0;
	float percent_defensive_half = 0;
	float percent_offensive_half = 0;
	float percent_behind_ball = 0;
	float percent_infront_ball = 0;
	float percent_most_back = 0;
	float percent_most_forward = 0;
	float percent_closest_to_ball = 0;
	float percent_farthest_from_ball = 0;
};

struct DemoStats {
	float inflicted = 0;
	float taken = 0;
};

struct PlayerStats
{
	CoreStats core;
	BoostStats boost;
	MovementStats movement;
	PositioningStats positioning;
	DemoStats demo;
};

struct IStatPlayer {
	virtual std::string GetName() = 0;
	virtual PlayerStats& GetPlayerStats() = 0;
};




struct CumulativePlayerStats : PlayerStats
{
	int		games;
	int		wins;
	float	win_percentage;
	int		play_duration;
};

struct BaseStatPlayer {
	std::string name;
	PlayerStats stats;
	CumulativePlayerStats cumulative_stats;
};

struct Team
{
	std::string name;
	int goals = 0;
	struct Player : BaseStatPlayer {
		//std::string name;
		struct Id {
			std::string platform;
			std::string id;
		};
		Id id;
		int score;
		//PlayerStats stats;

	};
	std::vector<Player> players;
};

struct ReplayData
{
	std::string replay_title;
	std::string id;
	std::string status;

	Team blue;
	Team orange;
};

struct GroupPlayer:  BaseStatPlayer
{
	std::string platform;
	std::string id;
	//std::string name;
	std::string team;
	//CumulativePlayerStats cumulative;
	//PlayerStats game_average;

};



struct GroupData
{
	std::string id;
	std::string link;
	std::string name;
	std::string created;
	std::string status;
	bool shared;

	std::vector<GroupPlayer> players;

	RequestState subGroupsRequested = RequestState::UNKNOWN;
	std::vector<std::string> subgroups;
	std::vector<ReplayData> groupReplays;
};

struct GetReplaysResponse
{
	int count;
	std::vector<ReplayData> replays;
};

struct GetReplayGroupsResponseData
{
	std::string next;
	std::vector<GroupData> list;
};


void from_json(const nlohmann::json& j, ReplayData& p);
void from_json(const nlohmann::json& j, GetReplaysResponse& p);
void from_json(const nlohmann::json& j, Team& p);
void from_json(const nlohmann::json& j, Team::Player& p);
void from_json(const nlohmann::json& j, Team::Player::Id& p);
void from_json(const nlohmann::json& j, PlayerStats& p);


void from_json(const nlohmann::json& j, CoreStats& p);
void from_json(const nlohmann::json& j, BoostStats& p);
void from_json(const nlohmann::json& j, MovementStats& p);
void from_json(const nlohmann::json& j, PositioningStats& p);
void from_json(const nlohmann::json& j, DemoStats& p);

void from_json(const nlohmann::json& j, GetReplayGroupsResponseData& p);
void from_json(const nlohmann::json& j, GroupData& p);
void from_json(const nlohmann::json& j, GroupPlayer& p);
void from_json(const nlohmann::json& j, CumulativePlayerStats& p);

