//First two functions
//Author: BeholderOf(Forum name)
//URL: http://www.vbforums.com/showthread.php?261522-C-C-Loading-Bitmap-Files-(Manually)
//Date: (Last post edit) Sep 18th, 2003

#include <stdio.h>
#include "bitmap.h"

#include "video_gr.h"
Bitmap* loadBitmap(const char* filename) {
	// allocating necessary size
	Bitmap* bmp = (Bitmap*) malloc(sizeof(Bitmap));

	// open filename in read binary mode
	FILE *filePtr;
	filePtr = fopen(filename, "rb");
	if (filePtr == NULL)
		return NULL;


	// read the bitmap file header
	BitmapFileHeader bitmapFileHeader;
	fread(&bitmapFileHeader, 2, 1, filePtr);

	// verify that this is a bmp file by check bitmap id
	if (bitmapFileHeader.type != 0x4D42) {
		fclose(filePtr);
		return NULL;
	}

	int rd;
	do {
		if ((rd = fread(&bitmapFileHeader.size, 4, 1, filePtr)) != 1)
			break;
		if ((rd = fread(&bitmapFileHeader.reserved, 4, 1, filePtr)) != 1)
			break;
		if ((rd = fread(&bitmapFileHeader.offset, 4, 1, filePtr)) != 1)
			break;
	} while (0);

	if (rd = !1) {
		fprintf(stderr, "Error reading file\n");
		exit(-1);
	}

	// read the bitmap info header
	BitmapInfoHeader bitmapInfoHeader;
	fread(&bitmapInfoHeader, sizeof(BitmapInfoHeader), 1, filePtr);

	// move file pointer to the begining of bitmap data
	fseek(filePtr, bitmapFileHeader.offset, SEEK_SET);

	// allocate enough memory for the bitmap image data
	unsigned char* bitmapImage = (unsigned char*) malloc(
			bitmapInfoHeader.imageSize);

	// verify memory allocation
	if (!bitmapImage) {
		free(bitmapImage);
		fclose(filePtr);
		return NULL;
	}

	// read in the bitmap image data
	fread(bitmapImage, bitmapInfoHeader.imageSize, 1, filePtr);

	// make sure bitmap image data was read
	if (bitmapImage == NULL) {
		fclose(filePtr);
		return NULL;
	}

	// close file and return bitmap image data
	fclose(filePtr);

	bmp->bitmapData = bitmapImage;
	bmp->bitmapInfoHeader = bitmapInfoHeader;

	return bmp;
}

void drawBitmap(Bitmap* bmp, int x, int y, Alignment alignment) {
	if (bmp == NULL)
		return;

	int width = bmp->bitmapInfoHeader.width;
	int drawWidth = width;
	int height = bmp->bitmapInfoHeader.height;

	if (alignment == ALIGN_CENTER)
		x -= width / 2;
	else if (alignment == ALIGN_RIGHT)
		x -= width;

	if (x + width < 0 || x > getH_res() || y + height < 0
			|| y > getV_res())
		return;

	int xCorrection = 0;
	if (x < 0) {
		xCorrection = -x;
		drawWidth -= xCorrection;
		x = 0;

		if (drawWidth > getH_res())
			drawWidth = getH_res();
	} else if (x + drawWidth >= getH_res()) {
		drawWidth = getH_res() - x;
	}

	char* bufferStartPos;
	char* imgStartPos;

	/*unsigned char * map = bmp->bitmapData;
	//char * ptr;

	unsigned int i,j;
	for(i = y + height; i >= y;i++){
		for(j = x; j < x + width;j++){
			//if ((*map) != transparent_color){
				vg_pixel(j,i,*map);
			//}
			map += 2;
		}
	}*/

	int i;
    for (i = 0; i < height; i++) {
        int pos = y + height - 1 - i;

        if (pos < 0 || pos >= getV_res())
            continue;

        bufferStartPos = getBuffer();
      bufferStartPos += x * 2 + pos * getH_res() * 2;

        imgStartPos = bmp->bitmapData + xCorrection * 2 + i * width * 2;

        memcpy(bufferStartPos, imgStartPos, drawWidth * 2);
    }
}

void deleteBitmap(Bitmap* bmp) {
	if (bmp == NULL)
		return;

	free(bmp->bitmapData);
	free(bmp);
}
