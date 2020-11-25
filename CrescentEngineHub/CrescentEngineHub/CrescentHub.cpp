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

	Aws::InitAPI(options);

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

				case 80: 
					DownloadFile();
					break; 
			}
		}
	}
}

void CrescentHub::AddToFilesMap(int fileKey_, FileDetails fileDetails_) {
	filesMap.insert(std::pair<int, FileDetails>(fileKey_, fileDetails_));
}

void CrescentHub::DownloadFile() {
	std::cout << "Downloading File..." << std::endl; 

	auto s3Client = Aws::MakeShared<Aws::S3::S3Client>(ALLOC_TAG);
	auto thread_executor = Aws::MakeShared<Aws::Utils::Threading::PooledThreadExecutor>(ALLOC_TAG, 4);
	TransferManagerConfiguration transferConfig(thread_executor.get());
	transferConfig.s3Client = s3Client;
	
	DownloadConfiguration downloadConfig;
	downloadConfig.versionId = "Yu_oGmhApIJG_3Evgx3mQfTAESlWMY64";

	transferConfig.transferStatusUpdatedCallback =
		[](const TransferManager *,
			const std::shared_ptr<const TransferHandle> &handle) {
				std::cout << "Transfer Status = "
					<< static_cast<int>(handle->GetStatus()) << "\n";
	};

	transferConfig.downloadProgressCallback =
		[](const TransferManager *,
			const std::shared_ptr<const TransferHandle> &handle) {
				std::cout << "Download Progress: " << handle->GetBytesTransferred()
					<< " of " << handle->GetBytesTotalSize() << " bytes\n";
	};

	auto transferManager = TransferManager::Create(transferConfig);
	auto transferHandle = transferManager->DownloadFile(BUCKET, "Test.cpp", "C:/Users/RuthlessLua/Desktop/AWSDownloads/Test.cpp", downloadConfig);
	transferHandle->WaitUntilFinished();

	std::cout << "Test.cpp Download Complete!\n" << std::endl; 

	transferHandle = transferManager->DownloadFile(BUCKET, "Test.h", "C:/Users/RuthlessLua/Desktop/AWSDownloads/Test.h");
	transferHandle->WaitUntilFinished();
	std::cout << "Test.h Download Complete!" << std::endl;
}

void CrescentHub::UploadFile(std::string dir_, std::string filename_) {
		S3Client client;
		std::cout << "\nUploading File: " << filename_ << std::endl; 
		ifstream newFileToUpload(dir_ + "/" + filename_);
		std::stringstream fileContent;
		fileContent << newFileToUpload.rdbuf();
	
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

void CrescentHub::Update() {
	fileWatcher.update(); 
}