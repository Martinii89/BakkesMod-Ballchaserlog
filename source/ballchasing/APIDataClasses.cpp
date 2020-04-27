#include "pch.h"
#include "../json/json.hpp"
#include "APIDataClasses.h"
using json = nlohmann::json;

#define JOPTIONAL(var) if (j.find(#var) != j.end()){j.at(#var).get_to(p.var);}
#define JOPTIONAL2(var, var2) if (j.find(#var) != j.end()){j.at(#var).get_to(p.var2);}
#define J(var) j.at(#var).get_to(p.var);
#define J2(var, var2) j.at(#var).get_to(p.var2);


void from_json(const json& j, GetReplayResponseData& p) {
	JOPTIONAL(replay_title);
	JOPTIONAL2(title, replay_title);
	JOPTIONAL(status);
	J(id);
	J(blue);
	J(orange);
}

void from_json(const json& j, GetReplaysResponse& p) {
	J(count);
	J2(list, replays);
}

void from_json(const nlohmann::json& j, Team& p)
{
	JOPTIONAL(name);
	JOPTIONAL(goals);
	J(players);
}

void from_json(const nlohmann::json& j, Team::Player& p)
{
	J(name);
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
	J(core);
	J(boost);
	J(movement);
	J(positioning);
	J(demo);
}

void from_json(const nlohmann::json& j, CoreStats& p)
{
	J(shots);
	J(shots_against);
	J(goals);
	J(goals_against);
	J(saves);
	J(assists);
	J(score);
	J(mvp);
	J(shooting_percentage);
}

void from_json(const nlohmann::json& j, BoostStats& p)
{
	J(bpm);
	J(bcpm);
	J(avg_amount);
	J(amount_collected);
	J(amount_stolen);
	J(amount_collected_big);
	J(amount_stolen_big);
	J(amount_collected_small);
	J(amount_stolen_small);
	J(count_collected_big);
	J(count_stolen_big);
	J(count_collected_small);
	J(count_stolen_small);
	J(amount_overfill);
	J(amount_overfill_stolen);
	J(amount_used_while_supersonic);
	J(time_zero_boost);
	J(percent_zero_boost);
	J(time_full_boost);
	J(percent_full_boost);
	J(time_boost_0_25);
	J(time_boost_25_50);
	J(time_boost_50_75);
	J(time_boost_75_100);
	J(percent_boost_0_25);
	J(percent_boost_25_50);
	J(percent_boost_50_75);
	J(percent_boost_75_100);
}

void from_json(const nlohmann::json& j, MovementStats& p)
{
	J(avg_speed);
	J(total_distance);
	J(time_supersonic_speed);
	J(time_boost_speed);
	J(time_slow_speed);
	J(time_ground);
	J(time_low_air);
	J(time_high_air);
	J(time_powerslide);
	J(count_powerslide);
	J(avg_powerslide_duration);
	J(avg_speed_percentage);
	J(percent_slow_speed);
	J(percent_boost_speed);
	J(percent_supersonic_speed);
	J(percent_ground);
	J(percent_low_air);
	J(percent_high_air);
}

void from_json(const nlohmann::json& j, PositioningStats& p)
{
	J(avg_distance_to_ball);
	J(avg_distance_to_ball_possession);
	J(avg_distance_to_ball_no_possession);
	J(avg_distance_to_mates);
	J(time_defensive_third);
	J(time_neutral_third);
	J(time_offensive_third);
	J(time_defensive_half);
	J(time_offensive_half);
	J(time_behind_ball);
	J(time_infront_ball);
	JOPTIONAL(time_most_back);
	JOPTIONAL(time_most_forward);
	JOPTIONAL(goals_against_while_last_defender);
	JOPTIONAL(time_closest_to_ball);
	JOPTIONAL(time_farthest_from_ball);
	J(percent_defensive_third);
	J(percent_offensive_third);
	J(percent_neutral_third);
	J(percent_defensive_half);
	J(percent_offensive_half);
	J(percent_behind_ball);
	J(percent_infront_ball);
	JOPTIONAL(percent_most_back);
	JOPTIONAL(percent_most_forward);
	JOPTIONAL(percent_closest_to_ball);
	JOPTIONAL(percent_farthest_from_ball);
}

void from_json(const nlohmann::json& j, DemoStats& p)
{
	J(inflicted);
	J(taken);
}


#undef J
#undef JOPTIONAL