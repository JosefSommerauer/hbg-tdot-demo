# OpenCV Demo
UAS Hagenberg Open House 2016 

##1st step: Install the Dependencies
```Bash
sudo apt-get -y --force-yes install libopencv-dev build-essential cmake git libgtk2.0-dev pkg-config python-dev python-numpy libdc1394-22 libdc1394-22-dev libjpeg-dev libpng12-dev libtiff4-dev libjasper-dev libavcodec-dev libavformat-dev libswscale-dev libxine-dev libgstreamer0.10-dev libgstreamer-plugins-base0.10-dev libv4l-dev libtbb-dev libqt4-dev libfaac-dev libmp3lame-dev libopencore-amrnb-dev libopencore-amrwb-dev libtheora-dev libvorbis-dev libxvidcore-dev x264 v4l-utils unzip libvtk5-dev
```

##2nd step: Install OpenCV
```Bash
git clone https://github.com/Itseez/opencv.git
git clone https://github.com/Itseez/opencv_contrib.git

cd opencv
mkdir build
cd build
cmake -D -DOPENCV_EXTRA_MODULES_PATH=../../opencv_contrib/modules CMAKE_BUILD_TYPE=Release -D CMAKE_INSTALL_PREFIX=/usr/local ..
make -j4 # runs 4 jobs in parallel
sudo make install
```

##3th Download, Make and Run Showcase
```Bash
cd ..
wget https://github.com/JosefSommerauer/hbg-tdot-demo/archive/master.zip -O tdot.zip
unzip tdot.zip
rm tdot.zip
cd hbg-tdot-demo-master/
make
chmod +x run_showcase.sh 
sudo su
./run_showcase.sh 
```








