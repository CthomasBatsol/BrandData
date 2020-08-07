#include <iostream>
#include <cstring>
#include <vector>
#include <string>
#include <fstream>
#include "Image.h"
#include <filesystem>
using namespace std;
namespace fs = std::filesystem;

/****************************************************************************
*                                                                           *
* 1. Read in image of battery that needs to be identified                   *
* 2. Process image for feature extraction                                   *
* 3. Perform feature extraction method                                      *
* 3. Run image through learning algorithm for classification                *
* 4. Check to see if battery was properly classified                        *
* 5. Write battery brand data to a file                                     *
*                                                                           *
****************************************************************************/


void iteration(const fs::path& pathToShow, vector<char*> brands ,vector<Image*>& container, Image* img, char arr[]) {
    int count = 0;
    string begin = arr;
    string end;
  
    for (const auto& entry : fs::directory_iterator(pathToShow)) {
        const auto foldernameStr = entry.path().filename().string();
        char* folder = new char[foldernameStr.length()+1];
        
        strcpy(folder,foldernameStr.c_str());                
        begin = begin + folder + '/';
        char* temp = new char[begin.length() + 1];
        strcpy(temp, begin.c_str());
        
        const fs::path pathToFile{ temp };
            
            for (const auto& file : fs::directory_iterator(pathToFile)) {
                const auto filenameStr = file.path().filename().string();
                char* bmp = new char[begin.length() + filenameStr.length()+1];
                end = begin + filenameStr;
                strcpy(bmp, end.c_str());
                brands.push_back(bmp);
                cout << brands[count] << endl;
                img = new Image(brands[count]);
                container.push_back(img);
                ++count;
                delete[] bmp;
                
            }

            delete[] folder;
            delete[] temp;
            begin = arr;
    }
   
}

void print_dct(vector<Image*> container) {
    for (long unsigned int i = 0; i < container.size(); i++) {
        cout << "Image: " << i + 1 << endl;
        container[i]->Image_DCT();
        container[i]->Image_print_dct();
    }
}

void destroy(vector<Image*> container_1,vector<char*> container_2) {
    for (long unsigned int i = 0; i < container_1.size(); i++){
        delete container_1[i];
    }

    for (long unsigned int i = 0; i < container_2.size(); i++) {
        delete container_2[i];
    }

}


int main(int argc, char* argv[])
{
    const fs::path pathToShow{ argc >= 2 ? argv[1] : fs::current_path() };
    Image* img = NULL;
    vector<Image*> images;
    vector<char*> location;

    iteration(pathToShow,location,images,img,argv[1]);
    print_dct(images);
    destroy(images,location);
    return 0;
}


