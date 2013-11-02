#ifndef _ERROR_HELPER_H_
#define _ERROR_HELPER_H_

class ErrorHelper
{
public:
    static const char* GetErrorMsg(uint _val);
    static void PrintMsg(uint _val);
};

std::string& ErrorHelper::GetErrorMsg(uint _val)
{
    switch (_val)
    {
    case WSAEPROCLIM:
        return "Too many processes.";
    case WSASYSNOTREADY:
        return "Network subsystem is unavailable.";
    case 
    }
}

void ErrorHelper::PrintMsg(uint _val)
{
    std::cout << "Error Code : " << _val << ", "<< GetErrorMsg(_val) << std::endl;
}

#endif