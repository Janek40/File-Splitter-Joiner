#include "stdafx.h"
#include "FSJ_Utilities.h"
#include "RapidXML/rapidxml_print.hpp"
#include "RapidXML/rapidxml.hpp"
#include "JException.hpp"
#include <fstream>
#include <iostream>
#include <vector>

std::string FSJ_Utilities::GetOriginalFileName(std::string file)
{
	std::string result;
	
	//start at the end of the string and work backwards
	int cutOff = 0;
	for (int i = (int)file.length();i >= 0;i--)
	{
		if (file[i] != '.')
		{
			cutOff++;
		}
		else
		{
			break;
		}
	}

	if (cutOff == file.length())
	{
		cutOff = 0;
	}

	for (int i = 0;i < file.length() - cutOff;i++)
	{
		result += file[i];
	}
	return result;
}

void FSJ_Utilities::JoinAndWriteFile(std::string originalFileName, std::string commonFileName)
{
	bool cont = true;
	std::string currentFilename;
	long long int i = 0;
	std::ifstream currentFile;
	std::ofstream outputFile(originalFileName, std::ios::out | std::ios::binary);
	if (!outputFile.is_open())
	{
		throw new JException(std::string("Unable to create original file - Do you have permission?"));
	}

	long long int size;
	while (cont)
	{
		currentFilename = commonFileName + '.' + std::to_string(i);
		currentFile.open(currentFilename, std::ios::in | std::ios::binary);
		i++;
		if (currentFile.is_open())
		{
			//std::cout << "Joining: " << currentFilename << '\n';
			size = GetFileSizeO(&currentFile);
			std::vector<char> buffer(size);
			currentFile.read(&buffer[0], size);
			outputFile.write(&buffer[0], size);
		}
		//no need to throw an error if it is not open, take it as a job done
		else
		{
			cont = false;
			outputFile.close();
		}
		currentFile.close();
	}
}

long long int FSJ_Utilities::GetFileSizeO(std::ifstream* myFile)
{
	long long int size;
	(*myFile).seekg(0, std::ios::end);
	size = (*myFile).tellg();
	(*myFile).seekg(0, std::ios::beg);
	return size;
}

void FSJ_Utilities::SplitAndWriteFile(std::string file, std::string commonFileName, long long int chunkSize, long long int fileSize)
{
	std::ifstream myInputFile(file, std::ios::binary);
	if (myInputFile.is_open())
	{
		long long int remainder = fileSize;
		long long int currentChunk = chunkSize;
		char* buffer = new char[currentChunk];

		std::ofstream myOutputFile;
		std::string outputName;
		long long int i = 0;

		while (remainder > 0)
		{
			myInputFile.read(buffer, currentChunk);
			outputName = commonFileName + '.' + std::to_string(i);
			myOutputFile.open(outputName, std::ios::binary);
			if (!myOutputFile.is_open())
			{
				myInputFile.close();
				throw new JException(std::string("Could not write split files to disk! do you have permission?"));
			}
			myOutputFile.write(buffer, currentChunk);
			myOutputFile.close();

			remainder = remainder - chunkSize;
			if (remainder < chunkSize)
			{
				currentChunk = remainder;
			}
			i++;
		}
		myInputFile.close();
	}
	else
	{
		throw new JException(std::string("Unable to open file for splitting"));
	}
}

long long int FSJ_Utilities::GetFileSize(std::string file)
{
	std::ifstream myFile(file);
	long long int fsize;
	if (myFile.is_open())
	{
		myFile.seekg(0, std::ios::end);
		fsize = myFile.tellg();
		myFile.close();
		return fsize;
	}
	else
	{
		throw new JException(std::string("Unable to open file - trying to find file size"));
	}
}

bool FSJ_Utilities::FileExists(std::string file)
{
	std::ifstream myFile(file);
	if (myFile.is_open())
	{
		myFile.close();
		return true;
	}
	else
	{
		return false;
	}
}

std::string* FSJ_Utilities::ReadSettings(std::string settings)
{
	char* text;
	std::ifstream myFile(settings);
	std::string line;
	std::string settingsContents;
	std::string* nameAndValue = new std::string[2];

	//if the file is open
	if (myFile.is_open())
	{
		myFile.seekg(0, std::ios::end);
		//if the file is larger than 5kb it is probably not correct
		if (myFile.tellg() > 5000)
		{
			throw new JException(std::string("Settings file is larger than 5KB, it should only be ~1KB?"));
		}
		myFile.seekg(0, std::ios::beg);

		//get each line and write it to a string
		//e.g. read the whole file to a string
		while (getline(myFile, line))
		{
			settingsContents += line;
		}
		myFile.close();
		//set the char* for rapidXML to start here
		text = &settingsContents[0u];
	}
	//file could not be opened
	else
	{
		throw new JException(std::string("Unable to open " + settings));
	}

	rapidxml::xml_document<char> doc;
	doc.parse<0>(text);

	//If the tag <settings> is present in the file
	if (rapidxml::xml_node<char>* root_node = doc.first_node("settings"))
	{
		//If <split> is inside the <settings>
		if (rapidxml::xml_node<char> * settings_node = root_node->first_node("split"))
		{
			rapidxml::xml_attribute<char>* nameAttrib;
			rapidxml::xml_attribute<char>* valueAttrib;

			//If the attribute of 'name' is present
			if (nameAttrib = settings_node->first_attribute("name"))
			{
				nameAndValue[0] = nameAttrib->value();
			}
			else
			{
				throw new JException(std::string("The attribute 'name' was not present"));
			}
			//if the attribute of 'value' is present
			if (valueAttrib = settings_node->first_attribute("value"))
			{
				nameAndValue[1] = valueAttrib->value();
			}
			else
			{
				throw new JException(std::string("The attribute 'value' was not present"));
			}
		}
		else
		{
			throw new JException(std::string("Unable to find <split> tag in file"));
		}
	}
	else
	{
		throw new JException(std::string("Unable to find <settings> tag in file!"));
	}
	return nameAndValue;
}