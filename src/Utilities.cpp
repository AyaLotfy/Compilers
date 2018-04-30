
#include <bits/stdc++.h>
#include "Utilities.h"

using namespace std;



vector<string> Utilities::read_file(string file_path) {
    vector<string> vec;
    ifstream myFile(file_path);
    string line;
    if(myFile.is_open()){
        while(getline(myFile,line)){
            vec.push_back(line);
        }
    }else{
        cout << "Can not open the file" << endl;
    }
    myFile.close();
    return vec;
}
