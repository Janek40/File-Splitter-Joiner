#ifndef _STRINGUTILS_H
#define _STRINGUTILS_H
#include <iostream>

#define STRING_UTILS_EXPORTS 

#ifdef STRING_UTILS_EXPORTS 
#define STRING_UTILS_API __declspec(dllexport)   
#else  
#define STRING_UTILS_API __declspec(dllimport)   
#endif 


class STRING_UTILS_API StringUtils
{
    private:
        

    public:
		void Print2DArray(int num, char** inputs);

		//int CmdLineInputContains_Fast(int* num, char** inputs, char* check, int offset);
		int CmdLineInputContains(int num, char** inputs, int offset, char* check);
		bool StringsMatch(char* str1, char* str2);
		int StrLen(char* x);
		int checkIfContains_Simple(std::string* key, std::string* mess);
		std::string getValueAfter(std::string* mess, int position);
		std::string merge2DCharArray(int len, char** arr, int offset);

		StringUtils(void);
		~StringUtils(void);
};
#endif