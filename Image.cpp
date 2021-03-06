//*****************************************************************************
//
// Image.cpp : Defines the class operations on images
//
// Author - Parag Havaldar
// Code used by students as starter code to display and modify images
//
//*****************************************************************************

#include "Image.h"
#include <cmath>

// Constructor and Desctructors
MyImage::MyImage() 
{
	Data = NULL;
	Width = -1;
	Height = -1;
	ImagePath[0] = 0;
}

MyImage::~MyImage()
{
	if ( Data )
		delete Data;
}


// Copy constructor
MyImage::MyImage( MyImage *otherImage)
{
	Height = otherImage->Height;
	Width  = otherImage->Width;
	Data   = new char[Width*Height*3];
	strcpy(otherImage->ImagePath, ImagePath );

	for ( int i=0; i<(Height*Width*3); i++ )
	{
		Data[i]	= otherImage->Data[i];
	}
}



// = operator overload
MyImage & MyImage::operator= (const MyImage &otherImage)
{
	Height = otherImage.Height;
	Width  = otherImage.Width;
	Data   = new char[Width*Height*3];
	strcpy( (char *)otherImage.ImagePath, ImagePath );

	for ( int i=0; i<(Height*Width*3); i++ )
	{
		Data[i]	= otherImage.Data[i];
	}
	
	return *this;

}


void rotate_point(float cx, float cy, float angle, float *newx, float *newy)
{
	float s = sin(angle);
	float c = cos(angle);

	// translate point back to origin:
	*newx -= cx;
	*newy -= cy;

	// rotate point
	float xnew = *newx * c - *newy * s;
	float ynew = *newx * s + *newy * c;

	// translate point back:
	*newx = xnew + cx;
	*newy = ynew + cy;
}


void draw_helper(char *Data, int x1, int y1, int x2, int y2, int Width, int Height) {
	int start_x = x1, start_y = y1, end_x = x2, end_y = y2;
	int dx = x2 - x1;
	int dy = y2 - y1;
	double slope = 0;

	bool anchor_x;
	if (dx != 0) slope = dy / (double)dx;

	if (abs(slope) <= 1 && dx != 0) {
		anchor_x = true;
		if (dx < 0) {
			start_x = x2;
			start_y = y2;
			end_x = x1;
			end_y = y1;
		}
		slope = (end_y - start_y) / (double)(end_x - start_x);
	}
	else {
		anchor_x = false;
		if (dy < 0) {
			start_x = x2;
			start_y = y2;
			end_x = x1;
			end_y = y1;
		}
		if (dx == 0) { slope = 0; }
		else { slope = (end_x - start_x) / (double)(end_y - start_y); }
	}

	// set starting point to zero first
	Data[start_y*Width * 3 + start_x * 3] = 0;
	Data[start_y*Width * 3 + start_x * 3 + 1] = 0;
	Data[start_y*Width * 3 + start_x * 3 + 2] = 0;

	if (anchor_x) {
		double y = start_y + 0.5;
		for (int x = start_x + 1; x <= end_x; x++) {
			y = y + slope;
			int int_y = (int)floor(y);
			Data[int_y*Width * 3 + x * 3] = 0;
			Data[int_y*Width * 3 + x * 3 + 1] = 0;
			Data[int_y*Width * 3 + x * 3 + 2] = 0;
		}
	}
	else {
		double x = start_x + 0.5;
		for (int y = start_y + 1; y <= end_y; y++) {
			x = x + slope;
			int int_x = (int)floor(x);
			Data[y*Width * 3 + int_x * 3] = 0;
			Data[y*Width * 3 + int_x * 3 + 1] = 0;
			Data[y*Width * 3 + int_x * 3 + 2] = 0;
		}
	}
}

void slope (float x1, float y1, float x2, float y2, float *m, float *c) {
	float dx, dy;
	dx = x2 - x1;
	dy = y2 - y1;

	if (dx == 0)
		dx = 1;
	*m = dy / dx;
	*c = y1 - (*m) * x1;
}

void intersection(float x1, float y1, float x2, float y2, float *newx, float *newy)
{
	float m1, c1, m2, c2;
	float lx1, ly1, lx2, ly2;

	for (int i = 0; i < 4; i++) {
		if (i == 0) {
			lx1 = 255; ly1 = 0; lx2 = 256; ly2 = 256;
		} else if (i == 1) {
			lx1 = 0; ly1 = 255; lx2 = 256; ly2 = 256;
		} else if (i == 2) {
			lx1 = 0; ly1 = 0; lx2 = 1, ly2 = 256;
		} else {
			lx1 = 0, ly1 = 0; lx2 = 256, ly2 = 1;
		}

		slope (lx1, ly1, lx2, ly2, &m1, &c1);
		slope (x1, y1, x2, y2, &m2, &c2);

		if ((m1 - m2) != 0) {
			*newx = (c2 - c1) / (m1 - m2);
			*newy = m1 * (*newx) + c1;
			break;
		}
	}
}


// MyImage::CreatImageCanv
// Function to create white image with two dots connected
bool MyImage::CreatImageCanv()
{	
	// Allocate Data structure and copy
	Data = new char[Width*Height*3];
	float x1, y1, x2, y2;

	for (int i = 0; i < Height*Width; i++)
	{
		Data[3*i]	= 255;
		Data[3*i+1]	= 255;
		Data[3*i+2]	= 255;
	}

	// two coordinates to connect a line, x is for height, y is for row
	int width = Width;
	int height = Height;

	x1 = 128;  y1 = 128; x2 = 255; y2 = 0;
	for (int i = 45; i < 360; i += 45) {
		float radian = 0.01746031746 * i;
		draw_helper(Data, x1, y1, x2, y2, width, height);
		x1 = 128;  y1 = 128; x2 = 255; y2 = 0;
		rotate_point(x1, x2, radian, &x2, &y2);
		intersection(x1, y1, x2, y2, &x2, &y2);
		if (x2 < 0 || y2 < 0)
			break;
		if (x2 > 255)
			x2 = 255;
		if (y2 > 255)
			y2 = 255;
	}
    
	return true;
}

// MyImage::ReadImage
// Function to read the image given a path
bool MyImage::ReadImage()
{

	// Verify ImagePath
	if (ImagePath[0] == 0 || Width < 0 || Height < 0 )
	{
		fprintf(stderr, "Image or Image properties not defined");
		fprintf(stderr, "Usage is `Image.exe Imagefile w h`");
		return false;
	}
	
	// Create a valid output file pointer
	FILE *IN_FILE;
	IN_FILE = fopen(ImagePath, "rb");
	if ( IN_FILE == NULL ) 
	{
		fprintf(stderr, "Error Opening File for Reading");
		return false;
	}

	// Create and populate RGB buffers
	int i;
	char *Rbuf = new char[Height*Width]; 
	char *Gbuf = new char[Height*Width]; 
	char *Bbuf = new char[Height*Width]; 

	for (i = 0; i < Width*Height; i ++)
	{
		Rbuf[i] = fgetc(IN_FILE);
	}
	for (i = 0; i < Width*Height; i ++)
	{
		Gbuf[i] = fgetc(IN_FILE);
	}
	for (i = 0; i < Width*Height; i ++)
	{
		Bbuf[i] = fgetc(IN_FILE);
	}
	
	// Allocate Data structure and copy
	Data = new char[Width*Height*3];
	for (i = 0; i < Height*Width; i++)
	{
		Data[3*i]	= Bbuf[i];
		Data[3*i+1]	= Gbuf[i];
		Data[3*i+2]	= Rbuf[i];
	}

	// Clean up and return
	delete Rbuf;
	delete Gbuf;
	delete Bbuf;
	fclose(IN_FILE);

	return true;

}



// MyImage functions defined here
bool MyImage::WriteImage()
{
	// Verify ImagePath
	// Verify ImagePath
	if (ImagePath[0] == 0 || Width < 0 || Height < 0 )
	{
		fprintf(stderr, "Image or Image properties not defined");
		return false;
	}
	
	// Create a valid output file pointer
	FILE *OUT_FILE;
	OUT_FILE = fopen(ImagePath, "wb");
	if ( OUT_FILE == NULL ) 
	{
		fprintf(stderr, "Error Opening File for Writing");
		return false;
	}

	// Create and populate RGB buffers
	int i;
	char *Rbuf = new char[Height*Width]; 
	char *Gbuf = new char[Height*Width]; 
	char *Bbuf = new char[Height*Width]; 

	for (i = 0; i < Height*Width; i++)
	{
		Bbuf[i] = Data[3*i];
		Gbuf[i] = Data[3*i+1];
		Rbuf[i] = Data[3*i+2];
	}

	
	// Write data to file
	for (i = 0; i < Width*Height; i ++)
	{
		fputc(Rbuf[i], OUT_FILE);
	}
	for (i = 0; i < Width*Height; i ++)
	{
		fputc(Gbuf[i], OUT_FILE);
	}
	for (i = 0; i < Width*Height; i ++)
	{
		fputc(Bbuf[i], OUT_FILE);
	}
	
	// Clean up and return
	delete Rbuf;
	delete Gbuf;
	delete Bbuf;
	fclose(OUT_FILE);

	return true;

}




// Here is where you would place your code to modify an image
// eg Filtering, Transformation, Cropping, etc.
bool MyImage::Modify()
{

	// TO DO by student
	
	// sample operation
	for ( int i=0; i<Width*Height; i++ )
	{
		Data[3*i] = 0;
		Data[3*i+1] = 0;
	}

	return false;
}