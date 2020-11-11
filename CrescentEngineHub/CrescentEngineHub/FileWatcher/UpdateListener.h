#include <map>
#include <iostream>
#include <stdio.h>
#include "FileWatcher.h"

struct FileDetails {
	std::string fileName; 
	std::string directory; 
};

/// Processes a file action
class UpdateListener : public FW::FileWatchListener
{
public:
	UpdateListener() {}
	std::string response; 
	void handleFileAction(FW::WatchID watchid, const FW::String& dir, const FW::String& filename,
		FW::Action action);

	int fileKey;
	FileDetails fileDetails; 
};