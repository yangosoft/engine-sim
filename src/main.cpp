#include "../include/engine_sim_application.h"

#include <iostream>

int main(int argc, char **argv){
    EngineSimApplication application;
    application.initialize((void *)&hInstance, ysContextObject::DeviceAPI::DirectX11);
    application.run();
    application.destroy();

    return 0;
}
