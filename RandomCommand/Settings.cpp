#include "pch.h"
#include "RandomCommand.h"
#include "GuiBase.h"

int RandomCommand::selectedCommandListIndex = 0;


void RandomCommand::RenderSettings() {

	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();

	ImGui::TextColored(ImVec4(1, 0, 1, 1), "Inspired by u/420PepeMILFHunter69");

	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();

	ImGui::Separator();

	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();

	// open bindings window button
	if (ImGui::Button("Command List Menu", ImVec2(0, 0))) {
		gameWrapper->Execute([this](GameWrapper* gw) {
			cvarManager->executeCommand("togglemenu " + GetMenuName());
		});
	}
	if (ImGui::IsItemHovered()) {
		ImGui::SetTooltip("Tip:\tmore commands = higher randomization");
	}

	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();

	ImGui::TextColored(ImVec4(1, 1, 0, 1), "Usage:");

	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();

	ImGui::Text("Create command lists in the Command List Menu above");

	ImGui::Spacing();
	ImGui::Spacing();

	ImGui::Text("... then run or bind\t");
	ImGui::SameLine();
	
	ImGui::PushItemWidth(265);
	std::string commandToBeCopied = "randomCommand \"your command list name\"";
	ImGui::InputText("", &commandToBeCopied, ImGuiInputTextFlags_ReadOnly);
	ImGui::PopItemWidth();

}


void RandomCommand::RenderWindow() {

	RenderAllCommandListNames();
	ImGui::SameLine();
	RenderCommandListDetails();
	ImGui::EndTabItem();

}


void RandomCommand::RenderAllCommandListNames() {
	if (ImGui::BeginChild("##CommandListsList", ImVec2(300, 0), true)) {

		ImGui::TextUnformatted("Command lists:");
		ImGui::Separator();

		for (int i = 0; i < CommandLists.size(); i++) {
			CommandList list = CommandLists[i];
			if (ImGui::Selectable((list.listName + "##" + std::to_string(i)).c_str(), i == selectedCommandListIndex)) {
				selectedCommandListIndex = i;
			}
		}

		ImGui::Spacing();
		ImGui::Spacing();

		if (ImGui::Button("Add New List", ImVec2(-1, 0))) {

			AddEmptyCommandList();

			selectedCommandListIndex = CommandLists.empty() ? 0 : CommandLists.size() - 1;

		}
	}
	ImGui::EndChild();
}


void RandomCommand::RenderCommandListDetails() {
	if (ImGui::BeginChild("##CommandListView", ImVec2(0, 0), true)) {

		if (CommandLists.empty()) {
			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::Spacing();

			ImGui::TextUnformatted("add a command list...");

			ImGui::EndChild();
			return;
		}

		CommandList selectedCommandList = CommandLists[selectedCommandListIndex];

		// command list display section title
		ImGui::TextUnformatted(selectedCommandList.listName.c_str());
		ImGui::Separator();

		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();


		// command list name
		std::string commandListName = selectedCommandList.listName;
		ImGui::InputTextWithHint("list name", "load training pack", &commandListName);
		CommandLists[selectedCommandListIndex].listName = commandListName;


		ImGui::Spacing();
		ImGui::Spacing();


		// command list (raw text)
		std::string commandListRawStr = selectedCommandList.unparsedString;
		//ImGui::InputTextMultiline("commands", &commandListRawStr, ImVec2(390, 350));		// fixed width/height text box that's a bit wider by default, but doesn't resize
		ImGui::InputTextMultiline("commands", &commandListRawStr, ImVec2(0,350));
		CommandLists[selectedCommandListIndex].unparsedString = commandListRawStr;

		ImGui::Spacing();
		ImGui::Spacing();

		ImGui::Text("(commands should be separated by a new line)");


		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();

		if (ImGui::Button("Save")) {
			UpdateDataFromCommandStr();
			WriteCommandListsToJson();
		}

		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();

		// delete variation list button
		ImGui::PushID(0);
		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.0f, 0.6f, 0.6f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.0f, 0.7f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.0f, 0.8f, 0.8f));
		if (ImGui::Button("Delete List", ImVec2(0, 0))) {
			ImGui::PopStyleColor(3);
			ImGui::PopID();
			DeleteCommandList(selectedCommandListIndex);
		}
		else {
			ImGui::PopStyleColor(3);
			ImGui::PopID();
		}

	}

	ImGui::EndChild();
}