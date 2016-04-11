#ifndef FACES_H_INCLUDED
#define FACES_H_INCLUDED

#include <mutex>

#include "opencv2/core.hpp"

class Faces {

private:

  //std::mutex mMutex;
  int const DEFAULT_TTL = 20;

public:

	void addFace(cv::Rect &face);

	void tick();

	//std::mutex &getMutex();
	std::vector<cv::Rect> getFaces();

	struct FaceEntry {
	 cv::Rect face;
	 int ttl;
	};

	std::vector<FaceEntry> mFaces;

};

#endif
