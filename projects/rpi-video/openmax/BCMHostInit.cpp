#include "BCMHostInit.h"

#include <bcm_host.h>

#include <stdio.h>

BMCHost::BMCHost(){
    initialized = false;
}

BMCHost* BMCHost::Instance(){
    static BMCHost instance;
    return &instance;
}

void BMCHost::init(){
    if (initialized)
        return;
    initialized = true;
    bcm_host_init();
    fprintf(stderr,"BMCHost::init OK\n");
}
