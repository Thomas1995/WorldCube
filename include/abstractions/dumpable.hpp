#pragma once

class Dumpable {
public:
	virtual void Dump(ostream &out) = 0;
};
