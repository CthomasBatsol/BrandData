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

void construct_training(const fs::path& pathToShow, multimap<Image*,string>& data, Image*& img, char arr[]) {
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
            data.insert(pair<Image*,string>(img,folder));
            
            delete[] bmp;

        }

        delete[] folder;
        delete[] temp;
        begin = arr;
    }

}

void dct(Image*& test, multimap<Image*, string>& data) {
    map<Image*,string>::iterator itr;
    for (itr = data.begin(); itr != data.end();++itr) {
        itr->first->Image_DCT();
    }
    
    test->Image_DCT();
}

void nnda(Image*& test, multimap<Image*, string>& data) {
    int temp = 0;
    float sum = 0;
    float x_E = 1000000;
    float x_I = 1000000;
    multimap<float,string> values;
    multimap<float, string> extraClass,intraClass;

   
    for (multimap<Image*,string>::iterator itr = data.begin(); itr != data.end(); itr++) {
        sum = 0;
        for (int j = 0; j < 16; j++) {
            for (int k = 0; k < 16; k++) {
                temp = pow(test->Image_at(k, j) - itr->first->Image_at(k, j), 2);
                sum = sum + temp;
            }
        }
        values.insert(pair<float,string>(sqrt(sum),itr->second));
        
    }

    for (multimap<float,string>::iterator itr_1 = values.begin(); itr_1 != values.end(); ++itr_1) {
        for (multimap<float, string>::iterator itr_2 = values.begin(); itr_2 != values.end(); ++itr_2) {
            if (itr_1->second.compare(itr_2->second) == 0) {
                if (itr_1->first > itr_2->first) {
                    x_I = itr_2->first;
                }
            }
            else {
                if (itr_1->first > itr_2->first) {
                    x_E = itr_2->first;
                }
            }
        }
        intraClass.insert(pair<float, string>(x_I, itr_1->second));
        extraClass.insert(pair<float, string>(x_E, itr_1->second));
  }

    
    for (multimap<float, string>::iterator itr_1 = extraClass.begin(); itr_1 != extraClass.end(); ++itr_1) {
        cout << itr_1->second << ": " << itr_1->first << endl;
    }
    cout << endl << endl;
    for (multimap<float, string>::iterator itr_1 = intraClass.begin(); itr_1 != intraClass.end(); ++itr_1) {
        cout << itr_1->second << ": " << itr_1->first << endl;
    }
}

void destroy(multimap<Image*,string>& data) {
    map<Image*,string>::iterator itr;
    for (itr = data.begin(); itr != data.end();++itr) {
        delete itr->first;
    }
}


int main(int argc, char* argv[])
{
    const fs::path pathToShow{ argc >= 2 ? argv[1] : fs::current_path() };
    Image* img = NULL;
    Image* test = new Image(argv[2]);
    multimap<Image*, string> data;
    
    construct_training(pathToShow, data, img, argv[1]);
    dct(test,data);
    nnda(test,data);
    
    destroy(data);
    delete test;
    
    return 0;
}


