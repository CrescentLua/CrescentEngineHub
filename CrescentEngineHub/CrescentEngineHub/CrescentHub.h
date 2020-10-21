#ifndef CRESCENTHUB_H 
#define CRESCENTHUB_H 

#include <memory>
#include <fstream>
#include <aws/s3/S3Client.h>
#include <aws/s3/model/PutObjectRequest.h>
#include <aws/s3/model/GetObjectRequest.h>
#include <aws/core/Aws.h>
#include <aws/core/utils/memory/stl/AWSStringStream.h> 
#include "FileWatcher/UpdateListener.h"

using namespace std;
using namespace std::string_literals;
using namespace Aws::S3;
using namespace Aws::S3::Model;

static const char* BUCKET = "crescentenginebucket";
class CrescentHub
{
public: 
	CrescentHub(const CrescentHub&) = delete;
	CrescentHub(CrescentHub&&) = delete;
	CrescentHub& operator=(const CrescentHub&) = delete;
	CrescentHub& operator=(CrescentHub&&) = delete;

	static CrescentHub* GetInstance();
	void Run(); 
	void UploadFile(std::string dir_, std::string filename_); 

private: 
	CrescentHub(); 
	~CrescentHub();
	static std::unique_ptr<CrescentHub> hubInstance;
	friend std::default_delete<CrescentHub>;

	Aws::SDKOptions options;
	
	bool isRunning; 
	void Update();

	// create the listener (before the file watcher - so it gets destroyed after the file watcher)
	UpdateListener listener;

	// create the file watcher object
	FW::FileWatcher fileWatcher;
};
#endif 
