
#ifndef DFA_H_
#define DFA_H_


#include <bits/stdc++.h>
#include <string>
#include <vector>
#include "AcceptedState.h"
#include "FileScanner.h"


using namespace std;
class DFAState {
public:
    DFAState();

};
extern std::vector<std::vector<std::vector<int>>> DFA;
extern std::vector<AcceptedState> finalacceptedStates;
std::vector<std::vector<std::vector<int>>> NFAtoDFA();
#endif
