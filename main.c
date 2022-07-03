#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define BLACK 90
#define WHITE 170

unsigned char* color_to_grey(unsigned char* Image, int w, int h, int n){
	int i, k = 0;
	int size;
	size = w * h * n;
	unsigned char* gray_img = (unsigned char*)malloc(w * h * sizeof(unsigned char));
	if (gray_img == NULL) {
            printf("Memory allocation error at color_to_gray(): %d, %d\n", w, h );
            return Image;
        }
	for (i = 0; i < size; i += n){
		gray_img[k] = 0.299 * Image[i] + 0.587 * Image[i + 1] + 0.114 * Image[i + 2];
		k++;
	}
	return gray_img;
}

unsigned char*  contrast( unsigned char* Image, int w, int h) {
	int i, j;    	
	for (i = 2; i < h - 1; i++)
        	for (j = 2; j < w - 1; j++) {
            		if (Image[w * i + j] < BLACK)
				Image[w * i + j] = 0;
            		if (Image[w * i + j] > WHITE)
				Image[w * i + j] = 255;
        	}
	return Image;
}

int main(){
	char* inputPath = "hampster.png";
	int w, h, n;
	unsigned char* data = stbi_load(inputPath, &w, &h, &n, 0);
	if (data == NULL){
		printf ("ERROR: can't read file! %s\n", inputPath);
		return -1;
	}
	int i, j, k = 0;
	unsigned char* odata = (unsigned char*) malloc(h * w * n * sizeof(unsigned char));
	unsigned char* My_img = (unsigned char*) malloc(h * w * sizeof(unsigned char));
	if ((odata == NULL) || (My_img == NULL)) {
            	printf("Memory allocation error at main()" );
		return 1;
        }
	My_img = color_to_grey(data, w, h, n);
	
	My_img = contrast(My_img, w, h);

	char* outputPath = "final.png";
	stbi_write_png(outputPath, w, h, 1, My_img, 0);

	printf ("^-^");
	return 0;
}
	
