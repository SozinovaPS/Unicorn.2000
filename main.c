#include "stb_image.h"
#include <stb_image_write.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

int main(){
	char* input_Path = "my.png";
	int iw, ih, n;
	unsigned char* idata = stbi_load(input_Path, &iw, &ih, &n, 0);
	if (idata == NULL){
		printf ("ERROR: can't read file! %s\n", input_Path);
		return -1;
	}
	int i, j, k = 0;
	unsigned char* odata = malloc(ih * iw * n * sizeof(unsigned char));
	unsigned char* newImage = malloc(ih * iw * sizeof(unsigned char));
	
	return 0;
}
