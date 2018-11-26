#ifndef _FSJ_DEFINES_C
#define _FSJ_DEFINES_C
    #define PROGRAM_NAME "splitter"
    #define PROGRAM_VERSION "1.0"
    #define HELP "-help"
    #define VERSION "-version"
    #define DASH_B "-b"
    #define DASH_KB "-kb"
    #define DASH_MB "-mb"
    #define DASH_GB "-gb"
    #define DASH_SPLIT "-sp"
    #define SETTINGS "-settings"
    #define FILE_J "-file"
    #define DEFAULT_SETTINGS_FILE "settings.xml"
#endif

#include "stdafx.h"
#include "JException.hpp"
#include "FSJ_Utilities.h"
#include "StringUtils.hpp"
#include <string>

void printHelp()
{
	std::cout << "Help:\n\n"
		<< "If you drag your file onto this exe the following will be run with the settings\nin settings.xml: ./" << PROGRAM_NAME << " fileToSplit\n"
		<< "\nOther example: ./" << PROGRAM_NAME << " fileToSplit -kb 32\n"
		<< "\n\nOptions:\n"
		<< " -b 32000, each file will be 32000bytes in size\n"
		<< " -kb 32, same as above but kilobytes\n"
		<< " -mb 10, each file will be split into 10 mb pieces\n"
		<< " -gb 5, this is here for future proof reasons\n"
		<< " -sp 4, the file will be split into 4 parts\n"
		<< " -settings settings.s, use the settings from this file\n"
		<< " -file MyFile.zip, this file will have the splitting operations applied"
		<< "\nNote: -b/kb/mb/gb and -sp are not compatible, i.e. don't use them at the same\ntime, use one or the other\n";
}

void printVersion()
{
	std::cout << "Version: " << PROGRAM_VERSION << '\n';
}


int main(int num, char** inputs)
{
	StringUtils su;
	FSJ_Utilities util;
	std::string filename;
	std::string method;
	int value;
	int position=0;
	bool useSplit = false;
	long long int splitInBytes=0;


	try
	{
		if ((su.CmdLineInputContains(num, inputs, 1, (char*)HELP))!=-1)
		{
			printHelp();
			return 0;
		}
		else if ((su.CmdLineInputContains(num, inputs, 1, (char*)VERSION)) != -1)
		{
			printVersion();
			return 0;
		}
		else if (((position = su.CmdLineInputContains(num, inputs, 1, (char*)SETTINGS)) != -1) || num==2)
		{
			std::string* nameAndValue;
			if (num == 2)
			{
				nameAndValue = util.ReadSettings((char*)DEFAULT_SETTINGS_FILE);
			}
			else
			{
				nameAndValue = util.ReadSettings(inputs[position + 1]);
			}
			method = '-';
			method += nameAndValue[0];
			value = atoi(nameAndValue[1].c_str());

			char* methodC = (char*)method.c_str();
			if (su.StringsMatch(methodC, (char*)DASH_SPLIT))
			{
				useSplit = true;
			}
			else if (su.StringsMatch(methodC, (char*)DASH_B))
			{
				splitInBytes = value;
			}
			else if (su.StringsMatch(methodC, (char*)DASH_KB))
			{
				splitInBytes = value * 1000;
			}
			else if (su.StringsMatch(methodC, (char*)DASH_MB))
			{
				splitInBytes = value * 1000000;
			}
			else if (su.StringsMatch(methodC, (char*)DASH_GB))
			{
				splitInBytes = value * 1000000000;
			}
			else
			{
				throw new JException(std::string("Invalid input, use -help if you are unsure\n"));
			}
		}
		else if ((position = su.CmdLineInputContains(num, inputs, 1, (char*)DASH_SPLIT)) != -1)
		{
			method = std::string("sp");
			value = atoi(inputs[position+1]);
			useSplit = true;
		}
		//-b 32
		else if ((position = su.CmdLineInputContains(num, inputs, 1, (char*)DASH_B)) != -1)
		{
			method = std::string("b");
			value = atoi(inputs[position+1]);
			splitInBytes = value;
		}
		//-kb 1024
		else if ((position = su.CmdLineInputContains(num, inputs, 1, (char*)DASH_KB)) != -1)
		{
			method = std::string("kb");
			value = atoi(inputs[position+1]);
			splitInBytes = value * 1000;
		}
		//-mb 50
		else if ((position = su.CmdLineInputContains(num, inputs, 1, (char*)DASH_MB)) != -1)
		{
			method = std::string("mb");
			value = atoi(inputs[position+1]);
			splitInBytes = value * 1000000; //* 1000 * 1000;
		}
		//-gb 2
		else if ((position = su.CmdLineInputContains(num, inputs, 1, (char*)DASH_GB)) != -1)
		{
			method = std::string("gb");
			value = atoi(inputs[position+1]);
			splitInBytes = value * 1000000000; //* 1000 * 1000 * 1000;
		}
		else if (num == 1)
		{
			printHelp();
			return 0;
		}
		else
		{
			throw new JException(std::string("Invalid input, use -help if you are unsure\n"));
		}


		//check if they included a file - in one of 2 ways
		if ((position = su.CmdLineInputContains(num, inputs, 1, (char*)FILE_J)) != -1)
		{
			//std::cout << "Line:\n" << merged << "\n";
			filename = inputs[position + 1];
			if (!util.FileExists(filename))
			{
				throw new JException(std::string("Given filename does not exist! : " + filename));
			}
		}
		//could be the user dragged the file onto the exe
		else if (num == 2)
		{
			filename = std::string(inputs[1]);
			if (!util.FileExists(filename))
			{
				throw new JException(std::string("Given filename does not exist! 2"));
			}
		}
		//no given file!
		else
		{
			throw new JException(std::string("You must specify a file!"));
		}

		//at this point we have a valid file
		long long int fileSize = util.GetFileSize(filename);

		//-sp was selected
		if (useSplit)
		{
			splitInBytes = fileSize / value;
			if (splitInBytes == 0)
			{
				splitInBytes = 1;
			}
		}

		if (splitInBytes > fileSize)
		{
			splitInBytes = fileSize;
		}

		util.SplitAndWriteFile(filename, filename, splitInBytes, fileSize);
		std::cout << "Done!\n";
	}
	catch (JException* e)
	{
		e->PrintMessage();
	}
	std::string hold;
	std::cin >> hold;
	return 0;
}