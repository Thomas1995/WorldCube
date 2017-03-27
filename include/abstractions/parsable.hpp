#pragma once

class Parsable {
public:
	virtual void ParseFromXML(XMLNode node) = 0;
	virtual std::string GetTagName() = 0;
};
