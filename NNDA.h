#ifndef NNDA_H
#define NNDA_H

#include "Image.h"
#include <iostream>
#include <string>
#include <map>

class nnda {
public:
    
    nnda(std::multimap<std::string, Image*>& map_in, std::multimap<std::string, std::string>& brands_in) {
        for (std::multimap<std::string, Image*>::iterator itr = map_in.begin(); itr != map_in.end(); itr++) {
            itr->second->Image_DCT();
            data_1.insert(std::pair<std::string, Image*>(itr->first, itr->second));
            data_2.insert(std::pair<std::string, Image*>(itr->first, itr->second));
            data_3.insert(std::pair<std::string, Image*>(itr->first, itr->second));
        }

        for (std::multimap<std::string, std::string>::iterator itr_1 = brands_in.begin(); itr_1 != brands_in.end(); itr_1++) {
            brand_info.insert(std::pair<std::string, std::string>(itr_1->first, itr_1->second));
        }
    }

    void nnda_neighbors(Image*& test) {
        int temp = 0; float sum = 0; float x_E = 0; float x_I = 0; 

        for (std::multimap<std::string, Image*>::iterator itr = data_1.begin(); itr != data_1.end(); itr++) {
            sum = 0;
            for (int j = 0; j < 16; j++) {
                for (int k = 0; k < 16; k++) {
                    temp = pow(test->Image_at(k, j) - itr->second->Image_at(k, j), 2);
                    sum = sum + temp;
                }
            }
            values.insert(std::pair<float, std::string>(sqrt(sum), itr->first));

        }

        for (std::multimap<float, std::string>::iterator itr_1 = values.begin(); itr_1 != values.end(); ++itr_1) {
            for (std::multimap<float, std::string>::iterator itr_2 = values.begin(); itr_2 != values.end(); ++itr_2) {
                if (itr_1->second.compare(itr_2->second) == 0) {
                    if (itr_1->first > itr_2->first) {
                        x_I = itr_2->first;
                    }
                    else
                        x_I = itr_1->first;
                }
            }
            
            intraClass.insert(std::pair<float, std::string>(x_I, itr_1->second));
        }
        

        for (std::multimap<float, std::string>::iterator itr_1 = values.begin(); itr_1 != values.end(); ++itr_1) {
            for (std::multimap<float, std::string>::iterator itr_2 = values.begin(); itr_2 != values.end(); ++itr_2) {
                if (itr_1->second.compare(itr_2->second) != 0) {
                    if (itr_1->first > itr_2->first) {
                        x_E = itr_2->first;
                    }
                    else
                        x_E = itr_1->first;
                }
            }
           
            extraClass.insert(std::pair<std::string, float>(itr_1->second, x_E));
        }
       
    }

    void nnda_distances() {
        std::multimap<std::string, std::string>::iterator itr = brand_info.begin();
        std::multimap<std::string, Image*>::iterator itrs = data_3.begin();

        for (std::multimap<std::string, Image*>::iterator itr_1 = data_1.begin(); itr_1 != data_1.end(); itr_1++) {
            for (std::multimap<std::string, float>::iterator itr_2 = extraClass.begin(); itr_2 != extraClass.end(); itr_2++) {
                if (itr_2->first.compare(itr_1->first) != 0 && itr != brand_info.end() && itrs != data_3.end()) {
                    char* folder = new char[itr->second.length() + 1];
                    strcpy(folder, itr->second.c_str());
                    itr_1->second = new Image(folder);
                    tracker.push_back(itr_1->second);
                    
                    for (int i = 0; i < 16; i++) {
                        for (int j = 0; j < 16; j++) {
                            itr_1->second->fill(j, i, itrs->second->Image_at(j, i) - itr_2->second);
                        }
                    }
                    
                    ++itr;
                    ++itrs;
                    delta_E.insert(std::pair<Image*,std::string>(itr_1->second, itr_1->first));
                    delete[] folder;
                }
            }
        }
        
        itr = brand_info.begin();
        itrs = data_3.begin();

        for (std::multimap<std::string, Image*>::iterator itr_1 = data_2.begin(); itr_1 != data_2.end(); itr_1++) {
            for (std::multimap<float, std::string>::iterator itr_2 = intraClass.begin(); itr_2 != intraClass.end(); itr_2++) {
                if (itr_2->second.compare(itr_1->first) == 0 && itr != brand_info.end() && itrs != data_3.end()) {
                    //std::cout << itr_1->first << "   " << itr_2->second << std::endl;
                    
                    char* folder = new char[itr->second.length() + 1];
                    strcpy(folder, itr->second.c_str());
                    std::cout << folder << std::endl;
                    itr_1->second = new Image(folder);
                    tracker.push_back(itr_1->second);

                    for (int i = 0; i < 16; i++) {
                        for (int j = 0; j < 16; j++) {
                            itr_1->second->fill(j, i, itrs->second->Image_at(j, i) - itr_2->first);
                        }
                    }
                   
                    ++itr;
                    ++itrs;
                    delta_I.insert(std::pair<Image*, std::string>(itr_1->second, itr_1->first));
                    delete[] folder;
                }
                
            }
        }

    }

    void print() {
      /*  std::multimap<std::string, Image*>::iterator itr_data = data_3.begin();
        //std::multimap<float, std::string>::iterator itr_values = values.begin();
        //std::multimap<std::string, float>::iterator itr_extra = extraClass.begin();
        //std::multimap<std::string, float>::iterator itr_intra = intraClass.begin();
        //std::multimap<Image*, std::string>::iterator itr_delta_E = delta_E.begin();
        std::multimap<Image*, std::string>::iterator itr_delta_I = delta_I.begin();
        //std::multimap<std::string, std::string>::iterator itr_brands = brand_info.begin();
        
        int count = 0;
        while (itr_data != data_3.end()) {
            std::cout << itr_data->first << std::endl;
            count++;
            itr_data++;
       }
        std::cout << count << std::endl;*/
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
    std::multimap<float, std::string> values;
    std::multimap<std::string, float> extraClass;
    std::multimap<float, std::string> intraClass;
    std::multimap<Image*, std::string> delta_E;
    std::multimap<Image*, std::string> delta_I;
    std::multimap<std::string, std::string> brand_info;
    std::vector<Image*> tracker;

};




#endif
