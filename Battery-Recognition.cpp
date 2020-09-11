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
void construct_training(const fs::path& pathToShow, Image*& img, multimap<string,Image*>& data, multimap<string, string>& locations, char arr[]) {
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
            data.insert(pair<string, Image*>(folder, img));
            locations.insert(pair<string, string>(folder,bmp));

            delete[] bmp;
        }

        delete[] folder;
        delete[] temp;
        begin = arr;
    }
    
}

void train(nnda& classifier,Image*& test) {
    classifier.nnda_neighbors(test);
    classifier.nnda_distances();
    classifier.nnda_scatter_matrices();
    //classifier.print();
}
 
void destroy(multimap<string, Image*>& data) {
    for (multimap<string, Image*>::iterator itr = data.begin(); itr != data.end(); itr++) {
        delete itr->second;
    }
}





int main(int argc, char* argv[])
{
    const fs::path pathToShow{ argc >= 2 ? argv[1] : fs::current_path() };
    Image* img = NULL;
    Image* test = new Image(argv[2]);
    multimap<string, Image*> data;
    multimap<string, string> locations;

    construct_training(pathToShow, img, data, locations, argv[1]);
    nnda classify(data, locations);
    train(classify, test);

    delete test;
    destroy(data);
    return 0;
}


