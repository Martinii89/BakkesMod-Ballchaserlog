#include "pch.h"
#include "GUISettings.h"

TableColumn::TableColumn(std::string _name, std::function<void(BaseStatPlayer&)> rendercellImp, bool _visible) :
	name(_name), 
	RenderCellImp(rendercellImp),
	customHeaderImp(false),
	visible(_visible)
{}

TableColumn::TableColumn(std::string _name, std::function<void(BaseStatPlayer&)> rendercellImp, std::function<void(Team&)> renderHeaderImp, bool _visible) :
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

void TableColumn::RenderCell(BaseStatPlayer& player)
{
	if (visible){
		RenderCellImp(player);
		ImGui::NextColumn();
	}
}

void morphNumericString(char* s, int n) {
	char* p;
	int count;

	p = strchr(s, '.');         // Find decimal point, if any.
	if (p != NULL) {
		count = n;              // Adjust for more or less decimals.
		while (count >= 0) {    // Maximum decimals allowed.
			count--;
			if (*p == '\0')    // If there's less than desired.
				break;
			p++;               // Next character.
		}

		*p-- = '\0';            // Truncate string.
		while (*p == '0')       // Remove trailing zeros.
			*p-- = '\0';

		if (*p == '.') {        // If all decimals were zeros, remove ".".
			*p = '\0';
		}
	}
}

void nDecimals(char* s, double d, int n) {
	int sz; double d2;

	// Allow for negative.

	d2 = (d >= 0) ? d : -d;
	sz = (d >= 0) ? 0 : 1;

	// Add one for each whole digit (0.xx special case).

	if (d2 < 1) sz++;
	while (d2 >= 1) { d2 /= 10.0; sz++; }

	// Adjust for decimal point and fractionals.

	sz += 1 + n;

	// Create format string then use it.

	sprintf(s, "%*.*f", sz, n, d);
}

void RenderCell(int value) {
	ImGui::Text("%i", value);
}

void RenderCell(std::string value)
{
	ImGui::Text("%s", value.c_str());
}

// https://stackoverflow.com/questions/277772/avoid-trailing-zeroes-in-printf
void RenderCell(float f, int precision, std::string postfix) {
	char str[50];
	nDecimals(str, f, precision);
	morphNumericString(str, precision);
	ImGui::Text("%s%s", str, postfix);
	//ImGui::Text("%.*f%s", precision, f, postfix);
}

void TimePercentage(float value, float percentage)
{
	ImGui::Text("%.0fs (%.0f%%)", value, percentage);
}
