// gcc comp_system_tests.c -o comp_system_tests
// ./comp_system_tests
#include <stdio.h>
#include "comp_system.c"

int main()
{
    printf("*** computing system tests ***\n");
    
    printf("Creating computing system with 16 Gb RAM, 4 cpu threads and 1 GPU (8 Gb, 30 SMs): \n");    
    comp_system_node* node = comp_system_node_create_1GPU(16, 4, 8, 30);
    comp_system_node_print("NODE: ", node);

    printf("*** computing system tests completed ***\n");
    return 0;
}