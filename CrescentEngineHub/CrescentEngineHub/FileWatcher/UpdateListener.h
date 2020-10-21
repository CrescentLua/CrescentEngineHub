#include <iostream>
#include <stdio.h>
#include "FileWatcher.h"
/// Processes a file action
class UpdateListener : public FW::FileWatchListener
{
public:
	UpdateListener() {}
	std::string response; 
	void handleFileAction(FW::WatchID watchid, const FW::String& dir, const FW::String& filename,
		FW::Action action);
};