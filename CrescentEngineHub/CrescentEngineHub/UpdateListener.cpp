#include "CrescentHub.h"

void UpdateListener::handleFileAction(FW::WatchID watchid, const FW::String& dir, const FW::String& filename,
	FW::Action action) {
	if (action == 4 && filename.substr(filename.find_last_of(".") + 1) == "cpp" || action == 4 && filename.substr(filename.find_last_of(".") + 1) == "h") {
		std::cout << "DIR (" << dir << ") FILE (" << filename << ") has been modified\n" << std::endl;
		std::cout << "Would you like to upload the modified file to the CrescentEngine database? type 'yes' or 'no'\n" << std::endl;
		std::cin >> response; 

		if (response == "yes") {
			std::cout << "Uploading file..." << std::endl;
			CrescentHub::GetInstance()->UploadFile(dir, filename);
		}

		else {
			std::cout << "Alright, not uploading.\n" << std::endl;
		}
	}
}