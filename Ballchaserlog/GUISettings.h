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
		{"Team Goals",			[](Team::Player& p) {ImGui::Text(""); }, [](Team& t) {ImGui::Text("%i", t.goals); }, true},
		{"Player names",		[](Team::Player& p) {ImGui::Text("%s", p.name.c_str()); }, [](Team& t) {ImGui::Text("%s", t.name.c_str()); }, true},
		{"Score",				[](Team::Player& p) {ImGui::Text("%i", p.stats.core.score); }, true},
		{"Goals",				[](Team::Player& p) {ImGui::Text("%i", p.stats.core.goals); }, true},
		{"Assists",				[](Team::Player& p) {ImGui::Text("%i", p.stats.core.assists); }, true},
		{"Saves",				[](Team::Player& p) {ImGui::Text("%i", p.stats.core.saves); }, true},
		{"Shots",				[](Team::Player& p) {ImGui::Text("%i", p.stats.core.shots); }, true},
		}};

	TableSettings coreTableConfig{ {
		{"Player Name",			[](Team::Player& p) {ImGui::Text("%s", p.name.c_str()); }, true},
		{"Score",				[](Team::Player& p) {ImGui::Text("%i",   p.stats.core.score); }, true},
		{"Goals",				[](Team::Player& p) {ImGui::Text("%i",   p.stats.core.goals); }, true},
		{"Assists",				[](Team::Player& p) {ImGui::Text("%i",   p.stats.core.assists); }, true},
		{"Saves",				[](Team::Player& p) {ImGui::Text("%i",   p.stats.core.saves); }, true},
		{"Shots",				[](Team::Player& p) {ImGui::Text("%i",   p.stats.core.shots); }, true},
		{"Shots against",		[](Team::Player& p) {ImGui::Text("%i",   p.stats.core.shots_against); },	  false},
		{"Goals against",		[](Team::Player& p) {ImGui::Text("%i",   p.stats.core.goals_against); },	  false},
		{"Shooting%",			[](Team::Player& p) {ImGui::Text("%.1f", p.stats.core.shooting_percentage);}, false},
		{"Demos inflicted",		[](Team::Player& p) {ImGui::Text("%i",   p.stats.demo.inflicted); }, true},
		{"Demos taken",			[](Team::Player& p) {ImGui::Text("%i",   p.stats.demo.taken); },			  false},
		{"MVP",					[](Team::Player& p) {ImGui::Text(p.stats.core.mvp ? "Yes" : "No"); },         false},
		} };

	TableSettings boostTableConfig{ {
		{"Player Name",					[](Team::Player& p) {ImGui::Text("%s",   p.name.c_str()); }, true},
		{"BPM",							[](Team::Player& p) {ImGui::Text("%i",   p.stats.boost.bpm); }, true},
		{"BCPM",						[](Team::Player& p) {ImGui::Text("%.1f", p.stats.boost.bcpm); }, false},
		{"Average",					[](Team::Player& p) {ImGui::Text("%.1f", p.stats.boost.avg_amount); }, true},
		{"Amount collected",			[](Team::Player& p) {ImGui::Text("%i",   p.stats.boost.amount_collected); }, false},
		{"Amount stolen",				[](Team::Player& p) {ImGui::Text("%i",   p.stats.boost.amount_stolen); }, true},
		{"Collected small",		[](Team::Player& p) {ImGui::Text("%i",   p.stats.boost.amount_collected_small); }, false},
		{"Collected big",		[](Team::Player& p) {ImGui::Text("%i",   p.stats.boost.amount_collected_big); }, false},
		{"Stolen small",			[](Team::Player& p) {ImGui::Text("%i",   p.stats.boost.amount_stolen_small); }, false},
		{"Stolen big",			[](Team::Player& p) {ImGui::Text("%i",   p.stats.boost.amount_stolen_big); }, false},
		{"#Collected small",		[](Team::Player& p) {ImGui::Text("%i",   p.stats.boost.count_collected_small); }, false},
		{"#Collected big",			[](Team::Player& p) {ImGui::Text("%i",   p.stats.boost.count_collected_big); }, false},
		{"#Stolen small",			[](Team::Player& p) {ImGui::Text("%i",   p.stats.boost.count_stolen_small); }, false},
		{"#Stolen big",			[](Team::Player& p) {ImGui::Text("%i",   p.stats.boost.count_stolen_big); }, false},
		{"Overfill",				[](Team::Player& p) {ImGui::Text("%i",   p.stats.boost.amount_overfill); }, false},
		{"Overfill stolen",		[](Team::Player& p) {ImGui::Text("%i",   p.stats.boost.amount_overfill_stolen); }, false},
		{"Supersonic waste",[](Team::Player& p) {ImGui::Text("%i",   p.stats.boost.amount_used_while_supersonic); }, true},
		{"No Boost",				[](Team::Player& p) {ImGui::Text("%.0fs (%.0f%%)", p.stats.boost.time_zero_boost,  p.stats.boost.percent_zero_boost); }, true},
		{"0-25% Boost",				[](Team::Player& p) {ImGui::Text("%.0fs (%.0f%%)", p.stats.boost.time_boost_0_25, p.stats.boost.percent_boost_0_25); }, true},
		{"25-50% Boost",			[](Team::Player& p) {ImGui::Text("%.0fs (%.0f%%)", p.stats.boost.time_boost_25_50, p.stats.boost.percent_boost_25_50); }, true},
		{"50-75% Boost",			[](Team::Player& p) {ImGui::Text("%.0fs (%.0f%%)", p.stats.boost.time_boost_50_75, p.stats.boost.percent_boost_50_75); }, true},
		{"75-100% Boost",			[](Team::Player& p) {ImGui::Text("%.0fs (%.0f%%)", p.stats.boost.time_boost_75_100, p.stats.boost.percent_boost_75_100); }, true},
		{"Full Boost",				[](Team::Player& p) {ImGui::Text("%.0fs (%.0f%%)", p.stats.boost.time_full_boost, p.stats.boost.percent_full_boost); }, true},
	} };

	TableSettings movementTableConfig{ {
		{"Player Name",				[](Team::Player& p) {ImGui::Text("%s", p.name.c_str()); }, true},
		{"Avg speed",				[](Team::Player& p) {ImGui::Text("%.1f", p.stats.movement.avg_speed); }, true},
		{"Distance",			[](Team::Player& p) {ImGui::Text("%i", p.stats.movement.total_distance); }, false},
		{"Time Supersonic",	[](Team::Player& p) {ImGui::Text("%.1f", p.stats.movement.time_supersonic_speed); }, true},
		{"Time boost speed",		[](Team::Player& p) {ImGui::Text("%.1f", p.stats.movement.time_boost_speed); }, true},
		{"Time slow speed",			[](Team::Player& p) {ImGui::Text("%.1f", p.stats.movement.time_slow_speed); }, true},
		{"Time ground",				[](Team::Player& p) {ImGui::Text("%.1f", p.stats.movement.time_ground); }, true},
		{"Time low air",			[](Team::Player& p) {ImGui::Text("%.1f", p.stats.movement.time_low_air); }, true},
		{"Time high air",			[](Team::Player& p) {ImGui::Text("%.1f", p.stats.movement.time_high_air); }, true},
		{"Time powerslide",			[](Team::Player& p) {ImGui::Text("%.1f", p.stats.movement.time_powerslide); }, false},
		{"Count powerslide",		[](Team::Player& p) {ImGui::Text("%i",	 p.stats.movement.count_powerslide); }, false},
		{"Avg powerslide duration",	[](Team::Player& p) {ImGui::Text("%.1f", p.stats.movement.avg_powerslide_duration); }, false},
		{"Avg speed%",	[](Team::Player& p) {ImGui::Text("%.1f", p.stats.movement.avg_speed_percentage); }, false},
		{"%%slow",		[](Team::Player& p) {ImGui::Text("%.1f", p.stats.movement.percent_slow_speed); }, false},
		{"%%boost speed",		[](Team::Player& p) {ImGui::Text("%.1f", p.stats.movement.percent_boost_speed); }, false},
		{"%%supersonic",[](Team::Player& p) {ImGui::Text("%.1f", p.stats.movement.percent_supersonic_speed); }, false},
		{"%%ground",			[](Team::Player& p) {ImGui::Text("%.1f", p.stats.movement.percent_ground); }, false},
		{"%%low air",			[](Team::Player& p) {ImGui::Text("%.1f", p.stats.movement.percent_low_air); }, false},
		{"%%high air",		[](Team::Player& p) {ImGui::Text("%.1f", p.stats.movement.percent_high_air); }, false}
	} };

	TableSettings positioningTableConfig{ {
		{"Player Name",							[](Team::Player& p) {ImGui::Text("%s",   p.name.c_str()); }, true},
		{"Avg dist to ball",				[](Team::Player& p) {ImGui::Text("%i",   p.stats.positioning.avg_distance_to_ball); }, true},
		{"avg_distance_to_ball_possession",		[](Team::Player& p) {ImGui::Text("%i",   p.stats.positioning.avg_distance_to_ball_possession); }, false},
		{"avg_distance_to_ball_no_possession",	[](Team::Player& p) {ImGui::Text("%i",   p.stats.positioning.avg_distance_to_ball_no_possession); }, false},
		{"Avg distance to team",				[](Team::Player& p) {ImGui::Text("%i",   p.stats.positioning.avg_distance_to_mates); }, true},
		{"Time defensive 1/3",				[](Team::Player& p) {ImGui::Text("%i",   p.stats.positioning.time_defensive_third); }, false},
		{"Time neutral 1/3",					[](Team::Player& p) {ImGui::Text("%.1f", p.stats.positioning.time_neutral_third); }, false},
		{"Time offensive 1/3",				[](Team::Player& p) {ImGui::Text("%.1f", p.stats.positioning.time_offensive_third); }, false},
		{"Time defensive 1/2",					[](Team::Player& p) {ImGui::Text("%.1f", p.stats.positioning.time_defensive_half); }, false},
		{"Time offensive 1/2",					[](Team::Player& p) {ImGui::Text("%.1f", p.stats.positioning.time_offensive_half); }, false},
		{"Time behind ball",					[](Team::Player& p) {ImGui::Text("%.1f", p.stats.positioning.time_behind_ball); }, false},
		{"Time infront ball",					[](Team::Player& p) {ImGui::Text("%.1f", p.stats.positioning.time_infront_ball); }, false},
		{"Time most back",						[](Team::Player& p) {ImGui::Text("%.1f", p.stats.positioning.time_most_back); }, false},
		{"Time most forward",					[](Team::Player& p) {ImGui::Text("%.1f", p.stats.positioning.time_most_forward); }, false},
		{"Goals against while last defender",	[](Team::Player& p) {ImGui::Text("%i",   p.stats.positioning.goals_against_while_last_defender); }, false},
		{"Time closest to ball",				[](Team::Player& p) {ImGui::Text("%.1f", p.stats.positioning.time_closest_to_ball); }, false},
		{"Time farthest from ball",				[](Team::Player& p) {ImGui::Text("%.1f", p.stats.positioning.time_farthest_from_ball); }, false},
		{"%% defensive 1/3",				[](Team::Player& p) {ImGui::Text("%.1f", p.stats.positioning.percent_defensive_third); }, true},
		{"%% offensive 1/3",				[](Team::Player& p) {ImGui::Text("%.1f", p.stats.positioning.percent_offensive_third); }, true},
		{"%% neutral 1/3",				[](Team::Player& p) {ImGui::Text("%.1f", p.stats.positioning.percent_neutral_third); }, true},
		{"%% defensive 1/2",				[](Team::Player& p) {ImGui::Text("%.1f", p.stats.positioning.percent_defensive_half); }, false},
		{"%% offensive 1/2",				[](Team::Player& p) {ImGui::Text("%.1f", p.stats.positioning.percent_offensive_half); }, false},
		{"%% behind ball",					[](Team::Player& p) {ImGui::Text("%.1f", p.stats.positioning.percent_behind_ball); }, false},
		{"%% infront ball",				[](Team::Player& p) {ImGui::Text("%.1f", p.stats.positioning.percent_infront_ball); }, false},
		{"%% most back",					[](Team::Player& p) {ImGui::Text("%.1f", p.stats.positioning.percent_most_back); }, false},
		{"%% most forward",				[](Team::Player& p) {ImGui::Text("%.1f", p.stats.positioning.percent_most_forward); }, false},
		{"%% closest to ball",				[](Team::Player& p) {ImGui::Text("%.1f", p.stats.positioning.percent_closest_to_ball); }, false},
		{"%% farthest from ball",			[](Team::Player& p) {ImGui::Text("%.1f", p.stats.positioning.percent_farthest_from_ball); }, false}
	} };
};