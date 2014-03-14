

#include <iostream>
using namespace std;

#include "ConfigFile.h"

int main(int argc, char* argv[], char* envp[])
{
	// read config file with environment variable expansion support
	ConfigFile config("demo.cnf", envp);

	// basic usage of properties
	if (config.pBool("showHello"))
	{
		int cnt = config.pInt("helloCount");
		string msg = config.pString("helloMessage");
		for (int i = 0; i < cnt; ++i)
		{
			cout << msg << endl;
		}
		cout << endl;
	}

	// properties with expanded names (no difference in using)
	cout << "tempFolder    = '" << config.pString("tempFolder") << "'" << endl;
	cout << "tempSubFolder = '" << config.pString("tempSubFolder") << "'" << endl;
	cout << endl;

	// get properties for all subgroups starting with prefix
	map<string, ConfigFile*> messages = config.getGroups(); // all groups
	const string messagePrefix = "message"; // prefix for group name
	for (map<string, ConfigFile*>::iterator i = messages.begin(); i != messages.end(); ++i)
	{
		string groupName = i->first;
		ConfigFile* group = i->second;

		// test group name for prefix
		if (groupName.substr(0, messagePrefix.length()) == messagePrefix)
		{
			// display group contents
			cout << group->pString("name") << ":" << endl;
			cout << "   " << group->pString("text") << endl;
		}
	}

	system("pause");
	return 0;
}