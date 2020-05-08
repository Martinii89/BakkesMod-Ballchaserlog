#include "pch.h"
#include "../json/json.hpp"
#include "APIDataClasses.h"

#define JOPTIONAL(var) if (j.find(#var) != j.end()){j.at(#var).get_to(p.var);}
#define JOPTIONAL2(var, var2) if (j.find(#var) != j.end()){j.at(#var).get_to(p.var2);}
#define J(var) j.at(#var).get_to(p.var);
#define J2(var, var2) j.at(#var).get_to(p.var2);


void from_json(const nlohmann::json & j, BaseReplayGroup & p) {
	JOPTIONAL(id);
	JOPTIONAL(name);
	JOPTIONAL(link);
	JOPTIONAL(created);
}


void from_json(const nlohmann::json & j, GetReplayGroupsResponseData & p) {
	JOPTIONAL(next);
	JOPTIONAL(list)
}

void from_json(const nlohmann::json& j, GetReplayResponseData& p) {
	JOPTIONAL(replay_title);
	JOPTIONAL2(title, replay_title);
	JOPTIONAL(status);
	JOPTIONAL(id);
	JOPTIONAL(blue);
	JOPTIONAL(orange);
}

void from_json(const nlohmann::json& j, GetReplaysResponse& p) {
	JOPTIONAL(count);
	JOPTIONAL2(list, replays);
}

void from_json(const nlohmann::json& j, Team& p)
{
	JOPTIONAL(name);
	JOPTIONAL(goals);
	JOPTIONAL(players);
}

void from_json(const nlohmann::json& j, Team::Player& p)
{
	JOPTIONAL(name);
	JOPTIONAL(score);
	JOPTIONAL(id);
	JOPTIONAL(stats);
}

void from_json(const nlohmann::json& j, Team::Player::Id& p)
{
	JOPTIONAL(id);
	JOPTIONAL(platform);
}

void from_json(const nlohmann::json& j, PlayerStats& p)
{
	JOPTIONAL(core);
	JOPTIONAL(boost);
	JOPTIONAL(movement);
	JOPTIONAL(positioning);
	JOPTIONAL(demo);
}

void from_json(const nlohmann::json& j, CoreStats& p)
{
	JOPTIONAL(shots);
	JOPTIONAL(shots_against);
	JOPTIONAL(goals);
	JOPTIONAL(goals_against);
	JOPTIONAL(saves);
	JOPTIONAL(assists);
	JOPTIONAL(score);
	JOPTIONAL(mvp);
	JOPTIONAL(shooting_percentage);
}

void from_json(const nlohmann::json& j, BoostStats& p)
{
	JOPTIONAL(bpm);
	JOPTIONAL(bcpm);
	JOPTIONAL(avg_amount);
	JOPTIONAL(amount_collected);
	JOPTIONAL(amount_stolen);
	JOPTIONAL(amount_collected_big);
	JOPTIONAL(amount_stolen_big);
	JOPTIONAL(amount_collected_small);
	JOPTIONAL(amount_stolen_small);
	JOPTIONAL(count_collected_big);
	JOPTIONAL(count_stolen_big);
	JOPTIONAL(count_collected_small);
	JOPTIONAL(count_stolen_small);
	JOPTIONAL(amount_overfill);
	JOPTIONAL(amount_overfill_stolen);
	JOPTIONAL(amount_used_while_supersonic);
	JOPTIONAL(time_zero_boost);
	JOPTIONAL(percent_zero_boost);
	JOPTIONAL(time_full_boost);
	JOPTIONAL(percent_full_boost);
	JOPTIONAL(time_boost_0_25);
	JOPTIONAL(time_boost_25_50);
	JOPTIONAL(time_boost_50_75);
	JOPTIONAL(time_boost_75_100);
	JOPTIONAL(percent_boost_0_25);
	JOPTIONAL(percent_boost_25_50);
	JOPTIONAL(percent_boost_50_75);
	JOPTIONAL(percent_boost_75_100);
}

void from_json(const nlohmann::json& j, MovementStats& p)
{
	JOPTIONAL(avg_speed);
	JOPTIONAL(total_distance);
	JOPTIONAL(time_supersonic_speed);
	JOPTIONAL(time_boost_speed);
	JOPTIONAL(time_slow_speed);
	JOPTIONAL(time_ground);
	JOPTIONAL(time_low_air);
	JOPTIONAL(time_high_air);
	JOPTIONAL(time_powerslide);
	JOPTIONAL(count_powerslide);
	JOPTIONAL(avg_powerslide_duration);
	JOPTIONAL(avg_speed_percentage);
	JOPTIONAL(percent_slow_speed);
	JOPTIONAL(percent_boost_speed);
	JOPTIONAL(percent_supersonic_speed);
	JOPTIONAL(percent_ground);
	JOPTIONAL(percent_low_air);
	JOPTIONAL(percent_high_air);
}

void from_json(const nlohmann::json& j, PositioningStats& p)
{
	JOPTIONAL(avg_distance_to_ball);
	JOPTIONAL(avg_distance_to_ball_possession);
	JOPTIONAL(avg_distance_to_ball_no_possession);
	JOPTIONAL(avg_distance_to_mates);
	JOPTIONAL(time_defensive_third);
	JOPTIONAL(time_neutral_third);
	JOPTIONAL(time_offensive_third);
	JOPTIONAL(time_defensive_half);
	JOPTIONAL(time_offensive_half);
	JOPTIONAL(time_behind_ball);
	JOPTIONAL(time_infront_ball);
	JOPTIONAL(time_most_back);
	JOPTIONAL(time_most_forward);
	JOPTIONAL(goals_against_while_last_defender);
	JOPTIONAL(time_closest_to_ball);
	JOPTIONAL(time_farthest_from_ball);
	JOPTIONAL(percent_defensive_third);
	JOPTIONAL(percent_offensive_third);
	JOPTIONAL(percent_neutral_third);
	JOPTIONAL(percent_defensive_half);
	JOPTIONAL(percent_offensive_half);
	JOPTIONAL(percent_behind_ball);
	JOPTIONAL(percent_infront_ball);
	JOPTIONAL(percent_most_back);
	JOPTIONAL(percent_most_forward);
	JOPTIONAL(percent_closest_to_ball);
	JOPTIONAL(percent_farthest_from_ball);
}

void from_json(const nlohmann::json& j, DemoStats& p)
{
	JOPTIONAL(inflicted);
	JOPTIONAL(taken);
}

void from_json(const nlohmann::json& j, GroupData& p)
{
	JOPTIONAL(id);
	JOPTIONAL(name);
	JOPTIONAL(created);
	JOPTIONAL(status);
	JOPTIONAL(players);
}

void from_json(const nlohmann::json& j, GroupPlayer& p)
{
	JOPTIONAL(platform);
	JOPTIONAL(id);
	JOPTIONAL(name);
	JOPTIONAL(team);
	JOPTIONAL(cumulative);
	JOPTIONAL(game_average);
}

void from_json(const nlohmann::json& j, CumulativePlayerStats& p)
{
	from_json(j, (PlayerStats&)p);
	JOPTIONAL(games);
	JOPTIONAL(wins);
	JOPTIONAL(win_percentage);
	JOPTIONAL(play_duration);
}


#undef J
#undef JOPTIONAL