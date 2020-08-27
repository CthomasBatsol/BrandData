#include <iostream>
#include <cstring>
#include <vector>
#include <string>
#include <fstream>
#include "Image.h"
#include "NNDA.h"
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

vector<nnda*> construct_training(const fs::path& pathToShow, multimap<Image*,string>& data, Image*& img, char arr[],Image* & test) {
    string begin = arr;
    string end;
    nnda *classify;
    vector<nnda*> classes;

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
            data.insert(pair<Image*, string>(img, bmp));
            classify = new nnda(data, img, bmp, folder);
            classes.push_back(classify);

            delete[] bmp;
        }

        delete[] folder;
        delete[] temp;
        begin = arr;
    }
    return classes;
}


void dct(Image*& test, multimap<Image*, string>& data) {
    for (multimap<Image*, string>::iterator itr = data.begin(); itr != data.end(); ++itr) {
        itr->first->Image_DCT();
    }
    test->Image_DCT();
}

void destroy(multimap<Image*, string>& data,vector<nnda*> classes ){
    for (multimap<Image*, string>::iterator itr = data.begin(); itr != data.end(); ++itr) {
        delete itr->first;
    }

    for (int i = 0; i < classes.size(); i++) {
        delete classes[i];
    }
}


int main(int argc, char* argv[])
{
    const fs::path pathToShow{ argc >= 2 ? argv[1] : fs::current_path() };
    Image* img = NULL;
    Image* test = new Image(argv[2]);
    multimap<Image*, string> data;
    vector<nnda*> container;

    container = construct_training(pathToShow, data, img, argv[1], test);
    dct(test, data);

    destroy(data,container);
    delete test;
    
    return 0;
}


