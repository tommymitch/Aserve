/* Aclient created by Tom Mitchell */

#include "aservelibs/aservelib.h"


int main()
{

    // ..your code goes here!
    aserveOscillator (0, 1000, 1.0, 1);
    while (1)
    {
        aserveSleep (1000);
    }

    return 0;
}
