#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <strings.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

void dfs(int v, int color, int iw, int ih, int* col, unsigned char* mat) {
    //printf("In dfs with v = %d\n", v);
    int i, j, u, d, r, l;
    col[v] = color;
    if (v+iw+1 < iw*ih) {
        r = mat[v+iw+1];
        if ((col[v+iw+1] == 0) && (abs(mat[v] - r) < 5)) dfs(v+iw+1, color, iw, ih, col, mat);
    }

    if (v-iw-1 > 0) {
        l = mat[v-iw-1];
        if ((col[v-iw-1] == 0) && (abs(mat[v] - l) < 5)) dfs(v-iw-1, color, iw, ih, col, mat);
    }


    if (v-iw+1 > 0) {
        u = mat[v-iw+1];
        if ((col[v-iw+1] == 0) && (abs(mat[v] - u) < 5)) dfs(v-iw+1, color, iw, ih, col, mat);
    }

    if (v+iw-1 < ih*iw) {
        d = mat[v+iw-1];
        if ((col[v+iw-1] == 0) && (abs(mat[v] - d) < 5)) dfs(v+iw-1, color, iw, ih, col, mat);
    }

}

int main() {

    // строка, путь к файлу
    char*inputPath = "Rider-Karl-Bryullov-1.png";
    //char*inputPath = "tooth.png";
    //char*inputPath = "arm_break.png";
    //char*inputPath = "Arrow_head.png";
    int iw, ih, n; //ширина, высота и количество цветовых каналов

    // Загружаем изображение, чтобы получить информацию о ширине, высоте и цветовом канале
    unsigned char *idata = stbi_load(inputPath, &iw, &ih, &n, 0);
    if (idata == NULL) {
        printf("ERROR: can't read file %s\n", inputPath );
        return 1;
    }

    int i, k, j;
    k = 0;
    unsigned char* MyImage = (unsigned char*)malloc(ih*iw*sizeof(unsigned char));
    unsigned char* odata = (unsigned char*)malloc(ih*iw*n*sizeof(unsigned char));
    unsigned char* newIm = (unsigned char*)malloc(ih*iw*sizeof(unsigned char));
    for (i = 0; i < ih*iw*n; i = i + n) {
        MyImage[k] = 0.299*idata[i] + 0.587*idata[i + 1] + 0.114*idata[i + 2];
        k = k + 1;
    }

    //preparation
    for (i = 2; i < ih-1; i++) {
        for (j = 2; j < iw-1; j++) {
            if (MyImage[iw*i+j] < 100) MyImage[iw*i+j] = 0;
            if (MyImage[iw*i+j] > 160) MyImage[iw*i+j] = 255;
        }
    }

    //only Gauss
    for (i = 1; i < ih-1; i++) {
        for (j = 2; j < iw-1; j++) {
            newIm[iw*i+j] = 0.15*MyImage[iw*i+j] + 0.12*MyImage[iw*(i+1)+j] + 0.12*MyImage[iw*(i-1)+j];
            newIm[iw*i+j] = newIm[iw*i+j] + 0.12*MyImage[iw*i+(j+1)] + 0.12*MyImage[iw*i+(j-1)];
            newIm[iw*i+j] = newIm[iw*i+j] + 0.09*MyImage[iw*(i+1)+(j+1)] + 0.09*MyImage[iw*(i+1)+(j-1)];
            newIm[iw*i+j] = newIm[iw*i+j] + 0.09*MyImage[iw*(i-1)+(j+1)] + 0.09*MyImage[iw*(i-1)+(j-1)];
        }
    }
/*
    //don't want it right now
    //contrast again
    for (i = 0; i < ih*iw; i++) {
        if (newIm[i] < 20) newIm[i] = 0;
        if (newIm[i] > 165) newIm[i] = 255;
        if ((newIm[i] != 0) && (newIm[i] != 255)) newIm[i] = 100;
    }
*/
//Filter operators
    unsigned char x, y, s;
    for (i = 2; i < ih - 1; i++) {
        for (j = 2; j < iw - 1; j++) {

            //Roberts
            x = newIm[iw*(i-1)+(j-1)] - newIm[iw*i+j];
            y = newIm[iw*(i-1)+j] - newIm[iw*i+(j+1)];
            s = sqrt(x*x + y*y);
            MyImage[iw*i+j] = s;
        }
    }
/*
    //third image craetion
    for (i = 0; i < ih*iw; i++) {
        if (MyImage[i] > newIm[i]) {
            newIm[i] = MyImage[i];
        }
    }
*/
    int col[iw*ih];
    for (i = 0; i < iw*ih; i++) col[i] = 0;
    k = 55;
    printf("problem in dfs\n");
    //dfs making
    for (i = 0; i < iw*ih; i++) {
        if (col[i] == 0) {
            dfs(i, k, iw, ih, col, newIm);
            k = k + 50;
        }
    }
    printf("Problem with coloring\n");
    //now have to color the colors from col
    for (i = 0; i < iw*ih; i++) {
        odata[i*n] = 100+col[i]+0.20*col[i-1];
        odata[i*n+1] = 149+col[i];
        odata[i*n+2] = 237+col[i];
        if (n == 4) odata[i*n+3] = 255;
    }
    //printf("There are %d Vs, in general: %d\n", ih*iw, ih*iw*4);
    //going back to n channels

    //char* outputPath = "output_arrow_head.png";
    //char* outputPath = "output_arm_break.png";
    //char* outputPath = "output_hamster.png";
    //char* outputPath = "output_tooth.png";
    char* outputPath = "output.png";

    // записываем картинку
    int one = 1; int zero = 0;
    //stbi_write_png(outputPath, iw, ih, n, odata, 0);
    stbi_write_png(outputPath, iw, ih, n, odata, 0);
    //stbi_image_write(outputPath, iw, ih, 2, MyImage, 0);
    //printf("Изображение размера %d в высоту и %d в ширину с количеством каналов %d считано", ih, iw, n);
    stbi_image_free(idata);
    stbi_image_free(newIm);
    stbi_image_free(MyImage);
    //stbi_image_free(odata);
	printf ("^-^");
	printf ("\n");
    return 0;
}
