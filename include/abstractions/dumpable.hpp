#ifndef __DUMPABLE_H_
#define __DUMPABLE_H_

class Dumpable {
public:
	virtual void Dump(ostream &out) = 0;
};

#endif