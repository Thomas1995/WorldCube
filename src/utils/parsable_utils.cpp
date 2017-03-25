#include "utils/parsable_utils.hpp"
#include <iostream>

// Action

void Action::Dump(ostream &out) {
  out << "Action \"" << name << "\"\n";
  out << "InfoMessages: ";
  for (auto x : infoMessages) {
    out << "\"" << x.message << "\" ";
  }
  out << "\nNeedChanges: ";
  for (auto x : needChanges) {
    out << "[" << x.name << " -> " << x.delta << "] ";
  }
  out << endl;
}

void Item::Dump(ostream &out) {}

// InfoMessage

void InfoMessage::Print(unsigned long long t, void* context) {
	std::cout << t << ": " << static_cast<InfoMessage*>(context)->message << "\n";
}