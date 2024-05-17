#include "pch.h"
#include "RandomCommand.h"
#include <random>
#include "nlohmann.hpp"




std::vector<std::string> splitStringByNewline(const std::string& input) {
	std::vector<std::string> lines;
	std::istringstream iss(input);
	std::string line;

	// Read each line using std::getline with '\n' as delimiter
	while (std::getline(iss, line)) {
		lines.push_back(line);
	}

	return lines;
}





std::string RandomCommand::GetRandomCommandFromList(const std::string& listName) {
	if (CommandLists.empty()) {
		LOG("Error: There are no command lists ...");
		return "";
	}

	for (auto list : CommandLists) {
		if (list.listName == listName) {

			if (list.parsedCommands.empty()) {
				LOG("Error: '{}' command list has no commands!", listName);
				return "";
			}


			// Seed the random number generator with the current time
			static std::mt19937 rng(static_cast<unsigned int>(std::time(nullptr)));

			// Create a uniform distribution from 0 to vec.size() - 1
			std::uniform_int_distribution<std::size_t> dist(0, list.parsedCommands.size() - 1);

			// Get a random index
			std::size_t randomIndex = dist(rng);

			// Return the random element from the vector
			return list.parsedCommands[randomIndex];


		}
	}

	LOG("Error: '{}' command list doesn't exist!", listName);
	return "";
}







void RandomCommand::UpdateData() {
	std::string jsonFileRawStr = readContent(commandListsFilePath);

	try {
		auto commandListsJsonData = nlohmann::json::parse(jsonFileRawStr);
		auto commandLists = commandListsJsonData["commandLists"];

		if (commandLists.size() > 0) {
			for (int i = 0; i < commandLists.size(); i++) {

				// read data from each command list obj and update CommandLists vector
				auto commandListObj = commandLists[i];

				CommandList commandList;
				commandList.listName = commandListObj["listName"];
				commandList.unparsedString = commandListObj["unparsedString"];

				for (int i = 0; i < commandListObj["parsedCommands"].size(); i++) {
					commandList.parsedCommands.push_back(commandListObj["parsedCommands"][i]);
				}


				CommandLists.push_back(commandList);

			}
		}
	}
	catch (...) {
		LOG("*** Couldn't read the 'CommandLists.json' file! Make sure it contains valid JSON... ***");
	}
}


void RandomCommand::AddEmptyCommandList() {
	CommandList list;
	list.listName = "";
	list.unparsedString = "";
	list.parsedCommands = {};

	CommandLists.push_back(list);
}


void RandomCommand::DeleteCommandList(int idx) {
	if (CommandLists.empty()) { return; }

	// erase command list at given index
	CommandLists.erase(CommandLists.begin() + idx);

	// reset selected command list index
	selectedCommandListIndex = CommandLists.empty() ? 0 : CommandLists.size() - 1;

	// update JSON
	WriteCommandListsToJson();
}


void RandomCommand::UpdateDataFromCommandStr() {
	for (auto& list : CommandLists) {		// reference used to modify the actual element itself (instead of just a local copy)
		std::vector<std::string> parsedCommands = splitStringByNewline(list.unparsedString);
		list.parsedCommands = parsedCommands;
	}
}


void RandomCommand::WriteCommandListsToJson() {
	nlohmann::json commandListsJsonObj;
	commandListsJsonObj["commandLists"] = {};

	for (auto& list : CommandLists) {
		nlohmann::json commandList;

		commandList["listName"] = list.listName;
		commandList["unparsedString"] = list.unparsedString;
		commandList["parsedCommands"] = {};

		for (auto& command : list.parsedCommands) {
			commandList["parsedCommands"].push_back(command);
		}

		commandListsJsonObj["commandLists"].push_back(commandList);
	}

	writeContent(commandListsFilePath, commandListsJsonObj.dump(4));
	LOG("Updated 'CommandLists.json' :)");
}


void RandomCommand::GetFilePaths() {
	std::filesystem::path bmDataFolderFilePath = gameWrapper->GetDataFolder();
	randomCommandFolder = bmDataFolderFilePath / "RandomCommand";
	commandListsFilePath = randomCommandFolder / "CommandLists.json";
}


void RandomCommand::CheckJsonFiles() {
	// create 'RandomCommand' folder if it doesn't exist
	if (!std::filesystem::exists(randomCommandFolder)) {
		std::filesystem::create_directory(randomCommandFolder);
		LOG("'RandomCommand' folder didn't exist... so I created it.");
	}

	// create JSON file if it doesn't exist
	if (!std::filesystem::exists(commandListsFilePath)) {
		std::ofstream NewFile(commandListsFilePath);

		NewFile << "{ \"commandLists\": [] }";
		NewFile.close();
		LOG("'CommandLists.json' didn't exist... so I created it.");
	}
}



std::string RandomCommand::readContent(const std::filesystem::path& FileName) {
	std::ifstream Temp(FileName);
	std::stringstream Buffer;
	Buffer << Temp.rdbuf();
	return (Buffer.str());
}


void RandomCommand::writeContent(const std::filesystem::path& FileName, const std::string& Buffer) {
	std::ofstream File(FileName, std::ofstream::trunc);
	File << Buffer;
	File.close();
}