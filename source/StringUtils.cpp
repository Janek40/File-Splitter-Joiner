#include "stdafx.h"
#include "StringUtils.hpp"
#include "JException.hpp"
#include <iostream>




StringUtils::StringUtils(void)
{

}
StringUtils::~StringUtils(void)
{

}

void StringUtils::Print2DArray(int num, char** inputs)
{
	for (int i = 0;i < num;i++)
	{
		std::cout << inputs[i] << '\n';
	}
}

int StringUtils::CmdLineInputContains(int num, char** inputs, int offset, char* check)
{
	for (int i = offset;i < num;i++)
	{
		if (StringsMatch(inputs[i], check))
		{
			return i;
		}
	}
	return -1;
}

bool StringUtils::StringsMatch(char* str1, char* str2)
{
	int len1 = StrLen(str1);
	int len2 = StrLen(str2);
	if (len1 != len2)
	{
		return false;
	}
	else
	{
		for (int i = 0;i < len1;i++)
		{
			if (str1[i] != str2[i])
			{
				return false;
			}
		}
		return true;
	}
}

int StringUtils::StrLen(char* x)
{
	int a = 0;
	while (x[a] != '\0') a++;
	return a;
}


std::string StringUtils::getValueAfter(std::string* mess, int position)
{
	std::string valueStr="";
	char curr;
	for (int i = position; i < (*mess).length(); i++)
	{
		if ((curr=(*mess)[i]) != ' ')
		{
			valueStr += curr;
		}
		else
		{
			break;
		}
	}
	
	if (valueStr.length() > 0)
	{
		return valueStr;
	}
	else
	{
		throw new JException(std::string("You did not provide a value!"));
	}
}


//Best case scenario: O(1)
//Average case: O(m)
//Worst case: O(mk)
// Function returns true if key is present in message
int StringUtils::checkIfContains_Simple(std::string* key, std::string* mess)
{
	bool test = true;
	int a=0;
	if (key->length() > mess->length())
	{
		return 0;
	}
	
	for (int i = 0;i<mess->length();i++)
	{
		
		//if the main string contains the first letter of the key
		if ((*mess)[i] == (*key)[0] && (mess->length() - i + 1 >= key->length()))
		{
			//if (split) std::cout << "\nPossible:\n";
			//for the length of the key-1
			for (a = 1;a<key->length();a++)
			{
				//if they don't match up at all
				//if (split) std::cout << "Comparing: " << (*mess)[i + a] << " and " << (*key)[a] << '\n';
				if ((*mess)[i + a] != (*key)[a])
				{
					test = false;
					break;
				}
			}
			//reset this variable, keep looping
			if (test == false)
			{
				test = true;
			}
			//match!
			else
			{
				return i+a;
			}
		}

		
	}
	//if it got this far then it was never found
	return 0;
}


//Best case scenario: O(1)
//Average case: O(n)
//Worst case: O(n)
//Function takes in a C style 2D array and outputs a single c++ string
std::string StringUtils::merge2DCharArray(int len, char** arr, int offset)
{
    std::string ans;
	ans += ' ';
    for(int i=offset;i<len;i++)
    {
        ans += arr[i];
        ans += ' ';
    }
	//std::cout << "returning: " << ans << '\n';
    return ans;
}
