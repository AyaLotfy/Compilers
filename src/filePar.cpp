

#include <bits/stdc++.h>
#include "filePar.h"

using namespace std;

vector<string> filePar::read_file(string file_path) {
    vector<string> vec;
    ifstream myFile(file_path);
    string line;
    if(myFile.is_open()){
        while(getline(myFile,line)){
            vec.push_back(line);
        }
    }else{
        cout << "error while opening the file" << endl;
    }
    myFile.close();
    return vec;
}
