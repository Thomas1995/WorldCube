#include "utils/parsable_utils.hpp"
#include <iostream>
#include "person.hpp"

// Action

void Action::Dump(ostream &out) {
  out << "Action \"" << name << "\"\n";
  out << "InfoMessages: ";
  for (auto x : infoMessages) {
    out << "\"" << x.message << "\" ";
  }
  out << "\nEffects: ";
  for (auto x : effects) {
    out << "[" << x.name << " -> " << x.delta << "] ";
  }
  out << endl;
}

void Item::Dump(ostream &out) {}

// InfoMessage

void InfoMessage::s_Print(unsigned long long t, void* context, void* additionalInfo) {
	static_cast<InfoMessage*>(context)->Print(t, static_cast<Person*>(additionalInfo));
}

void InfoMessage::Print(unsigned long long t, Person* owner) {
	std::string printMsg, var;
	bool inVar = false;

	for (const char& c : message) {
		if (c == '%') {
			if (inVar) {
				printMsg += owner->GetEnvVar(var);
				var.clear();
			}
			inVar = !inVar;
			continue;
		}

		if (inVar) {
			var.push_back(c);
		}
		else {
			printMsg.push_back(c);
		}
	}

	std::cout << t << ": " << printMsg << "\n";
}