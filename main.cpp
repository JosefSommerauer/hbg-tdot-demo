//
//  main.cpp
//  OpenCV
//
//  Created by Christian Roman Mendoza on 21/12/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <iostream>
#include <string>
//#include <opencv2/highgui/highgui.hpp>
/*
#include "opencv2/cudaobjdetect.hpp"
#include "opencv2/cudaimgproc.hpp"
#include "opencv2/cudaarithm.hpp"
#include "opencv2/cudawarping.hpp"
#include "opencv2/cudafeatures2d.hpp"
#include "opencv2/cudafilters.hpp"
#include "opencv2/cudaoptflow.hpp"
#include "opencv2/cudabgsegm.hpp"
#include "opencv2/cudaoptflow.hpp"
*/


//#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include <time.h>

#include "FaceDetector.h"
#include "EdgeDetector.h"
#include "OpticalFlow.h"

using namespace std;
using namespace cv;

const int cv_Upkey    = 1113938;
const int cv_DownKey  = 1113940;
const int cv_LeftKey  = 1113937;
const int cv_RightKey = 1113939;
const int cv_Space    = 1048608;
const int cv_Delete   = 1114111;
const int cv_Esc      = 1048603;
const int cv_Enter    = 1048586;

#define FOURCC(c1, c2, c3, c4) (((c1) & 255) + (((c2) & 255) << 8) + (((c3) & 255) << 16) + (((c4) & 255) << 24))

int main (int argc, const char * argv[])
{
	const int nr_hats = 7;

	bool gpu = true;
	bool debug = false;
	bool fullscreen = true;

	int cam_res_w = 960;
	int cam_res_h = 480;
	int display_res_w = 1920; //960; //1920;
	int display_res_h = 1080; //1200; //600; //1200;
	int down_scale_factor = 4;
	int calc_res_w = display_res_w / down_scale_factor;//480;  //384; //320;
	int calc_res_h = display_res_h / down_scale_factor;//300;  //240; //200;

	// frame buffer
	Mat frame, frame_out;

	//capture = cvCreateCameraCapture(0);
	VideoCapture capture(0);
	if(!capture.isOpened()) return 0;

	// set camera settings
   double codec = FOURCC('M', 'J', 'P', 'G');
   capture.set(cv::CAP_PROP_FOURCC, codec);
   capture.set(cv::CAP_PROP_FRAME_WIDTH, cam_res_w);
   capture.set(cv::CAP_PROP_FRAME_HEIGHT, cam_res_h); 

	// setup windows
	cv::namedWindow("image", WINDOW_NORMAL | WINDOW_FREERATIO );
	cv::setWindowProperty("image", WND_PROP_FULLSCREEN, WINDOW_FULLSCREEN );
	cv::setWindowProperty("image", WND_PROP_OPENGL, WINDOW_OPENGL );

	// face detector
	std::string frontalface = "haarcascade_frontalface_alt.xml";
	FaceDetector fd(frontalface, calc_res_w, calc_res_h, display_res_w, display_res_h);
	int hat_offset = 0;

	// edge detector
	EdgeDetector ed(calc_res_w, calc_res_h, display_res_w, display_res_h);

	// optical flow
	OpticalFlow of(calc_res_w, calc_res_h, display_res_w, display_res_h);

	enum ImageFilter { eFaceDetector = 0, eEdgeDetector = 1, eOpticalFlow = 2 };
	int filter = eFaceDetector;
	unsigned int nr_filter = 3;
	

	double time = 0;

	for(;;) {
		

		capture >> frame;   // grab current frame from the camera
		cv::flip(frame, frame, 1); // mirror image
		
		time = (double) getTickCount();

		switch(filter) {
			case eFaceDetector:
				frame_out = fd.doit(frame, gpu, hat_offset, debug);
				break;
			case eEdgeDetector:
				frame_out = ed.doit(frame, gpu);
				break;
			case eOpticalFlow:
				frame_out = of.doit(frame, gpu);
				break;
		}

		double frame_rate = ((double) getTickCount() - time) / getTickFrequency();
		

		if(debug) {
			cv::Point pos(50,65); 
			cv::rectangle(frame_out, cv::Point(40, 40), cv::Point(153, 75), cv::Scalar(0, 0, 0), -1);
			   
		   std::stringstream ss; ss.precision(3);
			ss << "FPS: " << 1/frame_rate;
		   cv::putText(frame_out, ss.str(), pos, FONT_HERSHEY_PLAIN, 1.2, Scalar(128, 255, 255));
		}

		cv::imshow("image", frame_out);
	
		// user menu		
		int c = cv::waitKey(1); //cvWaitKey(1);
		switch (c) {
			case 'g' + 0x100000:  
				gpu = !gpu;
				std::cout << "use gpu:" << gpu << std::endl;
				break;

			case 'd' + 0x100000:  
				debug = !debug;
				std::cout << "debug:" << debug << std::endl;
				break;

			case cv_Enter:  
				fullscreen = !fullscreen;
				std::cout << "fullscreen:" << fullscreen << std::endl;

				if(fullscreen) {
					cv::setWindowProperty("image", WND_PROP_FULLSCREEN, WINDOW_FULLSCREEN );
				} else {
					cv::setWindowProperty("image", WND_PROP_FULLSCREEN, WINDOW_NORMAL );
				}
				break;
				
			case cv_Upkey: 
				break;

			case cv_LeftKey: 
				filter--; filter %= nr_filter;
				break;

			case cv_RightKey:
				filter++; filter %= nr_filter;
				break;

			case cv_DownKey: 
				break;

			case cv_Space: 
				hat_offset++; //hat_offset %= nr_hats;
				break;

			default:				
				break;
		}

		
		if(c != -1)	
			std::cout << "key: " << int(c) << " c:" 
						 << char(c) << " " << int(char(c))
						 << "\t filter: " 
						 << filter << std::endl;
		

		if( c == cv_Esc ) break;
		
	}

	cv::destroyWindow("image");

	return 0;
}
