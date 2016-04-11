# OpenCV Demo - UAS Hagenberg Open House 2016 

##1st step: Install the Dependencies
```Bash
sudo apt-get -y install libopencv-dev build-essential cmake git libgtk2.0-dev pkg-config python-dev python-numpy libdc1394-22 libdc1394-22-dev libjpeg-dev libpng12-dev libtiff4-dev libjasper-dev libavcodec-dev libavformat-dev libswscale-dev libxine-dev libgstreamer0.10-dev libgstreamer-plugins-base0.10-dev libv4l-dev libtbb-dev libqt4-dev libfaac-dev libmp3lame-dev libopencore-amrnb-dev libopencore-amrwb-dev libtheora-dev libvorbis-dev libxvidcore-dev x264 v4l-utils unzip
```

##2nd step: Download OpenCV 3.1
```Bash
  mkdir opencv
  cd opencv
  wget https://github.com/Itseez/opencv/archive/3.1.0.zip -O opencv-3.1.0.zip
  unzip opencv-3.1.0.zip
```

##3rd step: Install OpenCV
```Bash
cd opencv-3.0.0-alpha
mkdir build
cd build
cmake -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=/usr/local -D WITH_TBB=ON -D WITH_V4L=ON -D WITH_QT=ON -D       WITH_OPENGL=ON ..
make -j $(nproc)
sudo make install
```

##4th Download, Make and Run Showcase
```Bash
cd~
wget https://github.com/JosefSommerauer/hbg-tdot-demo/archive/master.zip -O tdot.zip
unzip tdot.zip
rm tdot.zip
cd tdot
make
sudo su
./run_showcase.sh 
```








