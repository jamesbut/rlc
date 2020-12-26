#include <domains/cliff_walking.h>

int main() 
{


    //display_agent(agent);
    
    struct Cliff cliff;
    cliff.width = 5;
    cliff.height = 3;
    cliff.agent.x_pos = 0;
    cliff.agent.y_pos = cliff.height-2;
    display_cliff(cliff);

    return 0;

}
