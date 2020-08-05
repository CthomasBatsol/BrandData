#include "Image.h"
#include "unit_test_framework.h"

using namespace std;

TEST(test_print){
	char file[] = "Brands/Mastercell/Mastercell.bmp";
	char* input = file;
	Image img(input);
	img.Image_print();
}

TEST(test_write) {
	char file[] = "Brands/Mastercell/Mastercell.bmp";
	char* input = file;
	ofstream oin;
	string csv = "image.csv";
	Image img(input);
	img.Image_write(oin, csv);
}

TEST(test_dct) {
	
}

TEST_MAIN()