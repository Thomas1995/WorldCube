#pragma once

class Dumpable {
public:
	virtual void Dump(std::ostream &out) = 0;
};
