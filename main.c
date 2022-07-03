#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <math.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define BLACK 94
#define WHITE 166

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
            		if (Image[w * i + j] <  BLACK)
				Image[w * i + j] = 0;
            		if (Image[w * i + j] > WHITE)
				Image[w * i + j] = 255;
        	}
	return Image;
}

void DFS (int i, int j, int w, int h, int size,  int graph[size], unsigned char* Img, int m){
	graph[w * i + j] = m;
    	if((i >= 1) && (i <= h - 1) && (j - 2 >= 1) && (j - 2 <= w - 1))
        	if((fabs(Img[w * i + j] - Img[w * i + (j - 2)]) <= 60) && (graph[w * i + (j - 2)] == 0))
            		DFS(i, j - 2, w, h, size, graph, Img, m);
        
    
    	if((i - 2 >= 1) && (i - 2 <= h - 1) && (j + 1 >= 1) && (j + 1 <= w - 1))
        	if((fabs(Img[w * i + j] - Img[w * (i - 2) + (j + 1)]) <= 60) && (graph[w * (i - 2) + (j + 1)] == 0))
            		DFS(i - 2, j + 1, w, h, size, graph, Img, m);
       
    
    	if((i + 2 >= 1) && (i + 2 <= h - 1) && (j + 1 >= 1) && (j + 1 <= w - 1))
        	if((fabs(Img[w * i + j] - Img[w * (i + 2) + (j + 1)]) <= 60) && (graph[w * (i + 2) + (j + 1)] == 0))
            		DFS(i + 2, j + 1, w, h, size, graph, Img, m);
     
}

unsigned char* coloration(unsigned char* Img, int w, int h, int n, int size, int graph[size]){
	int i;
	for (i = 0; i < size; i++){
        	Img[i * n] = 100 + graph[i] + 0.20 * graph[i - 1];
        	Img[i * n + 1] = 149 + graph[i];
        	Img[i * n + 2] = 237 + graph[i];
        	if (n == 4) 
			Img[i * n + 3] = 255;
   	}
	return Img;
}

int main(){
	char* inputPath = "Rider-Karl-Bryullov-1.png";
	int w, h, n;
	unsigned char* data = stbi_load(inputPath, &w, &h, &n, 0);
	if (data == NULL){
		printf ("ERROR: can't read file! %s\n", inputPath);
		return -1;
	}
	int i, j, k = 55;
	unsigned char* odata = (unsigned char*) malloc(h * w * n * sizeof(unsigned char));
	unsigned char* My_img = (unsigned char*) malloc(h * w * sizeof(unsigned char));
	if ((odata == NULL) || (My_img == NULL)) {
            	printf("Memory allocation error at main()" );
		return 1;
        }
	My_img = color_to_grey(data, w, h, n);
	
	My_img = contrast(My_img, w, h);
	
	int size = w * h;
	int graph[size];
	for (i = 0; i < size; i++)
		graph[i] = 0;

	int m = 0;
	for (i = 1; i < h - 1; i++)
		for (j = 1; j < w - 1; j++)
			if (graph[w * i + j] == 0){
				m += 1;
				DFS(i, j, w, h, size, graph,  My_img, m);
			}

	My_img = coloration(odata, w, h, n, size, graph);

	char* outputPath = "prekol.png";
	stbi_write_png(outputPath, w, h, n, My_img, 0);

	printf ("^-^");
	printf ("\n");
	return 0;
}
	
