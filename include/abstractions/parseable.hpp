#ifndef __PARSEABLE_H_
#define __PARSEABLE_H_

class Parseable {
public:
	virtual void ParseFromXML(XMLNode node) = 0;
	virtual string GetTagName() = 0;
};

#endif