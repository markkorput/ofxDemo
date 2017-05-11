#pragma once

// addons
#include "ofMain.h"

#ifdef OFXOPERATIONS
    #include "ofxOperations.h"
#endif

namespace ofxDemo {

    template<class DemoClass>
    class DemoManager {

    public: // methods

        DemoManager() :
        #ifdef OFXOPERATIONS
                        generatedOperations(nullptr),
        #endif
                        activeDemo(nullptr)
                        {}

         ~DemoManager(){ destroy(); }
        void destroy();
        bool update();
        bool draw();
        bool drawDebug();

        void add(shared_ptr<DemoClass> newDemo);
        void deactivate();
        void activate(int i);
        void activate(const string& name);
        void activate(shared_ptr<DemoClass> demo);

        shared_ptr<DemoClass> getActive();
        int getCount(){ return demos.size(); }
        int getActiveIndex();
        const string getActiveName();
        shared_ptr<DemoClass> getByName(const string &name);

    // public:
    //
    //     ofEvent<DemoClass> activateEvent;
    //     ofEvent<DemoClass> deactivateEvent;

    private: // attributes

        std::vector<shared_ptr<DemoClass>> demos;
        shared_ptr<DemoClass> activeDemo;

    // ofxOperations support must be explicitly enabled
    // through this pre-oprocessor macro
    #ifdef OFXOPERATIONS
        public: // methods
            shared_ptr<ofxOperations::OperationGroup> generateOperations();
        public: // attributes
            shared_ptr<ofxOperations::OperationGroup> generatedOperations;
        private: // callbacks
            void onDestroyOpEnd(ofxOperations::Operation &op);
            void onActivateOpEnd(ofxOperations::Operation &op);
    #endif
    };

//
// Helper Class
//

#ifdef OFXOPERATIONS

    // Operation subclass with demo attribute
    template<class DemoClass>
    class DemoOp : public ofxOperations::Operation {
    public:
        shared_ptr<DemoClass> demo;
    };

    // Creates a group of activate operations, and destroy operations
    // for all demos which have been setup-ed.
    // It stores the group locally in the public 'generatedOperations' attribure,
    // which should be 'followed' by the main application's operation group, as
    // new destroy operations will appear there as new demos get activated, and
    // existing destroy operations will disappear after they are executed
    template<class DemoClass>
    shared_ptr<ofxOperations::OperationGroup> DemoManager<DemoClass>::generateOperations(){
        // the local generatedOperations attribute basically
        // caches the execution of this method
        if(generatedOperations != nullptr)
            return generatedOperations;

        generatedOperations = make_shared<ofxOperations::OperationGroup>();

        for(auto demo : demos){
            // create an 'Activate' operation for every demo
            {
                auto op = make_shared<DemoOp<DemoClass>>();
                op->demo = demo;
                op->setName("Activate demo \"" + demo->getName() + "\"");
                generatedOperations->add(op);
                ofAddListener(op->endEvent, this, &DemoManager<DemoClass>::onActivateOpEnd);
            }

            // Create a 'Destroy' (Cleanup) operation if the demo isSetup
            if(demo->isSetup())
            {
                auto op = make_shared<DemoOp<DemoClass>>();
                op->demo = demo;
                op->setName("Cleanup demo \"" + demo->getName() + "\"");
                generatedOperations->add(op);
                ofAddListener(op->endEvent, this, &DemoManager<DemoClass>::onDestroyOpEnd);
            }
        }

        return generatedOperations;
    }

    template<class DemoClass>
    void DemoManager<DemoClass>::onDestroyOpEnd(ofxOperations::Operation &op){
        if(generatedOperations != nullptr)
            generatedOperations->remove(op);

        auto demo = ((DemoOp<DemoClass>*)&op)->demo;

        if(this->getActive() == demo)
            deactivate();

        demo->_destroy();
    }

    template<class DemoClass>
    void DemoManager<DemoClass>::onActivateOpEnd(ofxOperations::Operation &op){
        this->activate(((DemoOp<DemoClass>*)&op)->demo);
    }

#endif

} // namespace ofxDemo



using namespace ofxDemo;

template <class DemoClass>
void DemoManager<DemoClass>::destroy(){
    // even though all demos are shared_ptr, we can't just do demos.clear here
    // (or even just let the demos vector destructor do all the work automatically)
    // because inside destructors virtual functions don't work, so the inheriting classes'
    // destroy methods will not be invoked by the DemoClass destructor.
    // If we call _destroy manually they will though...
    for(auto demo : demos){
        demo->_destroy();
    }

    demos.clear();
}

template <class DemoClass>
bool DemoManager<DemoClass>::update(){
    auto demo = getActive();

    if(demo == nullptr)
        return false;

    demo->update();
    return true;
}

template <class DemoClass>
bool DemoManager<DemoClass>::draw(){
    auto demo = getActive();

    if(demo == nullptr)
        return false;

    demo->draw();
    return true;
}

template <class DemoClass>
bool DemoManager<DemoClass>::drawDebug(){
    auto demo = getActive();

    if(demo == nullptr)
        return false;

    demo->drawDebug();
    return true;
}

template <class DemoClass>
void DemoManager<DemoClass>::add(shared_ptr<DemoClass> newDemo){
    demos.push_back(newDemo);
    if(newDemo->activeParam.get()){
        activate(newDemo);
    }
}

template <class DemoClass>
void DemoManager<DemoClass>::deactivate(){
    for(auto demo : demos){
        if(demo->activeParam.get()){
            // ofNotifyListeners(deactivateEvent, *demo.get());
            demo->activeParam.set(false);
        }
    }

    activeDemo = nullptr;
}

template <class DemoClass>
void DemoManager<DemoClass>::activate(shared_ptr<DemoClass> demo){
    deactivate();

    activeDemo = demo;

    if(demo == nullptr){
#ifdef DEBUG
        ofSetWindowTitle("No Active Demo");
#endif
        return;
    }

    if(!demo->isSetup()){
        demo->_setup();
#ifdef OFXOPERATIONS
        if(generatedOperations != nullptr){
            // generate cleanup operation
            auto op = make_shared<DemoOp<DemoClass>>();
            op->demo = demo;
            op->setName("Cleanup demo \"" + demo->getName() + "\"");
            generatedOperations->add(op);
            ofAddListener(op->endEvent, this, &DemoManager<DemoClass>::onDestroyOpEnd);
        }
#endif // OFXOPERATIONS
    }

    demo->activeParam.set(true);
    // ofNotifyListeners(activateEvent, *demo.get());

#ifdef DEBUG
    ofSetWindowTitle(demo->getName());
#endif
}

template <class DemoClass>
void DemoManager<DemoClass>::activate(int i){
    if(i < demos.size())
        activate(demos[i]);
}

template <class DemoClass>
void DemoManager<DemoClass>::activate(const string& name){
    for(auto demo : demos){
        if(demo->getName() == name){
            activate(demo);
            return;
        }
    }

    ofLogWarning() << "could not find demo '" << name << "' to activate";
}

template <class DemoClass>
shared_ptr<DemoClass> DemoManager<DemoClass>::getActive(){
    return activeDemo;
}

template <class DemoClass>
int DemoManager<DemoClass>::getActiveIndex(){
    int i=0;
    for(auto demo : demos){
        if(demo->activeParam.get())
            return i;
        i++;
    }

    return -1;
}

template <class DemoClass>
const string DemoManager<DemoClass>::getActiveName(){
    return (activeDemo == nullptr) ? "" : activeDemo->getName();
}

template <class DemoClass>
shared_ptr<DemoClass> DemoManager<DemoClass>::getByName(const string &name){
    for(auto &demo : demos){
        if(demo->getName() == name){
            return demo;
        }
    }

    return nullptr;
}
