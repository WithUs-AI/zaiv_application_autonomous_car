#include "JSONIncludes.h"
#include <string>
#include "JSONBase.h"
#include <fstream>
#include <sstream>
#include <iostream>

std::string JSONBase::Serialize() const
{
	rapidjson::StringBuffer ss;
	rapidjson::Writer<rapidjson::StringBuffer> writer(ss);		// Can also use Writer for condensed formatting
	if (Serialize(&writer))
		return ss.GetString();
	return "";
}

bool JSONBase::Deserialize(const std::string& s)
{
	rapidjson::Document doc;
	if (!InitDocument(s, doc)) {
        std::cout << "Failed to init json documents" << std::endl;
		return false;
    }

	Deserialize(doc);

	return true;
}

bool JSONBase::DeserializeFromFile(const std::string& filePath)
{
	std::ifstream f(filePath);
	std::stringstream buffer;
	buffer << f.rdbuf();
	f.close();

	return Deserialize(buffer.str());
}

bool JSONBase::SerializeToFile(const std::string& filePath)
{
	std::ofstream f(filePath);
	std::string s = Serialize();
	f << s;
	f.flush();
	f.close();

	return true;
}

bool JSONBase::InitDocument(const std::string& s, rapidjson::Document& doc)
{
	if (s.empty()) {
        std::cout << "There is no data." << std::endl;
		return false;
    }

	std::string validJson(s);

	return !doc.Parse(validJson.c_str()).HasParseError() ? true : false;
}