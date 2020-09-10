#ifndef NNDA_H
#define NNDA_H

#include "Image.h"
#include <iostream>
#include <string>
#include <map>
#include <algorithm>

class nnda {
public:
    
    nnda(std::multimap<std::string, Image*>& map_in, std::multimap<std::string, std::string>& brands_in) {
        //std::multimap<std::string, std::string>::iterator itr_2 = brands_in.begin();
        
        
        for (std::multimap<std::string, Image*>::iterator itr = map_in.begin(); itr != map_in.end(); itr++) {
            /*char* folder = new char[itr_2->second.length() + 1];
            strcpy(folder, itr_2->second.c_str());
            itr->second = new Image(folder);*/
            itr->second->Image_DCT();
            data_1.insert(std::pair<std::string, Image*>(itr->first, itr->second));
            data_2.insert(std::pair<std::string, Image*>(itr->first, itr->second));
            data_3.insert(std::pair<std::string, Image*>(itr->first, itr->second));
            //tracker.push_back(itr->second);
            //++itr_2;
        }

        for (std::multimap<std::string, std::string>::iterator itr_1 = brands_in.begin(); itr_1 != brands_in.end(); itr_1++) {
            brand_info.insert(std::pair<std::string, std::string>(itr_1->first, itr_1->second));
        }
    }

    void nnda_neighbors(Image*& test) {
        int temp = 0; float sum = 0; float x_I = 0; float x_E = 0; std::vector<float> extra;
        test->Image_DCT();
        for (std::multimap<std::string, Image*>::iterator itr = data_1.begin(); itr != data_1.end(); itr++) {
            sum = 0;
            for (int j = 0; j < 16; j++) {
                for (int k = 0; k < 16; k++) {
                    temp = pow(test->Image_at(k, j) - itr->second->Image_at(k, j), 2);
                    sum = sum + temp;
                }
            }
            values.insert(std::pair<std::string, float>(itr->first, sqrt(sum)));
        }

        for (std::multimap<std::string, float>::iterator itr_1 = values.begin(); itr_1 != values.end(); ++itr_1) {
            for (std::multimap<std::string, float>::iterator itr_2 = values.begin(); itr_2 != values.end(); ++itr_2) {
                if (itr_1->first.compare(itr_2->first) == 0) {
                    if (itr_1->second > itr_2->second) {
                        x_I = itr_2->second;
                    }
                    else
                        x_I = itr_1->second;
                }
                else
                {
                    extra.push_back(itr_2->second);
                }
            }
            x_E = *std::min_element(extra.begin(), extra.end());
            intraClass.insert(std::pair<std::string, float>(itr_1->first, x_I));
            extraClass.insert(std::pair<std::string, float>(itr_1->first, x_E));
            extra.clear();
        }
       
    }


    //**********************************************************************************************//
    // 1. Objective: Implement the nonparametric extra class and intra class distances.             //  
    // 2. Delta_n_I = X_n - X_n_I => An image minus the min value INSIDE the class of that image    // 
    // 3. Delta_n_E = X_n - X_n_E => An Image minus the min value OUTSIDE the class of that image   //
    //**********************************************************************************************//
    void nnda_distances() {

        std::multimap<std::string, std::string>::iterator itr = brand_info.begin();
        std::multimap<std::string, Image*>::iterator itr_data_1 = data_1.begin();
        std::multimap<std::string, Image*>::iterator itr_data_2 = data_2.begin();
        std::multimap<std::string, Image*>::iterator itr_data_3 = data_3.begin();
        std::multimap<std::string, float>::iterator itr_intra = intraClass.begin();
        std::multimap<std::string, float>::iterator itr_extra = extraClass.begin();

        itr_data_1->second->Image_print_dct();
        std::cout << std::endl;
        std::cout<< "Intra: " << itr_intra->second << std::endl;
        std::cout << "Extra: " << itr_extra->second << std::endl;

        while (itr_data_1 != data_1.end() && itr_data_2 != data_2.end() && itr_data_3 != data_3.end() && itr_intra != intraClass.end() && itr_extra != extraClass.end()) {
            
            char* folder = new char[itr->second.length() + 1];
            strcpy(folder, itr->second.c_str());
            itr_data_1->second = new Image(folder);
            itr_data_2->second = new Image(folder);
            tracker.push_back(itr_data_1->second);
            tracker.push_back(itr_data_2->second);
            
            if (itr_data_1->first.compare(itr_intra->first) == 0) {
                for (int i = 0; i < 16; i++) {
                    for (int j = 0; j < 16; j++) {
                        itr_data_1->second->fill(j, i, itr_data_3->second->Image_at(j, i) - itr_intra->second);
                        itr_data_2->second->fill(j, i, itr_data_3->second->Image_at(j, i) - itr_extra->second);
                    }
                }
                ++itr_intra;
                ++itr_extra;
            }
            
            delete[] folder;
            ++itr_data_1;
            ++itr_data_2;
            ++itr_data_3;

        }
    }

    void print() {
        std::multimap<std::string, Image*>::iterator itr_data_1 = data_1.begin();
        std::multimap<std::string, Image*>::iterator itr_data_2 = data_2.begin();
        /*std::multimap<float, std::string>::iterator itr_values = values.begin();
        std::multimap<std::string, float>::iterator itr_intra = intraClass.begin();
        std::multimap<std::string, std::string>::iterator itr_brands = brand_info.begin();
        std::multimap<Image*, std::string>::iterator itr_delta_E = delta_E.begin();
        std::multimap<std::string, float>::iterator itr_extra = extraClass.begin();*/
        //std::multimap<std::string, Image*>::iterator itr_delta_I = delta_I.begin();
        
        int count = 0;
        while (itr_data_1 != data_1.end() && itr_data_2 != data_2.end()) {
            
            ++itr_data_1;
            ++itr_data_2;
            std::cout << "Delta_I: ";
            std::cout << itr_data_1->first << "\n";
            itr_data_1->second->Image_print_dct();
            std::cout << std::endl << std::endl;
            
            std::cout << "Delta_E: ";
            std::cout << itr_data_2->first << "\n";
            itr_data_2->second->Image_print_dct();
            std::cout << std::endl << std::endl;
            
            ++count;
            itr_data_1 = data_1.end();
            itr_data_2 = data_2.end();
       }
        std::cout << count << std::endl;
    }

    ~nnda() {
        for (long unsigned int i = 0; i < tracker.size(); i++) {
            delete tracker[i];
        }
    
    }

private:
    std::multimap<std::string, Image*> data_1;
    std::multimap<std::string, Image*> data_2;
    std::multimap<std::string, Image*> data_3;
    std::multimap<std::string,float> values;
    std::multimap<std::string, float> extraClass;
    std::multimap<std::string, float> intraClass;
    std::multimap<Image*, std::string> delta_E;
    std::multimap<std::string,Image*> delta_I;
    std::multimap<std::string, std::string> brand_info;
    std::vector<Image*> tracker;

};




#endif
