#include "header\DFA.h"
#include <iostream>
#include <vector>
#include <string>
#include<set>

using namespace std;
vector<vector<int>> newStates;
vector<int> start;
vector<int> sortVec;
vector<int> closVec;
void sortVector();
int counter = 0;
void closureState(int index);
bool checkExistingVector(vector<int> vec);
std::vector<std::vector<int>> containerV = std::vector<std::vector<int>>();
std::vector<std::vector<std::vector<int>>> DFA;
std::vector<AcceptedState> finalacceptedStates;
std::set<int> setOfStates;

void printfinalacceptedStates() {
	for(int j =0; j<newStates.size();j++ ){
		for(int k =0;k<newStates[j].size();k++){
			cout<<newStates[j][k]<<" ";
		}
		cout<<"\n";
	}
	for (unsigned i = 0; i < finalacceptedStates.size(); i++) {
		std::cout << finalacceptedStates[i].stateName << ",\n"
				<< finalacceptedStates[i].stateNumber << "\n";
	}
}

void printDFA() {

	std::cout << "DFA \n";

	for (unsigned i = 0; i < DFA.size(); i++) {
		for (unsigned j = 0; j < DFA[i].size(); j++) {
			std::cout << i << " " << (char) DFA[i][j][0] << " " << DFA[i][j][1]
					<< "\t";
		}
		std::cout << "\n";
	}
	cout << "finalacceptedStates" << finalacceptedStates.size();
}

void generateAcceptanceStates() {
	bool found = false;
	string stateMatchedString = "";
	int newNumber;
	closVec.clear();
	for (int i = 0; i < newStates.size(); i++) {
		closVec.clear();
		closVec.reserve(newStates[i].size());
		copy(newStates[i].begin(),newStates[i].end(),back_inserter(closVec));
		sortVec.clear();
		sortVector();
		newStates[i].clear();
		newStates[i].reserve(closVec.size());
		copy(closVec.begin(),closVec.end(),back_inserter(newStates[i]));
	}
	for (int i = 0; i < newStates.size(); i++) {
		for (int j = 0; j < newStates[i].size(); j++) {

			for (int k = 0; k < acceptedStates.size(); k++) {
				if (acceptedStates[k].stateNumber == newStates[i][j]) {
					found = true;
					stateMatchedString = acceptedStates[k].stateName;
					break;
				}
			}
			if (found) {
				AcceptedState accept(stateMatchedString, i);
				finalacceptedStates.push_back(accept);
				found = false;
				break;

			}
		}
	}

}

void closureState(int index) {
	for (int i = 0; i < NFS[index].size(); i++) {

		if (NFS[index][i][0] == -1) {

			if(setOfStates.find(NFS[index][i][1]) == setOfStates.end()){
				    setOfStates.insert(NFS[index][i][1]);


					closureState(NFS[index][i][1]);
			}

		}

	}

}


void searchInput(int i, int j, int input) {
	for (int y = 0; y < newStates[i].size(); y++) {
		//int search =  NFS[newStates[i][y]];
		int search = newStates[i][y];

		if (search != newStates[i][j]) {
			for (int x = 0; x < NFS[newStates[i][y]].size(); x++) {
				int searchInp = NFS[newStates[i][y]][x][0];

				if (input == searchInp) {
					setOfStates.insert(NFS[newStates[i][y]][x][1]);

					closureState(NFS[newStates[i][y]][x][1]);
					closVec.clear();

										std::copy(setOfStates.begin(), setOfStates.end(), std::back_inserter(closVec));

										//sortVector();
										setOfStates.clear();

					//sortVector();

					break;
				}
			}
		}

	}

}

bool checkInput(int input) {


	if (!containerV.empty()) {
		for (int i = 0; i < containerV.size(); i++) {
			if (containerV[i][0] == input) {
				return false;
			}
		}

	}
	return true;
}
std::vector<std::vector<std::vector<int>>> NFAtoDFA() {
	setOfStates.insert(0);

	closureState(0);
	std::copy(setOfStates.begin(), setOfStates.end(), std::back_inserter(closVec));
	sortVector();
	newStates.push_back(sortVec);
	sortVec.clear();
	closVec.clear();
	setOfStates.clear();

	std::vector<std::vector<int>> containerV = std::vector<std::vector<int>>();
	for (int i = 0; i < newStates.size(); i++) {
		for (int j = 0; j < newStates[i].size(); j++) {
			for (int k = 0; k < NFS[newStates[i][j]].size(); k++) {
				int input = NFS[newStates[i][j]][k][0];
				int state = NFS[newStates[i][j]][k][1];

				checkInput(input);
				if (input != -1 && checkInput(input)) {

					vector<int> tempSates;
					tempSates.push_back(state);

					setOfStates.insert(state);
					closureState(state);


					//break;
					searchInput(i, j, input);
										std::copy(setOfStates.begin(), setOfStates.end(), std::back_inserter(closVec));


										//sortVector();
										setOfStates.clear();






					if (!checkExistingVector(closVec)) {
						newStates.push_back(closVec);

						counter = newStates.size() - 1;
					}

					vector<int> v = std::vector<int>();
					v.push_back(input);

					v.push_back(counter);
					containerV.push_back(v);
					tempSates.clear();
					closVec.clear();
				}
			}
		}
		DFA.push_back(containerV);

		containerV.clear();

	}
	generateAcceptanceStates();

	printDFA();

	printfinalacceptedStates();
	cout<<"\n";
	for (int k = 0; k < acceptedStates.size(); k++) {
		cout<<"acceptedStates[k].stateNumber  "<<acceptedStates[k].stateNumber <<"acceptedStates[k].stateName  "<<acceptedStates[k].stateName<<"\n";

				}

	return DFA;
}
void sortVector() {
	for (int i = 0; i < closVec.size(); i++)
		sortVec.push_back(closVec[i]);

	closVec.clear();
	for (int i = 0; i < sortVec.size(); i++)
		closVec.push_back(sortVec[i]);
	int miniPos;
	int temp;
	for (int i = 0; i < sortVec.size(); i++) {
		miniPos = i;
		for (int j = i + 1; j < sortVec.size(); j++) {
			if (sortVec[j] < sortVec[miniPos]) {
				miniPos = j;
			}
		}

		temp = sortVec[miniPos];
		sortVec[miniPos] = sortVec[i];
		sortVec[i] = temp;
	}
	/*std::cout
			<< "sortggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggg"
			<< "\n";
	for (int soso = 0; soso < sortVec.size(); soso++) {
		std::cout << "sort" << sortVec[soso] << "\n";
	}
	std::cout << "\n";*/
	closVec.clear();
	for (int i = 0; i < sortVec.size(); i++)
		closVec.push_back(sortVec[i]);
}

bool checkExistingVector(vector<int> vec) {
	for (int i = 0; i < newStates.size(); i++) {
		if (vec == newStates[i]) {
			counter = i;
			return true;
		}
	}
	return false;
}
