#include <iostream>
#include "../include/Utils.h"
#include "../include/Config.h"
#include "../include/ConfigLoader.h"

using namespace std;

//template cellDiv
static void test_cellDiv() {
    cout<<"ceilDiv(10,3) expected 4, got "<<cellDiv(10,3)<<endl;
    cout<<"ceilDiv(6,3) expected 2, got "<<cellDiv(6,3)<<endl;
}

//ConfigLoader (verifica ca fisierul se poate citi)
static void test_configLoad() {
    Config config;
    bool ok = ConfigLoader::load("simulation_setup.txt", config);
    if(ok == 1)
        cout << "File loaded!"<<endl;
    else
        cout<<"Error in loading file!"<<endl;
}

void runAllTests() {
    test_cellDiv();
    test_configLoad();
}
