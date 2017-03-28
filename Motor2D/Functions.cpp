#include "Functions.h"
#include "j1App.h"
#include <stdio.h>
#include <cmath>
#include "p2Defs.h"
#include "p2Log.h"

// Returns the angle between two points in degrees
float AngleFromTwoPoints(float x1, float y1, float x2, float y2)
{
	float deltaY = y2 - y1;
	float deltaX = x2 - x1;

	return (atan2(deltaY, deltaX) * RADTODEG);
}

float DistanceFromTwoPoints(float x1, float y1, float x2, float y2)
{
	int distance_x = x2 - x1;
	int distance_y = y2 - y1;
	float sign = ((distance_x * distance_x) + (distance_y * distance_y));
	float dist = abs((distance_x * distance_x) + (distance_y * distance_y));

	if(sign > 0)
		return sqrt(dist);
	else
		return -sqrt(dist);
}

bool TextCmp(const char * text1, const char * text2)
{
	bool ret = false;

	if (strcmp(text1, text2) == 0)
		ret = true;

	return ret;
}

void Tokenize(std::string string, const char separator, std::list<std::string>& tokens)
{
	uint i = 0;
	const char* str = string.c_str();
	while (*(str + i) != 0) 
	{
		std::string temporal;
		while (*(str + i) != separator && *(str + i) && *(str + i) != '\n') 
		{
			temporal.push_back(*(str + i));
			i++;
		}
		tokens.push_back(temporal);
		if (*(str + i)) i++;
	}
}

void ToLowerCase(std::string str)
{
	for (uint i = 0; i < str.size() - 1; i++) {
		str[i] = tolower(str[i]);
	}
}

fPoint Bezier(float curr_time, float end_time, vector<fPoint>& points)
{
	fPoint ret = NULLPOINT;

	float rel_time = curr_time / end_time;

	LOG("%f", rel_time);
	
	int i = points.size() - 1;

	while (i > 0)
	{
		for (int k = 0; k < i; k++)
		{
			points.at(k).x = points.at(k).x + (rel_time * (points.at(k + 1).x - points.at(k).x));
			points.at(k).y = points.at(k).y + (rel_time * (points.at(k + 1).y - points.at(k).y));
		}
		i--;
	}

	ret = points.at(0);
	return ret;
}

float getPt(float n1, float n2, float perc)
{
	float diff = n2 - n1;

	return n1 + (diff * perc);
}

fPoint Bezier2(float curr_time, float end_time, vector<fPoint>& points)
{
	fPoint ret = NULLPOINT;

	float rel_time = curr_time / end_time;

	float x1 = points.at(0).x;
	float y1 = points.at(0).y;

	float x2 = points.at(1).x;
	float y2 = points.at(1).y;

	float x3 = points.at(2).x;
	float y3 = points.at(2).y;

	float x4 = points.at(3).x;
	float y4 = points.at(3).y;

	float xa = getPt(x1, x2, rel_time);
	float ya = getPt(y1, y2, rel_time);
	float xb = getPt(x2, x3, rel_time);
	float yb = getPt(y2, y3, rel_time);
	float xc = getPt(x3, x4, rel_time);
	float yc = getPt(y3, y4, rel_time);

	// The Blue Line
	float xm = getPt(xa, xb, rel_time);
	float ym = getPt(ya, yb, rel_time);
	float xn = getPt(xb, xc, rel_time);
	float yn = getPt(yb, yc, rel_time);

	// The Black Dot
	ret.x = getPt(xm, xn, rel_time);
	ret.y = getPt(ym, yn, rel_time);

	return ret;
}

