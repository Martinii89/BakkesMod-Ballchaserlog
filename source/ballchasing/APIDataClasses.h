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
	float assists = 0;
	float goals = 0;
	float goals_against = 0;
	float saves = 0;
	float score = 0;
	float shooting_percentage = 0;
	float shots = 0;
	float shots_against = 0;
	int mvp = 0;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_OPTIONAL(CoreStats, shots, shots_against, goals, goals_against, saves, assists,
                                            score, shooting_percentage, mvp)


struct CoreStatsTeam
{
	float assists = 0;
	float goals = 0;
	float goals_against = 0;
	float saves = 0;
	float score = 0;
	float shooting_percentage = 0;
	float shots = 0;
	float shots_against = 0;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_OPTIONAL(CoreStatsTeam, shots, shots_against, goals, goals_against, saves, assists,
                                            score, shooting_percentage)

struct BoostStats
{
	float amount_collected = 0;
	float amount_collected_big = 0;
	float amount_collected_small = 0;
	float amount_overfill = 0;
	float amount_overfill_stolen = 0;
	float amount_stolen = 0;
	float amount_stolen_big = 0;
	float amount_stolen_small = 0;
	float amount_used_while_supersonic = 0;
	float count_collected_big = 0;
	float count_collected_small = 0;
	float count_stolen_big = 0;
	float count_stolen_small = 0;
	float percent_full_boost = 0;
	float percent_zero_boost = 0;
	float time_boost_0_25 = 0;
	float time_boost_25_50 = 0;
	float time_boost_50_75 = 0;
	float time_boost_75_100 = 0;
	float time_full_boost = 0;
	float time_zero_boost = 0;
	// Following properties are missing for team stats
	float avg_amount = 0;
	float bcpm = 0;
	float bpm = 0;
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


struct BoostStatsTeamAverage
{
	float amount_collected = 0;
	float amount_collected_big = 0;
	float amount_collected_small = 0;
	float amount_overfill = 0;
	float amount_overfill_stolen = 0;
	float amount_stolen = 0;
	float amount_stolen_big = 0;
	float amount_stolen_small = 0;
	float amount_used_while_supersonic = 0;
	float count_collected_big = 0;
	float count_collected_small = 0;
	float count_stolen_big = 0;
	float count_stolen_small = 0;
	float percent_full_boost = 0;
	float percent_zero_boost = 0;
	float time_boost_0_25 = 0;
	float time_boost_25_50 = 0;
	float time_boost_50_75 = 0;
	float time_boost_75_100 = 0;
	float time_full_boost = 0;
	float time_zero_boost = 0;
	float avg_amount = 0;
	float bcpm = 0;
	float bpm = 0;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_OPTIONAL(BoostStatsTeamAverage, bpm, bcpm, avg_amount, amount_collected, amount_stolen,
	amount_collected_big, amount_stolen_big, amount_collected_small,
	amount_stolen_small, count_collected_big, count_stolen_big,
	count_collected_small, count_stolen_small, amount_overfill,
	amount_overfill_stolen, amount_used_while_supersonic, time_zero_boost,
	percent_zero_boost, time_full_boost, percent_full_boost, time_boost_0_25,
	time_boost_25_50, time_boost_50_75, time_boost_75_100);

struct BoostStatsTeamCumulative
{
	float amount_collected = 0;
	float amount_collected_big = 0;
	float amount_collected_small = 0;
	float amount_overfill = 0;
	float amount_overfill_stolen = 0;
	float amount_stolen = 0;
	float amount_stolen_big = 0;
	float amount_stolen_small = 0;
	float amount_used_while_supersonic = 0;
	float count_collected_big = 0;
	float count_collected_small = 0;
	float count_stolen_big = 0;
	float count_stolen_small = 0;
	float percent_full_boost = 0;
	float percent_zero_boost = 0;
	float time_boost_0_25 = 0;
	float time_boost_25_50 = 0;
	float time_boost_50_75 = 0;
	float time_boost_75_100 = 0;
	float time_full_boost = 0;
	float time_zero_boost = 0;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_OPTIONAL(BoostStatsTeamCumulative,  amount_collected, amount_stolen,
                                            amount_collected_big, amount_stolen_big, amount_collected_small,
                                            amount_stolen_small, count_collected_big, count_stolen_big,
                                            count_collected_small, count_stolen_small, amount_overfill,
                                            amount_overfill_stolen, amount_used_while_supersonic, time_zero_boost,
                                            percent_zero_boost, time_full_boost, percent_full_boost, time_boost_0_25,
                                            time_boost_25_50, time_boost_50_75, time_boost_75_100);


struct MovementStats
{
	float count_powerslide = 0;
	float time_boost_speed = 0;
	float time_ground = 0;
	float time_high_air = 0;
	float time_low_air = 0;
	float time_powerslide = 0;
	float time_slow_speed = 0;
	float time_supersonic_speed = 0;
	float total_distance = 0;
	// Following properties are missing for team stats
	float avg_powerslide_duration = 0;
	float avg_speed = 0;
	float avg_speed_percentage = 0;
	float percent_boost_speed = 0;
	float percent_ground = 0;
	float percent_high_air = 0;
	float percent_low_air = 0;
	float percent_slow_speed = 0;
	float percent_supersonic_speed = 0;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_OPTIONAL(MovementStats, avg_speed, total_distance, time_supersonic_speed,
                                            time_boost_speed, time_slow_speed, time_ground, time_low_air, time_high_air,
                                            time_powerslide, count_powerslide, avg_powerslide_duration,
                                            avg_speed_percentage, percent_slow_speed, percent_boost_speed,
                                            percent_supersonic_speed, percent_ground, percent_low_air,
                                            percent_high_air);

struct MovementStatsTeam
{
	float count_powerslide = 0;
	float time_boost_speed = 0;
	float time_ground = 0;
	float time_high_air = 0;
	float time_low_air = 0;
	float time_powerslide = 0;
	float time_slow_speed = 0;
	float time_supersonic_speed = 0;
	float total_distance = 0;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_OPTIONAL(MovementStatsTeam, total_distance, time_supersonic_speed,
                                            time_boost_speed, time_slow_speed, time_ground, time_low_air, time_high_air,
                                            time_powerslide, count_powerslide);

struct PositioningStats
{
	float avg_distance_to_ball = 0;
	float avg_distance_to_ball_no_possession = 0;
	float avg_distance_to_ball_possession = 0;
	float goals_against_while_last_defender = 0;
	float percent_behind_ball = 0;
	float percent_defensive_half = 0;
	float percent_defensive_third = 0;
	float percent_infront_ball = 0;
	float percent_neutral_third = 0;
	float percent_offensive_half = 0;
	float percent_offensive_third = 0;
	float time_behind_ball = 0;
	float time_closest_to_ball = 0;
	float time_defensive_half = 0;
	float time_defensive_third = 0;
	float time_farthest_from_ball = 0;
	float time_infront_ball = 0;
	float time_most_back = 0;
	float time_most_forward = 0;
	float time_neutral_third = 0;
	float time_offensive_half = 0;
	float time_offensive_third = 0;
	// Following properties only in ReplayDetails
	float percent_closest_to_ball = 0;
	float percent_farthest_from_ball = 0;
	float percent_most_back = 0;
	float percent_most_forward = 0;
	float avg_distance_to_mates = 0;
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

struct PositioningGroupPlayerStats
{
	float avg_distance_to_ball = 0;
	float avg_distance_to_ball_no_possession = 0;
	float avg_distance_to_ball_possession = 0;
	float goals_against_while_last_defender = 0;
	float percent_behind_ball = 0;
	float percent_defensive_half = 0;
	float percent_defensive_third = 0;
	float percent_infront_ball = 0;
	float percent_neutral_third = 0;
	float percent_offensive_half = 0;
	float percent_offensive_third = 0;
	float time_behind_ball = 0;
	float time_closest_to_ball = 0;
	float time_defensive_half = 0;
	float time_defensive_third = 0;
	float time_farthest_from_ball = 0;
	float time_infront_ball = 0;
	float time_most_back = 0;
	float time_most_forward = 0;
	float time_neutral_third = 0;
	float time_offensive_half = 0;
	float time_offensive_third = 0;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_OPTIONAL(PositioningGroupPlayerStats, avg_distance_to_ball, avg_distance_to_ball_possession,
                                            avg_distance_to_ball_no_possession,
                                            time_defensive_third, time_neutral_third, time_offensive_third,
                                            time_defensive_half, time_offensive_half, time_behind_ball,
                                            time_infront_ball, time_most_back, time_most_forward,
                                            goals_against_while_last_defender, time_closest_to_ball,
                                            time_farthest_from_ball, percent_defensive_third, percent_offensive_third,
                                            percent_neutral_third, percent_defensive_half, percent_offensive_half,
                                            percent_behind_ball, percent_infront_ball);


//"avg_distance_to_ball": 0,
//"avg_distance_to_ball_no_possession" : 0,
//"avg_distance_to_ball_possession" : 0,
//"time_behind_ball" : 2090.35,
//"time_defensive_half" : 1923.8198,
//"time_defensive_third" : 1442.63,
//"time_infront_ball" : 845.58,
//"time_neutral_third" : 913.07,
//"time_offensive_half" : 1012.10004,
//"time_offensive_third" : 580.22003

struct PositioningGroupTeamStats
{
	float avg_distance_to_ball = 0;
	float avg_distance_to_ball_no_possession = 0;
	float avg_distance_to_ball_possession = 0;
	float time_behind_ball = 0;
	float time_defensive_half = 0;
	float time_defensive_third = 0;
	float time_infront_ball = 0;
	float time_neutral_third = 0;
	float time_offensive_half = 0;
	float time_offensive_third = 0;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_OPTIONAL(PositioningGroupTeamStats, avg_distance_to_ball, avg_distance_to_ball_possession,
	avg_distance_to_ball_no_possession,
	time_defensive_third, time_neutral_third, time_offensive_third,
	time_defensive_half, time_offensive_half, time_behind_ball,
	time_infront_ball);

struct DemoStats
{
	float inflicted = 0;
	float taken = 0;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_OPTIONAL(DemoStats, inflicted, taken);

struct CameraSettings
{
	int distance = 0;
	int fov = 0;
	int height = 0;
	int pitch = 0;
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


struct CumulativePlayerStats
{
	CoreStats core;
	BoostStats boost;
	MovementStats movement;
	PositioningStats positioning;
	DemoStats demo;
	int games = 0;
	int wins = 0;
	float win_percentage = 0;
	int play_duration = 0;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_OPTIONAL(CumulativePlayerStats, core, boost, movement, positioning, demo, games,
                                            wins, win_percentage, play_duration);

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

struct BallchasingUser
{
	std::string avatar;
	std::string name;
	std::string profile_url;
	std::string steam_id;
};

struct GroupStatsCreator
{
	std::string avatar;
	std::string avatar_full; // seems to only exists for the GroupStats::creator (but still empty?)
	std::string avatar_medium; // seems to only exists for the GroupStats::Creator (but still empty?)
	std::string name;
	std::string profile_url;
	std::string steam_id;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_OPTIONAL(BallchasingUser, avatar, name, profile_url, steam_id);
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_OPTIONAL(GroupStatsCreator, avatar, avatar_full, avatar_medium, name, profile_url,
                                            steam_id);


struct ReplayList
{
	struct ReplayData
	{
		struct Rank
		{
			int division = 0;
			std::string id;
			std::string name;
			int tier = 0;
		};

		struct Team
		{
			struct Player
			{
				struct Id
				{
					std::string id;
					std::string platform;
				};

				float end_time = 0;
				Id id;
				bool mvp = false;
				std::string name;
				Rank rank;
				int score = 0;
				float start_time = 0;
			};

			int goals = 0;
			std::string name;
			std::vector<Player> players;
		};

		std::string created;
		std::string date;
		bool date_has_tz = false;
		int duration = 0;
		std::string id;
		std::string link;
		std::string map_code;
		std::string map_name;
		Rank max_rank;
		Rank min_rank;
		bool overtime = false;
		std::string playlist_id;
		std::string playlist_name;
		std::string replay_title;
		std::string rocket_league_id;
		int season = 0;
		std::string season_type;
		BallchasingUser uploader;
		std::string visibility;
		Team blue;
		Team orange;
	};

	int count = 0;
	std::vector<ReplayData> list;
	std::string next;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_OPTIONAL(ReplayList::ReplayData::Rank, division, id, name, tier);


NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_OPTIONAL(ReplayList::ReplayData::Team::Player::Id, id, platform);
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_OPTIONAL(ReplayList::ReplayData::Team::Player, end_time, id, mvp, name, rank, score,
                                            start_time);
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_OPTIONAL(ReplayList::ReplayData::Team, goals, name, players);

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_OPTIONAL(ReplayList::ReplayData, created, date, date_has_tz, duration, id, link,
                                            map_code, map_name, max_rank, min_rank, overtime, playlist_id,
                                            playlist_name, replay_title, rocket_league_id, season, season_type,
                                            uploader, visibility, blue, orange);
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_OPTIONAL(ReplayList, count, list, next)


struct GroupList
{
	struct GroupData
	{
		std::string created;
		int direct_replays = 0;
		std::string id;
		int indirect_replays = 0;
		std::string link;
		std::string name;
		std::string player_identification;
		bool shared = false;
		std::string team_identification;
		BallchasingUser user;
	};

	std::vector<GroupData> list;
};


NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_OPTIONAL(GroupList::GroupData, created, direct_replays, id, indirect_replays,
                                            link, name, player_identification, shared, team_identification, user);
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_OPTIONAL(GroupList, list)

struct GroupPlayerStatsCumulative
{
	CoreStats core;
	BoostStats boost;
	MovementStats movement;
	PositioningGroupPlayerStats positioning;
	DemoStats demo;
	int games = 0;
	int wins = 0;
	float win_percentage = 0;
	int play_duration = 0;
};

struct GroupPlayerStatsGameAverage
{
	CoreStats core;
	BoostStats boost;
	MovementStats movement;
	PositioningGroupPlayerStats positioning;
	DemoStats demo;
};

struct GroupTeamStatsCumulative
{
	CoreStatsTeam core;
	BoostStatsTeamCumulative boost;
	MovementStatsTeam movement;
	PositioningGroupTeamStats positioning;
	DemoStats demo;
	int games = 0;
	int wins = 0;
	float win_percentage = 0;
	int play_duration = 0;
};

struct GroupTeamStatsGameAverage
{
	CoreStatsTeam core;
	BoostStatsTeamAverage boost;
	MovementStatsTeam movement;
	PositioningGroupTeamStats positioning;
	DemoStats demo;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_OPTIONAL(GroupPlayerStatsCumulative, core, boost, movement, positioning, demo, games,
                                            wins, win_percentage, play_duration);
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_OPTIONAL(GroupPlayerStatsGameAverage, core, boost, movement, positioning, demo);

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_OPTIONAL(GroupTeamStatsCumulative, core, boost, movement, positioning, demo, games,
                                            wins, win_percentage, play_duration);
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_OPTIONAL(GroupTeamStatsGameAverage, core, boost, movement, positioning, demo);

struct GroupDetail
{
	struct Player
	{
		GroupPlayerStatsCumulative cumulative;
		GroupPlayerStatsGameAverage game_average;
		std::string name;
		std::string id;
		std::string platform;
		std::string team;
	};

	struct Team
	{
		std::string name;
		GroupTeamStatsCumulative cumulative;
		GroupTeamStatsGameAverage game_average;
		struct Player
		{
			std::string name;
			std::string id;
			std::string platform;
			std::string team;
		};
		std::vector<Player> players;
	};

	std::string created;
	GroupStatsCreator creator;
	std::string id;
	std::string link;
	std::string name;
	std::string player_identification;
	std::vector<Player> players;
	bool shared = false;
	std::string status;
	std::string team_identification;
	std::vector<Team> teams;
};


NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_OPTIONAL(GroupDetail::Player, name, id, platform, team, cumulative, game_average)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_OPTIONAL(GroupDetail::Team::Player, name, id, platform, team)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_OPTIONAL(GroupDetail::Team, name, players, cumulative, game_average)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_OPTIONAL(GroupDetail, created, creator, id, link, name, player_identification,
                                            players, shared, status, team_identification, teams)
