#include "stdafx.h"
#include "targetver.h"
#include "Share.h"
#include "Core.h"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <set>
#include <algorithm>    // std::shuffle, sort
#include <array>        // std::array
#include <random>       // std::default_random_engine
#include <chrono>       // std::chrono::system_clock
#include <iostream>     // std::cout
#include <vector>       // std::vector
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>

#if defined(_MSC_VER) && _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4146 4244 4800)
#endif
#include <mpirxx.h>
#if defined(_MSC_VER) && _MSC_VER
#pragma warning(pop)
#endif

using namespace cv;
using namespace std;

Share::Share(int i)
{
	index		= i;
	angle		= 0;
	revAngle	= 0;
	widthSS 	= 0;
	heightSS 	= 0;
	width		= 0;
	height		= 0;
	rotWidth	= 0;
	rotHeight	= 0;
	paramLength	= ((16*3) + 20 + 10);
}

Share::~Share() {
   return;
}

void Share::setAvailableOrientation(int widthSI, int heightSI){
	if((widthSS >= widthSI)&&(widthSS >= heightSI)&&(heightSS >= widthSI)&&(heightSS >= heightSI)){
		availableOrientation.clear();
		availableOrientation.push_back(0);
		availableOrientation.push_back(1);
		availableOrientation.push_back(2);
		availableOrientation.push_back(3);
	} else {
		if((widthSS >= widthSI)&&(heightSS >= heightSI)){
			availableOrientation.clear();
			availableOrientation.push_back(0);
			availableOrientation.push_back(2);
		} else {
			availableOrientation.clear();
			availableOrientation.push_back(1);
			availableOrientation.push_back(3);
		}
	}
}

void Share::getRotationPoints(){
	int tempY, tempX;
	Point2i co;

	if (angle == 0){
		rotP[0] = p[0];
		rotP[1] = p[1];
		rotP[2] = p[2];
	} else {
		if (angle == 90){
			for(int i = 0; i < 3; i = i + 1){
				tempY		= p[i].x;
				tempX		= heightSS - p[i].y + 1;
				rotP[i]		= Point(tempX, tempY);
			}
		} else {
			if (angle == 270) {
				for(int i = 0; i < 3; i = i + 1){
					tempY		= widthSS - p[i].x + 1;
					tempX		= p[i].y;
					rotP[i]		= Point(tempX, tempY);
				}    
			} else {
				if (angle == 180){
					for(int i = 0; i < 3; i = i + 1){
						tempY		= heightSS - p[i].y + 1;
						tempX		= widthSS - p[i].x + 1;
						rotP[i]		= Point(tempX, tempY);
					}          
				}
			}
		}
	}
}

void Share::getRotationSize(){
	if((angle == 0)||(angle == 180)){
		rotWidthSS	= widthSS;
		rotHeightSS	= heightSS;
	} else {
		rotWidthSS	= heightSS;
		rotHeightSS	= widthSS;
	}
}

void Share::setIntersection(int startX, int endX, int startY, int endY){
	intersection.clear();
	intersection.push_back(Point(startX,startY));
	intersection.push_back(Point(endX,startY));
	intersection.push_back(Point(startX,endY));
	intersection.push_back(Point(endX,endY));
}

string Share::getBinaryParam(string strP, string strGen){
	string binTempP, binP, binGen, binParam, binBeta, binGamma, binDelta, binTethaInt, binTethaPrec;
	
	binTempP	= Core::str2bin(strP,10,20);
	binGen		= Core::str2bin(strGen,10,20);
	binBeta		= Core::str2bin(beta,16,16);
	binGamma	= Core::str2bin(gamma,16,16);
	binDelta	= Core::str2bin(delta,16,16);
	if(index == 1){
		binP		= binTempP.substr(0,10);
	} else {
		if(index == 2){
			binP		= binTempP.substr(10,10);
		}
	}
	binParam	= binBeta + binGamma + binDelta + binGen + binP;

	return binParam;
}

void Share::embedParam(string binParam){
	int counterParam, flag;
	vector<Mat> channels(4);
	split(share,channels);

	counterParam	= 0;
	flag			= 0;

	for(int i = 0; i < height; i++){
		for(int j = 0; j < width; j++){
			int p, a;

			if(binParam[counterParam] == '0'){
				p	= 255;
				a	= 0;
			} else {
				p	= 0;
				a	= 255;
			}

			channels[0].at<uchar>(i,j) = p;
			channels[1].at<uchar>(i,j) = p;
			channels[2].at<uchar>(i,j) = p;
			channels[3].at<uchar>(i,j) = a;

			counterParam++;

			if(counterParam == binParam.length()){
				flag	= 1;
				break;
			}
		}
		if(flag){
			break;
		}
	}

	merge(channels, share);
}

void Share::setShare(string path){
	vector<Mat> tempChannels(4);
	int flagS	= 0;
	share		= imread(path + "share" + to_string(index) + ".png", IMREAD_UNCHANGED);
	if(share.data > 0){
		split(share,tempChannels);
		channels	= tempChannels;
		width 		= share.cols;
		height 		= share.rows;
		flagS		= 1;
	} else {
		cout << "\nNo share found" << endl;
	}
}

string Share::getParam(){
	string binParam;
	int counter, flag;

	binParam		= "";
	counter			= 0;
	flag			= 0;

	for(int i = 0; i < height; i++){
		for(int j = 0; j < width; j++){
			int p = channels[0].at<uchar>(i,j);
			if(p < 128){
				binParam.push_back('1');
			} else {
				binParam.push_back('0');
			}

			counter++;

			if(counter == paramLength){
				flag	= 1;
				break;
			}
		}
		if(flag){
			break;
		}
	}

	return binParam;
}

void Share::get3OP(string v1, string v2, string v3){
	mpz_t mpzV1, mpzV2, mpzV3;
	mpz_init_set_str (mpzV1, v1.c_str(), 16);
	mpz_init_set_str (mpzV2, v2.c_str(), 16);
	mpz_init_set_str (mpzV3, v3.c_str(), 16);

	mpz_class mpzX1, mpzY1, mpzX2, mpzY2, mpzX3, mpzY3;
	mpz_class mpzW(width/2);
	mpz_class mpzH(height/2);
	mpz_mod(mpzX1.get_mpz_t(), mpzV1, mpzW.get_mpz_t());
	mpz_mod(mpzY1.get_mpz_t(), mpzV1, mpzH.get_mpz_t());
	mpz_mod(mpzX2.get_mpz_t(), mpzV2, mpzW.get_mpz_t());
	mpz_mod(mpzY2.get_mpz_t(), mpzV2, mpzH.get_mpz_t());
	mpz_mod(mpzX3.get_mpz_t(), mpzV3, mpzW.get_mpz_t());
	mpz_mod(mpzY3.get_mpz_t(), mpzV3, mpzH.get_mpz_t());

	p[0]	= Point(mpzX1.get_ui() + 1, mpzY1.get_ui() + 1);
	p[1]	= Point(mpzX2.get_ui() + 1, mpzY2.get_ui() + 1);
	p[2]	= Point(mpzX3.get_ui() + 1, mpzY3.get_ui() + 1);
}

void Share::getNormal3OP(int i, int type){
	int crPoint, scPoint, thPoint;
	if(type == 1){
		crPoint = i;
		scPoint = (i + 1) % 3;
		thPoint = (i + 2) % 3;
	} else {
		crPoint = i;
		scPoint = (i + 2) % 3;
		thPoint = (i + 1) % 3;
	}
	
	nP[crPoint] = p[crPoint];
	nP[scPoint] = Point(p[scPoint].x, p[crPoint].y);
	nP[thPoint] = Point(p[crPoint].x, p[thPoint].y);
}

void Share::getShape3OP(int i, int type){
	int crPoint, scPoint, thPoint;
	if(type == 1){
		crPoint = i;
		scPoint = (i + 1) % 3;
		thPoint = (i + 2) % 3;
	} else {
		crPoint = i;
		scPoint = (i + 2) % 3;
		thPoint = (i + 1) % 3;
	}

	if((nP[scPoint].y == nP[crPoint].y)&&(nP[scPoint].x > nP[crPoint].x)&&(nP[thPoint].x == nP[crPoint].x)&&(nP[thPoint].y > nP[crPoint].y)){
		shape3OP = 1;
	} else {
		if((nP[scPoint].y == nP[crPoint].y)&&(nP[scPoint].x < nP[crPoint].x)&&(nP[thPoint].x == nP[crPoint].x)&&(nP[thPoint].y > nP[crPoint].y)){
			shape3OP = 2;
		} else {
			if((nP[scPoint].y == nP[crPoint].y)&&(nP[scPoint].x < nP[crPoint].x)&&(nP[thPoint].x == nP[crPoint].x)&&(nP[thPoint].y < nP[crPoint].y)){
				shape3OP = 3;
			} else {
				if((nP[scPoint].y == nP[crPoint].y)&&(nP[scPoint].x > nP[crPoint].x)&&(nP[thPoint].x == nP[crPoint].x)&&(nP[thPoint].y < nP[crPoint].y)){
					shape3OP = 4;
				}
			}
		}
	}
}

void Share::shape2angle(int opositeShape){
	int diff;

	if(opositeShape == shape3OP){
		angle = 0;
	} else {
		diff = opositeShape - shape3OP;
		if(diff>0){
			angle = diff * 90;
		} else {
			// to produce positive angle
			angle = 360 + (diff * 90);
		}
	}
}

void Share::getRotationNormalPoints(){
	int tempY, tempX;
	Point2i co;

	if (angle == 0){
		rotNP[0] = nP[0];
		rotNP[1] = nP[1];
		rotNP[2] = nP[2];
	} else {
		if (angle == 90){
			for(int i = 0; i < 3; i = i + 1){
				tempY		= nP[i].x;
				tempX		= (height/2) - nP[i].y + 1;
				rotNP[i]	= Point(tempX, tempY);
			}
		} else {
			if (angle == 270) {
				for(int i = 0; i < 3; i = i + 1){
					tempY		= (width/2) - nP[i].x + 1;
					tempX		= nP[i].y;
					rotNP[i]	= Point(tempX, tempY);
				}    
			} else {
				if (angle == 180){
					for(int i = 0; i < 3; i = i + 1){
						tempY		= (height/2) - nP[i].y + 1;
						tempX		= (width/2) - nP[i].x + 1;
						rotNP[i]	= Point(tempX, tempY);
					}          
				}
			}
		}
	}
}