#include "utils/parsable_utils.hpp"

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
