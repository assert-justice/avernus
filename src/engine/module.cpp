#include "module.hpp"

void Module::init(bool* hasError){
    if(*hasError) return;
    if(initInternal()) isInitalized = true;
    else *hasError = true;
}
void Module::cleanup(){
    if(!isInitalized) return;
    cleanupInternal();
}