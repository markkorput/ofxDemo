//--------------------------------------------------------------
// ofApp.h
//--------------------------------------------------------------

#include "ofMain.h"
#include "ofxDemo.h"

class DemoGreen : public ofxDemo::AbstractDemo {
public:
    virtual void draw(){
        ofBackground(ofColor::green);
    }
};

class DemoRed : public ofxDemo::AbstractDemo {
public:
    virtual void draw(){
        ofBackground(ofColor::red);
    }
};

class ofApp : public ofBaseApp{

    public:
        void setup();
        void draw();
        void update();
        void keyPressed(int key);

    private:
        ofxDemo::DemoManager<ofxDemo::AbstractDemo> demoManager;
};

//--------------------------------------------------------------
// ofApp.cpp
//--------------------------------------------------------------

void ofApp::setup(){
    demoManager.add((shared_ptr<ofxDemo::AbstractDemo>)make_shared<DemoGreen>());
    demoManager.add((shared_ptr<ofxDemo::AbstractDemo>)make_shared<DemoRed>());
    demoManager.activate(0);
}

//--------------------------------------------------------------
void ofApp::draw(){
    auto demo = demoManager.getActive();
    if(demo){
        demo->draw();
        return;
    }

    ofBackground(0);
}

void ofApp::update(){
    // returns true when active
    if(demoManager.update())
        return;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    // 1 and 2 keys each activate a demo
    if(key >= '1' && key <= '4'){
        int idx = key-'1';
        demoManager.activate(idx);
        return;
    }

    // zero deactivates all dmeos
    if(key == '0' && demoManager.getActive() != nullptr){
        demoManager.activate(nullptr);
        return;
    }
}

//--------------------------------------------------------------
// main.cpp
//--------------------------------------------------------------

int main( ){
    ofSetupOpenGL(1024,768,OF_WINDOW);
    ofRunApp(new ofApp());
}
