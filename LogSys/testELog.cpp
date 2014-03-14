
#include <iostream>
using namespace std;

template<>
LogSys *AppLogSys::instance = nullptr;

int main()
{
	AppLogSys::getInstancePtr()->CommonLogInit("log_testLogSys");

	AppLogSys::getInstancePtr()->LogOutLn("\t*****************************");
	AppLogSys::getInstancePtr()->LogOutLn("\t*         testLogSys        *");
	AppLogSys::getInstancePtr()->LogOutLn("\t*****************************");

	system("pause");

	return 0;
}