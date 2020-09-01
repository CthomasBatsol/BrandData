#ifndef NNDA_H
#define NNDA_H

#include "Image.h"
#include <iostream>
#include <string>
#include <map>

class nnda {
public:
    
    nnda() {}

    void nnda_neighbors(std::multimap<std::string,Image*>& data, Image*& test) {
        int temp = 0; float sum = 0; float x_E = 0; float x_I = 0;

        for (std::multimap<std::string, Image*>::iterator itr = data.begin(); itr != data.end(); itr++) {
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

    void nnda_distances() {
        
        for (std::multimap<std::string, float>::iterator itr_1 = extraClass.begin(); itr_1 != extraClass.end(); itr_1++) {
            for (std::multimap<std::string, Image*>::iterator itr_2 = delta_E.begin(); itr_2 != delta_E.end(); itr_2++) {
                if (itr_2->first.compare(itr_1->first) != 0) {
                    for (int i = 0; i < 16; i++) {
                        for (int j = 0; j < 16; j++) {
                            itr_2->second->fill(j, i, itr_2->second->Image_at(j, i) - itr_1->second);
                        }
                    }
                }
            }
        }

        for (std::multimap<std::string, float>::iterator itr_1 = intraClass.begin(); itr_1 != intraClass.end(); itr_1++) {
            for (std::multimap<std::string, Image*>::iterator itr_2 = delta_I.begin(); itr_2 != delta_I.end(); itr_2++) {
                if (itr_2->first.compare(itr_1->first) == 0) {
                    for (int i = 0; i < 16; i++) {
                        for (int j = 0; j < 16; j++) {
                            itr_2->second->fill(j, i, itr_2->second->Image_at(j, i) - itr_1->second);
                        }
                    }
                }
            }
        }

    }

    void print() {
    
    }

    ~nnda() {
        
        for (std::multimap<std::string, Image*>::iterator itr = delta_I.begin(); itr != delta_I.end(); ++itr) {
            delete itr->second;
        }
   
    }

private:
    std::multimap<float, std::string> values;
    std::multimap<std::string, float> extraClass;
    std::multimap<std::string, float> intraClass;
    std::multimap<std::string, Image*> delta_E;
    std::multimap<std::string, Image*> delta_I;

};




#endif
