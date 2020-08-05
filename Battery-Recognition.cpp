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

<<<<<<< HEAD


void iteration(const fs::path& pathToShow, vector<Image*>& container, Image* img, string& input) {
    int count = 0;
    
    for (const auto& entry : fs::directory_iterator(pathToShow)) {
        const auto foldernameStr = entry.path().filename().string();
        
        char* path = new char[input.length()+1];
        char* begin = strcpy(path, input.c_str());

        if (entry.is_directory()) {
            
            char* brandName = new char[foldernameStr.length()+1];
            char* location = strcpy(brandName,foldernameStr.c_str());
            char arr[] = "/1.bmp";
            char* bmp = strcat(location, arr);/**********************/
            char* final_path = strcat(begin, bmp);/******************/

            container.push_back(new Image(final_path));
            //container[count]->Image_print();
            container[count]->Image_DCT();
           
            count++;
            delete[] brandName;
        }
        delete[] path;
    }
}

void print_dct(vector<Image*> container) {
    for (long unsigned int i = 0; i < container.size(); i++) {
        cout << "Image: " << i + 1 << endl;
        container[i]->Image_print_dct();
    }
}

void destroy(vector<Image*> container) {
    for (long unsigned int i = 0; i < container.size(); i++){
        delete container[i];
=======
//char* filename = strcpy(new char[filenameStr.length() + 1], filenameStr.c_str());
/*char location[] = "Brands/Duracell/1.bmp";
    char* startingPath = location;
    Image* image = new Image(startingPath);*/

void iteration(const fs::path& pathToShow) {
    fs::path p();
    Image* img;
    vector<Image*> container;

    for (const auto& entry : fs::recursive_directory_iterator(pathToShow)) {
        const auto filenameStr = entry.path().filename().string();
        
        if (entry.is_regular_file()) {
            char* filename = strcpy(new char[filenameStr.length() + 1], filenameStr.c_str());
            img = new Image(filename);
            img->Image_DCT();
            container.push_back(img);
            cout << "File: " << filenameStr << '\n';
            img->Image_print_dct();
            cout << endl;
        }
        else {
            cout << "Please specify a valid folder." << endl;
        }
>>>>>>> 6ccc32f9dc3b8abdd2fb0fa1aa29a490dfce46ea
    }
}


int main(int argc, char* argv[])
{
    const fs::path pathToShow{ argc >= 2 ? argv[1] : fs::current_path() };
<<<<<<< HEAD
    string input = argv[1];
    Image* img = NULL;
    vector<Image*> images;
    
    iteration(pathToShow,images,img,input);
    print_dct(images);
               
    destroy(images);
    return 0;
=======
    iteration(pathToShow);
   
    return 0;
   
>>>>>>> 6ccc32f9dc3b8abdd2fb0fa1aa29a490dfce46ea
}


