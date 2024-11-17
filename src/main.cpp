int debug_counter = 0;

#include "app.h"
#include "dout.h"


int main() 
{
    dbgout("Starting application");
    App app;
    app.Run();
    return 0;
}
