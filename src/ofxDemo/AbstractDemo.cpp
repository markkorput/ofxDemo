#include "AbstractDemo.h"

using namespace ofxDemo;

AbstractDemo::AbstractDemo(){
    activeParam.set("active", false);
    bSetup = false;
}

void AbstractDemo::_setup(){
    setup();
    activeParam.addListener(this, &AbstractDemo::_onActiveParamChange);
    bSetup=true;
}

void AbstractDemo::_destroy(){
    activeParam.set(false);
    activeParam.removeListener(this, &AbstractDemo::_onActiveParamChange);

    if(bSetup){
        destroy();
        bSetup = false;
    }
}

void AbstractDemo::_activate(bool activate){
    if(activate){
        ofAddListener(ofEvents().keyPressed, this, &AbstractDemo::onKeyPressed);
        ofAddListener(ofEvents().keyReleased, this, &AbstractDemo::onKeyReleased);

        ofAddListener(ofEvents().mouseDragged, this, &AbstractDemo::onMouseDragged);
        ofAddListener(ofEvents().mousePressed, this, &AbstractDemo::onMousePressed);
        ofAddListener(ofEvents().mouseReleased, this, &AbstractDemo::onMouseReleased);

        ofAddListener(ofEvents().windowResized, this, &AbstractDemo::onWindowResized);
    } else {
        ofRemoveListener(ofEvents().keyPressed, this, &AbstractDemo::onKeyPressed);
        ofRemoveListener(ofEvents().keyReleased, this, &AbstractDemo::onKeyReleased);

        ofRemoveListener(ofEvents().mouseDragged, this, &AbstractDemo::onMouseDragged);
        ofRemoveListener(ofEvents().mousePressed, this, &AbstractDemo::onMousePressed);
        ofRemoveListener(ofEvents().mouseReleased, this, &AbstractDemo::onMouseReleased);

        ofRemoveListener(ofEvents().windowResized, this, &AbstractDemo::onWindowResized);
    }
}

void AbstractDemo::_onActiveParamChange(bool &newValue){
    _activate(newValue);
    activate(newValue);
}

void AbstractDemo::setName(const string &newName){
    name = newName;
}
