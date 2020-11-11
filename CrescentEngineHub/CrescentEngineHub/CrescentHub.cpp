#include "CrescentHub.h"
std::unique_ptr<CrescentHub> CrescentHub::hubInstance = nullptr;


CrescentHub::CrescentHub() {
	isRunning = false; 
	isUploading = false; 
}

CrescentHub::~CrescentHub() {
	Aws::ShutdownAPI(options);
}

CrescentHub* CrescentHub::GetInstance()
{
	if (hubInstance.get() == nullptr)
	{
		hubInstance.reset(new CrescentHub());
	}
	return hubInstance.get();
}

void CrescentHub::Run() {
	isRunning = true; 

	std::string dir = "C:/Users/RuthlessLua/Desktop/CrescentEngineRepository/Crescent Engine/GameEngines4/Engine/Core/WatchTest";
	FW::WatchID watchID = fileWatcher.addWatch(dir, &listener, true);

	while (isRunning) {
		Update(); 

		if (_kbhit())
		{
			char ch = _getch();
			switch (ch)
			{
				case 13: //When User presses Enter, upload all the files 
					if (filesMap.size() > 0 && isUploading == false) {
						isUploading = true; 
						for (auto file : filesMap) {
							UploadFile(file.second.directory, file.second.fileName);
						}

						isUploading = false; 
						filesMap.clear(); //Remove all the files within the map after uploading
					}
					
					break;
			}
		}
	}
}

void CrescentHub::AddToFilesMap(int fileKey_, FileDetails fileDetails_) {
	filesMap.insert(std::pair<int, FileDetails>(fileKey_, fileDetails_));
}

void CrescentHub::UploadFile(std::string dir_, std::string filename_) {
	Aws::InitAPI(options);
	{
		std::cout << "\nUploading File: " << filename_ << std::endl; 
		ifstream newFileToUpload(dir_ + "/" + filename_);
		std::stringstream fileContent;
		fileContent << newFileToUpload.rdbuf();
		S3Client client;

		//first put an object into s3
		PutObjectRequest putObjectRequest;
		putObjectRequest.WithKey(filename_)
			.WithBucket(BUCKET);
	
		//this can be any arbitrary stream (e.g. fstream, stringstream etc...)
		auto requestStream = Aws::MakeShared<Aws::StringStream>(filename_.c_str());
		*requestStream << fileContent.str();

		//set the stream that will be put to s3
		putObjectRequest.SetBody(requestStream);

		auto putObjectOutcome = client.PutObject(putObjectRequest);

		if (putObjectOutcome.IsSuccess())
		{
			std::cout << "Successfully uploaded the file!" << std::endl;
		}
		else
		{
			std::cout << "Error while putting Object " << putObjectOutcome.GetError().GetExceptionName() <<
				" " << putObjectOutcome.GetError().GetMessage() << std::endl;
		}
	}
}

void CrescentHub::Update() {
	fileWatcher.update(); 
}