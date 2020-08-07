#ifndef IMAGE_H
#define IMAGE_H

#include <iostream>
#include <fstream>
#include <cmath>
#include "Matrix.h"

class Image {
public:
	Image(char* filename)
	{
		FILE* f = fopen(filename, "rb");
		unsigned char info[54];
		

		// read the 54-byte header
		fread(info, sizeof(unsigned char), 54, f);

		// extract image height and width from header
		width = *(int*)&info[18]*3;
		height = *(int*)&info[22];
		mat = new Matrix(width, height);
		dct_mat = new Matrix(width / 3, height);

		// allocate 3 bytes per pixel
		int size = width * height;
		unsigned char *data = new unsigned char[size];

		// read the rest of the data at once
		fread(data, sizeof(unsigned char), size, f);
		fclose(f);

		for (int i = 0; i < size; i += 3)
		{
			// flip the order of every 3 bytes
			unsigned char tmp = data[i];
			data[i] = data[i + 2];
			data[i + 2] = tmp;
		}

		//Fill the Image with the RGB values in the file
		int count = 0;
		while (count < size) {
			for (int i = 0; i < mat->Row_count(); i++) {
				for (int j = 0; j < mat->Column_count(); j++) {
					mat->Matrix_fill(j, i, (int)data[count]);
					count++;
				}
			}
		}
		delete[] data;
		
	}
	
	

	void Image_print() {
		std::cout << "42 4D" << std::endl;
		std::cout << width << " ";
		std::cout << height << std::endl;
		int count = 0;

		for (int i = 0; i < height; i++) {
			std::cout << "Row " << count << ": ";
			count++;
			for (int j = 0; j < width; j += 3) {
				std::cout << "[" << mat->Matrix_at(j, i) << " " << mat->Matrix_at(j + 1, i) << " " << mat->Matrix_at(j + 2, i) << "] ";
			}
			std::cout << std::endl;
		}

	}
	
	void Image_DCT() {
		int mask = 16;
		float pi = 3.142857;
		float value, dct1, ci, cj, sum;
		
		float** average = new float* [width];
		for (int i = 0; i < width; i++) {
			average[i] = new float[height];
		}

		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j+=3) {
				value = (mat->Matrix_at(j, i) + mat->Matrix_at(j + 1, i) + mat->Matrix_at(j + 2, i)) / 3;
				average[j][i] = value;
			}
		}
		
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				if (j % 3 == 0) {
					dct_mat->Matrix_fill(j / 3, i, average[j][i]);
				}
			}
		}
 
		for (int i = 0; i < mask; i++) {
			for (int j = 0; j < mask; j++) {

				if (i == 0)
					ci = 1 / sqrt(height);
				else
					ci = sqrt(2) / sqrt(height);
				if (j == 0)
					cj = 1 / sqrt(width/3);
				else
					cj = sqrt(2) / sqrt(width/3);

				sum = 0;
				for (int k = 0; k < height; k++) {
					for (int l = 0; l < width/3; l++) {
						dct1 =  dct_mat->Matrix_at(l,k)*
							cos((2 * k + 1) * i * pi / (2 * height)) *
							cos((2 * l + 1) * j * pi / (2 * width/3));
						sum = sum + dct1;
					}
				}
				dct_mat->Matrix_fill(j,i,ci*cj*sum);
			}
		}
		
		for (int i = 0; i < height; i++) {
			for (int j = mask; j < width / 3; j++) {
				dct_mat->Matrix_fill(j, i, 0);
			}
		}
		
		for (int i = mask; i < height; i++) {
			for (int j = 0; j < width / 3; j++) {
				dct_mat->Matrix_fill(j, i, 0);
			}
		}
		

		for (int i = 0; i < width; i++)
			delete[] average[i];

		delete[] average;
	
	}

	void Image_nnda() {

	}

	void Image_write(std::ofstream& output, std::string& name) {
		output.open(name);

		if (!output.is_open()) {
			std::cout << "Error opening file: " << "image.csv" << std::endl;
		}

		else {

			for (int i = 0; i < height; i++) {
				for (int j = 0; j < width; j += 3) {
					output << mat->Matrix_at(j, i) << "," << mat->Matrix_at(j + 1, i) << "," << mat->Matrix_at(j + 2, i) << ",";
				}
				output << std::endl;
			}
		}

		output.close();

	}

	void Image_write_dct(std::ofstream& output, std::string& name) {
		output.open(name);

		if (!output.is_open()) {
			std::cout << "Error opening file: " << "image.csv" << std::endl;
		}

		else {

			for (int i = 0; i < dct_mat->Row_count(); i++) {
				for (int j = 0; j < dct_mat->Column_count(); j++) {
					output << dct_mat->Matrix_at(j, i) << "," ;
				}
				output << std::endl;
			}
		}

		output.close();

	}

	void Image_print_dct() {
		for (int i = 0; i < 16; i++) {
			for (int j = 0; j < 18 ; j += 3) {
				std::cout << "[" << dct_mat->Matrix_at(j, i) << " " << dct_mat->Matrix_at(j+1, i) << " " << dct_mat->Matrix_at(j+2,i) << "] ";
			}
			std::cout << std::endl;
		}
	}
	
	
	void Image_size() {
		std::cout <<"Width: " << mat->Column_count() << std::endl;
		std::cout << "Height: " << mat->Row_count() << std::endl;
		std::cout << "Disk Space Used: " << (mat->Column_count() * mat->Row_count())/1000 << " KB" << std::endl;
	}
	
	void Image_dct_size() {
		std::cout << "Width: " << dct_mat->Column_count() << std::endl;
		std::cout << "Height: " << dct_mat->Row_count() << std::endl;
		std::cout << "Disk Space Used: " << (dct_mat->Column_count() * dct_mat->Row_count()) / 1000 << " KB" << std::endl;
	}

	int Image_width() {
		return width;
	}

	int Image_height() {
		return height;
	}

	~Image() {
		std::cout << "Image Dtor" << std::endl;
		delete mat;
		delete dct_mat;
	}
	 

private:

	int width;
	int height;
	Matrix* mat;
	Matrix* dct_mat;

};

#endif