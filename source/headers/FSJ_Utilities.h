#ifndef FSJ_UTILITIES
#define FSJ_UTILITIES
#include <string>


#define STRING_UTILS_EXPORTS 

#ifdef STRING_UTILS_EXPORTS 
#define STRING_UTILS_API __declspec(dllexport)   
#else  
#define STRING_UTILS_API __declspec(dllimport)   
#endif 


class STRING_UTILS_API FSJ_Utilities
{
    public:
		std::string GetOriginalFileName(std::string file);
		void JoinAndWriteFile(std::string originalFileName, std::string commonFileName);
		void SplitAndWriteFile(std::string file, std::string commonFileName, long long int chunkSize, long long int fileSize);
		long long int GetFileSizeO(std::ifstream* file);
		long long int GetFileSize(std::string file);
		bool FileExists(std::string file);
	    std::string* ReadSettings(std::string settings);
};
#endif
