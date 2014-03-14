
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

#include "ConfigFile.h"

ConfigFile::ConfigFile(string name, string parentDebugInfo)
{
	debugInfo = parentDebugInfo + ", " + name;
}

ConfigFile::ConfigFile(string configFile, char** envp)
{
	while ( envp && *envp )
	{
		string envEntry = *envp;
		size_t pos = envEntry.find('=');
		if (pos != string::npos)
		{
			string name = envEntry.substr(0, pos);
			string value = envEntry.substr(pos+1, string::npos);
			envSymbols[name] = value;
			printf("[ConfigFile::ConfigFile]environment symbol: name[%s] = value[%s]\n",name.c_str(), value.c_str() );
		}
		++envp;
	}

	debugInfo = configFile;
	groupStack.push_front(this);

	FILE* in;
	fopen_s( &in, configFile.c_str(), "r");
	if ( !in )
	{
		printf("[ConfigFile::ConfigFile] cannot open input file[%s]\n",configFile.c_str());
		system("pause");
		exit(2);
	}

	char buff[1024];
	while (fgets(buff, 1024, in))
	{
		string line=buff;
		if ( (line.length() > 2) && (line[0] != '#') && (line.find(')') == string::npos) )
		{
			string name;
			string value;
			split(line, name, value, '=');

			if (value == "(")
			{
				printf("[ConfigFile::ConfigFile]config: new group[%s]\n",name.c_str());
				ConfigFile* newGroup = new ConfigFile(name, debugInfo);
				groupStack.front()->groups[name] = newGroup;
				groupStack.push_front(newGroup);
			}
			else
			{
				for (list<ConfigFile*>::reverse_iterator i = groupStack.rbegin(); i != groupStack.rend(); ++i)
				{
					(*i)->symbolExpand(value);
				}
				envSymbolExpand(value);
				printf("[ConfigFile::ConfigFile] name[%s] value[%s]\n",name.c_str(), value.c_str() );
				groupStack.front()->add(name, value);
			}
		}
		if ( (line.length() > 0) && (line[0] != '#') && (line.find(')') != string::npos) )
		{
			printf("[ConfigFile::ConfigFile] end of group\n");
			groupStack.pop_front();
		}
	}

	fclose(in);
}

ConfigFile::~ConfigFile()
{
	for (map<string, ConfigFile*>::iterator i = groups.begin(); i != groups.end(); ++i)
	{
		delete i->second;
	}
}

void ConfigFile::add(string name, string value)
{
	symbols[name] = value;
}

void ConfigFile::split(string in, string& left, string& right, char c)
{
	size_t pos = in.find_first_of(c);
	if(pos == string::npos)
	{
		left = in;
		trim(left);
		right = "";
	}
	else if (pos <= 1)
	{
		left = "";
		right = in.substr(pos+1, string::npos);
		trim(right);
	}
	else
	{
		left = in.substr(0, pos-1);
		trim(left);
		right = in.substr(pos+1, string::npos);
		trim(right);
	}
}

void ConfigFile::trim(string& s)
{
	while ( (s.length() > 1) && ( (s[0] == ' ') || (s[0] =='\t') ) )
	{
		s = s.substr(1, string::npos);
	}
	while ( (s.length() > 1) &&
		( (s[s.length()-1] == ' ') ||
		(s[s.length()-1] == '\t') || 
		(s[s.length()-1] == '\n') || 
		(s[s.length()-1] == '\r') ) )
	{
		s = s.substr(0, s.length()-1);
	}
	if ( (s.length() > 1) && (s[0] == '"') )
	{
		s = s.substr(1, string::npos);
	}
	if ( (s.length() > 1) && (s[s.length()-1] == '"') )
	{
		s = s.substr(0, s.length()-1);
	}
}

void ConfigFile::symbolExpand(string& s)
{
	symbolExpand(symbols, s);
}

void ConfigFile::envSymbolExpand(string& s)
{
	symbolExpand(envSymbols, s);
}

void ConfigFile::symbolExpand(map<string, string>& symbols, string& s)
{
	bool expanded;
	do
	{
		expanded = false;
		for (map<string, string>::iterator i = symbols.begin(); i != symbols.end(); ++i)
		{
			string search = "%" + i->first + "%";
			string replace = i->second;
			size_t pos = s.find(search);
			if (pos != string::npos)
			{
				expanded = true;
				s.replace(pos, search.length(), replace);
			}
		}
	} while (expanded);
}

string ConfigFile::pString(string name)
{
	map<string, string>::iterator i = symbols.find(name);
	if (i == symbols.end())
	{
		printf("[ConfigFile::pString]access of missing property[%s] debug info[%s]\n", name.c_str(), debugInfo.c_str());
		system("pause");
		exit(4);
	}
	return i->second;
}

bool ConfigFile::pBool(string name)
{
	string val = pString(name);

	if ( (val == "yes") ||
		(val == "Yes") ||
		(val == "YES") ||
		(val == "true") ||
		(val == "True") ||
		(val == "TRUE") )
	{
		return true;
	}

	return false;
}

double ConfigFile::pDouble(string name)
{
	string val = pString(name);

	return atof(val.c_str());
}

int ConfigFile::pInt(string name)
{
	string val = pString(name);

	return atoi(val.c_str());
}