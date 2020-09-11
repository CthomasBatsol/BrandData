#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <cassert>



class Matrix {
public:	
	Matrix(int column_count, int row_count) :
		width(column_count), height(row_count)
	{
		 data = new float* [width];		 
		 for (int i = 0; i < width; i++) {
			 data[i] = new float[height];
		 }

	}


	int Matrix_at(int column, int row) {
		return data[column][row];
	}

	void Matrix_fill(int column,int row, float value) {
		data[column][row] = value;
	}

	void Print_matrix() {
		int count = 1;
		for (int i = 0; i < height; i++) {
			std::cout << "Row:" << count << "	";
			for (int j = 0; j < width; j++) {
				std::cout << data[j][i] << " ";
			}
			std::cout << std::endl;
			count++;
		}
	}

	void Matrix_fill_border(int value) {
		
		for (int i = 0; i < width; i++) {
			*data[i] = value;
			data[i][height - 1] = value;
		}
		for (int i = 0; i < height; i++) {
			data[0][i] = value;
			data[width - 1][i] = value;
		}
		
	}

	int Column_count() {
		return width;
	}

	int Row_count() {
		return height;
	}

	~Matrix() {
		for (int i = 0; i < width; i++) {
			delete[] data[i];
		}
		
		delete[] data;
	}




private:
	int width;
	int height;
	float **data;
};



#endif