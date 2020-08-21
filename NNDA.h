#ifndef NNDA_H
#define NNDA_H

#include "Image.h"
#include <iostream>
#include <string>
#include <map>

class nnda {
public:
    nnda(){}


    void nnda_neighbors(Image*& test, multimap<Image*, string>& data) {
        int temp = 0; float sum = 0; float x_E = 0; float x_I = 0;

        for (multimap<Image*, string>::iterator itr = data.begin(); itr != data.end(); itr++) {
            sum = 0;
            for (int j = 0; j < 16; j++) {
                for (int k = 0; k < 16; k++) {
                    temp = pow(test->Image_at(k, j) - itr->first->Image_at(k, j), 2);
                    sum = sum + temp;
                }
            }
            values.insert(pair<float, string>(sqrt(sum), itr->second));
        }

        for (multimap<float, string>::iterator itr_1 = values.begin(); itr_1 != values.end(); ++itr_1) {
            for (multimap<float, string>::iterator itr_2 = values.begin(); itr_2 != values.end(); ++itr_2) {
                if (itr_1->second.compare(itr_2->second) == 0) {
                    if (itr_1->first > itr_2->first) {
                        x_I = itr_2->first;
                    }
                    else
                        x_I = itr_1->first;

                    intraClass.insert(pair<string, float>(itr_1->second, x_I));
                }
            }
        }

        for (multimap<float, string>::iterator itr_1 = values.begin(); itr_1 != values.end(); ++itr_1) {
            for (multimap<float, string>::iterator itr_2 = values.begin(); itr_2 != values.end(); ++itr_2) {
                if (itr_1->second.compare(itr_2->second) != 0) {
                    if (itr_1->first > itr_2->first) {
                        x_E = itr_2->first;
                    }
                    else
                        x_E = itr_1->first;

                    extraClass.insert(pair<string, float>(itr_1->second, x_E));
                }
            }
        }
       
    }


private:
    multimap<float, string> values;
    map<string, float> extraClass;
    map<string, float> intraClass;
};




#endif
