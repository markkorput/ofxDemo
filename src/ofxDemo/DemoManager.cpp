/*#include "../defs.h"
#ifdef DEMOS

#include "DemoManager.hpp"

using namespace ofWarpingHalos::demos;



#ifdef OFXOPERATIONS

// Operation subclass with demo attribute
class DemoOp : public ofxOperations::Operation {
public:
    shared_ptr<AbstractDemo> demo;
};

// Creates a group of activate operations, and destroy operations
// for all demos which have been setup-ed.
// It stores the group locally in the public 'generatedOperations' attribure,
// which should be 'followed' by the main application's operation group, as
// new destroy operations will appear there as new demos get activated, and
// existing destroy operations will disappear after they are executed
shared_ptr<ofxOperations::OperationGroup> DemoManager::generateOperations(){
    // the local generatedOperations attribute basically
    // caches the execution of this method
    if(generatedOperations != nullptr)
        return generatedOperations;
    
    generatedOperations = make_shared<ofxOperations::OperationGroup>();

    for(auto demo : demos){
        // create an 'Activate' operation for every demo
        {
            auto op = make_shared<DemoOp>();
            op->demo = demo;
            op->setName("Activate demo \"" + demo->getName() + "\"");
            generatedOperations->add(op);
            ofAddListener(op->endEvent, this, &DemoManager::onActivateOpEnd);
        }

        // Create a 'Destroy' (Cleanup) operation if the demo isSetup
        if(demo->isSetup())
        {
            auto op = make_shared<DemoOp>();
            op->demo = demo;
            op->setName("Cleanup demo \"" + demo->getName() + "\"");
            generatedOperations->add(op);
            ofAddListener(op->endEvent, this, &DemoManager::onDestroyOpEnd);
        }
    }

    return generatedOperations;
}

void DemoManager::onDestroyOpEnd(ofxOperations::Operation &op){
    if(generatedOperations != nullptr)
        generatedOperations->remove(op);

    auto demo = ((DemoOp*)&op)->demo;

    if(this->getActive() == demo)
        deactivate();

    demo->_destroy();
}

void DemoManager::onActivateOpEnd(ofxOperations::Operation &op){
    this->activate(((DemoOp*)&op)->demo);
}

#endif

void DemoManager::destroy(){
    // even though all demos are shared_ptr, we can't just do demos.clear here
    // (or even just let the demos vector destructor do all the work automatically)
    // because inside destructors virtual functions don't work, so the inheriting classes'
    // destroy methods will not be invoked by the AbstractDemo destructor.
    // If we call _destroy manually they will though...
    for(auto demo : demos){
        demo->_destroy();
    }

    demos.clear();
}

bool DemoManager::update(){
    auto demo = getActive();
    
    if(demo == nullptr)
        return false;
    
    demo->update();
    return true;
}

bool DemoManager::draw(){
    auto demo = getActive();

    if(demo == nullptr)
        return false;

    demo->draw();
    return true;
}

void DemoManager::add(shared_ptr<AbstractDemo> newDemo){
    demos.push_back(newDemo);
    if(newDemo->activeParam.get()){
        activate(newDemo);
    }
}

void DemoManager::deactivate(){
    for(auto demo : demos){
        demo->activeParam.set(false);
    }
    
    activeDemo = nullptr;
}

void DemoManager::activate(shared_ptr<AbstractDemo> demo){
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
            auto op = make_shared<DemoOp>();
            op->demo = demo;
            op->setName("Cleanup demo \"" + demo->getName() + "\"");
            generatedOperations->add(op);
            ofAddListener(op->endEvent, this, &DemoManager::onDestroyOpEnd);
        }
#endif // OFXOPERATIONS
    }

    demo->activeParam.set(true);

#ifdef DEBUG
    ofSetWindowTitle(demo->getName());
#endif
}

void DemoManager::activate(int i){
    if(i < demos.size())
        activate(demos[i]);
}

shared_ptr<AbstractDemo> DemoManager::getActive(){
    return activeDemo;
}

int DemoManager::getActiveIndex(){
    int i=0;
    for(auto demo : demos){
        if(demo->activeParam.get())
            return i;
        i++;
    }

    return -1;
}

const string DemoManager::getActiveName(){
    return (activeDemo == nullptr) ? "" : activeDemo->getName();
}

shared_ptr<AbstractDemo> DemoManager::getByName(const string &name){
    for(auto &demo : demos){
        if(demo->getName() == name){
            return demo;
        }
    }
    
    return nullptr;
}

#endif // DEMOS
*/
