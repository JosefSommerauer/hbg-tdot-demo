#include <string>

#include "FaceDetector.h"
#include "Hut.h"
#include "faces.h"

FaceDetector::FaceDetector(std::string cascadefile, 
	int calc_res_w, int calc_res_h, int display_res_w, int display_res_h) :
		mCalcResW(calc_res_w), mCalcResH(calc_res_h), 
		mDisplayResW(display_res_w), mDisplayResH(display_res_h), 
		mFaceCascade_cpu(cascadefile)
 {
	mScaleW = display_res_w / calc_res_w;
	mScaleH = display_res_h / calc_res_h;

	mMinObjectSize = cv::Size(20, 20);
	mMaxObjectSize = cv::Size(0, 0);
	
	// add huts                                     /scale,x,y
	hats.push_back(Hut(std::string("hat/hat01.png"),2.1,0.5 ,0.9));
	hats.push_back(Hut(std::string("hat/hat02.png"),1.0,0.52,-0.1));
	hats.push_back(Hut(std::string("hat/hat03.png"),1.0,0.5 ,0.8));
	hats.push_back(Hut(std::string("hat/hat04.png"),1.5,0.5 ,0.7));
	hats.push_back(Hut(std::string("hat/hat05.png"),1.3,0.5 ,0.7));
	hats.push_back(Hut(std::string("hat/hat06.png"),1.2,0.5 ,0.8));
	hats.push_back(Hut(std::string("hat/hat07.png"),1.7,0.5 ,0.7));
	hats.push_back(Hut(std::string("hat/hat08.png"),2.5,0.5 ,0.7));


	// CascadeClassifier
	mFrontalfaceFile = cascadefile;
	mFaceCascade_gpu = cv::cuda::CascadeClassifier::create(mFrontalfaceFile);
	mFaceCascade_gpu->setScaleFactor(1.1);
	mFaceCascade_gpu->setMinObjectSize(mMinObjectSize);
	mFaceCascade_gpu->setMaxObjectSize(mMaxObjectSize);
	mFaceCascade_gpu->setMinNeighbors(3);
}

cv::Mat & FaceDetector::doit(cv::Mat & frame, bool gpu, int hat_offset, bool debug) {
	cv::resize(frame, frame_out, cv::Size(mDisplayResW, mDisplayResH), 0, 0, cv::INTER_LINEAR);
	cv::resize(frame, frame_lowres, cv::Size(mCalcResW , mCalcResH), 0, 0, cv::INTER_LINEAR);

	if(gpu) {
		// gpu
		d_frame.upload(frame_lowres);
		cv::cuda::cvtColor(d_frame, d_frame, cv::COLOR_BGR2GRAY);
		mFaceCascade_gpu->detectMultiScale(d_frame, d_faces);
		mFaceCascade_gpu->convert(d_faces, faces);
	} else {
		// cpu
		cv::cvtColor(frame_lowres, frame_gray, cv::COLOR_BGR2GRAY);		
		mFaceCascade_cpu.detectMultiScale(frame_gray,faces,1.1,3,0, mMinObjectSize, mMaxObjectSize);
	}

	int j=0;	

	mFaces.tick(); // age faces

	for (size_t i = 0; i < faces.size(); i++) { 
		mFaces.addFace(faces[i]);

		if(debug) {
			int face_w = (double(faces[i].width)) * mScaleW;
			int face_h = (double(faces[i].height)) * mScaleH;
			int face_x = (double(faces[i].x)) * mScaleW;
			int face_y = (double(faces[i].y)) * mScaleH;

			cv::rectangle(frame_out, 
				cv::Point(face_x, face_y), 
				cv::Point(face_x+face_w, face_y+face_h), 
				cv::Scalar(0, 0, 255));
		}
	}

	for (size_t i = 0; i < mFaces.mFaces.size(); i++) {
		j = (i + hat_offset) % hats.size();

		int face_w = (double(mFaces.mFaces[i].face.width)) * mScaleW;
		int face_h = (double(mFaces.mFaces[i].face.height)) * mScaleH;
		int face_x = (double(mFaces.mFaces[i].face.x)) * mScaleW;
		int face_y = (double(mFaces.mFaces[i].face.y)) * mScaleH;

		int hat_h  = (double(mFaces.mFaces[i].face.width) / hats[j].Img.cols) * hats[j].Img.rows * hats[j].Scale * mScaleH;
		int hat_w  = (double(mFaces.mFaces[i].face.width) / hats[j].Img.cols) * hats[j].Img.cols * hats[j].Scale * mScaleW;

		int hat_x  = face_x + (face_w/2) - (hat_w*hats[j].OffsetX);
		int hat_y  = face_y - (hat_h*hats[j].OffsetY);

		cv::resize(hats[j].Img, tmp, cv::Size(hat_w, hat_h), 0, 0, cv::INTER_LINEAR);
		overlayImage(&frame_out, &tmp, cv::Point(hat_x,hat_y) );
	}
	
	return frame_out;
}

void FaceDetector::overlayImage(cv::Mat* src, cv::Mat* overlay, const cv::Point& location) {
    for (int y = cv::max(location.y, 0); y < src->rows; ++y) {
        int fY = y - location.y;

        if (fY >= overlay->rows)
            break;

        for (int x = cv::max(location.x, 0); x < src->cols; ++x)
        {
            int fX = x - location.x;

            if (fX >= overlay->cols)
                break;

            double opacity = ((double)overlay->data[fY * overlay->step + fX * overlay->channels() + 3]) / 255;

            for (int c = 0; opacity > 0 && c < src->channels(); ++c)
            {
                unsigned char overlayPx = overlay->data[fY * overlay->step + fX * overlay->channels() + c];
                unsigned char srcPx = src->data[y * src->step + x * src->channels() + c];
                src->data[y * src->step + src->channels() * x + c] = srcPx * (1. - opacity) + overlayPx * opacity;
            }
        }
    }
}
