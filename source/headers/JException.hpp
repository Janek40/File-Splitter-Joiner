#ifndef _JEXCEPTION_H
#define _JEXCEPTION_H
#include <iostream>
#include <string>

class JException
{
    private:
        std::string Message;
        int SeverityLevel;

    public:

        std::string GetMessage()
        {
            return Message;
        }
        int GetSeverityLevel()
        {
            return SeverityLevel;
        }
		void PrintMessage()
		{
			std::cout << Message;
		}
        void PrintMessageFull()
        {
            std::cout << Message << ' ' << SeverityLevel;
        }
    

        JException(std::string inM, int Sev)
        {
            Message = inM;
            SeverityLevel = Sev;
        }
        JException(std::string inM)
        {
            Message = inM;
            SeverityLevel = 0;
        }
		JException()
		{
			Message = "";
			SeverityLevel = 0;
		}
};
#endif