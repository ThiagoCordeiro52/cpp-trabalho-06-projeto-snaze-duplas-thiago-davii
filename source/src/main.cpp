#include "snaze_manager.h"

int main(int argc, char * argv[]) {
    SnazeManager manager{argc, argv};

    while (not manager.has_ended()) {
        manager.process();
        manager.update();
        manager.render();
    }
    
    return EXIT_SUCCESS;
}
