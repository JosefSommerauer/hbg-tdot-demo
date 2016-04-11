#ifndef HUT_H
#define HUT_H

#include <string>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include "opencv2/cudaobjdetect.hpp"
#include "opencv2/cudaimgproc.hpp"
#include "opencv2/cudaarithm.hpp"

struct Hut {
public:
	Hut(std::string file, double scale_factor, double offset_x, double offset_y) {
		Img = cv::imread(file,cv::IMREAD_UNCHANGED);
		Scale = scale_factor;
		OffsetX = offset_x;
		OffsetY = offset_y;

		if (Img.empty()) {
			std::cout << "Could not read input image files" << std::endl;
		} else {
			std::cout << file << " read success!" << std::endl;
		} 
	}

	cv::Mat Img;
	double Scale;
	double OffsetX;
	double OffsetY;
};

#endif // HUT_H
