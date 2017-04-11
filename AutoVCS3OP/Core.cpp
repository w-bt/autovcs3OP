#include "stdafx.h"
#include "Core.h"
#include "Share.h"
#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <math.h>
#include <direct.h>
#include <time.h>
#include <io.h>
#include <set>
#include <algorithm>    // std::shuffle, sort
#include <array>        // std::array
#include <random>       // std::default_random_engine
#include <chrono>       // std::chrono::system_clock
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <thread>
#include <fstream>
#include <sstream>
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

int M0[2][4] = {
		{255, 0, 255, 0},
		{255, 0, 255, 0}
	};

int M1[2][4] = {
		{255, 0, 255, 0},
		{0, 255, 0, 255}
	};

array<int,4> pos = {0,1,2,3};

Core::Core(){
	reset();

	_mkdir("result/");
	_mkdir("result/encoding/");
	_mkdir("result/decoding/");
}

void Core::reset(){
	widthSI		= 0;
	heightSI	= 0;
	widthCanvas = 0;
	heightCanvas= 0;
	p			= "";
	q			= "";
	g			= "";
	h			= "";
	v1			= "";
	v2			= "";
	v3			= "";
	SI.release();
	filteredSI.release();
	canvas.release();
}

void Core::centerString(char* s)
{
	int l 	= strlen(s);
   	int pos = (int)((80-l)/2);
   	for(int i = 0; i < pos; i++)
    	cout << " ";

   	cout << s;
}

int Core::viewMainMenu(){
	system("cls");
	int ans = 0;

	cout << "================================================================================" << endl;
	cout << "||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||" << endl;
	cout << "================================================================================" << endl;
	cout << "\n";
	centerString("Option :");
	cout << "\n\n" << endl;
	centerString("(1) Encoding");
	cout << "\n" << endl;
	centerString("(2) Decoding");
	cout << "\n" << endl;
	centerString("(3) Exit");
	cout << "\n\n" << endl;
	cout << "================================================================================" << endl;
	cout << "||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||" << endl;
	cout << "================================================================================" << endl;
	while((ans != 1)&&(ans != 2)&&(ans != 3)){
		cout << "\n> Your choice (1/2/3) ? ";
		cin >> ans;
		cin.ignore (std::numeric_limits<std::streamsize>::max(), '\n');
	}

	return ans;
}

void Core::viewEncoding(){
	system("cls");
	// *************************************************************************************
	// ******************************** PRE-DECOMPOSING ************************************
	// *************************************************************************************
	// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	cout << "*******************************************************************************" << endl;
	cout << "***************************** PRE-DECOMPOSING *********************************" << endl;
	cout << "*******************************************************************************" << endl;
	// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	// =====================================================================================
	// ============================== DETERMINING SECRET IMAGE =============================
	// =====================================================================================
	// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	cout << "\n===============================================================================" << endl;
	cout << "========================== DETERMINING SECRET IMAGE ===========================" << endl;
	cout << "===============================================================================" << endl;
	// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	setSecretImagePath();
	// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	// =====================================================================================

	// =====================================================================================
	// =============================== DETERMINING SS SIZE =================================
	// =====================================================================================
	// determining the size of shadow share
	// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	cout << "\n===============================================================================" << endl;
	cout << "========================== DETERMINING SS SIZE ================================" << endl;
	cout << "===============================================================================" << endl;
	// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	Share S1(1);
	Share S2(2);
	setShadowSharePath(S1, S2);
	setChameleonVariable();
	string type;
	vector<string> fileList;
	for(int i = 0; i < ssSize.size(); i++){
		dur1.push_back(0);
		dur2.push_back(0);
		dur3.push_back(0);
		dur4.push_back(0);

		setSSSize(S1,S2,ssSize[i]);
		if(siPath.size() == 1){
			singlePath	= siPath[0];
			type		= "1";
		} else {
			singlePath	= siPath[i];
			if(i == 0){
				if(ssSize[0] == ssSize[1]){
					type		= "3";
				} else {
					type		= "2";
				}
			} else {
				if(ssSize[i] == ssSize[i-1]){
					type		= "3";
				} else {
					type		= "2";
				}
			}
		}
		if((S1.widthSS == S2.widthSS)&&(S1.heightSS == S2.heightSS)){
			scheme 		= "imsvcs";
		} else {
			scheme 		= "nimsvcs";
		}
		setSecretImage(singlePath);

		// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
		S2.setAvailableOrientation(widthSI, heightSI);

		if(S2.availableOrientation.size() == 4){
			cout << "\nSS2's available orientation = " << S2.availableOrientation[0] << ", " << S2.availableOrientation[1] << ", " << S2.availableOrientation[2] << ", and " << S2.availableOrientation[3] << endl;
		} else {
			cout << "\nSS2's available orientation = " << S2.availableOrientation[0] << " and " << S2.availableOrientation[1] << endl;
		}
		// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
		// =====================================================================================

		// =====================================================================================
		// ================================ GENERATING 3OP =====================================
		// =====================================================================================
		// generating share
		// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
		cout << "\n===============================================================================" << endl;
		cout << "================================ GENERATING 3OP ===============================" << endl;
		cout << "===============================================================================" << endl;
		// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
		cout << "\n\t[p] : " << p << endl;
		cout << "\t[q] : " << q << endl;
		cout << "\t[g] : " << g << endl; // a = 130814
		cout << "\t[h] : " << h << endl; // x = 26
		// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

		cout << "\n> Generating 3OP" << endl;
		int flagValid = 0;
		while(flagValid == 0){
			clock_t tStart1	= clock();
			genPoints(S1, S2);
			double duration1	= (double)(clock() - tStart1)/CLOCKS_PER_SEC;
			int size1			= dur1.size();
			dur1[size1-1]		= duration1;
			int flagBreak	= 0;
			for(int i = 0; i < 3; i++){
				for(int j = 0; j < 3; j++){
					for(int k = 1; k < 3; k++){
						S1.getNormal3OP(i, k);
						S1.getShape3OP(i, k);
						for(int l = 1; l < 3; l++){
							S2.getNormal3OP(j, l);
							S2.getShape3OP(j, l);

							S2.shape2angle(S1.shape3OP);
							S2.revAngle	= (360 - S2.angle) % 360;

							S2.getRotationPoints();
							S2.getRotationSize();
							getIntersectionProp(S1, S2, i, j);
							if((widthIA >= widthSI)&&(heightIA >= heightSI)){
								flagBreak = 1;
								flagValid = 1;
								break;
							}
						}
						if(flagBreak == 1){
							break;
						}
					}
					if(flagBreak == 1){
						break;
					}
				}
				if(flagBreak == 1){
					break;
				}
			}
		}

		cout << "\nCoordinates of 3OP in SS1 : " << endl;
		cout << "\t[1] (" << S1.p[0].x << "," << S1.p[0].y << ")" << endl;
		cout << "\t[2] (" << S1.p[1].x << "," << S1.p[1].y << ")" << endl;
		cout << "\t[3] (" << S1.p[2].x << "," << S1.p[2].y << ")" << endl;

		cout << "\nCoordinates of 3OP in SS2 : " << endl;
		cout << "\t[1] (" << S2.p[0].x << "," << S2.p[0].y << ")" << endl;
		cout << "\t[2] (" << S2.p[1].x << "," << S2.p[1].y << ")" << endl;
		cout << "\t[3] (" << S2.p[2].x << "," << S2.p[2].y << ")" << endl;

		cout << "\nAngle rotation of SS2 : " << S2.angle << endl;

		cout << "\nSeeds for S1 : " << endl;
		cout << "\n\t[beta] : " << S1.beta << endl;
		cout << "\t[gamma] : " << S1.gamma << endl;
		cout << "\t[delta] : " << S1.delta << endl;

		cout << "\nSeeds for S2 : " << endl;
		cout << "\n\t[beta] : " << S2.beta << endl;
		cout << "\t[gamma] : " << S2.gamma << endl;
		cout << "\t[delta] : " << S2.delta << endl;

		cout << "\nIntersection size " << heightIA << " x " << widthIA << endl;
		cout << "\nIntersection area for SS1 : " << endl;
		cout << "\t[1] (" << S1.intersection[0].x << "," << S1.intersection[0].y << ")" << endl;
		cout << "\t[2] (" << S1.intersection[1].x << "," << S1.intersection[1].y << ")" << endl;
		cout << "\t[3] (" << S1.intersection[2].x << "," << S1.intersection[2].y << ")" << endl;
		cout << "\t[4] (" << S1.intersection[3].x << "," << S1.intersection[3].y << ")" << endl;
		cout << "\nIntersection area for rotated SS2 : " << endl;
		cout << "\t[1] (" << S2.intersection[0].x << "," << S2.intersection[0].y << ")" << endl;
		cout << "\t[2] (" << S2.intersection[1].x << "," << S2.intersection[1].y << ")" << endl;
		cout << "\t[3] (" << S2.intersection[2].x << "," << S2.intersection[2].y << ")" << endl;
		cout << "\t[4] (" << S2.intersection[3].x << "," << S2.intersection[3].y << ")" << endl;

		// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
		// =====================================================================================

		// =====================================================================================
		// ============================ GENERATE RANDOM SI POSITION ============================
		// =====================================================================================
		// generating random SI position
		// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
		cout << "\n===============================================================================" << endl;
		cout << "======================== GENERATE RANDOM SI POSITION ==========================" << endl;
		cout << "===============================================================================" << endl;
		// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
		cout << "\n> Generate random SI position" << endl;

		randomSIPosition(S1, S2);

		cout << "\nSI position in SS1:" << endl;
		cout << "\t[A] (" << S1.position[0].x << "," << S1.position[0].y << ")" << endl;
		cout << "\t[B] (" << S1.position[1].x << "," << S1.position[1].y << ")" << endl;
		cout << "\t[C] (" << S1.position[2].x << "," << S1.position[2].y << ")" << endl;
		cout << "\t[D] (" << S1.position[3].x << "," << S1.position[3].y << ")" << endl;

		cout << "\nSI position in rotated SS2:" << endl;
		cout << "\t[A] (" << S2.position[0].x << "," << S2.position[0].y << ")" << endl;
		cout << "\t[B] (" << S2.position[1].x << "," << S2.position[1].y << ")" << endl;
		cout << "\t[C] (" << S2.position[2].x << "," << S2.position[2].y << ")" << endl;
		cout << "\t[D] (" << S2.position[3].x << "," << S2.position[3].y << ")" << endl;

		// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
		// =====================================================================================

		// *************************************************************************************
		// *********************************** DECOMPOSING *************************************
		// *************************************************************************************
		// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
		cout << "\n*******************************************************************************" << endl;
		cout << "******************************** DECOMPOSING **********************************" << endl;
		cout << "*******************************************************************************" << endl;
		// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

		// =====================================================================================
		// =============================== FILTERING SECRET IMAGE ==============================
		// =====================================================================================
		// filtering secret image
		// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
		cout << "\n===============================================================================" << endl;
		cout << "========================== FILTERING SECRET IMAGE =============================" << endl;
		cout << "===============================================================================" << endl;
		// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
		cout << "\n> Filtering secret image" << endl;
		clock_t tStart2	= clock();
		filter();
		double duration2		= (double)(clock() - tStart2)/CLOCKS_PER_SEC;
		int size2 = dur2.size();
		dur2[size2-1] = duration2;
		// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
		// =====================================================================================

		// =====================================================================================
		// ================================ GENERATING SHARE =================================
		// =====================================================================================
		// generating share
		// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
		cout << "\n===============================================================================" << endl;
		cout << "============================== GENERATING SHARE ===============================" << endl;
		cout << "===============================================================================" << endl;
		// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
		cout << "\n> Generating shares" << endl;
		clock_t tStart3	= clock();
		generateShare(S1, S2);
		double duration3		= (double)(clock() - tStart3)/CLOCKS_PER_SEC;
		int size3 = dur3.size();
		dur3[size3-1] = duration3;
		// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
		cout << "\n> Rotating share 2 to normal orientation" << endl;
		getNormalRotationImage(S2);
		// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
		// =====================================================================================

		// *************************************************************************************
		// ******************************** POST DECOMPOSING ***********************************
		// *************************************************************************************
		// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
		cout << "\n*******************************************************************************" << endl;
		cout << "***************************** POST DECOMPOSING ********************************" << endl;
		cout << "*******************************************************************************" << endl;
		// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

		// =====================================================================================
		// =============================== EMBEDDING PARAMETER =================================
		// =====================================================================================
		// embedding parameter
		// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
		cout << "\n===============================================================================" << endl;
		cout << "=========================== EMBEDDING PARAMETER ===============================" << endl;
		cout << "===============================================================================" << endl;
		// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
		string binParam1, binParam2;
		cout << "\n> Embedding parameter to S1" << endl;
		binParam1	= S1.getBinaryParam(p, g);
		clock_t tStart41	= clock();
		S1.embedParam(binParam1);
		double duration41		= (double)(clock() - tStart41)/CLOCKS_PER_SEC;
		// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
		cout << "\n> Embedding parameter to S2" << endl;
		binParam2	= S2.getBinaryParam(p, h);
		clock_t tStart42	= clock();
		S2.embedParam(binParam2);
		double duration42		= (double)(clock() - tStart42)/CLOCKS_PER_SEC;
		// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
		double duration4		= duration41 + duration42;
		int size4 = dur4.size();
		dur4[size4-1] = duration4;
		string folder;
		if(scheme == "imsvcs"){
			folder			= "result/encoding/imsvcs/";
			_mkdir(folder.c_str());
			folder			= folder + ssSize[i] + "/";
			_mkdir(folder.c_str());
			fileList.push_back(folder);
			imwrite(folder + "share1.png",S1.share);
			imwrite(folder + "share2.png",S2.share);
		} else {
			folder			= "result/encoding/nimsvcs/";
			_mkdir(folder.c_str());
			folder			= folder + type + "/";
			_mkdir(folder.c_str());
			if(type == "3"){
				folder			= folder + "s" + to_string(i) + "." + ssSize[i] + "/";
			} else {
				folder			= folder + ssSize[i] + "/";
			}
			_mkdir(folder.c_str());
			fileList.push_back(folder);
			imwrite(folder + "share1.png",S1.share);
			imwrite(folder + "share2.png",S2.share);
		}
		fstream file;
		file.open(folder + "info2.txt",fstream::out);
		file << "name : " << ssSize[i] << endl;

		file << "\nCoordinates of 3OP in SS1 : " << endl;
		file << "\t[1] (" << S1.p[0].x << "," << S1.p[0].y << ")" << endl;
		file << "\t[2] (" << S1.p[1].x << "," << S1.p[1].y << ")" << endl;
		file << "\t[3] (" << S1.p[2].x << "," << S1.p[2].y << ")" << endl;

		file << "\nCoordinates of 3OP in SS2 : " << endl;
		file << "\t[1] (" << S2.p[0].x << "," << S2.p[0].y << ")" << endl;
		file << "\t[2] (" << S2.p[1].x << "," << S2.p[1].y << ")" << endl;
		file << "\t[3] (" << S2.p[2].x << "," << S2.p[2].y << ")" << endl;

		file << "\nCoordinates of Normalized-3OP in SS1 : " << endl;
		file << "\t[1] (" << S1.nP[0].x << "," << S1.nP[0].y << ")" << endl;
		file << "\t[2] (" << S1.nP[1].x << "," << S1.nP[1].y << ")" << endl;
		file << "\t[3] (" << S1.nP[2].x << "," << S1.nP[2].y << ")" << endl;

		file << "\nCoordinates of Normalized-3OP in SS2 : " << endl;
		file << "\t[1] (" << S2.nP[0].x << "," << S2.nP[0].y << ")" << endl;
		file << "\t[2] (" << S2.nP[1].x << "," << S2.nP[1].y << ")" << endl;
		file << "\t[3] (" << S2.nP[2].x << "," << S2.nP[2].y << ")" << endl;

		file << "\nAngle rotation of SS2 : " << S2.angle << endl;

		file << "\nSeeds for S1 : " << endl;
		file << "\n\t[beta] : " << S1.beta << endl;
		file << "\t[gamma] : " << S1.gamma << endl;
		file << "\t[delta] : " << S1.delta << endl;

		file << "\nSeeds for S2 : " << endl;
		file << "\n\t[beta] : " << S2.beta << endl;
		file << "\t[gamma] : " << S2.gamma << endl;
		file << "\t[delta] : " << S2.delta << endl;

		file << "\nIntersection size " << (S1.intersection[2].y - S1.intersection[0].y) << " x " << (S1.intersection[1].x - S1.intersection[0].x) << endl;
		file << "\nIntersection area for SS1 : " << endl;
		file << "\t[1] (" << S1.intersection[0].x << "," << S1.intersection[0].y << ")" << endl;
		file << "\t[2] (" << S1.intersection[1].x << "," << S1.intersection[1].y << ")" << endl;
		file << "\t[3] (" << S1.intersection[2].x << "," << S1.intersection[2].y << ")" << endl;
		file << "\t[4] (" << S1.intersection[3].x << "," << S1.intersection[3].y << ")" << endl;

		file << "\nIntersection area for rotated SS2 : " << endl;
		file << "\t[1] (" << S2.intersection[0].x << "," << S2.intersection[0].y << ")" << endl;
		file << "\t[2] (" << S2.intersection[1].x << "," << S2.intersection[1].y << ")" << endl;
		file << "\t[3] (" << S2.intersection[2].x << "," << S2.intersection[2].y << ")" << endl;
		file << "\t[4] (" << S2.intersection[3].x << "," << S2.intersection[3].y << ")" << endl;

		file << "\nSI position in SS1:" << endl;
		file << "\t[A] (" << S1.position[0].x << "," << S1.position[0].y << ")" << endl;
		file << "\t[B] (" << S1.position[1].x << "," << S1.position[1].y << ")" << endl;
		file << "\t[C] (" << S1.position[2].x << "," << S1.position[2].y << ")" << endl;
		file << "\t[D] (" << S1.position[3].x << "," << S1.position[3].y << ")" << endl;

		file << "\nSI position in rotated SS2:" << endl;
		file << "\t[A] (" << S2.position[0].x << "," << S2.position[0].y << ")" << endl;
		file << "\t[B] (" << S2.position[1].x << "," << S2.position[1].y << ")" << endl;
		file << "\t[C] (" << S2.position[2].x << "," << S2.position[2].y << ")" << endl;
		file << "\t[D] (" << S2.position[3].x << "," << S2.position[3].y << ")" << endl;
		file.close();
		/*if(scheme == "nimsvcs"){
			// eksperimen tambahan
			S1.getNormal3OP();
			S2.getNormal3OP();
			S1.width 		= S1.widthSS * 2;
			S1.height 	= S1.heightSS * 2;
			S2.width 		= S2.widthSS * 2;
			S2.height 	= S2.heightSS * 2;
			if((S2.angle == 90)||(S2.angle == 270)){
				S2.rotWidth 	= S2.height;
				S2.rotHeight 	= S2.width;
			} else {
				S2.rotWidth 	= S2.width;
				S2.rotHeight 	= S2.height;
			}
			S2.getRotationNormalPoints();
			getCanvas(S1, S2);
			// end eksperimen tambahan
			file.open(folder + "info.txt",fstream::out);
			file << "name : " << S1.widthSS << "x" << S1.heightSS << "&" << S2.widthSS << "x" << S2.heightSS << endl;
			if(ext == "bmp"){
				if(type == "1"){
					file << "secret image : si/bw/nimsvcs1/" << heightSI << "x" << widthSI << ".bmp" << endl;
				} else {
					file << "secret image : si/bw/nimsvcs2/" << heightSI << "x" << widthSI << ".bmp" << endl;
				}
			} else {
				if(type == "1"){
					file << "secret image : si/gray/nimsvcs1/" << heightSI << "x" << widthSI << ".png" << endl;
				} else {
					file << "secret image : si/gray/nimsvcs2/" << heightSI << "x" << widthSI << ".png" << endl;
				}
			}
			file << "duration : " << duration << " seconds" << endl;
			file << "\nS2 rotation : " << S2.angle << endl;

			file << "\nIntersection size " << (S1.intersection[2].y - S1.intersection[0].y) << " x " << (S1.intersection[1].x - S1.intersection[0].x) << endl;
			file << "\nIntersection area for SS1 : " << endl;
			file << "\t[1] (" << S1.intersection[0].x << "," << S1.intersection[0].y << ")" << endl;
			file << "\t[2] (" << S1.intersection[1].x << "," << S1.intersection[1].y << ")" << endl;
			file << "\t[3] (" << S1.intersection[2].x << "," << S1.intersection[2].y << ")" << endl;
			file << "\t[4] (" << S1.intersection[3].x << "," << S1.intersection[3].y << ")" << endl;
			file << "\nIntersection area for rotated SS2 : " << endl;
			file << "\t[1] (" << S2.intersection[0].x << "," << S2.intersection[0].y << ")" << endl;
			file << "\t[2] (" << S2.intersection[1].x << "," << S2.intersection[1].y << ")" << endl;
			file << "\t[3] (" << S2.intersection[2].x << "," << S2.intersection[2].y << ")" << endl;
			file << "\t[4] (" << S2.intersection[3].x << "," << S2.intersection[3].y << ")" << endl;

			file << "\nSI position in SS1:" << endl;
			file << "\t[A] (" << S1.position[0].x << "," << S1.position[0].y << ")" << endl;
			file << "\t[B] (" << S1.position[1].x << "," << S1.position[1].y << ")" << endl;
			file << "\t[C] (" << S1.position[2].x << "," << S1.position[2].y << ")" << endl;
			file << "\t[D] (" << S1.position[3].x << "," << S1.position[3].y << ")" << endl;
			file << "\nSI position in rotated SS2:" << endl;
			file << "\t[A] (" << S2.position[0].x << "," << S2.position[0].y << ")" << endl;
			file << "\t[B] (" << S2.position[1].x << "," << S2.position[1].y << ")" << endl;
			file << "\t[C] (" << S2.position[2].x << "," << S2.position[2].y << ")" << endl;
			file << "\t[D] (" << S2.position[3].x << "," << S2.position[3].y << ")" << endl;

			file << "\nS1 position : (" << (S1.pointStart.x * 2) << "," << (S1.pointStart.y * 2) << ")" << endl;
			file << "S2 position : (" << (S2.pointStart.x * 2) << "," << (S2.pointStart.y * 2) << ")" << endl;
			file.close();

			S1.share.release();
			S2.share.release();
			S2.rotatedShare.release();
		}*/
	}
	fstream file;
	file.open("result/encoding/info.txt",fstream::out);
	for(int i = 0; i < fileList.size(); i++){
		file << fileList[i] << endl;
	}
	file.close();
	fileList.clear();
	file.open("result/encoding/durencode.csv",fstream::out);
	file << "no,3OP,filtering,decomposing,embedding" << endl;
	for(int i = 0; i < dur1.size(); i++){
		file << (i+1) << "," << fixed << setprecision(5) << dur1[i] << "," << fixed << setprecision(5) << dur2[i] << "," << fixed << setprecision(5)<< dur3[i] << "," << fixed << setprecision(5) << dur4[i] << endl;
	}

	file.close();
	dur1.clear();
	dur2.clear();
	dur3.clear();
	dur4.clear();
	cout << "\n> Finished" << endl;
	cout << "\n" ;
	system("pause");
	// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	// =====================================================================================
	int opt = viewMainMenu();
	S1.~Share();
	S2.~Share();
	reset();
	if(opt == 1){
		viewEncoding();
	} else {
		if(opt == 2){
			viewDecoding();
		} else {
			exit(0);
		}
	}
}

/*void Core::setSecretImagePath(){
	vector<string> names;
	string pathSI;
	int flagSI	= 0;
	Mat tempSI;
	cout << "\n> Input SI's path : ";
	getline (cin, pathSI);
	string path		= pathSI + "/*.*";
	_finddata_t data;
    int ff = _findfirst (path.c_str(), &data);
    if (ff != -1)
    {
        int res = 0;
		siPath.clear();
		siName.clear();
        while (res != -1)
        {
			if((data.attrib & _A_SUBDIR) != _A_SUBDIR){
				siPath.push_back(pathSI + "/" + data.name);
				siName.push_back(data.name);
			}
            res = _findnext(ff, &data);
        }
        _findclose(ff);
    }
}*/

void Core::setSecretImagePath(){
	string pathSI;
	cout << "\n> Input SI's path : ";
	getline (cin, pathSI);

    ifstream file;
	file.open(pathSI);
	while(!file){
		cout << "\nNo file found" << endl;
		cout << "\n> Input SI's path : ";
		cin >> pathSI;
		file.open(pathSI);
	}
	vector<string> tempPath = splitString(pathSI,'/');
	string basePath 		= "";
	for(int i = 0; i < (tempPath.size() - 1); i++){
		basePath = basePath + tempPath[i] + "/";
	}
    string str;
	int no	= 1;
	siPath.clear();
	siName.clear();
    while (getline(file, str))
    {
    	siPath.push_back(basePath + str);
		siName.push_back(str);
    }
}

void Core::setSecretImage(string pathSI)
{
	Mat tempSI;
	ext 		= pathSI.substr(pathSI.size() - 3);
	tempSI		= imread(pathSI,0);
	SI			= tempSI;
	widthSI		= tempSI.cols;
	heightSI	= tempSI.rows;
	cout << "\nSecret Image's size : " << heightSI << "x" << widthSI << endl;
}

void Core::setShadowSharePath(Share& S1, Share& S2){
	char pathSShare[100];
	cout << "\n> Input Shadow Share's path : ";
	cin >> pathSShare;
	ifstream file;
	file.open(pathSShare);
	while(!file){
		cout << "\nNo file found" << endl;
		cout << "\n> Input Shadow Share's path : ";
		cin >> pathSShare;
		file.open(pathSShare);
	}
    string str;
	int no	= 1;
	ssSize.clear();
    while (getline(file, str))
    {
		ssSize.push_back(str);
    }
}

void Core::setSSSize(Share& S1, Share& S2, string size){
	vector<string> tempSize = splitString(size, '&');
	vector<string> tempSize1 = splitString(tempSize[0], 'x');
	stringstream(tempSize1[0]) >> S1.heightSS;
	S1.height		= S1.heightSS * 2;
	stringstream(tempSize1[1]) >> S1.widthSS;
	S1.width		= S1.widthSS * 2;
	cout << "\nSS1's size : " << S1.heightSS << "x" << S1.widthSS << endl;
	cout << "\nS1's size : " << S1.height << "x" << S1.width << endl;
	vector<string> tempSize2 = splitString(tempSize[1], 'x');
	stringstream(tempSize2[0]) >> S2.heightSS;
	S2.height		= S2.heightSS * 2;
	stringstream(tempSize2[1]) >> S2.widthSS;
	S2.width		= S2.widthSS * 2;
	cout << "\nSS2's size : " << S2.heightSS << "x" << S2.widthSS << endl;
	cout << "\nS2's size : " << S2.height << "x" << S2.width << endl;
}

void Core::setChameleonVariable()
{
	char pathCham[100];
	cout << "\n> Input Chameleon File's path : ";
	cin >> pathCham;
	ifstream file;
	file.open(pathCham);
	while(!file){
		cout << "\nNo file found" << endl;
		cout << "\n> Input Chameleon File's path : ";
		cin >> pathCham;
		file.open(pathCham);
	}
    string str;
	int no	= 1;
    while (getline(file, str))
    {
        if(no == 1){
			p	= str;
		} else {
			if(no == 2){
				q	= str;
			} else {
				if(no == 3){
					g	= str;
				} else {
					if(no == 4){
						h	= str;
					}
				}
			}
		}
		no	= no + 1;
    }
}

int Core::getRandomNumber(){
	int result = rand();

	return result;
}

string Core::getChameleonHashValue(string seed1, string seed2){
	mpz_class s1, s2;
	mpz_init_set_str (s1.get_mpz_t(), seed1.c_str(), 16);
	mpz_init_set_str (s2.get_mpz_t(), seed2.c_str(), 16);

	mpz_class mpzP(p);
	mpz_class mpzG(g);
	mpz_class mpzH(h);

	mpz_class result, gResult, hResult;

	mpz_powm(gResult.get_mpz_t(),mpzG.get_mpz_t(),s1.get_mpz_t(),mpzP.get_mpz_t());
	mpz_powm(hResult.get_mpz_t(),mpzH.get_mpz_t(),s2.get_mpz_t(),mpzP.get_mpz_t());

	mpz_mul(result.get_mpz_t(), gResult.get_mpz_t(), hResult.get_mpz_t());
	mpz_mod(result.get_mpz_t(), result.get_mpz_t(), mpzP.get_mpz_t());

	string v = mpz_get_str(NULL,16,result.get_mpz_t());

	return v;
}

string Core::getSeed(int index, Share& S){
	int temp	= getRandomNumber();

	mpz_t s;
	mpz_class mpzQ;
    gmp_randstate_t r_state;

    gmp_randinit_default (r_state);
    gmp_randseed_ui(r_state, temp);
	mpz_init_set_str (mpzQ.get_mpz_t(), q.c_str(), 10);

    mpz_init(s);

    mpz_urandomm(s,r_state,mpzQ.get_mpz_t());

	string v = mpz_get_str(NULL,16,s);

	return v;
}

void Core::genPoints(Share& S1, Share& S2){
	int flag3, counter;

	genFirstPoint(S1, S2);
	genSecondPoint(S1, S2);
	genThirdPoint(S1, S2);

	/*flag3			= 1;
	counter			= 0;
	while(flag3 == 1){
		counter++;
		cout << "\nGenerate 3OP round " << counter << endl;
		int flag1			= genFirstPoint(S1, S2);
		if(flag1 != 0){
			vector<int>	flag2	= genSecondPoint(S1, S2, flag1);
			if(flag2.size() > 0){
				clock_t tStart1	= clock();
				flag3				= genThirdPoint(S1, S2, flag1, flag2);
				if(flag3 == 0){
					double duration1		= (double)(clock() - tStart1)/CLOCKS_PER_SEC;
					int size1 = dur1.size();
					dur1[size1-1] = duration1;
				}
			}
		}
	}*/
}

void Core::genFirstPoint(Share& S1, Share& S2){
	string seed1, seed2;

	seed1	= getSeed(1, S1);
	seed2	= getSeed(1, S2);
	mpz_class mpzBeta1;
	mpz_class mpzBeta2;
	mpz_init_set_str (mpzBeta1.get_mpz_t(), seed1.c_str(), 16);
	mpz_init_set_str (mpzBeta2.get_mpz_t(), seed2.c_str(), 16);
	S1.beta  = mpz_get_str(NULL,16,mpzBeta1.get_mpz_t());
	S2.beta  = mpz_get_str(NULL,16,mpzBeta2.get_mpz_t());

	v1		= getChameleonHashValue(seed1, seed2);

	mpz_t mpzV;
	mpz_init_set_str (mpzV, v1.c_str(), 16);

	mpz_class mpzX1, mpzY1, mpzX2, mpzY2;
	mpz_class mpzW1(S1.widthSS);
	mpz_class mpzH1(S1.heightSS);
	mpz_class mpzW2(S2.widthSS);
	mpz_class mpzH2(S2.heightSS);
	mpz_mod(mpzX1.get_mpz_t(), mpzV, mpzW1.get_mpz_t());
	mpz_mod(mpzY1.get_mpz_t(), mpzV, mpzH1.get_mpz_t());
	mpz_mod(mpzX2.get_mpz_t(), mpzV, mpzW2.get_mpz_t());
	mpz_mod(mpzY2.get_mpz_t(), mpzV, mpzH2.get_mpz_t());

	S1.p[0]	= Point(mpzX1.get_ui() + 1, mpzY1.get_ui() + 1);
	S2.p[0]	= Point(mpzX2.get_ui() + 1, mpzY2.get_ui() + 1);

	/*int round		= 0;
	int flag		= 0;
	mpz_class mpzArea1(S1.widthSS * S1.heightSS);
	mpz_class mpzArea2(S2.widthSS * S2.heightSS);
	while(flag == 0){
		round++;
		string seed1, seed2;

		seed1	= getSeed(1, S1);
		seed2	= getSeed(1, S2);
		v1		= getChameleonHashValue(seed1, seed2);

		

		mpz_t mpzV;
		mpz_init_set_str (mpzV, v1.c_str(), 16);

		mpz_class mpzV1, mpzV2;
		mpz_mod(mpzV1.get_mpz_t(), mpzV, mpzArea1.get_mpz_t());
		mpz_mod(mpzV2.get_mpz_t(), mpzV, mpzArea2.get_mpz_t());

		S1.i[0]	= mpzV1.get_ui() + 1;
		S2.i[0]	= mpzV2.get_ui() + 1;

		S1.p[0] = index2point(S1.i[0], S1.widthSS);
		S2.p[0] = index2point(S2.i[0], S2.widthSS);

		vector<int>	position1 = getPosition(S1);
		vector<int>	position2 = getPosition(S2);

		if(S2.availableOrientation.size() == 4){
			flag = checkFlag2(position1, position2);
			if(flag == 0){
				flag = checkFlag3(position1, position2);
			}
		} else {
			if((S2.availableOrientation[0] == 0)&&(S2.availableOrientation[1] == 2)){
				flag = checkFlag2(position1, position2);
			} else {
				if((S2.availableOrientation[0] == 1)&&(S2.availableOrientation[1] == 3)){
					flag = checkFlag3(position1, position2);
				}
			}
		}
		if(round == 100){
			break;
		}
	}*/
}

Point2i Core::index2point(int index, int width){
	Point2i result;
	int x = (index % width) == 0 ? width : index % width;
	int y = (index % width) == 0 ? (index/width) : (index/width) + 1;

	result = Point(x,y);
	return result;
}

vector<int>	Core::getPosition(Share S){
	vector<int> result;

	result.clear();
	result.push_back(S.p[0].x);
	result.push_back(S.widthSS - S.p[0].x);
	result.push_back(S.p[0].y);
	result.push_back(S.heightSS - S.p[0].y);

	return result;
}

vector<int>	Core::getMinDistance(int a1, int a2, int b1, int b2, int c1, int c2, int d1, int d2){
	vector<int> result;

	//left
	if(a1 < a2){
		result.push_back(a1);
	} else {
		result.push_back(a2);
	}

	//right
	if(b1 < b2){
		result.push_back(b1);
	} else {
		result.push_back(b2);
	}

	//top
	if(c1 < c2){
		result.push_back(c1);
	} else {
		result.push_back(c2);
	}

	//bottom
	if(d1 < d2){
		result.push_back(d1);
	} else {
		result.push_back(d2);
	}

	return result;
}

int	Core::checkFlag2(vector<int> position1, vector<int> position2){
	int flag0 = 0;
	int flag2 = 0;

	vector<int> minDistance1	= getMinDistance(position1[0], position2[0], position1[1], position2[1], position1[2], position2[2], position1[3], position2[3]);
	if(((minDistance1[0]+minDistance1[1]) >= widthSI)&&((minDistance1[2]+minDistance1[3]) >= heightSI)){
		flag0 = 1;
	}

	vector<int> minDistance2	= getMinDistance(position1[0], position2[1] - 1, position1[1], position2[0] + 1, position1[2], position2[3] - 1, position1[3], position2[2] + 1);
	if(((minDistance2[0]+minDistance2[1]) >= widthSI)&&((minDistance2[2]+minDistance2[3]) >= heightSI)){
		flag2 = 1;
	}

	int flag = 0;
	if((flag0 == 1) && (flag2 == 1)){
		flag = 1;
	} else {
		if(flag0 == 1){
			flag = 2;
		} else {
			if(flag2 == 1){
				flag = 3;
			}
		}
	}

	return flag;
}

int	Core::checkFlag3(vector<int> position1, vector<int> position2){
	int flag1 = 0;
	int flag3 = 0;

	vector<int> minDistance1	= getMinDistance(position1[0], position2[2], position1[1], position2[3], position1[2], position2[1], position1[3], position2[0]);
	if(((minDistance1[0]+minDistance1[1]) >= widthSI)&&((minDistance1[2]+minDistance1[3]) >= heightSI)){
		flag1 = 1;
	}

	vector<int> minDistance2	= getMinDistance(position1[0], position2[3] - 1, position1[1], position2[2] + 1, position1[2], position2[0] - 1, position1[3], position2[1] + 1);
	if(((minDistance2[0]+minDistance2[1]) >= widthSI)&&((minDistance2[2]+minDistance2[3]) >= heightSI)){
		flag3 = 1;
	}

	int flag = 0;
	if((flag1 == 1) && (flag3 == 1)){
		flag = 4;
	} else {
		if(flag1 == 1){
			flag = 5;
		} else {
			if(flag3 == 1){
				flag = 6;
			}
		}
	}

	return flag;
}

void	Core::genSecondPoint(Share& S1, Share& S2){
	string seed1, seed2;
	int flag	= 0;

	while(flag == 0){
		seed1	= getSeed(1, S1);
		seed2	= getSeed(1, S2);
		mpz_class mpzGamma1;
		mpz_class mpzGamma2;
		mpz_init_set_str (mpzGamma1.get_mpz_t(), seed1.c_str(), 16);
		mpz_init_set_str (mpzGamma2.get_mpz_t(), seed2.c_str(), 16);
		S1.gamma  = mpz_get_str(NULL,16,mpzGamma1.get_mpz_t());
		S2.gamma  = mpz_get_str(NULL,16,mpzGamma2.get_mpz_t());

		v2		= getChameleonHashValue(seed1, seed2);

		mpz_t mpzV;
		mpz_init_set_str (mpzV, v2.c_str(), 16);

		mpz_class mpzX1, mpzY1, mpzX2, mpzY2;
		mpz_class mpzW1(S1.widthSS);
		mpz_class mpzH1(S1.heightSS);
		mpz_class mpzW2(S2.widthSS);
		mpz_class mpzH2(S2.heightSS);
		mpz_mod(mpzX1.get_mpz_t(), mpzV, mpzW1.get_mpz_t());
		mpz_mod(mpzY1.get_mpz_t(), mpzV, mpzH1.get_mpz_t());
		mpz_mod(mpzX2.get_mpz_t(), mpzV, mpzW2.get_mpz_t());
		mpz_mod(mpzY2.get_mpz_t(), mpzV, mpzH2.get_mpz_t());

		S1.p[1]	= Point(mpzX1.get_ui() + 1, mpzY1.get_ui() + 1);
		S2.p[1]	= Point(mpzX2.get_ui() + 1, mpzY2.get_ui() + 1);

		if((S1.p[1].x != S1.p[0].x)&&(S1.p[1].y != S1.p[0].y)&&(S2.p[1].x != S2.p[0].x)&&(S2.p[1].y != S2.p[0].y)){
			flag = 1;
		}
	}

	/*vector<int> result;
	int flag = 0;
	mpz_class mpzArea1(S1.widthSS * S1.heightSS);
	mpz_class mpzArea2(S2.widthSS * S2.heightSS);
	int round	= 0;
	while(flag == 0){
		round++;
		string seed1, seed2;

		seed1	= getSeed(2, S1);
		seed2	= getSeed(2, S2);
		v2		= getChameleonHashValue(seed1, seed2);

		mpz_class mpzBeta1;
		mpz_class mpzBeta2;
		mpz_init_set_str (mpzBeta1.get_mpz_t(), seed1.c_str(), 16);
		mpz_init_set_str (mpzBeta2.get_mpz_t(), seed2.c_str(), 16);
		S1.beta  = mpz_get_str(NULL,16,mpzBeta1.get_mpz_t());
		S2.beta  = mpz_get_str(NULL,16,mpzBeta2.get_mpz_t());

		mpz_t mpzV;
		mpz_init_set_str (mpzV, v2.c_str(), 16);

		mpz_class mpzV1, mpzV2;
		mpz_mod(mpzV1.get_mpz_t(), mpzV, mpzArea1.get_mpz_t());
		mpz_mod(mpzV2.get_mpz_t(), mpzV, mpzArea2.get_mpz_t());

		S1.i[1]	= mpzV1.get_ui() + 1;
		S2.i[1]	= mpzV2.get_ui() + 1;

		S1.p[1] = index2point(S1.i[1], S1.widthSS);
		S2.p[1] = index2point(S2.i[1], S2.widthSS);
		if((S1.p[1].x != S1.p[0].x)&&(S2.p[1].x != S2.p[0].x)){
			result.clear();
			if((flag1 == 1)||(flag1 == 4)||(flag1 == 5)||(flag1 == 6)){
				if(S1.p[1].x < S1.p[0].x){
					result.push_back(0);
				} else {
					result.push_back(1);
				}
				if(S2.p[1].x < S2.p[0].x){
					result.push_back(0);
				} else {
					result.push_back(1);
				}
				flag	= 1;
			} else {
				if(flag1 == 2){
					if((S1.p[1].x < S1.p[0].x)&&(S2.p[1].x < S2.p[0].x)){
						result.push_back(0);
						result.push_back(0);
						flag	= 1;
					}
					if((S1.p[1].x > S1.p[0].x)&&(S2.p[1].x > S2.p[0].x)){
						result.push_back(1);
						result.push_back(1);
						flag	= 1;
					}
				} else {
					if(flag1 == 3){
						if((S1.p[1].x < S1.p[0].x)&&(S2.p[1].x > S2.p[0].x)){
							result.push_back(0);
							result.push_back(1);
							flag	= 1;
						}
						if((S1.p[1].x > S1.p[0].x)&&(S2.p[1].x < S2.p[0].x)){
							result.push_back(1);
							result.push_back(0);
							flag	= 1;
						}
					}
				}
			}
		}
		if(round == 100){
			break;
		}
	}*/
}

void Core::genThirdPoint(Share& S1, Share& S2){
	string seed1, seed2;
	int flag	= 0;

	while(flag == 0){
		seed1	= getSeed(1, S1);
		seed2	= getSeed(1, S2);
		mpz_class mpzDelta1;
		mpz_class mpzDelta2;
		mpz_init_set_str (mpzDelta1.get_mpz_t(), seed1.c_str(), 16);
		mpz_init_set_str (mpzDelta2.get_mpz_t(), seed2.c_str(), 16);
		S1.delta  = mpz_get_str(NULL,16,mpzDelta1.get_mpz_t());
		S2.delta  = mpz_get_str(NULL,16,mpzDelta2.get_mpz_t());

		v3		= getChameleonHashValue(seed1, seed2);

		mpz_t mpzV;
		mpz_init_set_str (mpzV, v3.c_str(), 16);

		mpz_class mpzX1, mpzY1, mpzX2, mpzY2;
		mpz_class mpzW1(S1.widthSS);
		mpz_class mpzH1(S1.heightSS);
		mpz_class mpzW2(S2.widthSS);
		mpz_class mpzH2(S2.heightSS);
		mpz_mod(mpzX1.get_mpz_t(), mpzV, mpzW1.get_mpz_t());
		mpz_mod(mpzY1.get_mpz_t(), mpzV, mpzH1.get_mpz_t());
		mpz_mod(mpzX2.get_mpz_t(), mpzV, mpzW2.get_mpz_t());
		mpz_mod(mpzY2.get_mpz_t(), mpzV, mpzH2.get_mpz_t());

		S1.p[2]	= Point(mpzX1.get_ui() + 1, mpzY1.get_ui() + 1);
		S2.p[2]	= Point(mpzX2.get_ui() + 1, mpzY2.get_ui() + 1);
		if((S1.p[2].x != S1.p[1].x)&&(S1.p[2].y != S1.p[1].y)&&(S1.p[2].x != S1.p[0].x)&&(S1.p[2].y != S1.p[0].y)&&(S2.p[2].x != S2.p[1].x)&&(S2.p[2].y != S2.p[1].y)&&(S2.p[2].x != S2.p[0].x)&&(S2.p[2].y != S2.p[0].y)){
			flag = 1;
		}
	}
	/*int round		= 0;
	int flagbreak	= 0;
	int flag		= 0;
	mpz_class mpzArea1(S1.widthSS * S1.heightSS);
	mpz_class mpzArea2(S2.widthSS * S2.heightSS);
	while(flag == 0){
		round++;

		string seed1, seed2;

		seed1	= getSeed(3, S1);
		seed2	= getSeed(3, S2);
		v3		= getChameleonHashValue(seed1, seed2);

		mpz_class mpzGamma1;
		mpz_class mpzGamma2;
		mpz_init_set_str (mpzGamma1.get_mpz_t(), seed1.c_str(), 16);
		mpz_init_set_str (mpzGamma2.get_mpz_t(), seed2.c_str(), 16);
		S1.gamma  = mpz_get_str(NULL,16,mpzGamma1.get_mpz_t());
		S2.gamma  = mpz_get_str(NULL,16,mpzGamma2.get_mpz_t());

		mpz_t mpzV;
		mpz_init_set_str (mpzV, v3.c_str(), 16);

		mpz_class mpzV1, mpzV2;
		mpz_mod(mpzV1.get_mpz_t(), mpzV, mpzArea1.get_mpz_t());
		mpz_mod(mpzV2.get_mpz_t(), mpzV, mpzArea2.get_mpz_t());

		S1.i[2]	= mpzV1.get_ui() + 1;
		S2.i[2]	= mpzV2.get_ui() + 1;

		S1.p[2] = index2point(S1.i[2], S1.widthSS);
		S2.p[2] = index2point(S2.i[2], S2.widthSS);

		if((S1.p[2].y != S1.p[0].y)&&(S1.p[2].y != S1.p[1].y)&&(S2.p[2].y != S2.p[0].y)&&(S2.p[2].y != S2.p[1].y)){
			if(flag1 == 1){
				if(flag2[0] == flag2[1]){
					if(((S1.p[2].y < S1.p[0].y)&&(S2.p[2].y < S2.p[0].y))||((S1.p[2].y > S1.p[0].y)&&(S2.p[2].y > S2.p[0].y))){
						flag		= 1;
						S2.angle	= 0;
					}
				} else {
					if(((S1.p[2].y < S1.p[0].y)&&(S2.p[2].y > S2.p[0].y))||((S1.p[2].y > S1.p[0].y)&&(S2.p[2].y < S2.p[0].y))){
						flag		= 1;
						S2.angle	= 180;
					}
				}
			} else {
				if(flag1 == 2){
					if(((S1.p[2].y < S1.p[0].y)&&(S2.p[2].y < S2.p[0].y))||((S1.p[2].y > S1.p[0].y)&&(S2.p[2].y > S2.p[0].y))){
						flag		= 1;
						S2.angle	= 0;
					}
				} else {
					if(flag1 == 3){
						if(((S1.p[2].y < S1.p[0].y)&&(S2.p[2].y > S2.p[0].y))||((S1.p[2].y > S1.p[0].y)&&(S2.p[2].y < S2.p[0].y))){
							flag		= 1;
							S2.angle	= 180;
						}
					} else {
						if(flag1 == 4){
							if(flag2[0] == flag2[1]){
								if(((flag2[0] == 0)&&(flag2[1] == 0)&&(S1.p[2].y > S1.p[0].y)&&(S2.p[2].y < S2.p[0].y))||((flag2[0] == 1)&&(flag2[1] == 1)&&(S1.p[2].y < S1.p[0].y)&&(S2.p[2].y > S2.p[0].y))){
									flag		= 1;
									S2.angle	= 270;
								}
								if(((flag2[0] == 0)&&(flag2[1] == 0)&&(S1.p[2].y < S1.p[0].y)&&(S2.p[2].y > S2.p[0].y))||((flag2[0] == 1)&&(flag2[1] == 1)&&(S1.p[2].y > S1.p[0].y)&&(S2.p[2].y < S2.p[0].y))){
									flag		= 1;
									S2.angle	= 90;
								}
							} else {
								if(((flag2[0] == 1)&&(flag2[1] == 0)&&(S1.p[2].y > S1.p[0].y)&&(S2.p[2].y > S2.p[0].y))||((flag2[0] == 0)&&(flag2[1] == 1)&&(S1.p[2].y < S1.p[0].y)&&(S2.p[2].y < S2.p[0].y))){
									flag		= 1;
									S2.angle	= 270;
								}
								if(((flag2[0] == 0)&&(flag2[1] == 1)&&(S1.p[2].y > S1.p[0].y)&&(S2.p[2].y > S2.p[0].y))||((flag2[0] == 1)&&(flag2[1] == 0)&&(S1.p[2].y < S1.p[0].y)&&(S2.p[2].y < S2.p[0].y))){
									flag		= 1;
									S2.angle	= 90;
								}
							}
						} else {
							if(flag1 == 5){
								if(flag2[0] == 0){
									if(flag2[1] == 0){
										if((S1.p[2].y > S1.p[0].y)&&(S2.p[2].y < S2.p[0].y)){
											flag	= 1;
										}
									} else {
										if((S1.p[2].y < S1.p[0].y)&&(S2.p[2].y < S2.p[0].y)){
											flag	= 1;
										}
									}
								} else {
									if(flag2[1] == 0){
										if((S1.p[2].y > S1.p[0].y)&&(S2.p[2].y > S2.p[0].y)){
											flag	= 1;
										}
									} else {
										if((S1.p[2].y < S1.p[0].y)&&(S2.p[2].y > S2.p[0].y)){
											flag	= 1;
										}
									}
								}
								if(flag == 1){
									S2.angle = 270;
								}
							} else {
								if(flag1 == 6){
									if(flag2[0] == 0){
										if(flag2[1] == 0){
											if((S1.p[2].y < S1.p[0].y)&&(S2.p[2].y > S2.p[0].y)){
												flag	= 1;
											}
										} else {
											if((S1.p[2].y < S1.p[0].y)&&(S2.p[2].y < S2.p[0].y)){
												flag	= 1;
											}
										}
									} else {
										if(flag2[1] == 0){
											if((S1.p[2].y > S1.p[0].y)&&(S2.p[2].y > S2.p[0].y)){
												flag	= 1;
											}
										} else {
											if((S1.p[2].y > S1.p[0].y)&&(S2.p[2].y < S2.p[0].y)){
												flag	= 1;
											}
										}
									}
									if(flag == 1){
										S2.angle = 90;
									}
								}
							}
						}
					}
				}
			}
		}
		if(round == 100){
			flagbreak = 1;
			break;
		}
	}
	S2.revAngle	= (360 - S2.angle) % 360;

	return flagbreak;*/
}

void Core::getIntersectionProp(Share& S1, Share& S2, int index1, int index2){
	int start1X, end1X, start1Y, end1Y, start2X, end2X, start2Y, end2Y;

	if(S1.p[index1].x < S2.rotP[index2].x){
		start1X = 0;
		start2X = S2.rotP[index2].x - S1.p[index1].x;
		if((S1.widthSS + start2X)< S2.rotWidthSS){
			end1X = S1.widthSS;
			end2X = S1.widthSS + start2X;
		} else {
			end1X = S1.widthSS - (start2X + S1.widthSS - S2.rotWidthSS);
			end2X = S2.rotWidthSS;
		}
	} else {
		start1X = S1.p[index1].x - S2.rotP[index2].x;
		start2X = 0;
		if((S2.rotWidthSS + start1X)< S1.widthSS){
			end1X = S2.rotWidthSS + start1X;
			end2X = S2.rotWidthSS;
		} else {
			end1X = S1.widthSS;
			end2X = S2.rotWidthSS - (start1X + S2.rotWidthSS - S1.widthSS);
		}
	}

	if(S1.p[index1].y < S2.rotP[index2].y){
		start1Y = 0;
		start2Y = S2.rotP[index2].y - S1.p[index1].y;
		if((S1.heightSS + start2Y)< S2.rotHeightSS){
			end1Y = S1.heightSS;
			end2Y = S1.heightSS + start2Y;
		} else {
			end1Y = S1.heightSS - (start2Y + S1.heightSS - S2.rotHeightSS);
			end2Y = S2.rotHeightSS;
		}
	} else {
		start1Y = S1.p[index1].y - S2.rotP[index2].y;
		start2Y = 0;
		if((S2.rotHeightSS + start1Y)< S1.heightSS){
			end1Y = S2.rotHeightSS + start1Y;
			end2Y = S2.rotHeightSS;
		} else {
			end1Y = S1.heightSS;
			end2Y = S2.rotHeightSS - (start1Y + S2.rotHeightSS - S1.heightSS);
		}
	}

	widthIA		= end1X - start1X + 1;
	heightIA	= end1Y - start1Y + 1;

	S1.setIntersection(start1X, end1X, start1Y, end1Y);
	S2.setIntersection(start2X, end2X, start2Y, end2Y);
}

void Core::randomSIPosition(Share& S1, Share& S2){
	int x, y, spaceX, spaceY;

	spaceX	= (S1.intersection[1].x - S1.intersection[0].x) - widthSI;
	spaceY	= (S1.intersection[2].y - S1.intersection[0].y) - heightSI;

	srand((unsigned)time(0));
	x	= (rand() % (spaceX+1));
	y	= (rand() % (spaceY+1));

	S1.position.clear();
	S1.position.push_back(Point(S1.intersection[0].x + x, S1.intersection[0].y + y));
	S1.position.push_back(Point(S1.intersection[0].x + x + widthSI, S1.intersection[0].y + y));
	S1.position.push_back(Point(S1.intersection[0].x + x, S1.intersection[0].y + y + heightSI));
	S1.position.push_back(Point(S1.intersection[0].x + x + widthSI, S1.intersection[0].y + y + heightSI));

	S2.position.clear();
	S2.position.push_back(Point(S2.intersection[0].x + x, S2.intersection[0].y + y));
	S2.position.push_back(Point(S2.intersection[0].x + x + widthSI, S2.intersection[0].y + y));
	S2.position.push_back(Point(S2.intersection[0].x + x, S2.intersection[0].y + y + heightSI));
	S2.position.push_back(Point(S2.intersection[0].x + x + widthSI, S2.intersection[0].y + y + heightSI));
}

void Core::filter(){
	Mat imgTemp = SI.clone();
	if(ext != "bmp"){
		// histogram equalization
		equalizeHist(SI, imgTemp);

		// image sharpening (laplacian)
		Mat sharp;
		Mat kernel = (Mat_<float>(3, 3) << 0, -1, 0, -1, 4, -1, 0, -1, 0);
		filter2D(imgTemp, sharp, imgTemp.depth(), kernel);

		// dithering floyd
		for (int r = 0; r < sharp.rows; r++) {
			for (int c = 0; c < sharp.cols; c++) {
				uchar oldPixel	= sharp.at<uchar>(r,c);
				uchar newPixel	= oldPixel > 127 ? 255 : 0;
				sharp.at<uchar>(r,c)	= newPixel;
				int quantError			= oldPixel - newPixel;
				if(c+1 < sharp.cols) sharp.at<uchar>(r,c+1)					+=  (7 * quantError) >> 4;
				if((c > 0)&&(r+1 < sharp.rows)) sharp.at<uchar>(r+1,c-1)	+=  (3 * quantError) >> 4;
				if(r+1 < sharp.rows) sharp.at<uchar>(r+1,c)					+=  (5 * quantError) >> 4;
				if((r+1 < sharp.rows)&&(c+1 < sharp.cols)) sharp.at<uchar>(r+1,c+1)	+=  (1 * quantError) >> 4;
			}
		}
	}

	filteredSI = imgTemp;
}

void Core::generateShare(Share& S1, Share& S2){
	Mat C1			= generateNewImg(S1.heightSS*2, S1.widthSS*2, 1);
	Mat A1			= Mat::zeros(S1.heightSS*2, S1.widthSS*2, CV_8UC1);
	Mat C2			= generateNewImg(S2.rotHeightSS*2, S2.rotWidthSS*2, 1);
	Mat A2			= Mat::zeros(S2.rotHeightSS*2, S2.rotWidthSS*2, CV_8UC1);

	vector<Mat> channels1, channels2;
	unsigned seed;

	int siCoordinateY	= S1.position[0].y - S1.intersection[0].y;
	int siCoordinateX	= S1.position[0].x - S1.intersection[0].x;
	if(scheme == "nimsvcs"){
		// outside intersection area for SS1
		// :::::::::::::::::::::::::::::::::::::::::
		cout << "\nGenerating area 1 of S1" << endl;
		// :::::::::::::::::::::::::::::::::::::::::
		for(int i = 0; i < S1.heightSS; i++){
			for(int j = 0; j < S1.widthSS; j++){
				int doit = 0;
				// assign matrix m except intersection area
				if((i >= S1.intersection[0].y) && (i < S1.intersection[2].y)){
					if((j < S1.intersection[0].x) || (j >= S1.intersection[1].x)){
						doit = 1;
					}
				} else {
					doit = 1;
				}
				if(doit){
					// obtain a time-based seed:
					// random permutation on 'position' to get matrix c
					seed = (chrono::system_clock::now().time_since_epoch().count())+j;
					shuffle (pos.begin(), pos.end(), default_random_engine(seed));

					int C[2][4];
					for(int u = 0; u < 2; u++){
						for(int v = 0; v < 4; v++){
							C[u][v] = M0[u][v];
						}
					}

					// transform 1D to 2D matrix m
					int m[2][2];
					for(int l = 0; l < 4; l++){
						int rowm		= l/2;
						m[rowm][l % 2]	= C[0][pos[l]];
					}

					// asign matrix m to the share
					int marky = 0;
					for(int newy = i*2; newy < (i*2)+2; newy++){
						int markx = 0;
						for(int newx = j*2; newx < (j*2)+2; newx++){
							C1.at<uchar>(newy,newx) = m[marky][markx];
							if(m[marky][markx] == 255){
								A1.at<uchar>(newy,newx) = 0;
							} else {
								A1.at<uchar>(newy,newx) = 255;
							}
							markx = markx + 1;
						}
						marky = marky + 1;
					}
				}
			}
		}
		// outside intersection area for SS2
		// :::::::::::::::::::::::::::::::::::::::::
		cout << "\nGenerating area 1 of S2" << endl;
		// :::::::::::::::::::::::::::::::::::::::::
		for(int i = 0; i < S2.rotHeightSS; i++){
			for(int j = 0; j < S2.rotWidthSS; j++){
				int doit = 0;
				// assign matrix m except intersection area
				if((i >= S2.intersection[0].y) && (i < S2.intersection[2].y)){
					if((j < S2.intersection[0].x) || (j >= S2.intersection[1].x)){
						doit = 1;
					}
				} else {
					doit = 1;
				}
				if(doit){
					// obtain a time-based seed:
					// random permutation on 'position' to get matrix c
					seed = (chrono::system_clock::now().time_since_epoch().count())+j;
					shuffle (pos.begin(), pos.end(), default_random_engine(seed));

					int C[2][4];
					for(int u = 0; u < 2; u++){
						for(int v = 0; v < 4; v++){
							C[u][v] = M0[u][v];
						}
					}

					// transform 1D to 2D matrix m
					int m[2][2];
					for(int l = 0; l < 4; l++){
						int rowm		= l/2;
						m[rowm][l % 2]	= C[1][pos[l]];
					}

					// asign matrix m to the share
					int marky = 0;
					for(int newy = i*2; newy < (i*2)+2; newy++){
						int markx = 0;
						for(int newx = j*2; newx < (j*2)+2; newx++){
							C2.at<uchar>(newy,newx) = m[marky][markx];
							if(m[marky][markx] == 255){
								A2.at<uchar>(newy,newx) = 0;
							} else {
								A2.at<uchar>(newy,newx) = 255;
							}
							markx = markx + 1;
						}
						marky = marky + 1;
					}
				}
			}
		}
	}
	// inside intersection area but outside SI
	// :::::::::::::::::::::::::::::::::::::::::
	cout << "\nGenerating area 2" << endl;
	// :::::::::::::::::::::::::::::::::::::::::
	for(int i = 0; i < (S1.intersection[2].y - S1.intersection[0].y); i++){
		for(int j = 0; j < (S1.intersection[1].x - S1.intersection[0].x); j++){
			int doit			= 0;
			// assign matrix m except intersection area
			if((i >= siCoordinateY) && (i < (siCoordinateY + heightSI))){
				if((j < siCoordinateX) || (j >= siCoordinateX + widthSI)){
					doit = 1;
				}
			} else {
				doit = 1;
			}
			if(doit){
				// obtain a time-based seed:
				// random permutation on 'position' to get matrix c
				seed = (chrono::system_clock::now().time_since_epoch().count())+j;
				shuffle (pos.begin(), pos.end(), default_random_engine(seed));

				int C[2][4];
				for(int u = 0; u < 2; u++){
					for(int v = 0; v < 4; v++){
						C[u][v] = M0[u][v];
					}
				}

				// transform 1D to 2D matrix m
				int m_1[2][2],m_2[2][2];
				for(int l = 0; l < 4; l++){
					int rowm		= l/2;
					m_1[rowm][l % 2]	= C[0][pos[l]];
					m_2[rowm][l % 2]	= C[1][pos[l]];
				}

				// asign matrix m to the share
				int marky = 0;
				for(int newy = i*2; newy < (i*2)+2; newy++){
					int markx = 0;
					for(int newx = j*2; newx < (j*2)+2; newx++){
						C1.at<uchar>(newy + (S1.intersection[0].y*2),newx + (S1.intersection[0].x*2)) = m_1[marky][markx];
						if(m_1[marky][markx] == 255){
							A1.at<uchar>(newy + (S1.intersection[0].y*2),newx + (S1.intersection[0].x*2)) = 0;
						} else {
							A1.at<uchar>(newy + (S1.intersection[0].y*2),newx + (S1.intersection[0].x*2)) = 255;
						}

						C2.at<uchar>(newy + (S2.intersection[0].y*2),newx + (S2.intersection[0].x*2)) = m_2[marky][markx];
						if(m_2[marky][markx] == 255){
							A2.at<uchar>(newy + (S2.intersection[0].y*2),newx + (S2.intersection[0].x*2)) = 0;
						} else {
							A2.at<uchar>(newy + (S2.intersection[0].y*2),newx + (S2.intersection[0].x*2)) = 255;
						}
						markx = markx + 1;
					}
					marky = marky + 1;
				}
			}
		}
	}
	// :::::::::::::::::::::::::::::::::::::::::
	cout << "\nGenerating area 3" << endl;
	// :::::::::::::::::::::::::::::::::::::::::
	for(int i = 0; i < heightSI; i++){
		for(int j = 0; j < widthSI; j++){
			// obtain a time-based seed:
			// random permutation on 'position' to get matrix c
			seed = (chrono::system_clock::now().time_since_epoch().count())+j;
			shuffle (pos.begin(), pos.end(), default_random_engine(seed));
			int C[2][4];
			// permasalahan ada di sini
			if(filteredSI.at<uchar>(i,j) < 128){
				for(int u = 0; u < 2; u++){
					for(int v = 0; v < 4; v++){
						C[u][v] = M1[u][v];
					}
				}
			} else {
				for(int u = 0; u < 2; u++){
					for(int v = 0; v < 4; v++){
						C[u][v] = M0[u][v];
					}
				}
			}
			// transform 1D to 2D matrix m
			int m_1[2][2],m_2[2][2];
			for(int l = 0; l < 4; l++){
				int rowm		= l/2;
				m_1[rowm][l % 2]	= C[0][pos[l]];
				m_2[rowm][l % 2]	= C[1][pos[l]];
			}
			// asign matrix m to the share
			int marky = 0;
			for(int newy = i*2; newy < (i*2)+2; newy++){
				int markx = 0;
				for(int newx = j*2; newx < (j*2)+2; newx++){
					C1.at<uchar>(newy + ((S1.intersection[0].y + siCoordinateY)*2),newx + ((S1.intersection[0].x + siCoordinateX)*2)) = m_1[marky][markx];
					if(m_1[marky][markx] == 255){
						A1.at<uchar>(newy + ((S1.intersection[0].y + siCoordinateY)*2),newx + ((S1.intersection[0].x + siCoordinateX)*2)) = 0;
					} else {
						A1.at<uchar>(newy + ((S1.intersection[0].y + siCoordinateY)*2),newx + ((S1.intersection[0].x + siCoordinateX)*2)) = 255;
					}

					C2.at<uchar>(newy + ((S2.intersection[0].y + siCoordinateY)*2),newx + ((S2.intersection[0].x + siCoordinateX)*2)) = m_2[marky][markx];
					if(m_2[marky][markx] == 255){
						A2.at<uchar>(newy + ((S2.intersection[0].y + siCoordinateY)*2),newx + ((S2.intersection[0].x + siCoordinateX)*2)) = 0;
					} else {
						A2.at<uchar>(newy + ((S2.intersection[0].y + siCoordinateY)*2),newx + ((S2.intersection[0].x + siCoordinateX)*2)) = 255;
					}
					markx = markx + 1;
				}
				marky = marky + 1;
			}
		}
	}

	channels1.push_back(C1);
    channels1.push_back(C1);
    channels1.push_back(C1);
	channels1.push_back(A1);

	channels2.push_back(C2);
    channels2.push_back(C2);
    channels2.push_back(C2);
	channels2.push_back(A2);

	merge(channels1, S1.share);
	merge(channels2, S2.rotatedShare);
}

void Core::getNormalRotationImage(Share& S){
	if (S.revAngle == 0){
		S.share = S.rotatedShare.clone();
	} else {
		if (S.revAngle == 90){
			transpose(S.rotatedShare, S.share);
			flip(S.share, S.share,1); //transpose+flip(1)=CW
		} else {
			if (S.revAngle == 270) {
				transpose(S.rotatedShare, S.share);
				flip(S.share, S.share,0); //transpose+flip(0)=CCW
			} else {
				if (S.revAngle == 180){
					flip(S.rotatedShare, S.share,-1);    //flip(-1)=180
				}
			}
		}
	}
}

string Core::str2bin(string str,int base,int pad){
	string result;

	mpz_t strx;
	mpz_init_set_str (strx, str.c_str(), base);
	result = mpz_get_str(NULL,2,strx);

	result = string(pad - result.length(), '0') + result;

	return result;
}

Mat Core::generateNewImg(int h, int w, int channel){
	Mat newImage;
	if(channel == 1){
		newImage.create(h, w, CV_8UC1);
		newImage = Scalar(255);
	} else {
		if(channel == 3){
			newImage.create(h, w, CV_8UC3);
			newImage = Scalar(255,255,255);
		} else {
			if(channel == 4){
				newImage.create(h, w, CV_8UC4);
				newImage = Scalar(255,255,255,0);
			}
		}
	}
	return newImage;
}

void Core::splitString(const string &s, char delim, vector<string> &elems){
    stringstream ss;
    ss.str(s);
    string item;
    while (getline(ss, item, delim)) {
        elems.push_back(item);
    }
}

vector<string> Core::splitString(const string &s, char delim){
    vector<string> elems;
    splitString(s, delim, elems);
    return elems;
}

void Core::viewDecoding()
{
	system("cls");
	setSharePath();
	for(int i = 0; i < sharesPath.size(); i++){
		dur1.push_back(0);
		dur2.push_back(0);
		vector<string> tempName = splitString(sharesPath[i], '/');
		if(tempName[tempName.size() - 1][0] == 's'){
			string temp		= tempName[tempName.size() - 1];
			int flagStart	= 0;
			singleSI		= "";
			for(int j = 0; j < temp.length(); j++){
				if(flagStart == 1){
					singleSI	= singleSI + temp[j];
				}
				if(temp[j] == '.'){
					flagStart = 1;
				}
			}
		} else {
			singleSI		= tempName[tempName.size() - 1];
		}
		string type 	= tempName[tempName.size() - 2];
		string scheme 	= tempName[2];
		// *************************************************************************************
		// ********************************* PRE-DECODING **************************************
		// *************************************************************************************
		// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
		cout << "\n*******************************************************************************" << endl;
		cout << "****************************** PRE-DECODING ***********************************" << endl;
		cout << "*******************************************************************************" << endl;
		// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
		cout << "\n> Input share 1 and share 2 " << endl;
		Share S1(1);
		S1.setShare(sharesPath[i]);
		cout << "\nS1's size = " << S1.height << "x" << S1.width << endl;

		Share S2(2);
		S2.setShare(sharesPath[i]);
		cout << "\nS2's size = " << S2.height << "x" << S2.width << endl;
		// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
		string binParam1, binParam2;
		cout << "\n> Reading parameters in S1 and S2" << endl;
		clock_t tStart1	= clock();
		binParam1 = S1.getParam();
		binParam2 = S2.getParam();
		double duration1		= (double)(clock() - tStart1)/CLOCKS_PER_SEC;
		int size1 = dur1.size();
		dur1[size1-1] = duration1;

		splitParam(binParam1, binParam2, S1, S2);

		cout << "\nChameleon variables : " << endl;
		cout << "\n\t[p] : " << p << endl;
		cout << "\t[g] : " << g << endl;
		cout << "\t[h] : " << h << endl;

		cout << "\nSeeds for S1 : " << endl;
		cout << "\n\t[alpha] : " << S1.beta << endl;
		cout << "\t[beta] : " << S1.gamma << endl;
		cout << "\t[gamma] : " << S1.delta << endl;

		cout << "\nSeeds for S2 : " << endl;
		cout << "\n\t[beta] : " << S2.beta << endl;
		cout << "\t[gamma] : " << S2.gamma << endl;
		cout << "\t[delta] : " << S2.delta << endl;
		//system("pause");
		// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
		// =====================================================================================

		// =====================================================================================
		// ================================ CALCULATING 3OP ====================================
		// =====================================================================================
		// calculating original 3OP
		// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
		cout << "\n===============================================================================" << endl;
		cout << "============================ CALCULATING 3OP ==================================" << endl;
		cout << "===============================================================================" << endl;
		cout << "\n> Calculating Chameleon Hash Values" << endl;

		v1	= getChameleonHashValue(S1.beta, S2.beta);
		v2	= getChameleonHashValue(S1.gamma, S2.gamma);
		v3	= getChameleonHashValue(S1.delta, S2.delta);

		cout << "\n> Calculating 3OP in S1" << endl;

		S1.get3OP(v1, v2, v3);

		cout << "\n3OP in S1 : " << endl;
		cout << "\n\t[1] (" << S1.p[0].x << "," << S1.p[0].y << ")" << endl;
		cout << "\t[2] (" << S1.p[1].x << "," << S1.p[1].y << ")" << endl;
		cout << "\t[3] (" << S1.p[2].x << "," << S1.p[2].y << ")" << endl;

		cout << "\n> Calculating 3OP in S2" << endl;

		S2.get3OP(v1, v2, v3);

		cout << "\n3OP in S2 : " << endl;
		cout << "\n\t[1] (" << S2.p[0].x << "," << S2.p[0].y << ")" << endl;
		cout << "\t[2] (" << S2.p[1].x << "," << S2.p[1].y << ")" << endl;
		cout << "\t[3] (" << S2.p[2].x << "," << S2.p[2].y << ")" << endl;
		// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
		// =====================================================================================
		//system("pause");
		// *************************************************************************************
		// ************************************ STACKING ***************************************
		// *************************************************************************************
		// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
		cout << "\n*******************************************************************************" << endl;
		cout << "********************************* STACKING ************************************" << endl;
		cout << "*******************************************************************************" << endl;
		stacking(S1, S2, scheme, singleSI, type, i);

		S1.~Share();
		S2.~Share();
		reset();
	}
	fstream file;
	file.open("result/decoding/durdecode.csv",fstream::out);
	file << "no,reading,stacking" << endl;
	for(int i = 0; i < dur1.size(); i++){
		file << (i+1) << "," << fixed << setprecision(5) << dur1[i] << endl;
	}

	file.close();
	dur1.clear();
	dur2.clear();
	cout << endl;
	cout << "\n> Finished" << endl;
	cout << "\n" ;
	system("pause");
	// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	// =====================================================================================
	int opt = viewMainMenu();
	if(opt == 1){
		viewEncoding();
	} else {
		if(opt == 2){
			viewDecoding();
		} else {
			exit(0);
		}
	}
}

void Core::setSharePath(){
	char pathShare[100];
	cout << "\n> Input Share's path : ";
	cin >> pathShare;
	ifstream file;
	file.open(pathShare);
	while(!file){
		cout << "\nNo file found" << endl;
		cout << "\n> Input Share's path : ";
		cin >> pathShare;
		file.open(pathShare);
	}
    string str;
	int no	= 1;
    while (getline(file, str))
    {
		sharesPath.push_back(str);
    }
}

void Core::splitParam(string binParam1, string binParam2, Share& S1, Share& S2){
	string binBeta1, binGamma1, binDelta1, binG, binP1;

	binBeta1		= binParam1.substr(0, 16);
	binGamma1		= binParam1.substr((16), 16);
	binDelta1		= binParam1.substr((16 * 2), 16);
	binG			= binParam1.substr((16 * 3), 20);
	binP1			= binParam1.substr((20 + (16 * 3)), 10);

	string binBeta2, binGamma2, binDelta2, binH, binP2;

	binBeta2		= binParam2.substr(0, 16);
	binGamma2		= binParam2.substr((16), 16);
	binDelta2		= binParam2.substr((16 * 2), 16);
	binH			= binParam2.substr((16 * 3), 20);
	binP2			= binParam2.substr((20 + (16 * 3)), 10);

	string binP		= binP1 + binP2;

	g			= str2number(binG,2,10);
	h			= str2number(binH,2,10);
	p			= str2number(binP,2,10);
	S1.beta		= str2number(binBeta1,2,16);
	S1.gamma	= str2number(binGamma1,2,16);
	S1.delta	= str2number(binDelta1,2,16);
	S2.beta		= str2number(binBeta2,2,16);
	S2.gamma	= str2number(binGamma2,2,16);
	S2.delta	= str2number(binDelta2,2,16);
}

void Core::stacking(Share& S1, Share& S2, string scheme, string singleSI, string type, int index){
	// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	// =====================================================================================
	// ================================ FINDING ORIENTATION ================================
	// =====================================================================================
	// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	cout << "\n===============================================================================" << endl;
	cout << "============================ FINDING ORIENTATION ==============================" << endl;
	cout << "===============================================================================" << endl;
	// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	string newFolder;
	if(scheme == "imsvcs"){
		newFolder	= "result/decoding/imsvcs/";
		_mkdir(newFolder.c_str());
		newFolder	= newFolder + singleSI + "/";
		_mkdir(newFolder.c_str());
	} else {
		newFolder	= "result/decoding/nimsvcs/";
		_mkdir(newFolder.c_str());
		newFolder	= newFolder + type + "/";
		_mkdir(newFolder.c_str());
		if(type == "3"){
			newFolder	= newFolder + "s" + to_string(index) + "." + singleSI + "/";
		} else {
			newFolder	= newFolder + singleSI + "/";
		}
		_mkdir(newFolder.c_str());
	}
	
	double durationtot = 0;
	fstream file;
	file.open(newFolder + "durstack.csv", fstream::out);
	for(int i = 0; i < 3; i++){
		for(int j = 0; j < 3; j++){
			for(int k = 1; k < 3; k++){
				for(int l = 1; l < 3; l++){
					clock_t tStart1	= clock();
					S1.getNormal3OP(i, k);
					S1.getShape3OP(i, k);
					S2.getNormal3OP(j, l);
					S2.getShape3OP(j, l);

					S2.shape2angle(S1.shape3OP);
				
					getRotationImage(S2);
					S2.getRotationNormalPoints();

					getCanvas(S1, S2, i, j);
					canvas = generateNewImg(heightCanvas * 2, widthCanvas * 2, 4);

					cout << "\n> Stacking S1 and S2" << endl;
					clock_t tStart2	= clock();
					stackShares2Canvas(S1, S2);
					double duration2		= (double)(clock() - tStart2)/CLOCKS_PER_SEC;
					durationtot				= durationtot + duration2;
					int size2				= dur2.size();
					dur2[size2-1]			= durationtot;

					if(scheme == "imsvcs"){
						stringstream ss;
						string name		= newFolder + "ri_";
						string ext		= ".png";
						ss<<name<<(i)<<"_"<<(j)<<"_"<<(k)<<"_"<<(l)<<ext;

						string filename = ss.str();
						ss.str("");
						imwrite(filename,canvas);

						file << i << "," << j << "," << k << "," << l << "," << fixed << setprecision(5) << durationtot << endl;

						fstream file2;
						file2.open(newFolder +  "info_" + to_string(i) + "_" + to_string(j) + "_" + to_string(k) + "_" + to_string(l) + ".txt",fstream::out);
						file2 << "name : " << singleSI << endl;

						file2 << "\nSeeds for S1 : " << endl;
						file2 << "\n\t[beta] : " << S1.beta << endl;
						file2 << "\t[gamma] : " << S1.gamma << endl;
						file2 << "\t[delta] : " << S1.delta << endl;

						file2 << "\nSeeds for S2 : " << endl;
						file2 << "\n\t[beta] : " << S2.beta << endl;
						file2 << "\t[gamma] : " << S2.gamma << endl;
						file2 << "\t[delta] : " << S2.delta << endl;

						file2 << "\nCoordinates of 3OP in SS1 : " << endl;
						file2 << "\t[1] (" << S1.p[0].x << "," << S1.p[0].y << ")" << endl;
						file2 << "\t[2] (" << S1.p[1].x << "," << S1.p[1].y << ")" << endl;
						file2 << "\t[3] (" << S1.p[2].x << "," << S1.p[2].y << ")" << endl;

						file2 << "\nCoordinates of 3OP in SS2 : " << endl;
						file2 << "\t[1] (" << S2.p[0].x << "," << S2.p[0].y << ")" << endl;
						file2 << "\t[2] (" << S2.p[1].x << "," << S2.p[1].y << ")" << endl;
						file2 << "\t[3] (" << S2.p[2].x << "," << S2.p[2].y << ")" << endl;

						file2 << "\nNormal 3OP S1 : " << endl;
						file2 << "\n\t[1] (" << S1.nP[0].x << "," << S1.nP[0].y << ")" << endl;
						file2 << "\t[2] (" << S1.nP[1].x << "," << S1.nP[1].y << ")" << endl;
						file2 << "\t[3] (" << S1.nP[2].x << "," << S1.nP[2].y << ")" << endl;

						file2 << "\nNormal 3OP S2 : " << endl;
						file2 << "\n\t[1] (" << S2.nP[0].x << "," << S2.nP[0].y << ")" << endl;
						file2 << "\t[2] (" << S2.nP[1].x << "," << S2.nP[1].y << ")" << endl;
						file2 << "\t[3] (" << S2.nP[2].x << "," << S2.nP[2].y << ")" << endl;

						file2 << "\nType S1 \t: " << S1.shape3OP << endl;
						file2 << "\nType S2 \t: " << S2.shape3OP << endl;

						file2 << "\nAngle Rotation \t: " << S2.angle << endl;

						file2 << "\nRotated Normal 3OP S2 : " << endl;
						file2 << "\n\t[1] (" << S2.rotNP[0].x << "," << S2.rotNP[0].y << ")" << endl;
						file2 << "\t[2] (" << S2.rotNP[1].x << "," << S2.rotNP[1].y << ")" << endl;
						file2 << "\t[3] (" << S2.rotNP[2].x << "," << S2.rotNP[2].y << ")" << endl;

						file2 << "\nS1 Position \t: (" << S1.pointStart.x << "," << S1.pointStart.y << ")" << endl;
						file2 << "\nS2 Position \t: (" << S2.pointStart.x << "," << S2.pointStart.y << ")" << endl;
						file2.close();
					} else {
						stringstream ss;
						string name		= newFolder + "ri_";
						string ext		= ".png";
						ss<<name<<(i)<<"_"<<(j)<<"_"<<(k)<<"_"<<(l)<<ext;

						string filename = ss.str();
						ss.str("");
						imwrite(filename,canvas);

						file << i << "," << j << "," << k << "," << l << "," << fixed << setprecision(5) << durationtot << endl;

						fstream file2;
						file2.open(newFolder +  "info_" + to_string(i) + "_" + to_string(j) + "_" + to_string(k) + "_" + to_string(l) + ".txt",fstream::out);
						file2 << "name : " << singleSI << endl;
						
						file2 << "\nSeeds for S1 : " << endl;
						file2 << "\n\t[beta] : " << S1.beta << endl;
						file2 << "\t[gamma] : " << S1.gamma << endl;
						file2 << "\t[delta] : " << S1.delta << endl;

						file2 << "\nSeeds for S2 : " << endl;
						file2 << "\n\t[beta] : " << S2.beta << endl;
						file2 << "\t[gamma] : " << S2.gamma << endl;
						file2 << "\t[delta] : " << S2.delta << endl;

						file2 << "\nCoordinates of 3OP in SS1 : " << endl;
						file2 << "\t[1] (" << S1.p[0].x << "," << S1.p[0].y << ")" << endl;
						file2 << "\t[2] (" << S1.p[1].x << "," << S1.p[1].y << ")" << endl;
						file2 << "\t[3] (" << S1.p[2].x << "," << S1.p[2].y << ")" << endl;

						file2 << "\nCoordinates of 3OP in SS2 : " << endl;
						file2 << "\t[1] (" << S2.p[0].x << "," << S2.p[0].y << ")" << endl;
						file2 << "\t[2] (" << S2.p[1].x << "," << S2.p[1].y << ")" << endl;
						file2 << "\t[3] (" << S2.p[2].x << "," << S2.p[2].y << ")" << endl;

						file2 << "\nNormal 3OP S1 : " << endl;
						file2 << "\n\t[1] (" << S1.nP[0].x << "," << S1.nP[0].y << ")" << endl;
						file2 << "\t[2] (" << S1.nP[1].x << "," << S1.nP[1].y << ")" << endl;
						file2 << "\t[3] (" << S1.nP[2].x << "," << S1.nP[2].y << ")" << endl;

						file2 << "\nNormal 3OP S2 : " << endl;
						file2 << "\n\t[1] (" << S2.nP[0].x << "," << S2.nP[0].y << ")" << endl;
						file2 << "\t[2] (" << S2.nP[1].x << "," << S2.nP[1].y << ")" << endl;
						file2 << "\t[3] (" << S2.nP[2].x << "," << S2.nP[2].y << ")" << endl;

						file2 << "\nType S1 \t: " << S1.shape3OP << endl;
						file2 << "\nType S2 \t: " << S2.shape3OP << endl;
						file2 << "\nAngle Rotation \t: " << S2.angle << endl;

						file2 << "\nRotated Normal 3OP S2 : " << endl;
						file2 << "\n\t[1] (" << S2.rotNP[0].x << "," << S2.rotNP[0].y << ")" << endl;
						file2 << "\t[2] (" << S2.rotNP[1].x << "," << S2.rotNP[1].y << ")" << endl;
						file2 << "\t[3] (" << S2.rotNP[2].x << "," << S2.rotNP[2].y << ")" << endl;

						file2 << "\nS1 Position \t: (" << S1.pointStart.x << "," << S1.pointStart.y << ")" << endl;
						file2 << "\nS2 Position \t: (" << S2.pointStart.x << "," << S2.pointStart.y << ")" << endl;
						file2.close();
					}
					canvas.release();
				}
			}
		}
	}
	file.close();
}

void Core::getRotationImage(Share& S){
	vector<Mat> tempChannels(4);

	if (S.angle == 0){
		S.rotatedShare 	= S.share.clone();
		S.rotWidth 			= S.width;
		S.rotHeight 		= S.height;
	} else {
		if (S.angle == 90){
			transpose(S.share, S.rotatedShare);
			flip(S.rotatedShare, S.rotatedShare,1); //transpose+flip(1)=CW
			S.rotWidth 			= S.height;
			S.rotHeight 		= S.width;
		} else {
			if (S.angle == 270) {
				transpose(S.share, S.rotatedShare);
				flip(S.rotatedShare, S.rotatedShare,0); //transpose+flip(0)=CCW
				S.rotWidth 			= S.height;
				S.rotHeight 		= S.width;
			} else {
				if (S.angle == 180){
					flip(S.share, S.rotatedShare,-1);    //flip(-1)=180
					S.rotWidth 			= S.width;
					S.rotHeight 		= S.height;
				}
			}
		}
	}

	split(S.rotatedShare,tempChannels);
	S.rotChannels	= tempChannels;
}

void Core::getCanvas(Share& S1, Share& S2, int index1, int index2){
	int start1X, end1X, start1Y, end1Y, start2X, end2X, start2Y, end2Y;
	// normalize x coordinate
	if(S1.nP[index1].x > S2.rotNP[index2].x){
		start2X			= S1.nP[index1].x - S2.rotNP[index2].x;
		start1X			= 0;
		
		if((S1.nP[index1].x - S2.rotNP[index2].x + (S2.rotWidth/2)) < (S1.width/2)){
			widthCanvas = (S1.width/2);
		} else {
			widthCanvas	= S1.nP[index1].x - S2.rotNP[index2].x + (S2.rotWidth/2);
		}
	} else if(S1.nP[index1].x < S2.rotNP[index2].x){
		start1X			= S2.rotNP[index2].x - S1.nP[index1].x;
		start2X			= 0;
		
		if((S2.rotNP[index2].x - S1.nP[index1].x + (S1.width/2)) < (S2.rotWidth/2)){
			widthCanvas = (S2.rotWidth/2);
		} else {
			widthCanvas	= S2.rotNP[index2].x - S1.nP[index1].x + (S1.width/2);
		}
	} else {
		start1X				= 0;
		start2X				= 0;
		
		if((S1.width/2) > (S2.rotWidth/2)){
			widthCanvas		= (S1.width/2);
		} else {
			widthCanvas		= (S2.rotWidth/2);
		}
	}
	// normalize y coordinate
	if(S1.nP[index1].y > S2.rotNP[index2].y){
		start2Y			= S1.nP[index1].y - S2.rotNP[index2].y;
		start1Y			= 0;
		
		if((S1.nP[index1].y - S2.rotNP[index2].y + (S2.rotHeight/2)) < (S1.height/2)){
			heightCanvas	= (S1.height/2);
		} else {
			heightCanvas	= S1.nP[index1].y - S2.rotNP[index2].y + (S2.rotHeight/2);
		}
	} else if(S1.nP[index1].y < S2.rotNP[index2].y){
		start1Y			= S2.rotNP[index2].y - S1.nP[index1].y;
		start2Y			= 0;
		
		if((S2.rotNP[index2].y - S1.nP[index1].y + (S1.height/2)) < (S2.rotHeight/2)){
			heightCanvas = (S2.rotHeight/2);
		} else {
			heightCanvas	= S2.rotNP[index2].y - S1.nP[index1].y + (S1.height/2);
		}
	} else {
		start1Y				= 0;
		start2Y				= 0;
		
		if((S1.height/2) > (S2.rotHeight/2)){
			heightCanvas	= (S1.height/2);
		} else {
			heightCanvas	= (S2.rotHeight/2);
		}
	}

	S1.pointStart = Point(start1X,start1Y);
	S2.pointStart = Point(start2X,start2Y);

	int X1, Y1, X2, Y2;
	if(S2.pointStart.x >= S1.pointStart.x){
		if(S2.pointStart.x + (S2.rotWidth/2) > (S1.width/2)){
			widthIA		= (S1.width/2) - S2.pointStart.x;
		} else {
			widthIA		= (S2.rotWidth/2);
		}
		X1			= S2.pointStart.x;
		X2			= 0;
	} else {
		if(S1.pointStart.x + (S1.width/2) > (S2.rotWidth/2)){
			widthIA		= (S2.rotWidth/2) - S1.pointStart.x;
		} else {
			widthIA		= (S1.width/2);
		}
		X1			= 0;
		X2			= S1.pointStart.x;
	}

	if(S2.pointStart.y >= S1.pointStart.y){
		if(S2.pointStart.y + (S2.rotHeight/2) > (S1.height/2)){
			heightIA	= (S1.height/2) - S2.pointStart.y;
		} else {
			heightIA	= (S2.rotHeight/2);
		}
		Y1			= S2.pointStart.y;
		Y2			= 0;
	} else {
		if(S1.pointStart.y + (S1.height/2) > (S2.rotHeight/2)){
			heightIA	= (S2.rotHeight/2) - S1.pointStart.y;
		} else {
			heightIA	= (S1.height/2);
		}
		Y1			= 0;
		Y2			= S1.pointStart.y;
	}

	S1.pointIA = Point(X1,Y1);
	S2.pointIA = Point(X2,Y2);
}

void Core::stackShares2Canvas(Share& S1, Share& S2){
	int positiony, positionx;
	vector<Mat> channelsBB;
	Mat C	= generateNewImg(heightIA * 2, widthIA * 2, 1);
	Mat A	= Mat::zeros(heightIA * 2, widthIA * 2, CV_8UC1);

	if((S1.width == S2.rotWidth)&&(S1.height == S2.rotHeight)){
		// stacking share 2 to share 1
		positiony = 0;
		for(int i = 0; i < (heightCanvas * 2); i++){
			positionx = 0;
			for(int j = 0; j < (widthCanvas * 2); j++){
				if((i >= (S2.pointStart.y * 2)) && (i < ((S2.pointStart.y*2) + (S2.rotHeight)))){
					if((j >= (S2.pointStart.x * 2)) && (j < ((S2.pointStart.x*2) + (S2.rotWidth)))){
						int currentP	= S2.rotChannels[0].at<uchar>(positiony,positionx);
						if(currentP < 128){
							S1.channels[0].at<uchar>(i,j)	= S2.rotChannels[0].at<uchar>(positiony,positionx);
							S1.channels[3].at<uchar>(i,j)	= 255;
						}
						positionx				= positionx + 1;
					}
				}
			}
			if((i >= (S2.pointStart.y*2))&&(i < ((S2.pointStart.y*2) + (S2.rotHeight)))){
				positiony = positiony + 1;
			}
		}

		channelsBB.push_back(S1.channels[0]);
		channelsBB.push_back(S1.channels[0]);
		channelsBB.push_back(S1.channels[0]);
		channelsBB.push_back(S1.channels[3]);

		merge(channelsBB, canvas);
	} else {
		for(int i = 0; i < (heightIA * 2); i++){
			for(int j = 0; j < (widthIA * 2); j++){
				int cur1	= S1.channels[0].at<uchar>((S1.pointIA.y*2)+i,(S1.pointIA.x*2)+j);
				int cur2	= S2.rotChannels[0].at<uchar>((S2.pointIA.y*2)+i,(S2.pointIA.x*2)+j);
				if((cur1 < 128)||(cur2 < 128)){
					C.at<uchar>(i,j)	= 0;
					A.at<uchar>(i,j)	= 255;
				} else {
					C.at<uchar>(i,j)	= 255;
				}
			}
		}
		/*positiony = 0;
		// stacking share 1 to big box
		for(int i = 0; i < (heightCanvas * 2); i++){
			positionx = 0;
			for(int j = 0; j < (widthCanvas * 2); j++){
				if((i >= (S1.pointStart.y * 2)) && (i < ((S1.pointStart.y*2) + (S1.height)))){
					if((j >= (S1.pointStart.x*2))&&(j < ((S1.pointStart.x*2) + (S1.width)))){
						int currentP	= S1.channels[0].at<uchar>(positiony,positionx);
						if(currentP < 128){
							C.at<uchar>(i,j)	= S1.channels[0].at<uchar>(positiony,positionx);
							A.at<uchar>(i,j)	= 255;
						}
						positionx				= positionx + 1;
					}
				}
			}
			if((i >= (S1.pointStart.y * 2))&&(i < ((S1.pointStart.y * 2) + (S1.height)))){
				positiony = positiony + 1;
			}
		}

		// stacking share 2 to big box
		positiony = 0;
		for(int i = 0; i < (heightCanvas * 2); i++){
			positionx = 0;
			for(int j = 0; j < (widthCanvas * 2); j++){
				if((i >= (S2.pointStart.y * 2)) && (i < ((S2.pointStart.y*2) + (S2.rotHeight)))){
					if((j >= (S2.pointStart.x * 2)) && (j < ((S2.pointStart.x*2) + (S2.rotWidth)))){
						int currentP	= S2.rotChannels[0].at<uchar>(positiony,positionx);
						if(currentP < 128){
							C.at<uchar>(i,j)	= S2.rotChannels[0].at<uchar>(positiony,positionx);
							A.at<uchar>(i,j)	= 255;
						}
						positionx				= positionx + 1;
					}
				}
			}
			if((i >= (S2.pointStart.y*2))&&(i < ((S2.pointStart.y*2) + (S2.rotHeight)))){
				positiony = positiony + 1;
			}
		}*/

		channelsBB.push_back(C);
		channelsBB.push_back(C);
		channelsBB.push_back(C);
		channelsBB.push_back(A);

		merge(channelsBB, canvas);
	}
}

string Core::str2number(string str, int baseSource, int baseDestination){
	string result;

	mpz_class strx;
	mpz_init_set_str (strx.get_mpz_t(), str.c_str(), baseSource);
	result = mpz_get_str(NULL,baseDestination,strx.get_mpz_t());

	return result;
}
