// gcc comp_system_tests.c -o comp_system_tests
// ./comp_system_tests
#include <stdio.h>
#include "comp_system.c"

int main()
{
    printf("*** computing system tests ***\n");
    
    printf("Creating computing system with 16 Gb RAM, 4 cpu threads and 1 GPU (8 Gb, 30 SMs): \n");
    comp_system_gpu* gpu = (comp_system_gpu*)malloc(sizeof(comp_system_gpu));
    gpu->vram_size_gb = 8;
    gpu->cpu_sm_max = 30;
    comp_system_gpu_print("GPU: ", gpu);

    comp_system_node* node1 = (comp_system_node*)malloc(sizeof(comp_system_node));
    node1->ram_size_gb = 16;
    node1->cpu_threads_max = 4;
    node1->gpu_num = 1;
    node1->gpus = (comp_system_gpu**)malloc(node1->gpu_num * sizeof(comp_system_gpu*));
    node1->gpus[0] = gpu;
    comp_system_node_print("NODE: ", node1);

    printf("*** computing system tests completed ***\n");
    return 0;
}