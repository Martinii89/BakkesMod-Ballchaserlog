#pragma once
#include <string>
#include <functional>
#include "ballchasing/APIDataClasses.h"
#include <algorithm>


class TableColumn {
public:
	TableColumn(std::string _name, std::function<void(BaseStatPlayer&)> rendercellImp, bool visible = true);
	TableColumn(std::string _name, std::function<void(BaseStatPlayer&)> rendercellImp, std::function<void(Team&)> renderHeaderImp, bool visible = true);
	bool visible;
	std::string name;

	void RenderHeader(Team& team);
	void RenderCell(BaseStatPlayer& player);
private:
	bool customHeaderImp = false;
	std::function<void(BaseStatPlayer&)> RenderCellImp;
	std::function<void(Team&)> RenderHeaderImp;
};

struct TableSettings {
	std::vector<TableColumn> Columns;
	int GetColumnCount() {
		return count_if(Columns.begin(), Columns.end(), [](TableColumn& i) {return i.visible == true; });
	}
	TableSettings(std::vector<TableColumn> columns) : Columns(columns) {};
};

void RenderCell(int value);
void RenderCell(std::string value);
void RenderCell(float value, int precision = 2, std::string postfix = "");
void TimePercentage(float value, float percentage);


struct GUISettings {

	TableSettings overviewTableConfig{ {
		{"Team Goals",				[](BaseStatPlayer& p) {ImGui::Text(""); }, [](Team& t) {RenderCell(t.goals); }, true},
		{"Player names",			[](BaseStatPlayer& p) {RenderCell(p.name); }, [](Team& t) {RenderCell(t.name); }, true},
		{"Score",					[](BaseStatPlayer& p) {RenderCell(p.stats.core.score); }, true},
		{"Goals",					[](BaseStatPlayer& p) {RenderCell(p.stats.core.goals); }, true},
		{"Assists",					[](BaseStatPlayer& p) {RenderCell(p.stats.core.assists); }, true},
		{"Saves",					[](BaseStatPlayer& p) {RenderCell(p.stats.core.saves); }, true},
		{"Shots",					[](BaseStatPlayer& p) {RenderCell(p.stats.core.shots); }, true},
		}};

	TableSettings coreTableConfig{ {
		{"Player Name",				[](BaseStatPlayer& p) {RenderCell(p.name); }, true},
		{"Score",					[](BaseStatPlayer& p) {RenderCell(p.stats.core.score); }, true},
		{"Goals",					[](BaseStatPlayer& p) {RenderCell(p.stats.core.goals); }, true},
		{"Assists",					[](BaseStatPlayer& p) {RenderCell(p.stats.core.assists); }, true},
		{"Saves",					[](BaseStatPlayer& p) {RenderCell(p.stats.core.saves); }, true},
		{"Shots",					[](BaseStatPlayer& p) {RenderCell(p.stats.core.shots); }, true},
		{"Shots against",			[](BaseStatPlayer& p) {RenderCell(p.stats.core.shots_against); },	  false},
		{"Goals against",			[](BaseStatPlayer& p) {RenderCell(p.stats.core.goals_against); },	  false},
		{"Shooting%%",				[](BaseStatPlayer& p) {RenderCell(p.stats.core.shooting_percentage, 1, "%");}, true},
		{"Demos inflicted",			[](BaseStatPlayer& p) {RenderCell(p.stats.demo.inflicted); }, true},
		{"Demos taken",				[](BaseStatPlayer& p) {RenderCell(p.stats.demo.taken); },			  true},
		//{"MVP",						[](Team::Player& p) {ImGui::Text(p.stats.core.mvp > 0 ? "Yes" : "No"); },         false}, //MVP bugged
		} };

	TableSettings boostTableConfig{ {
		{"Player Name",				[](BaseStatPlayer& p) {RenderCell(p.name); }, true},
		{"BPM",						[](BaseStatPlayer& p) {RenderCell(p.stats.boost.bpm); }, true},
		{"BCPM",					[](BaseStatPlayer& p) {RenderCell(p.stats.boost.bcpm, 1); }, false},
		{"Average",					[](BaseStatPlayer& p) {RenderCell(p.stats.boost.avg_amount, 1); }, true},
		{"Collected",				[](BaseStatPlayer& p) {RenderCell(p.stats.boost.amount_collected); }, false},
		{"Stolen",					[](BaseStatPlayer& p) {RenderCell(p.stats.boost.amount_stolen); }, true},
		{"Pads amount",				[](BaseStatPlayer& p) {RenderCell(p.stats.boost.amount_collected_small); }, false},
		{"Pills amount",			[](BaseStatPlayer& p) {RenderCell(p.stats.boost.amount_collected_big); }, false},
		{"Pads amount stolen  ",	[](BaseStatPlayer& p) {RenderCell(p.stats.boost.amount_stolen_small); }, false},
		{"Pills amount stolen",		[](BaseStatPlayer& p) {RenderCell(p.stats.boost.amount_stolen_big); }, false},
		{"Pads",					[](BaseStatPlayer& p) {RenderCell(p.stats.boost.count_collected_small); }, false},
		{"Pills",					[](BaseStatPlayer& p) {RenderCell(p.stats.boost.count_collected_big); }, false},
		{"Pads stolen",				[](BaseStatPlayer& p) {RenderCell(p.stats.boost.count_stolen_small); }, false},
		{"Pills Stolen",			[](BaseStatPlayer& p) {RenderCell(p.stats.boost.count_stolen_big); }, false},
		{"Overfill",				[](BaseStatPlayer& p) {RenderCell(p.stats.boost.amount_overfill); }, false},
		{"Overfill stolen",			[](BaseStatPlayer& p) {RenderCell(p.stats.boost.amount_overfill_stolen); }, false},
		{"Supersonic waste",		[](BaseStatPlayer& p) {RenderCell(p.stats.boost.amount_used_while_supersonic); }, true},
		{"No Boost",				[](BaseStatPlayer& p) {TimePercentage(p.stats.boost.time_zero_boost,  p.stats.boost.percent_zero_boost); }, true},
		{"0-25% Boost",				[](BaseStatPlayer& p) {TimePercentage(p.stats.boost.time_boost_0_25, p.stats.boost.percent_boost_0_25); }, true},
		{"25-50% Boost",			[](BaseStatPlayer& p) {TimePercentage(p.stats.boost.time_boost_25_50, p.stats.boost.percent_boost_25_50); }, true},
		{"50-75% Boost",			[](BaseStatPlayer& p) {TimePercentage(p.stats.boost.time_boost_50_75, p.stats.boost.percent_boost_50_75); }, true},
		{"75-100% Boost",			[](BaseStatPlayer& p) {TimePercentage(p.stats.boost.time_boost_75_100, p.stats.boost.percent_boost_75_100); }, true},
		{"Full Boost",				[](BaseStatPlayer& p) {TimePercentage(p.stats.boost.time_full_boost, p.stats.boost.percent_full_boost); }, true},
	} };

	TableSettings movementTableConfig{ {
		{"Player Name",				[](BaseStatPlayer& p) {RenderCell(p.name); }, true},
		{"Avg speed",				[](BaseStatPlayer& p) {RenderCell(p.stats.movement.avg_speed); }, true},
		{"Distance",				[](BaseStatPlayer& p) {RenderCell(p.stats.movement.total_distance); }, false},
		{"Time Supersonic",			[](BaseStatPlayer& p) {TimePercentage(p.stats.movement.time_supersonic_speed, p.stats.movement.percent_supersonic_speed); }, true},
		{"Time boost speed",		[](BaseStatPlayer& p) {TimePercentage(p.stats.movement.time_boost_speed, p.stats.movement.percent_boost_speed); }, true},
		{"Time slow speed",			[](BaseStatPlayer& p) {TimePercentage(p.stats.movement.time_slow_speed, p.stats.movement.percent_slow_speed); }, true},
		{"Time ground",				[](BaseStatPlayer& p) {TimePercentage(p.stats.movement.time_ground, p.stats.movement.percent_ground); }, true},
		{"Time low air",			[](BaseStatPlayer& p) {TimePercentage(p.stats.movement.time_low_air, p.stats.movement.percent_low_air); }, true},
		{"Time high air",			[](BaseStatPlayer& p) {TimePercentage(p.stats.movement.time_high_air, p.stats.movement.percent_high_air); }, true},
		{"Time powerslide",			[](BaseStatPlayer& p) {RenderCell(p.stats.movement.time_powerslide, 1); }, false},
		{"Count powerslide",		[](BaseStatPlayer& p) {RenderCell(p.stats.movement.count_powerslide); }, false},
		{"Avg powerslide duration",	[](BaseStatPlayer& p) {RenderCell(p.stats.movement.avg_powerslide_duration, 1); }, false},
		{"Avg speed%",				[](BaseStatPlayer& p) {RenderCell(p.stats.movement.avg_speed_percentage, 1, "%"); }, false},
	} };

	TableSettings positioningTableConfig{ {
		{"Player Name",							[](BaseStatPlayer& p) {RenderCell(p.name); }, true},
		{"Avg dist to ball",					[](BaseStatPlayer& p) {RenderCell(p.stats.positioning.avg_distance_to_ball); }, true},
		{"Avg ball distance w possession",		[](BaseStatPlayer& p) {RenderCell(p.stats.positioning.avg_distance_to_ball_possession); }, false},
		{"Avg ball distance w/o possession",	[](BaseStatPlayer& p) {RenderCell(p.stats.positioning.avg_distance_to_ball_no_possession); }, false},
		{"Avg distance to team",				[](BaseStatPlayer& p) {RenderCell(p.stats.positioning.avg_distance_to_mates); }, true},
		{"Time defensive 1/3",					[](BaseStatPlayer& p) {TimePercentage(p.stats.positioning.time_defensive_third, p.stats.positioning.percent_defensive_third); }, true},
		{"Time neutral 1/3",					[](BaseStatPlayer& p) {TimePercentage(p.stats.positioning.time_neutral_third, p.stats.positioning.percent_neutral_third); }, true},
		{"Time offensive 1/3",					[](BaseStatPlayer& p) {TimePercentage(p.stats.positioning.time_offensive_third, p.stats.positioning.percent_offensive_third); }, true},
		{"Time defensive 1/2",					[](BaseStatPlayer& p) {TimePercentage(p.stats.positioning.time_defensive_half, p.stats.positioning.percent_defensive_half); }, false},
		{"Time offensive 1/2",					[](BaseStatPlayer& p) {TimePercentage(p.stats.positioning.time_offensive_half, p.stats.positioning.percent_offensive_half); }, false},
		{"Time behind ball",					[](BaseStatPlayer& p) {TimePercentage(p.stats.positioning.time_behind_ball, p.stats.positioning.percent_behind_ball); }, false},
		{"Time infront ball",					[](BaseStatPlayer& p) {TimePercentage(p.stats.positioning.time_infront_ball, p.stats.positioning.percent_infront_ball); }, false},
		{"Time most back",						[](BaseStatPlayer& p) {TimePercentage(p.stats.positioning.time_most_back, p.stats.positioning.percent_most_back); }, false},
		{"Time most forward",					[](BaseStatPlayer& p) {TimePercentage(p.stats.positioning.time_most_forward, p.stats.positioning.percent_most_forward); }, false},
		{"Time closest to ball",				[](BaseStatPlayer& p) {TimePercentage(p.stats.positioning.time_closest_to_ball, p.stats.positioning.percent_closest_to_ball); }, false},
		{"Time farthest from ball",				[](BaseStatPlayer& p) {TimePercentage(p.stats.positioning.time_farthest_from_ball, p.stats.positioning.percent_farthest_from_ball); }, false},
		{"Goals against while last defender",	[](BaseStatPlayer& p) {RenderCell(p.stats.positioning.goals_against_while_last_defender); }, false},
	} };

	TableSettings cameraTableConfig{ {
	{"Player Name",		[](BaseStatPlayer& p) {RenderCell(p.name); }, true},
	{"FOV",				[](BaseStatPlayer& p) {RenderCell(p.camera.fov); }, true},
	{"Height",			[](BaseStatPlayer& p) {RenderCell(p.camera.height); }, true},
	{"Distance",		[](BaseStatPlayer& p) {RenderCell(p.camera.distance); }, true},
	{"Stiffness",		[](BaseStatPlayer& p) {RenderCell(p.camera.stiffness); }, true},
	{"Swivel Speed",	[](BaseStatPlayer& p) {RenderCell(p.camera.swivel_speed); }, true},
	{"Transition Speed",[](BaseStatPlayer& p) {RenderCell(p.camera.transition_speed); }, true},
} };

	TableSettings groupOverviewTableConfig{ {
	{"Player name",				[](BaseStatPlayer& p) {RenderCell(p.name); }, [](Team& t) {RenderCell(t.name); }, true},
	{"Games",					[](BaseStatPlayer& p) {RenderCell(p.cumulative_stats.games); }, true},
	{"Wins",					[](BaseStatPlayer& p) {RenderCell(p.cumulative_stats.wins); }, true},
	{"Win%%",					[](BaseStatPlayer& p) {RenderCell(p.cumulative_stats.win_percentage, 1, "%"); }, true},
	{"Play duration",			[](BaseStatPlayer& p) {RenderCell(p.cumulative_stats.play_duration, 2, "s"); }, true},
	} };
};