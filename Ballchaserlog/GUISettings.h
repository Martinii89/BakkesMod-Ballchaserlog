#pragma once
#include <string>
#include <functional>
#include "ballchasing/APIDataClasses.h"
#include <algorithm>

class TableColumn {
public:
	TableColumn(std::string _name, std::function<void(Team::Player&)> rendercellImp, bool visible = true);
	TableColumn(std::string _name, std::function<void(Team::Player&)> rendercellImp, std::function<void(Team&)> renderHeaderImp, bool visible = true);
	bool visible;
	std::string name;

	void RenderHeader(Team& team);
	void RenderCell(Team::Player& player);
private:
	bool customHeaderImp = false;
	std::function<void(Team::Player&)> RenderCellImp;
	std::function<void(Team&)> RenderHeaderImp;
};

struct TableSettings {
	std::vector<TableColumn> Columns;
	int GetColumnCount() {
		return count_if(Columns.begin(), Columns.end(), [](TableColumn& i) {return i.visible == true; });
	}
	TableSettings(std::vector<TableColumn> columns) : Columns(columns) {};
};




struct GUISettings {

	TableSettings overviewTableConfig{ {
		{"Team Goals",			[](Team::Player& p) {ImGui::Text(""); }, [](Team& t) {ImGui::Text("%i", t.goals); }},
		{"Player names",		[](Team::Player& p) {ImGui::Text("%s", p.name); }, [](Team& t) {ImGui::Text("%s", t.name); }},
		{"Score",				[](Team::Player& p) {ImGui::Text("%i", p.stats.core.score); }},
		{"Goals",				[](Team::Player& p) {ImGui::Text("%i", p.stats.core.goals); }},
		{"Assists",				[](Team::Player& p) {ImGui::Text("%i", p.stats.core.assists); }},
		{"Saves",				[](Team::Player& p) {ImGui::Text("%i", p.stats.core.saves); }},
		{"Shots",				[](Team::Player& p) {ImGui::Text("%i", p.stats.core.shots); }},
		}};

	TableSettings coreTableConfig{ {
		{"Player Name",		[](Team::Player& p) {ImGui::Text("%s", p.name); }},
		{"Score",				[](Team::Player& p) {ImGui::Text("%i",   p.stats.core.score); }},
		{"Goals",				[](Team::Player& p) {ImGui::Text("%i",   p.stats.core.goals); }},
		{"Assists",				[](Team::Player& p) {ImGui::Text("%i",   p.stats.core.assists); }},
		{"Saves",				[](Team::Player& p) {ImGui::Text("%i",   p.stats.core.saves); }},
		{"Shots",				[](Team::Player& p) {ImGui::Text("%i",   p.stats.core.shots); }},
		{"Shots against",		[](Team::Player& p) {ImGui::Text("%i",   p.stats.core.shots_against); }},
		{"Goals against",		[](Team::Player& p) {ImGui::Text("%i",   p.stats.core.goals_against); }},
		{"Shooting%",			[](Team::Player& p) {ImGui::Text("%.1f", p.stats.core.shooting_percentage); }},
		{"Demos inflicted",		[](Team::Player& p) {ImGui::Text("%i",   p.stats.demo.inflicted); }},
		{"Demos taken",			[](Team::Player& p) {ImGui::Text("%i",   p.stats.demo.taken); }},
		{"MVP",					[](Team::Player& p) {ImGui::Text(p.stats.core.mvp ? "Yes" : "No"); }},
		} };

	TableSettings boostTableConfig{ {
		{"Player Name",					[](Team::Player& p) {ImGui::Text("%s",   p.name); }},
		{"bpm",							[](Team::Player& p) {ImGui::Text("%i",   p.stats.boost.bpm); }},
		{"bcpm",						[](Team::Player& p) {ImGui::Text("%.1f", p.stats.boost.bcpm); }},
		{"avg_amount",					[](Team::Player& p) {ImGui::Text("%.1f", p.stats.boost.avg_amount); }},
		{"amount_collected",			[](Team::Player& p) {ImGui::Text("%i",   p.stats.boost.amount_collected); }},
		{"amount_stolen",				[](Team::Player& p) {ImGui::Text("%i",   p.stats.boost.amount_stolen); }},
		{"amount_collected_big",		[](Team::Player& p) {ImGui::Text("%i",   p.stats.boost.amount_collected_big); }},
		{"amount_stolen_big",			[](Team::Player& p) {ImGui::Text("%i",   p.stats.boost.amount_stolen_big); }},
		{"amount_collected_small",		[](Team::Player& p) {ImGui::Text("%i",   p.stats.boost.amount_collected_small); }},
		{"amount_stolen_small",			[](Team::Player& p) {ImGui::Text("%i",   p.stats.boost.amount_stolen_small); }},
		{"count_collected_big",			[](Team::Player& p) {ImGui::Text("%i",   p.stats.boost.count_collected_big); }},
		{"count_stolen_big",			[](Team::Player& p) {ImGui::Text("%i",   p.stats.boost.count_stolen_big); }},
		{"count_collected_small",		[](Team::Player& p) {ImGui::Text("%i",   p.stats.boost.count_collected_small); }},
		{"count_stolen_small",			[](Team::Player& p) {ImGui::Text("%i",   p.stats.boost.count_stolen_small); }},
		{"amount_overfill",				[](Team::Player& p) {ImGui::Text("%i",   p.stats.boost.amount_overfill); }},
		{"amount_overfill_stolen",		[](Team::Player& p) {ImGui::Text("%i",   p.stats.boost.amount_overfill_stolen); }},
		{"amount_used_while_supersonic",[](Team::Player& p) {ImGui::Text("%i",   p.stats.boost.amount_used_while_supersonic); }},
		{"time_zero_boost",				[](Team::Player& p) {ImGui::Text("%.1f", p.stats.boost.time_zero_boost); }},
		{"percent_zero_boost",			[](Team::Player& p) {ImGui::Text("%.1f", p.stats.boost.percent_zero_boost); }},
		{"time_full_boost",				[](Team::Player& p) {ImGui::Text("%.1f", p.stats.boost.time_full_boost); }},
		{"percent_full_boost",			[](Team::Player& p) {ImGui::Text("%.1f", p.stats.boost.percent_full_boost); }},
		{"time_boost_0_25",				[](Team::Player& p) {ImGui::Text("%.1f", p.stats.boost.time_boost_0_25); }},
		{"time_boost_25_50",			[](Team::Player& p) {ImGui::Text("%.1f", p.stats.boost.time_boost_25_50); }},
		{"time_boost_50_75",			[](Team::Player& p) {ImGui::Text("%.1f", p.stats.boost.time_boost_50_75); }},
		{"time_boost_75_100",			[](Team::Player& p) {ImGui::Text("%.1f", p.stats.boost.time_boost_75_100); }},
		{"percent_boost_0_25",			[](Team::Player& p) {ImGui::Text("%.1f", p.stats.boost.percent_boost_0_25); }},
		{"percent_boost_25_50",			[](Team::Player& p) {ImGui::Text("%.1f", p.stats.boost.percent_boost_25_50); }},
		{"percent_boost_50_75",			[](Team::Player& p) {ImGui::Text("%.1f", p.stats.boost.percent_boost_50_75); }},
		{"percent_boost_75_100",		[](Team::Player& p) {ImGui::Text("%.1f", p.stats.boost.percent_boost_75_100); }}
	} };

	TableSettings movementTableConfig{ {
		{"Player Name",				[](Team::Player& p) {ImGui::Text("%s", p.name); }},
		{"avg_speed",				[](Team::Player& p) {ImGui::Text("%.1f", p.stats.movement.avg_speed); }},
		{"total_distance",			[](Team::Player& p) {ImGui::Text("%i", p.stats.movement.total_distance); }},
		{"time_supersonic_speed",	[](Team::Player& p) {ImGui::Text("%.1f", p.stats.movement.time_supersonic_speed); }},
		{"time_boost_speed",		[](Team::Player& p) {ImGui::Text("%.1f", p.stats.movement.time_boost_speed); }},
		{"time_slow_speed",			[](Team::Player& p) {ImGui::Text("%.1f", p.stats.movement.time_slow_speed); }},
		{"time_ground",				[](Team::Player& p) {ImGui::Text("%.1f", p.stats.movement.time_ground); }},
		{"time_low_air",			[](Team::Player& p) {ImGui::Text("%.1f", p.stats.movement.time_low_air); }},
		{"time_high_air",			[](Team::Player& p) {ImGui::Text("%.1f", p.stats.movement.time_high_air); }},
		{"time_powerslide",			[](Team::Player& p) {ImGui::Text("%.1f", p.stats.movement.time_powerslide); }},
		{"count_powerslide",		[](Team::Player& p) {ImGui::Text("%i",	 p.stats.movement.count_powerslide); }},
		{"avg_powerslide_duration",	[](Team::Player& p) {ImGui::Text("%.1f", p.stats.movement.avg_powerslide_duration); }},
		{"avg_speed_percentage",	[](Team::Player& p) {ImGui::Text("%.1f", p.stats.movement.avg_speed_percentage); }},
		{"percent_slow_speed",		[](Team::Player& p) {ImGui::Text("%.1f", p.stats.movement.percent_slow_speed); }},
		{"percent_boost_speed",		[](Team::Player& p) {ImGui::Text("%.1f", p.stats.movement.percent_boost_speed); }},
		{"percent_supersonic_speed",[](Team::Player& p) {ImGui::Text("%.1f", p.stats.movement.percent_supersonic_speed); }},
		{"percent_ground",			[](Team::Player& p) {ImGui::Text("%.1f", p.stats.movement.percent_ground); }},
		{"percent_low_air",			[](Team::Player& p) {ImGui::Text("%.1f", p.stats.movement.percent_low_air); }},
		{"percent_high_air",		[](Team::Player& p) {ImGui::Text("%.1f", p.stats.movement.percent_high_air); }}
	} };

	TableSettings positioningTableConfig{ {
		{"Player Name",							[](Team::Player& p) {ImGui::Text("%s",   p.name); }},
		{"avg_distance_to_ball",				[](Team::Player& p) {ImGui::Text("%i",   p.stats.positioning.avg_distance_to_ball); }},
		{"avg_distance_to_ball_possession",		[](Team::Player& p) {ImGui::Text("%i",   p.stats.positioning.avg_distance_to_ball_possession); }},
		{"avg_distance_to_ball_no_possession",	[](Team::Player& p) {ImGui::Text("%i",   p.stats.positioning.avg_distance_to_ball_no_possession); }},
		{"avg_distance_to_mates",				[](Team::Player& p) {ImGui::Text("%i",   p.stats.positioning.avg_distance_to_mates); }},
		{"time_defensive_third",				[](Team::Player& p) {ImGui::Text("%i",   p.stats.positioning.time_defensive_third); }},
		{"time_neutral_third",					[](Team::Player& p) {ImGui::Text("%.1f", p.stats.positioning.time_neutral_third); }},
		{"time_offensive_third",				[](Team::Player& p) {ImGui::Text("%.1f", p.stats.positioning.time_offensive_third); }},
		{"time_defensive_half",					[](Team::Player& p) {ImGui::Text("%.1f", p.stats.positioning.time_defensive_half); }},
		{"time_offensive_half",					[](Team::Player& p) {ImGui::Text("%.1f", p.stats.positioning.time_offensive_half); }},
		{"time_behind_ball",					[](Team::Player& p) {ImGui::Text("%.1f", p.stats.positioning.time_behind_ball); }},
		{"time_infront_ball",					[](Team::Player& p) {ImGui::Text("%.1f", p.stats.positioning.time_infront_ball); }},
		{"time_most_back",						[](Team::Player& p) {ImGui::Text("%.1f", p.stats.positioning.time_most_back); }},
		{"time_most_forward",					[](Team::Player& p) {ImGui::Text("%.1f", p.stats.positioning.time_most_forward); }},
		{"goals_against_while_last_defender",	[](Team::Player& p) {ImGui::Text("%i",   p.stats.positioning.goals_against_while_last_defender); }},
		{"time_closest_to_ball",				[](Team::Player& p) {ImGui::Text("%.1f", p.stats.positioning.time_closest_to_ball); }},
		{"time_farthest_from_ball",				[](Team::Player& p) {ImGui::Text("%.1f", p.stats.positioning.time_farthest_from_ball); }},
		{"percent_defensive_third",				[](Team::Player& p) {ImGui::Text("%.1f", p.stats.positioning.percent_defensive_third); }},
		{"percent_offensive_third",				[](Team::Player& p) {ImGui::Text("%.1f", p.stats.positioning.percent_offensive_third); }},
		{"percent_neutral_third",				[](Team::Player& p) {ImGui::Text("%.1f", p.stats.positioning.percent_neutral_third); }},
		{"percent_defensive_half",				[](Team::Player& p) {ImGui::Text("%.1f", p.stats.positioning.percent_defensive_half); }},
		{"percent_offensive_half",				[](Team::Player& p) {ImGui::Text("%.1f", p.stats.positioning.percent_offensive_half); }},
		{"percent_behind_ball",					[](Team::Player& p) {ImGui::Text("%.1f", p.stats.positioning.percent_behind_ball); }},
		{"percent_infront_ball",				[](Team::Player& p) {ImGui::Text("%.1f", p.stats.positioning.percent_infront_ball); }},
		{"percent_most_back",					[](Team::Player& p) {ImGui::Text("%.1f", p.stats.positioning.percent_most_back); }},
		{"percent_most_forward",				[](Team::Player& p) {ImGui::Text("%.1f", p.stats.positioning.percent_most_forward); }},
		{"percent_closest_to_ball",				[](Team::Player& p) {ImGui::Text("%.1f", p.stats.positioning.percent_closest_to_ball); }},
		{"percent_farthest_from_ball",			[](Team::Player& p) {ImGui::Text("%.1f", p.stats.positioning.percent_farthest_from_ball); }}
	} };
};