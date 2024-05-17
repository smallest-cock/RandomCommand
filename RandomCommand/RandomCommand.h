#pragma once

#include "GuiBase.h"
#include "bakkesmod/plugin/bakkesmodplugin.h"
#include "bakkesmod/plugin/pluginwindow.h"
#include "bakkesmod/plugin/PluginSettingsWindow.h"

#include "version.h"
constexpr auto plugin_version = stringify(VERSION_MAJOR) "." stringify(VERSION_MINOR) "." stringify(VERSION_PATCH) "." stringify(VERSION_BUILD);


struct CommandList {
	std::string listName;
	std::string unparsedString;
	std::vector<std::string> parsedCommands;
};


class RandomCommand: public BakkesMod::Plugin::BakkesModPlugin
	,public SettingsWindowBase // Uncomment if you wanna render your own tab in the settings menu
	,public PluginWindowBase // Uncomment if you want to render your own plugin window
{

	//Boilerplate
	void onLoad() override;

	void GetFilePaths();
	void CheckJsonFiles();
	void UpdateData();

	void AddEmptyCommandList();
	void DeleteCommandList(int idx);
	void UpdateDataFromCommandStr();
	void WriteCommandListsToJson();

	std::string GetRandomCommandFromList(const std::string& listName);


	static std::vector<CommandList> CommandLists;
	static std::filesystem::path commandListsFilePath;
	static std::filesystem::path randomCommandFolder;

	// JSON stuff
	std::string readContent(const std::filesystem::path& FileName);
	void writeContent(const std::filesystem::path& FileName, const std::string& Buffer);


	// Edit command lists GUI
	void RenderAllCommandListNames();
	void RenderCommandListDetails();
	static int selectedCommandListIndex;


	//void onUnload() override; // Uncomment and implement if you need a unload method

public:
	void RenderSettings() override; // Uncomment if you wanna render your own tab in the settings menu
	void RenderWindow() override; // Uncomment if you want to render your own plugin window
};
