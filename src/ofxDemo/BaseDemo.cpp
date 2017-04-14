// local
#include "../defs.h"
#ifdef DEMOS
#include "BaseDemo.hpp"
// ofxAddons
#include "ofxGui.h"

using namespace ofWarpingHalos::demos;

BaseDemo::BaseDemo() : paramsFile(""){
}

void BaseDemo::_setup(){
    // load params if we have a filename
    if(paramsFile != ""){
        ofxPanel gui;
        gui.setup(params);
        gui.loadFromFile(paramsFile);
    }

    AbstractDemo::_setup();
}

void BaseDemo::_destroy(){
    // save params if we have a filename
    if(bSetup && paramsFile != ""){
        ofxPanel gui;
        gui.setup(params);
        gui.saveToFile(paramsFile);
    }

    AbstractDemo::_destroy();
}

void BaseDemo::setName(const string& newName){
    // set params group name
    params.setName(newName);
    // set params file name
    paramsFile = "params-"+newName+".xml";
    // setName parent class
    AbstractDemo::setName(newName);
}

#endif
