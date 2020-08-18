#include <iostream>
#include <cstring>
#include <vector>
#include <string>
#include <fstream>
#include "Image.h"
#include <filesystem>
#include <cmath>
#include <map>
#include <iterator>
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


/*void construct_training(const fs::path& pathToShow,vector<string>& brands,vector<Image*>& container, Image*& img, char arr[]) {
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
              
                img = new Image(bmp);
                container.push_back(img);
                delete[] bmp;
                
            }

        delete[] folder;
        delete[] temp;
        begin = arr;
    }
   
}*/

void construct_training(const fs::path& pathToShow, map<string,Image*>& data, Image*& img, char arr[]) {
    string begin = arr;
    string end;


    for (const auto& entry : fs::directory_iterator(pathToShow)) {
        const auto foldernameStr = entry.path().filename().string();
        char* folder = new char[foldernameStr.length() + 1];

        strcpy(folder, foldernameStr.c_str());
        begin = begin + folder + '/';

        char* temp = new char[begin.length() + 1];
        strcpy(temp, begin.c_str());

        const fs::path pathToFile{ temp };

        for (const auto& file : fs::directory_iterator(pathToFile)) {
            const auto filenameStr = file.path().filename().string();
            char* bmp = new char[begin.length() + filenameStr.length() + 1];
            end = begin + filenameStr;
            strcpy(bmp, end.c_str());
            img = new Image(bmp);
            data.insert(pair<string, Image*>(bmp, img));
            
            delete[] bmp;

        }

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

void dct(Image*& test, map<string,Image*>& data) {
    map<string, Image*>::iterator itr;
    for (itr = data.begin(); itr != data.end();++itr) {
        itr->second->Image_DCT();
    }
    
    test->Image_DCT();
}

void nnda(Image*& test, map<string,Image*>& data) {
    int temp = 0;
    float sum = 0;
    map<string, float> values;
   
    for (map<string, Image*>::iterator itr = data.begin(); itr != data.end(); itr++) {
        sum = 0;
        for (int j = 0; j < 16; j++) {
            for (int k = 0; k < 16; k++) {
                temp = pow(test->Image_at(k, j) - itr->second->Image_at(k, j), 2);
                sum = sum + temp;
            }
        }
        values.insert(pair<string, float>(itr->first, sqrt(sum)));
    }

    for (map<string, float>::iterator itr = values.begin(); itr != values.end(); ++itr) {
        
    }
}

void destroy(map<string,Image*>& data) {
    map<string, Image*>::iterator itr;
    for (itr = data.begin(); itr != data.end();++itr) {
        delete itr->second;
    }
}


int main(int argc, char* argv[])
{
    const fs::path pathToShow{ argc >= 2 ? argv[1] : fs::current_path() };
    Image* img = NULL;
    Image* test = new Image(argv[2]);
    map<string, Image*> data;
    
    construct_training(pathToShow, data, img, argv[1]);
    dct(test,data);
    nnda(test,data);
    destroy(data);
    delete test;
    
    return 0;
}


