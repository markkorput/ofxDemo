#pragma once

#include "ofMain.h"

namespace ofxDemo {

    class AbstractDemo {

    public: // methods

        AbstractDemo();
        ~AbstractDemo(){ _destroy(); }

        // methods not meant to be overloaded...
        void _setup();
        void _destroy();
        void _activate(bool activate=true);

        // these virtual methods should be overloaded by inheritors
        virtual void setup(){}
        virtual void destroy(){}
        virtual void update(){}
        virtual void draw(){}
        virtual void drawDebug(){}
        virtual void activate(bool activate=true){}

        string getName(){ return name; }
        void setName(const string &newName);

    public: // getters/setter

        bool isSetup(){ return bSetup; }

    protected: // callbacks

        void _onActiveParamChange(bool &newValue);
        virtual void onKeyPressed(ofKeyEventArgs &args){}
        virtual void onKeyReleased(ofKeyEventArgs &args){}
        virtual void onMouseDragged(ofMouseEventArgs &args){}
        virtual void onMousePressed(ofMouseEventArgs &args){}
        virtual void onMouseReleased(ofMouseEventArgs &args){}
        virtual void onWindowResized(ofResizeEventArgs &args){}

    protected: // attributes

        bool bSetup;
        string name = "AbstractDemo";

    public: // params

        ofParameter<bool> activeParam;
    }; // class AbstractDemo
} // namespace ofxDemo
