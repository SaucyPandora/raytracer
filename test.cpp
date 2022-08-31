#include <iostream>
#include <string>
#include "image_func.h"
int main()
{
    int channels_num = 3;
    int w = 128;
    int h = 64;
    int quality = 100;

    std::string file = "test2.jpg";

    write_jpg(w,h,channels_num,quality,file);

    
    


}