//
//  main.cpp
//  c11_test
//
//  Created by hidog on 2016/5/24.
//  Copyright © 2016年 hidog. All rights reserved.
//

#include <iostream>
#include "../c11_sample.h"
#include <vector>






int main(int argc, const char * argv[])
{
    //c11_sample_1();
    //c11_sample_2();
    //c11_sample_3();
    //c11_sample_4();
    //c11_sample_5();
    //c11_sample_6();
    //c11_sample_7();
    //c11_sample_8();
    //c11_sample_9();
    //c11_sample_10();
    //c11_sample_11();
    //c11_sample_12();
    //c11_sample_13();
    //c11_swap();
    
    while(1)
    {
        int *ptr = new int[1000000];
        ptr[0] = 342425;
        std::cout << ptr[0] << std::endl;
        delete [] ptr;
        ptr = NULL;
    }
    
    return 0;
}
