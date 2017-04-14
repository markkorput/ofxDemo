#pragma once

// OF
#include "ofMain.h"
// local
#include "AbstractDemo.h"

namespace ofxDemo {

    // This is an example ParamsDemo class that loads
    // and saves its own params xml file at setup/destroy
    class ParamsDemo : public AbstractDemo {

    public: // methods
        ParamsDemo();
        void _setup();
        void _destroy();

        void setName(const string& newName);

    private: // attributes

        string paramsFile;

    public: // params

        ofParameterGroup params;
    };

} // namespace ofxDemo
