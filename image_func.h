#ifndef IMAGE_FUNC
#define IMAGE_FUNC
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <string>
void write_jpg(int w, int h, int channels_num, unint quality, std::string filename)
{

    unsigned char data[w * h * channels_num];

    int index = 0;
    for (int y = h-1; y >= 0; --y)
    {
        for (int x = 0; x < w; ++x)
        {


            data[index++] = ir;
            data[index++] = ig;
            data[index++] = ib;
        }
    }
    const char *filestr = filename.c_str();
    stbi_write_jpg(filestr,w,h,channels_num,data,100);


}

#endif