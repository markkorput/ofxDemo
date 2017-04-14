// local
#include "ParamsDemo.h"

using namespace ofxDemo;

ParamsDemo::ParamsDemo() : paramsFile(""){
}

void ParamsDemo::_setup(){
    // load params if we have a filename
    if(paramsFile != ""){
        ofLogVerbose() << "Demo \"" << getName() << "\" loading params from: " << paramsFile;
        std::shared_ptr <ofBaseFileSerializer> serializer = std::make_shared<ofXml>();
        serializer->load(paramsFile);
        serializer->deserialize(params);
    }

    AbstractDemo::_setup();
}

void ParamsDemo::_destroy(){
    // save params if we have a filename
    if(bSetup && paramsFile != ""){
        ofLogVerbose() << "Demo \"" << getName() << "\" saving params to: " << paramsFile;
        std::shared_ptr <ofBaseFileSerializer> serializer = std::make_shared<ofXml>();
        serializer->load(paramsFile);
        serializer->serialize(params);
        serializer->save(paramsFile);
    }

    AbstractDemo::_destroy();
}

void ParamsDemo::setName(const string& newName){
    // set params group name
    params.setName(newName);
    // set params file name
    paramsFile = "params-"+newName+".xml";
    // setName parent class
    AbstractDemo::setName(newName);
}
