#pragma once
#include "Share.h"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace cv;
using namespace std;

class Core
{
public:
	int 	widthSI, heightSI, widthCanvas, heightCanvas, widthIA, heightIA;
	Mat 	SI, filteredSI, canvas;
	string	p, q, g, h, v1, v2, v3, ext, singlePath, singleSI, sharePath, scheme;
	vector<string> siPath, siName, sharesPath, ssSize;
	vector<double> dur1, dur2, dur3, dur4;

	Core();
	void 	reset();
	void 	centerString(char* s);
	int 	viewMainMenu();
	void 	viewEncoding();
	void 	viewDecoding();
	void  	setSecretImagePath();
	void 	setSecretImage(string pathSI);
	void 	setShadowSharePath(Share& S1, Share& S2);
	void  	setSSSize(Share& S1, Share& S2, string size);
	void 	setChameleonVariable();
	int 	getRandomNumber();
	string 	getChameleonHashValue(string seed1, string seed2);
	string 	getSeed(int index, Share& S);
	void 	genPoints(Share& S1, Share& S2);
	void 	genFirstPoint(Share& S1, Share& S2);
	static Point2i index2point(int index, int width);
	vector<int>	getPosition(Share S);
	vector<int>	getMinDistance(int a1, int a2, int b1, int b2, int c1, int c2, int d1, int d2);
	int		checkFlag2(vector<int> position1, vector<int> position2);
	int		checkFlag3(vector<int> position1, vector<int> position2);
	void	genSecondPoint(Share& S1, Share& S2);
	void 	genThirdPoint(Share& S1, Share& S2);
	void 	getIntersectionProp(Share& S1, Share& S2, int index1, int index2);
	void 	randomSIPosition(Share& S1, Share& S2);
	void 	filter();
	void 	generateShare(Share& S1, Share& S2);
	void 	getNormalRotationImage(Share& S);
	static string str2bin(string str,int base,int pad);
	static Mat	generateNewImg(int h, int w, int channel);
	void 	splitString(const string &s, char delim, vector<string> &elems);
	vector<string> splitString(const string &s, char delim);
	void 	setSharePath();
	void 	splitParam(string binParam1, string binParam2, Share& S1, Share& S2);
	void 	getRotationImage(Share& S);
	void 	getCanvas(Share& S1, Share& S2, int index1, int index2);
	void 	stackShares2Canvas(Share& S1, Share& S2);
	string 	str2number(string str, int baseSource, int baseDestination);
	void	stacking(Share& S1, Share& S2, string scheme, string singleSI, string type, int index);
};

