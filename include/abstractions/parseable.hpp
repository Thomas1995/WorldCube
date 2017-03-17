#pragma once

class Parseable {
public:
	virtual void ParseFromXML(XMLNode node) = 0;
	virtual string GetTagName() = 0;
};
