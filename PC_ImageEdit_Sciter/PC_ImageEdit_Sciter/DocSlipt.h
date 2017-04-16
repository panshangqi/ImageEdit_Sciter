#pragma once
#include "opencv\cv.h"
#include "opencv\highgui.h"

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
	int init();
	int getRect(string fileName, vector<regRect>& questionRect, vector<regRect>& graphRect);
	int getOcrResult(vector<regRect> questionRect, vector<regRect> graphRect, vector<string>& result, vector<int>& num);
	int getNum(vector<regRect> questionRect, vector<regRect> graphRect, vector<int>& num);
	string getOneOcr(int num);
	int poiConut;
	int poipoiCount;

	wchar_t dialogname[50];
	int taskID;

protected:
	int voteWord(vector<Rect> words);
	char numIs(Mat gray, Rect rect);
	int isChar(Rect word);
	float isCharacter(Rect word);
	int isBehand(Rect rect1, Rect rect2);
	int isBehandPlus(Rect rect1, Rect rect2);
	int isFraction(Rect rect, Rect& frect, Rect& numerator, Rect& denominator);
	int isRadical(Rect rect, Rect& content);
	int isPlus(Rect rect);
	int isParentheses(Rect rect);
	void poiSaveImg(Mat mat);

	int loopParentheses(vector<Rect>& leftParentheses, vector<Rect>& rightParentheses);

	int lineRange_v2(vector<Rect> line, int& top, int& bottom, float& b, float& wordBetween);

	int eqOcr(Rect rect, string& result);
	char* reg(Rect rect);

	tesseract::TessBaseAPI tess;
	tesseract::TessBaseAPI tess_chi;

	float wordWidth;
	float wordHeight;

	float meanWordSum;
	float globalWordBetween;

	vector<Rect> words;
	vector<Rect> wordsOrigin;
	//vector<Rect> lines;
	vector<Rect> graphs;
	vector<Rect> questions;
	vector<Rect> questionHead;

	vector<Rect> equations;
	vector<Rect> Characters;

	vector<Rect> parentheses;
	vector<Rect> fractions;
	vector<Rect> radicals;

	vector<Rect> parentheses_content;
	vector<Rect> fractions_numerator;
	vector<Rect> fractions_denominator;
	vector<Rect> radicals_content;

	vector<Rect> lineInQuestion;

	Mat dis;
	Mat thsMat;
	Mat gray;

	Rect Roi;
	Rect midLine;

	float* SVs;
	float* SVa;

	float SVMb;

};
