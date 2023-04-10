//
// Created by light on 3/26/2023.
//

#include "toyprotocol_demo.h"
#include <iostream>
#include <string>

using namespace std;

int main(){
    string cmd;
    getline(cin, cmd);

    if(cmd == "OT toyprotocol start"){
        cout << "initializing toyprotocol" <<  endl;
    }
    else{
        cout << cmd << " is not a recognized command" << endl;
    }
    return;
}