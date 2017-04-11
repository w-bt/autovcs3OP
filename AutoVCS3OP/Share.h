#pragma once
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace cv;
using namespace std;

class Share
{
public:
	int			index, width, height, rotWidth, rotHeight, widthSS, heightSS, rotWidthSS, rotHeightSS, i[3], angle, revAngle, paramLength, shape3OP;
	vector<int> availableOrientation;
	vector<Point2i> intersection, position;
	Mat			share, rotatedShare;
	string		beta, gamma, delta;
	Point2i		p[3], rotP[3], nP[3], rotNP[3], pointStart, pointIA;
	vector<Mat> channels, rotChannels;

	Share(int i);
	~Share();
	void 	setAvailableOrientation(int widthSI, int heightSI);
	void	getRotationPoints();
	void 	getRotationSize();
	void 	setIntersection(int startX, int endX, int startY, int endY);
	string 	getBinaryParam(string strP, string strGen);
	void 	embedParam(string binParam);
	void 	setShare(string path);
	string 	getParam();
	void 	get3OP(string v1, string v2, string v3);
	void 	getNormal3OP(int i, int type);
	void 	getShape3OP(int i, int type);
	void 	shape2angle(int opositeShape);
	void 	getRotationNormalPoints();
};

