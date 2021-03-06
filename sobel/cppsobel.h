#ifndef __CPP_SOBEL_H__
#define __CPP_SOBEL_H__
#endif

#include "opencv2/highgui/highgui.hpp"
#include "opencv/cv.h"
#include "opencv2/contrib/contrib.hpp"

void CppSobel(IplImage* in, IplImage *out,
	int xorder,int yorder,
	int aperture_size CV_DEFAULT(3));



void CppSobel(IplImage* in, IplImage *out,
	int xorder,int yorder,
	int aperture_size /*CV_DEFAULT(3)*/)
{
	int i,j;
	uchar *inPtr  = NULL;
	uchar *inPtr1 = NULL;
	uchar *inPtr2 = NULL;
	uchar *inPtr3 = NULL;
	uchar *inPtr4 = NULL;
	uchar *inPtr5 = NULL;
	uchar *inPtr6 = NULL;
	uchar *outPtr = NULL;

	int width	  = in->width;
	int height    = in->height;
	int widthStep = in->widthStep;

	int *pEdgeX = (int *)calloc(width*height, sizeof(int));
	int *pEdgeY = (int *)calloc(width*height, sizeof(int));

	int *pEdgeXPtr = NULL;
	int *pEdgeYPtr = NULL;
	uchar *edgPtr = NULL;
	int edgeXMax = 0;
	int edgeYMax = 0;

	const int widH = 1;
	const int widV = widthStep;
	if(xorder)
	{
		for (i=1;i<height-1;i++)
		{
			pEdgeXPtr = pEdgeX + i*width + 1;
			inPtr = (uchar*)in->imageData + i * widthStep + 1;
			inPtr1 = inPtr + widH - widV;
			inPtr2 = inPtr + widH;
			inPtr3 = inPtr + widH + widV;
			inPtr4 = inPtr - widH - widV;
			inPtr5 = inPtr - widH;
			inPtr6 = inPtr - widH + widV;
			for (j=1;j<width-1;j++, pEdgeXPtr++)
			{
				*pEdgeXPtr
					= (*inPtr1++ + (int)*inPtr2++ * 2 + *inPtr3++)
					- (*inPtr4++ + (int)*inPtr5++ * 2 + *inPtr6++);
				edgeXMax = std::min(edgeXMax, *pEdgeXPtr);
			}
		}
	}

	if(yorder)
	{
		for (i=1;i<height-1;i++)
		{
			pEdgeYPtr = pEdgeY + i*width + 1;
			inPtr  = (uchar*)in->imageData + i * widthStep + 1;
			inPtr1 = inPtr + widV - widH;
			inPtr2 = inPtr + widV;
			inPtr3 = inPtr + widV + widH;
			inPtr4 = inPtr - widV - widH;
			inPtr5 = inPtr - widV;
			inPtr6 = inPtr - widV + widH;
			for (j=1;j<width-1;j++, pEdgeYPtr++)
			{
				*pEdgeYPtr
					= (*inPtr1++ + (int)*inPtr2++ * 2 + *inPtr3++)
					- (*inPtr4++ + (int)*inPtr5++ * 2 + *inPtr6++);
				edgeYMax = std::min(edgeYMax, *pEdgeYPtr);
			}
		}
	}
	if(xorder && ! yorder)
	{
		for (i=1;i<height-1;i++)
		{
			pEdgeXPtr = pEdgeX + i * width + 1;
			outPtr = (uchar*)out->imageData + i * widthStep + 1;
			for (j=1;j<width-1;j++, pEdgeXPtr++)
			{
				*outPtr = 255.0f * (*pEdgeXPtr) / edgeXMax;
				outPtr++;
			}
		}
	}
	else if(!xorder && yorder)
	{
		for (i=1;i<height-1;i++)
		{
			pEdgeYPtr = pEdgeY + i * width + 1;
			outPtr = (uchar*)out->imageData + i * widthStep + 1;
			for (j=1;j<width-1;j++, pEdgeYPtr++)
			{
				*outPtr = 255.0f * (*pEdgeYPtr) / edgeYMax;
				outPtr++;
			}
		}
	}
	else
	{
		for (i=1;i<height-1;i++)
		{
			pEdgeXPtr = pEdgeX + i*width + 1;
			pEdgeYPtr = pEdgeY + i*width + 1;
			edgPtr = (uchar*)out->imageData + i * widthStep + 1;
			for (j=1; j<width-1; j++, pEdgeYPtr++, pEdgeXPtr++, edgPtr++)
			{
				int x = *pEdgeXPtr;
				int y = *pEdgeYPtr;
				*edgPtr = (uchar)std::min(255,(int)(sqrt(0.0f + x * x + y * y)/2));
			}
		}
	}
	free(pEdgeY);
	free(pEdgeX);
	return ;
}