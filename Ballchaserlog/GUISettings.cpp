#include "pch.h"
#include "GUISettings.h"

TableColumn::TableColumn(std::string _name, std::function<void(Team::Player&)> rendercellImp, bool _visible) : 
	name(_name), 
	RenderCellImp(rendercellImp),
	customHeaderImp(false),
	visible(_visible)
{}

TableColumn::TableColumn(std::string _name, std::function<void(Team::Player&)> rendercellImp, std::function<void(Team&)> renderHeaderImp, bool _visible) :
	name(_name), 
	RenderCellImp(rendercellImp), 
	RenderHeaderImp(renderHeaderImp),
	customHeaderImp(true),
	visible(_visible)
	{}

void TableColumn::RenderHeader(Team& team)
{
	if (visible) {
		if (customHeaderImp)
		{
			RenderHeaderImp(team);
		}
		else {
			ImGui::Text(name.c_str());
		}
		ImGui::NextColumn();
	}
}

void TableColumn::RenderCell(Team::Player& player)
{
	if (visible){
		RenderCellImp(player);
		ImGui::NextColumn();
	}
}
