#include "Matrix.h"
#include "unit_test_framework.h"
#include <assert.h>

using namespace std;

TEST(test_constructor) {
	Matrix mat(392, 222);
	ASSERT_EQUAL(mat.Row_count(), 222);
	ASSERT_EQUAL(mat.Column_count(), 392);
	
}


TEST(test_fill) {
	Matrix mat(10,15);
	float value[10][15];
	float x = 0;
	for (int i = 0; i < mat.Row_count(); i++) {
		for (int j = 0; j < mat.Column_count(); j++) {
			value[j][i] = x;
			mat.Matrix_fill(j,i,value[j][i]);
			x++;
		}
	}
	
	
	for (int i = 0; i < mat.Column_count(); i++) {
		for (int j = 0; j < mat.Row_count(); j++) {
			ASSERT_EQUAL(mat.Matrix_at(i, j), value[i][j]);
		}
	}
	mat.Print_matrix();
	
}

TEST(test_fill_border) {
	Matrix mat(4, 5);
	int value = 1;
	for (int i = 0; i < mat.Row_count(); i++) {
		for (int j = 0; j < mat.Column_count(); j++) {
			mat.Matrix_fill(j, i, value);
		}
	}
	mat.Matrix_fill_border(value);
	mat.Print_matrix();
}

TEST(test_print) {
	Matrix mat(5,10);
	int value = rand()%555;
	for (int i = 0; i < mat.Row_count(); i++) {
		for (int j = 0; j < mat.Column_count(); j++) {
			mat.Matrix_fill(j, i, value);
		}
	}
	mat.Print_matrix();
	
}

TEST_MAIN()