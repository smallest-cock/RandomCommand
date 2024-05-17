#include "pch.h"
#include "RandomCommand.h"


BAKKESMOD_PLUGIN(RandomCommand, "Random Command", plugin_version, PLUGINTYPE_FREEPLAY)

std::shared_ptr<CVarManagerWrapper> _globalCvarManager;



// declare member variables
std::vector<CommandList> RandomCommand::CommandLists;
std::filesystem::path RandomCommand::randomCommandFolder;
std::filesystem::path RandomCommand::commandListsFilePath;



void RandomCommand::onLoad()
{
	_globalCvarManager = cvarManager;


	// make sure JSON file is good to go, then read it to update data
	GetFilePaths();
	CheckJsonFiles();
	UpdateData();



	// command to run a random command from specified command list
	cvarManager->registerNotifier("randomCommand", [&](std::vector<std::string> args) {

		if (args.size() < 2) {
			LOG("Looks like you didn't provide a command list name....");
			return;
		}

		std::string commandListName = args[1];

		std::string randCommand = GetRandomCommandFromList(commandListName);

		if (randCommand == "") { return; }

		cvarManager->executeCommand(randCommand);


	}, "", 0);



	LOG("Random Command loaded! :)");
}
