#include<stdio.h>
#include<stdlib.h>
#include "myDES.h"

int K[] = {0,0,0,1,0,0,1,1,0,0,1,1,0,1,0,0,0,1,0,1,0,1,1,1,0,1,1,1,1,0,0,1,1,0,0,1,1,0,1,1,1,0,1,1,1,1,0,0,1,1,1,0,1,1,1,1,1,1,1,1,0,0,0,1};
    
int IV[] = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1};



typedef struct {
     unsigned char red,green,blue;
} PPMPixel;

typedef struct {
     int x, y;
     PPMPixel *data;
} PPMImage;

#define RGB_COMPONENT_COLOR 255

static PPMImage *readPPM(const char *filename)
{
         char buff[16];
         PPMImage *img;
         FILE *fp;
         int c, rgb_comp_color;
         //open PPM file for reading
         fp = fopen(filename, "rb");
         if (!fp) {
              fprintf(stderr, "Unable to open file '%s'\n", filename);
              exit(1);
         }

         //read image format
         if (!fgets(buff, sizeof(buff), fp)) {
              perror(filename);
              exit(1);
         }

    //check the image format
    if (buff[0] != 'P' || buff[1] != '6') {
         fprintf(stderr, "Invalid image format (must be 'P6')\n");
         exit(1);
    }

    //alloc memory form image
    img = (PPMImage *)malloc(sizeof(PPMImage));
    if (!img) {
         fprintf(stderr, "Unable to allocate memory\n");
         exit(1);
    }

    //check for comments
    c = getc(fp);
    while (c == '#') {
    while (getc(fp) != '\n') ;
         c = getc(fp);
    }

    ungetc(c, fp);
    //read image size information
    if (fscanf(fp, "%d %d", &img->x, &img->y) != 2) {
         fprintf(stderr, "Invalid image size (error loading '%s')\n", filename);
         exit(1);
    }

    //read rgb component
    if (fscanf(fp, "%d", &rgb_comp_color) != 1) {
         fprintf(stderr, "Invalid rgb component (error loading '%s')\n", filename);
         exit(1);
    }

    //check rgb component depth
    if (rgb_comp_color!= RGB_COMPONENT_COLOR) {
         fprintf(stderr, "'%s' does not have 8-bits components\n", filename);
         exit(1);
    }

    while (fgetc(fp) != '\n') ;
    //memory allocation for pixel data
    img->data = (PPMPixel*)malloc(img->x * img->y * sizeof(PPMPixel));

    if (!img) {
         fprintf(stderr, "Unable to allocate memory\n");
         exit(1);
    }

    //read pixel data from file
    if (fread(img->data, 3 * img->x, img->y, fp) != img->y) {
         fprintf(stderr, "Error loading image '%s'\n", filename);
         exit(1);
    }

    fclose(fp);
    return img;
}
void writePPM(const char *filename, PPMImage *img)
{
    FILE *fp;
    //open file for output
    fp = fopen(filename, "wb");
    if (!fp) {
         fprintf(stderr, "Unable to open file '%s'\n", filename);
         exit(1);
    }

    //write the header file
    //image format
    fprintf(fp, "P6\n");

    //image size
    fprintf(fp, "%d %d\n",img->x,img->y);

    // rgb component depth
    fprintf(fp, "%d\n",RGB_COMPONENT_COLOR);

    // pixel data
    fwrite(img->data, 3 * img->x, img->y, fp);
    fclose(fp);
}


void ECBencryt(PPMImage *img)
{
    int i;
    if(img){
         for(i=0;i<=img->x*img->y-8;i+=8){
         	  int res[64];
         	  int C[64];
              
         	  ascii2binINT(img->data[i].red,res);
         	  ascii2binINT(img->data[i].green,res+8);
         	  ascii2binINT(img->data[i].blue,res+16);
         	  ascii2binINT(img->data[i+1].red,res+24);
         	  ascii2binINT(img->data[i+1].green,res+32);
         	  ascii2binINT(img->data[i+1].blue,res+40);
         	  ascii2binINT(img->data[i+2].red,res+48);
         	  ascii2binINT(img->data[i+2].green,res+56);
         	  encrypt(res,K,C);
         	  img->data[i].red = bin2dec(C,8);
         	  img->data[i].green = bin2dec(C+8,8);
         	  img->data[i].blue = bin2dec(C+16,8);
         	  img->data[i+1].red = bin2dec(C+24,8);
         	  img->data[i+1].green = bin2dec(C+32,8);
         	  img->data[i+1].blue = bin2dec(C+40,8);
         	  img->data[i+2].red = bin2dec(C+48,8);
         	  img->data[i+2].green = bin2dec(C+56,8);

         	  ascii2binINT(img->data[i+2].blue,res);
         	  ascii2binINT(img->data[i+3].red,res+8);
         	  ascii2binINT(img->data[i+3].green,res+16);
         	  ascii2binINT(img->data[i+3].blue,res+24);
         	  ascii2binINT(img->data[i+4].red,res+32);
         	  ascii2binINT(img->data[i+4].green,res+40);
         	  ascii2binINT(img->data[i+4].blue,res+48);
         	  ascii2binINT(img->data[i+5].red,res+56);
         	  encrypt(res,K,C);
         	  img->data[i+2].blue = bin2dec(C,8);
         	  img->data[i+3].red = bin2dec(C+8,8);
         	  img->data[i+3].green = bin2dec(C+16,8);
         	  img->data[i+3].blue = bin2dec(C+24,8);
         	  img->data[i+4].red = bin2dec(C+32,8);
         	  img->data[i+4].green = bin2dec(C+40,8);
         	  img->data[i+4].blue = bin2dec(C+48,8);
         	  img->data[i+5].red = bin2dec(C+56,8);

         	  ascii2binINT(img->data[i+5].green,res);
         	  ascii2binINT(img->data[i+5].blue,res+8);
         	  ascii2binINT(img->data[i+6].red,res+16);
         	  ascii2binINT(img->data[i+6].green,res+24);
         	  ascii2binINT(img->data[i+6].blue,res+32);
         	  ascii2binINT(img->data[i+7].red,res+40);
         	  ascii2binINT(img->data[i+7].green,res+48);
         	  ascii2binINT(img->data[i+7].blue,res+56);
         	  encrypt(res,K,C);
         	  img->data[i+5].green = bin2dec(C,8);
         	  img->data[i+5].blue = bin2dec(C+8,8);
         	  img->data[i+6].red = bin2dec(C+16,8);
         	  img->data[i+6].green = bin2dec(C+24,8);
         	  img->data[i+6].blue = bin2dec(C+32,8);
         	  img->data[i+7].red = bin2dec(C+40,8);
         	  img->data[i+7].green = bin2dec(C+48,8);
         	  img->data[i+7].blue = bin2dec(C+56,8);
      
         }
    }
}
void ECBdecryt(PPMImage *img)
{
    int i;
    if(img){
         for(i=0;i<=img->x*img->y-8;i+=8){
         	  int res[64];
         	  int C[64];
              
         	  ascii2binINT(img->data[i].red,res);
         	  ascii2binINT(img->data[i].green,res+8);
         	  ascii2binINT(img->data[i].blue,res+16);
         	  ascii2binINT(img->data[i+1].red,res+24);
         	  ascii2binINT(img->data[i+1].green,res+32);
         	  ascii2binINT(img->data[i+1].blue,res+40);
         	  ascii2binINT(img->data[i+2].red,res+48);
         	  ascii2binINT(img->data[i+2].green,res+56);
         	  decrypt(res,K,C);
         	  img->data[i].red = bin2dec(C,8);
         	  img->data[i].green = bin2dec(C+8,8);
         	  img->data[i].blue = bin2dec(C+16,8);
         	  img->data[i+1].red = bin2dec(C+24,8);
         	  img->data[i+1].green = bin2dec(C+32,8);
         	  img->data[i+1].blue = bin2dec(C+40,8);
         	  img->data[i+2].red = bin2dec(C+48,8);
         	  img->data[i+2].green = bin2dec(C+56,8);

         	  ascii2binINT(img->data[i+2].blue,res);
         	  ascii2binINT(img->data[i+3].red,res+8);
         	  ascii2binINT(img->data[i+3].green,res+16);
         	  ascii2binINT(img->data[i+3].blue,res+24);
         	  ascii2binINT(img->data[i+4].red,res+32);
         	  ascii2binINT(img->data[i+4].green,res+40);
         	  ascii2binINT(img->data[i+4].blue,res+48);
         	  ascii2binINT(img->data[i+5].red,res+56);
         	  decrypt(res,K,C);
         	  img->data[i+2].blue = bin2dec(C,8);
         	  img->data[i+3].red = bin2dec(C+8,8);
         	  img->data[i+3].green = bin2dec(C+16,8);
         	  img->data[i+3].blue = bin2dec(C+24,8);
         	  img->data[i+4].red = bin2dec(C+32,8);
         	  img->data[i+4].green = bin2dec(C+40,8);
         	  img->data[i+4].blue = bin2dec(C+48,8);
         	  img->data[i+5].red = bin2dec(C+56,8);

         	  ascii2binINT(img->data[i+5].green,res);
         	  ascii2binINT(img->data[i+5].blue,res+8);
         	  ascii2binINT(img->data[i+6].red,res+16);
         	  ascii2binINT(img->data[i+6].green,res+24);
         	  ascii2binINT(img->data[i+6].blue,res+32);
         	  ascii2binINT(img->data[i+7].red,res+40);
         	  ascii2binINT(img->data[i+7].green,res+48);
         	  ascii2binINT(img->data[i+7].blue,res+56);
         	  decrypt(res,K,C);
         	  img->data[i+5].green = bin2dec(C,8);
         	  img->data[i+5].blue = bin2dec(C+8,8);
         	  img->data[i+6].red = bin2dec(C+16,8);
         	  img->data[i+6].green = bin2dec(C+24,8);
         	  img->data[i+6].blue = bin2dec(C+32,8);
         	  img->data[i+7].red = bin2dec(C+40,8);
         	  img->data[i+7].green = bin2dec(C+48,8);
         	  img->data[i+7].blue = bin2dec(C+56,8);
      
         }
    }
}
void CBCencrypt(PPMImage *img)
{
	int i;
   
    
    
    if(img){
    	int res[64];
        int C[64];
    	for(i=0;i<=img->x*img->y-8;i+=8){
    		  
              
         	  ascii2binINT(img->data[i].red,res);
         	  ascii2binINT(img->data[i].green,res+8);
         	  ascii2binINT(img->data[i].blue,res+16);
         	  ascii2binINT(img->data[i+1].red,res+24);
         	  ascii2binINT(img->data[i+1].green,res+32);
         	  ascii2binINT(img->data[i+1].blue,res+40);
         	  ascii2binINT(img->data[i+2].red,res+48);
         	  ascii2binINT(img->data[i+2].green,res+56);

         	  if(i==0){
         	  	for(int q=0;q<64;q++){
         	  		res[q] = res[q]^IV[q];
         	  	}
         	  }
         	  else{
         	  	for(int q=0;q<64;q++){
         	  		res[q] = res[q]^C[q];
         	  	}
         	  }


         	  encrypt(res,K,C);
         	  img->data[i].red = bin2dec(C,8);
         	  img->data[i].green = bin2dec(C+8,8);
         	  img->data[i].blue = bin2dec(C+16,8);
         	  img->data[i+1].red = bin2dec(C+24,8);
         	  img->data[i+1].green = bin2dec(C+32,8);
         	  img->data[i+1].blue = bin2dec(C+40,8);
         	  img->data[i+2].red = bin2dec(C+48,8);
         	  img->data[i+2].green = bin2dec(C+56,8);

         	  ascii2binINT(img->data[i+2].blue,res);
         	  ascii2binINT(img->data[i+3].red,res+8);
         	  ascii2binINT(img->data[i+3].green,res+16);
         	  ascii2binINT(img->data[i+3].blue,res+24);
         	  ascii2binINT(img->data[i+4].red,res+32);
         	  ascii2binINT(img->data[i+4].green,res+40);
         	  ascii2binINT(img->data[i+4].blue,res+48);
         	  ascii2binINT(img->data[i+5].red,res+56);

         	  for(int q=0;q<64;q++){
         	  		res[q] = res[q]^C[q];
         	  }


         	  encrypt(res,K,C);
         	  img->data[i+2].blue = bin2dec(C,8);
         	  img->data[i+3].red = bin2dec(C+8,8);
         	  img->data[i+3].green = bin2dec(C+16,8);
         	  img->data[i+3].blue = bin2dec(C+24,8);
         	  img->data[i+4].red = bin2dec(C+32,8);
         	  img->data[i+4].green = bin2dec(C+40,8);
         	  img->data[i+4].blue = bin2dec(C+48,8);
         	  img->data[i+5].red = bin2dec(C+56,8);

         	  ascii2binINT(img->data[i+5].green,res);
         	  ascii2binINT(img->data[i+5].blue,res+8);
         	  ascii2binINT(img->data[i+6].red,res+16);
         	  ascii2binINT(img->data[i+6].green,res+24);
         	  ascii2binINT(img->data[i+6].blue,res+32);
         	  ascii2binINT(img->data[i+7].red,res+40);
         	  ascii2binINT(img->data[i+7].green,res+48);
         	  ascii2binINT(img->data[i+7].blue,res+56);

         	  for(int q=0;q<64;q++){
         	  		res[q] = res[q]^C[q];
         	  }


         	  encrypt(res,K,C);
         	  img->data[i+5].green = bin2dec(C,8);
         	  img->data[i+5].blue = bin2dec(C+8,8);
         	  img->data[i+6].red = bin2dec(C+16,8);
         	  img->data[i+6].green = bin2dec(C+24,8);
         	  img->data[i+6].blue = bin2dec(C+32,8);
         	  img->data[i+7].red = bin2dec(C+40,8);
         	  img->data[i+7].green = bin2dec(C+48,8);
         	  img->data[i+7].blue = bin2dec(C+56,8);
    	}
    }
}

void CBCdecrypt(PPMImage *img)
{
	int i;
    
    if(img){
    	int res[64],temp[64],temp2[64];
        int C[64];
    	for(i=0;i<=img->x*img->y-8;i+=8){
    		  
              
         	  ascii2binINT(img->data[i].red,C);
         	  ascii2binINT(img->data[i].green,C+8);
         	  ascii2binINT(img->data[i].blue,C+16);
         	  ascii2binINT(img->data[i+1].red,C+24);
         	  ascii2binINT(img->data[i+1].green,C+32);
         	  ascii2binINT(img->data[i+1].blue,C+40);
         	  ascii2binINT(img->data[i+2].red,C+48);
         	  ascii2binINT(img->data[i+2].green,C+56);


         	  for(int q=0;q<64;q++){
         	  	temp2[q] = C[q];
         	  }

         	  decrypt(C,K,res);

         	  if(i==0){
         	  	for(int q=0;q<64;q++){
         	  		res[q] = res[q]^IV[q];
         	  	}
         	  }
         	  else{
         	  	for(int q=0;q<64;q++){
         	  		res[q] = res[q]^temp[q];
         	  	}
         	  }


         	  
         	  img->data[i].red = bin2dec(res,8);
         	  img->data[i].green = bin2dec(res+8,8);
         	  img->data[i].blue = bin2dec(res+16,8);
         	  img->data[i+1].red = bin2dec(res+24,8);
         	  img->data[i+1].green = bin2dec(res+32,8);
         	  img->data[i+1].blue = bin2dec(res+40,8);
         	  img->data[i+2].red = bin2dec(res+48,8);
         	  img->data[i+2].green = bin2dec(res+56,8);

         	  ascii2binINT(img->data[i+2].blue,C);
         	  ascii2binINT(img->data[i+3].red,C+8);
         	  ascii2binINT(img->data[i+3].green,C+16);
         	  ascii2binINT(img->data[i+3].blue,C+24);
         	  ascii2binINT(img->data[i+4].red,C+32);
         	  ascii2binINT(img->data[i+4].green,C+40);
         	  ascii2binINT(img->data[i+4].blue,C+48);
         	  ascii2binINT(img->data[i+5].red,C+56);

         	  for(int q=0;q<64;q++){
         	  	temp[q] = C[q];
         	  }

         	  decrypt(C,K,res);
         	  for(int q=0;q<64;q++){
         	  		res[q] = res[q]^temp2[q];
         	  }


         	  
         	  img->data[i+2].blue = bin2dec(res,8);
         	  img->data[i+3].red = bin2dec(res+8,8);
         	  img->data[i+3].green = bin2dec(res+16,8);
         	  img->data[i+3].blue = bin2dec(res+24,8);
         	  img->data[i+4].red = bin2dec(res+32,8);
         	  img->data[i+4].green = bin2dec(res+40,8);
         	  img->data[i+4].blue = bin2dec(res+48,8);
         	  img->data[i+5].red = bin2dec(res+56,8);

         	  ascii2binINT(img->data[i+5].green,C);
         	  ascii2binINT(img->data[i+5].blue,C+8);
         	  ascii2binINT(img->data[i+6].red,C+16);
         	  ascii2binINT(img->data[i+6].green,C+24);
         	  ascii2binINT(img->data[i+6].blue,C+32);
         	  ascii2binINT(img->data[i+7].red,C+40);
         	  ascii2binINT(img->data[i+7].green,C+48);
         	  ascii2binINT(img->data[i+7].blue,C+56);

         	  for(int q=0;q<64;q++){
         	  	temp2[q] = C[q];
         	  }

			 decrypt(C,K,res);
         	  for(int q=0;q<64;q++){
         	  		res[q] = res[q]^temp[q];
         	  }

         	  for(int q=0;q<64;q++){
         	  	temp[q] = temp2[q];
         	  }



         	  
         	  img->data[i+5].green = bin2dec(res,8);
         	  img->data[i+5].blue = bin2dec(res+8,8);
         	  img->data[i+6].red = bin2dec(res+16,8);
         	  img->data[i+6].green = bin2dec(res+24,8);
         	  img->data[i+6].blue = bin2dec(res+32,8);
         	  img->data[i+7].red = bin2dec(res+40,8);
         	  img->data[i+7].green = bin2dec(res+48,8);
         	  img->data[i+7].blue = bin2dec(res+56,8);
    	}
    }
}


int main(){
    PPMImage *image;
    image = readPPM("penguin.ppm");
    ECBencryt(image);
    writePPM("penguinECBencrypted.ppm",image);
    
    PPMImage *image2;
  	image2 = readPPM("penguin.ppm");
  	CBCencrypt(image2);
  	writePPM("penguinCBCencrypted.ppm",image2);

  	CBCdecrypt(image2);
  	writePPM("penguinCBCdecrypted.ppm",image2);

  	ECBdecryt(image);
  	writePPM("penguinECBdecrypted.ppm",image);


  	printf("Press any key...\n");
  	getchar();
  	printf("Done!\n");
    return 0;
}
