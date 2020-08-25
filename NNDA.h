#ifndef NNDA_H
#define NNDA_H

#include "Image.h"
#include <iostream>
#include <string>
#include <map>

class nnda {
public:
    nnda() {}


    void nnda_neighbors(Image*& test, std::multimap<Image*, std::string>& data) {
        int temp = 0; float sum = 0; float x_E = 0; float x_I = 0;

        for (std::multimap<Image*, std::string>::iterator itr = data.begin(); itr != data.end(); itr++) {
            sum = 0;
            for (int j = 0; j < 16; j++) {
                for (int k = 0; k < 16; k++) {
                    temp = pow(test->Image_at(k, j) - itr->first->Image_at(k, j), 2);
                    sum = sum + temp;
                }
            }
            values.insert(std::pair<float, std::string>(sqrt(sum), itr->second));
        }

        for (std::multimap<float, std::string>::iterator itr_1 = values.begin(); itr_1 != values.end(); ++itr_1) {
            for (std::multimap<float, std::string>::iterator itr_2 = values.begin(); itr_2 != values.end(); ++itr_2) {
                if (itr_1->second.compare(itr_2->second) == 0) {
                    if (itr_1->first > itr_2->first) {
                        x_I = itr_2->first;
                    }
                    else
                        x_I = itr_1->first;

                    intraClass.insert(std::pair<std::string, float>(itr_1->second, x_I));
                }
            }
        }

        for (std::multimap<float, std::string>::iterator itr_1 = values.begin(); itr_1 != values.end(); ++itr_1) {
            for (std::multimap<float, std::string>::iterator itr_2 = values.begin(); itr_2 != values.end(); ++itr_2) {
                if (itr_1->second.compare(itr_2->second) != 0) {
                    if (itr_1->first > itr_2->first) {
                        x_E = itr_2->first;
                    }
                    else
                        x_E = itr_1->first;

                    extraClass.insert(std::pair<std::string, float>(itr_1->second, x_E));
                }
            }
        }

    }

    void nnda_distances(std::multimap<Image*, std::string>& data) {

        for (std::multimap<std::string, float>::iterator itr_1 = extraClass.begin(); itr_1 != extraClass.end(); ++itr_1) {
            for (std::multimap<Image*, std::string>::iterator itr_2 = data.begin(); itr_2 != data.end(); ++itr_2) {
                if (itr_1->first.compare(itr_2->second) != 0) {
                    for (int i = 0; i < 16; i++) {
                        for (int j = 0; j < 16; j++) {
                            itr_2->first->fill(j, i, itr_2->first->Image_at(j, i) - itr_1->second);
                        }
                    }
                    delta_E.insert(std::pair<Image*, std::string>(itr_2->first, itr_2->second));
                }
            }

        }


        for (std::multimap<std::string, float>::iterator itr_1 = intraClass.begin(); itr_1 != intraClass.end(); ++itr_1) {
            for (std::multimap<Image*, std::string>::iterator itr_2 = data.begin(); itr_2 != data.end(); ++itr_2) {
                if (itr_1->first.compare(itr_2->second) == 0) {
                    for (int i = 0; i < 16; i++) {
                        for (int j = 0; j < 16; j++) {
                            itr_2->first->fill(j, i, itr_2->first->Image_at(j, i) - itr_1->second);
                        }
                    }
                    delta_I.insert(std::pair<Image*, std::string>(itr_2->first, itr_2->second));
                }
            }

        }
    }



    void print() {
        std::multimap<Image*, std::string>::iterator itr_1 = delta_E.begin();
        std::multimap<Image*, std::string>::iterator itr_2 = delta_I.begin();
        std::multimap<std::string, float>::iterator itr_3 = extraClass.begin();
        std::multimap<std::string, float>::iterator itr_4 = intraClass.begin();

        while (itr_1 != delta_E.end() && itr_2 != delta_I.end( )&& itr_3 != extraClass.end() && itr_4 != intraClass.end()) {
            std::cout << "Extra Class: " << std::endl;
            std::cout << itr_3->first << ": " << itr_3->second << std::endl;
            std::cout << itr_1->second << ": " << std::endl;
            itr_1->first->Image_print_dct();
            std::cout << std::endl;
            
            std::cout << "Intra Class: " << std::endl; 
            std::cout << itr_4->first << ": " << itr_4->second << std::endl;
            std::cout << itr_2->second << ": " << std::endl;
            itr_2->first->Image_print_dct();
            std::cout << std::endl;

            itr_1 = delta_E.end();
            itr_2 = delta_I.end();
            itr_3 = extraClass.end();
            itr_4 = intraClass.end();
        }
    }

    


private:
    std::multimap<float, std::string> values;
    std::multimap<std::string, float> extraClass;
    std::multimap<std::string, float> intraClass;
    std::multimap<Image*, std::string> delta_E;
    std::multimap<Image*, std::string> delta_I;
};




#endif
