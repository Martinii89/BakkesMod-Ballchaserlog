#pragma once
#include <string>
#include <vector>
#include "../json/json.hpp"

struct CoreStats {
	int shots = 0;
	int shots_against = 0;
	int goals = 0;
	int goals_against = 0;
	int saves = 0;
	int assists = 0;
	int score = 0;
	bool mvp = false;
	float shooting_percentage = 0;
};

struct BoostStats {
	int bpm = 0;
	float bcpm = 0;
	float avg_amount = 0;
	int amount_collected = 0;
	int amount_stolen = 0;
	int amount_collected_big = 0;
	int amount_stolen_big = 0;
	int amount_collected_small = 0;
	int amount_stolen_small = 0;
	int count_collected_big = 0;
	int count_stolen_big = 0;
	int count_collected_small = 0;
	int count_stolen_small = 0;
	int amount_overfill = 0;
	int amount_overfill_stolen = 0;
	int amount_used_while_supersonic = 0;
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
	int total_distance = 0;
	float time_supersonic_speed = 0;
	float time_boost_speed = 0;
	float time_slow_speed = 0;
	float time_ground = 0;
	float time_low_air = 0;
	float time_high_air = 0;
	float time_powerslide = 0;
	int count_powerslide = 0;
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
	int avg_distance_to_ball = 0;
	int avg_distance_to_ball_possession = 0;
	int avg_distance_to_ball_no_possession = 0;
	int avg_distance_to_mates = 0;
	float time_defensive_third = 0;
	float time_neutral_third = 0;
	float time_offensive_third = 0;
	float time_defensive_half = 0;
	float time_offensive_half = 0;
	float time_behind_ball = 0;
	float time_infront_ball = 0;
	float time_most_back = 0;
	float time_most_forward = 0;
	int goals_against_while_last_defender = 0;
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
	int inflicted = 0;
	int taken = 0;
};

struct PlayerStats
{
	CoreStats core;
	BoostStats boost;
	MovementStats movement;
	PositioningStats positioning;
	DemoStats demo;
};

struct Team
{
	std::string name;
	int goals = 0;
	struct Player {
		std::string name;
		struct Id {
			std::string platform;
			std::string id;
		};
		Id id;
		int score;
		PlayerStats stats;
	};
	std::vector<Player> players;
};
struct GetReplayResponseData
{
	std::string replay_title;
	std::string id;
	std::string status;

	Team blue;
	Team orange;
};

struct GetReplaysResponse
{
	int count;
	std::vector<GetReplayResponseData> replays;
};



void from_json(const nlohmann::json& j, GetReplayResponseData& p);
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