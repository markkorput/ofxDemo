#pragma once 

// OF
#include "ofMain.h"
// local
#include "AbstractDemo.hpp"

namespace ofWarpingHalos { namespace demos {
    
    class BaseDemo : public AbstractDemo {
        
    public: // methods
        BaseDemo();
        void _setup();
        void _destroy();

        void setName(const string& newName);

    private: // attributes

        string paramsFile;
    
    public: // params
        
        ofParameterGroup params;
    };

}}
