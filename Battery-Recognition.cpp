#include <iostream>
#include <cstring>
#include <vector>
#include <string>
#include <fstream>
#include "Image.h"
#include <filesystem>
#include <cmath>
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


void construct_training(const fs::path& pathToShow,vector<string>& brands,vector<Image*>& container, Image* img, char arr[]) {
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
                brands.push_back(end);
                strcpy(bmp, end.c_str());
           
                //cout << bmp << endl;
                
                img = new Image(bmp);
                container.push_back(img);
                delete[] bmp;
                
            }
            //cout << endl;

        delete[] folder;
        delete[] temp;
        begin = arr;
    }
   
}

void print(vector<Image*>& container) {
    for (long unsigned int i = 0; i < 1; i++) {
        container[i]->Image_print_dct();
    }
    cout << endl;
}

void dct(Image* test,vector<Image*>& container,vector<string> name) {

     for (long unsigned int i = 0; i < container.size(); i++) {
         container[i]->Image_DCT();
     }
     test->Image_DCT();
}

void nnda(vector<Image*>& train,Image* test) {
    int temp = 0;
    for (long unsigned int i = 0; i < train.size(); i++) {
        for (int j = 0; j < 16; j++) {
            for (int k = 0; k < 16; k++) {
                temp = pow(test->Image_at(k, j) - train[i]->Image_at(k, j),2);
                train[i]->fill(k, j, temp);
            }
        }
    }
}

void destroy(vector<Image*> container_1) {
    
    for (long unsigned int i = 0; i < container_1.size(); i++){
        delete container_1[i];
    }

}


int main(int argc, char* argv[])
{
    const fs::path pathToShow{ argc >= 2 ? argv[1] : fs::current_path() };
    Image* img = NULL;
    vector<Image*> images;
    vector<string> location;
    Image* test = new Image(argv[2]);
    
    construct_training(pathToShow, location, images, img, argv[1]);
    dct(test,images,location);
    print(images);
    nnda(images, test);
    print(images);
    destroy(images);
    delete test;
    
    return 0;
}


