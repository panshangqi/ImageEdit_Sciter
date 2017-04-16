#include "opencv/cv.h"
#include "opencv/highgui.h"
#include "opencv2/opencv.hpp"
#pragma once
#include "tesseract/baseapi.h"
#include "tesseract/strngs.h"

using namespace cv;

struct __declspec(dllexport) regRect
{
	int x;
	int y;
	int w;
	int h;
};

class __declspec(dllexport) DocSlipt
{
public:
	int init();//init return 0 for ok, return -1 for something wrong
	int getRect(string fileName, vector<regRect>& questionRect, vector<regRect>& graphRect);//init return 1 for ok, return 0 for something wrong

protected:
	int voteWord(vector<Rect> words);
	int isChar(Rect word);
	int isBehand(Rect rect1, Rect rect2);
	int isBehandPlus(Rect rect1, Rect rect2);
	char numIs(Mat gray, Rect rect);

	int lineRange_v2(vector<Rect> line, int& top, int& bottom);

	tesseract::TessBaseAPI tess;
	tesseract::TessBaseAPI tess_chi;

	float wordWidth;
	float wordHeight;

	vector<Rect> words;
	vector<Rect> lines;
	vector<Rect> graphs;
	vector<Rect> questions;

	Mat dis;
	Mat thsMat;

	Rect Roi;
	Rect midLine;
};
