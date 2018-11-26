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
	std::string merged = su.merge2DCharArray(num, inputs, 1);

	//Settings* settings;
	try
	{
		//More than just opening the program
		if (num > 1)
		{
			FSJ_Utilities util;
			std::string filename;
			std::string checkFor;
			int position;


			if ((position = su.CmdLineInputContains(num, inputs, 1, (char*)HELP)) != -1)
			{
				printHelp();
				return 0;
			}
			//Print version information
			else if ((position = su.CmdLineInputContains(num, inputs, 1, (char*)VERSION)) != -1)
			{
				printVersion();
				return 0;
			}
			

			//check if they included a file - in one of 2 ways
			if ((position = su.CmdLineInputContains(num, inputs, 1, (char*)FILE_J)) != -1)
			{
				std::cout << "have?\n";
				filename = inputs[position + 1];
			}
			//could be the user dragged the first file onto the exe
			else if (num == 2)
			{
				filename = std::string(inputs[1]);
			}
			//no given file!
			else
			{
				throw new JException(std::string("You must specify a file! 1"));
			}

			std::string origName = util.GetOriginalFileName(filename);
			//std::cout << "Original filename: " << origName << '\n';
			util.JoinAndWriteFile(origName, origName);

			
			std::cout << "Done!\n";
		}
		//They just ran the exe
		else
		{
			printHelp();
		}
	}
	catch (JException* e)
	{
		e->PrintMessage();
	}

	std::string hold;
	std::cin >> hold;
	return 0;
}