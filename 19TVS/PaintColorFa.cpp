#include "stdafx.h"
#include "19TVS.h"
#include "PaintColorFa.h"
#include "Error.h"

#include "Windows.h"
#include <string>
#include <fstream>

struct ColorClass
{
	int pos;
	double val;
	int group;
};

ColorClass Color1;


COLORREF ColorReference(std::string nominal, std::string inpStr, int colorCount, int mainVal)
{
	COLORREF cellColor(RGB(0,0,0));
	int MAX_COUNT;
	double i;
	int groupNumber=0;
	int zeroGroup;
	int red=0;
	int green=0;
	int blue=0;
	double minVal, maxVal, currentVal;
	double groupSize;

	////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////
	colorCount -= 1;
	maxVal = ReturnOrder(mainVal, 1);
	minVal = ReturnOrder(mainVal, 0);
	
	currentVal = stod(inpStr);
	double averVal;
	if (abs(maxVal) > abs(minVal))
		averVal = minVal;
	else
		averVal = maxVal;

	groupSize = (maxVal - minVal) / colorCount;
	// Определяем в какой группе наше значение
	for(int j = 0;j<colorCount;j++)
	{
		if (currentVal < (minVal+((j+1)*groupSize)))
		{
			groupNumber = j;
			break;
		}
	}
	// Определяем откуда отталкиваться
	if (stod(nominal) == -999)
		i = (int)(maxVal+minVal)/2;
	else
		i = stod(nominal);

	for (int j = 0; j<colorCount; j++)
	{
		if (i < minVal+((j+1)*groupSize))
		{
			zeroGroup = j;
			break;
		}
	}
	////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////
	// Задаем цветовые схемы
	int stColorVal = 120;
	int endColorVal = 255;
	double coldColorStep = (endColorVal - stColorVal) / ((double)colorCount);
	double hotColorStep = (endColorVal- stColorVal)/((colorCount - zeroGroup) / 2.);
	blue = endColorVal;
	red = stColorVal;
	green = stColorVal;
	double rm=0, gm=0, bm=0;
	
		if (groupNumber < zeroGroup)
		{
			if (groupNumber < zeroGroup / 2)
				gm = groupNumber * coldColorStep;
			else
			{
				bm = -1 * (zeroGroup-groupNumber)*coldColorStep;
				gm = 135;
			}
		}
		else
		{
			if (groupNumber <= (colorCount + zeroGroup) / 2)
			{
				rm = (groupNumber - zeroGroup)*hotColorStep;
				gm = 135;
				bm = -135;
			}
			else
			{
				rm = 135;
				gm =(colorCount - groupNumber)*hotColorStep;
				bm = -135;
			}
		}
		red = red + rm;
		blue = blue + bm;
		green = green + gm;
	
		if (green < 120) 
			green = 120;
		if (blue < 120) 
			blue = 120;
		if (red < 120) 
			red = 120;
		if (green > 255)
			green = 255;
		if (blue > 255)
			blue = 255;
		if (red > 255)
			red = 255;



	cellColor = RGB(red, green, blue);



	return cellColor;
}
