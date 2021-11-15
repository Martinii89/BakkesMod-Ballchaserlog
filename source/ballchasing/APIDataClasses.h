#pragma once
#include <string>
#include <vector>
#include "../json/json.hpp"
#include "../json/json_macro.h"

enum class RequestState
{
	UNKNOWN = 0,
	REQUESTED = 1,
	FAILED = 2,
	SUCCESS = 3
};

struct CoreStats
{
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

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_OPTIONAL(CoreStats, shots, shots_against, goals, goals_against, saves, assists,
	score, mvp, shooting_percentage)

struct BoostStats
{
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

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_OPTIONAL(BoostStats, bpm, bcpm, avg_amount, amount_collected, amount_stolen,
	amount_collected_big, amount_stolen_big, amount_collected_small,
	amount_stolen_small, count_collected_big, count_stolen_big,
	count_collected_small, count_stolen_small, amount_overfill,
	amount_overfill_stolen, amount_used_while_supersonic, time_zero_boost,
	percent_zero_boost, time_full_boost, percent_full_boost, time_boost_0_25,
	time_boost_25_50, time_boost_50_75, time_boost_75_100, percent_boost_0_25,
	percent_boost_25_50, percent_boost_50_75, percent_boost_75_100);

struct MovementStats
{
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

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_OPTIONAL(MovementStats, avg_speed, total_distance, time_supersonic_speed,
	time_boost_speed, time_slow_speed, time_ground, time_low_air, time_high_air,
	time_powerslide, count_powerslide, avg_powerslide_duration,
	avg_speed_percentage, percent_slow_speed, percent_boost_speed,
	percent_supersonic_speed, percent_ground, percent_low_air,
	percent_high_air);

struct PositioningStats
{
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

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_OPTIONAL(PositioningStats, avg_distance_to_ball, avg_distance_to_ball_possession,
	avg_distance_to_ball_no_possession, avg_distance_to_mates,
	time_defensive_third, time_neutral_third, time_offensive_third,
	time_defensive_half, time_offensive_half, time_behind_ball,
	time_infront_ball, time_most_back, time_most_forward,
	goals_against_while_last_defender, time_closest_to_ball,
	time_farthest_from_ball, percent_defensive_third, percent_offensive_third,
	percent_neutral_third, percent_defensive_half, percent_offensive_half,
	percent_behind_ball, percent_infront_ball, percent_most_back,
	percent_most_forward, percent_closest_to_ball, percent_farthest_from_ball);

struct DemoStats
{
	float inflicted = 0;
	float taken = 0;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_OPTIONAL(DemoStats, inflicted, taken);

struct CameraSettings
{
	int fov = 0;
	int height = 0;
	int pitch = 0;
	int distance = 0;
	float stiffness = 0;
	float swivel_speed = 0;
	float transition_speed = 0;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_OPTIONAL(CameraSettings, fov, height, pitch, distance, stiffness, swivel_speed,
	transition_speed);

struct PlayerStats
{
	CoreStats core;
	BoostStats boost;
	MovementStats movement;
	PositioningStats positioning;
	DemoStats demo;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_OPTIONAL(PlayerStats, core, boost, movement, positioning, demo);


struct CumulativePlayerStats : PlayerStats
{
	int games = 0;
	int wins = 0;
	float win_percentage = 0;
	int play_duration = 0;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_OPTIONAL(CumulativePlayerStats, games, wins, win_percentage, play_duration);

struct BaseStatPlayer
{
	std::string name;
	PlayerStats game_average;
	CumulativePlayerStats cumulative;
	CameraSettings camera;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_OPTIONAL(BaseStatPlayer, name, game_average, cumulative, camera);

struct Team
{
	std::string name;
	int goals = 0;

	struct Player : BaseStatPlayer
	{
		//std::string name;
		struct Id
		{
			std::string platform;
			std::string id;
		};

		Id id;
		int score = 0;

		//PlayerStats stats;
	};

	std::vector<Player> players;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_OPTIONAL(Team::Player::Id, id, platform);
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_OPTIONAL(Team::Player, id, score, name, game_average, cumulative, camera);
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_OPTIONAL(Team, name, goals, players);

struct ReplayData
{
	std::string replay_title;
	std::string id;
	std::string link;
	std::string status;

	Team blue;
	Team orange;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_OPTIONAL(ReplayData, replay_title, id, link, status, blue, orange);

struct GroupPlayer : BaseStatPlayer
{
	std::string platform;
	std::string id;
	std::string team;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_OPTIONAL(GroupPlayer, platform, id, team);



struct GroupData
{
	std::string id;
	std::string link;
	std::string name;
	std::string created;
	std::string status;
	bool shared = false;
	std::vector<GroupPlayer> players;
	RequestState subGroupsRequested = RequestState::UNKNOWN;
	std::vector<std::string> subgroups;
	std::vector<ReplayData> groupReplays;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_OPTIONAL(GroupData, id, link, name, created, status, shared, players);

struct GetReplaysResponse
{
	int count;
	std::vector<ReplayData> list;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_OPTIONAL(GetReplaysResponse, count, list)

struct GetReplayGroupsResponseData
{
	std::string next;
	std::vector<GroupData> list;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_OPTIONAL(GetReplayGroupsResponseData, next, list)

//
//void from_json(const nlohmann::json& j, ReplayData& p);
//void from_json(const nlohmann::json& j, GetReplaysResponse& p);
//void from_json(const nlohmann::json& j, Team& p);
//void from_json(const nlohmann::json& j, Team::Player& p);
//void from_json(const nlohmann::json& j, Team::Player::Id& p);
//void from_json(const nlohmann::json& j, PlayerStats& p);
//
//
//void from_json(const nlohmann::json& j, CoreStats& p);
//void from_json(const nlohmann::json& j, BoostStats& p);
//void from_json(const nlohmann::json& j, MovementStats& p);
//void from_json(const nlohmann::json& j, PositioningStats& p);
//void from_json(const nlohmann::json& j, DemoStats& p);
//void from_json(const nlohmann::json& j, CameraSettings& p);
//
//void from_json(const nlohmann::json& j, GetReplayGroupsResponseData& p);
//void from_json(const nlohmann::json& j, GroupData& p);
//void from_json(const nlohmann::json& j, GroupPlayer& p);
//void from_json(const nlohmann::json& j, CumulativePlayerStats& p);
