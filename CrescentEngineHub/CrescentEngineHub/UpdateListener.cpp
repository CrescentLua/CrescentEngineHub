#include "CrescentHub.h"

void UpdateListener::handleFileAction(FW::WatchID watchid, const FW::String& dir, const FW::String& filename,
	FW::Action action) {
	if (action == 4 && filename.substr(filename.find_last_of(".") + 1) == "cpp" || action == 4 && filename.substr(filename.find_last_of(".") + 1) == "h") {
		std::cout << "DIR (" << dir << ") FILE (" << filename << ") has been modified\n" << std::endl;
		std::cout << "File has been added to the list of modified files, press ENTER to upload file(s)!\n" << std::endl;
		
		fileDetails.directory = dir; 
		fileDetails.fileName = filename;

		fileKey += 1; 

		CrescentHub::GetInstance()->AddToFilesMap(fileKey, fileDetails);
	}
}