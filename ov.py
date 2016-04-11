import numpy as np
2 import cv2
3 
4 cap = cv2.VideoCapture('slow.flv')
5 
6 # params for ShiTomasi corner detection
7 feature_params = dict( maxCorners = 100,
8  qualityLevel = 0.3,
9  minDistance = 7,
10  blockSize = 7 )
11 
12 # Parameters for lucas kanade optical flow
13 lk_params = dict( winSize = (15,15),
14  maxLevel = 2,
15  criteria = (cv2.TERM_CRITERIA_EPS | cv2.TERM_CRITERIA_COUNT, 10, 0.03))
16 
17 # Create some random colors
18 color = np.random.randint(0,255,(100,3))
19 
20 # Take first frame and find corners in it
21 ret, old_frame = cap.read()
22 old_gray = cv2.cvtColor(old_frame, cv2.COLOR_BGR2GRAY)
23 p0 = cv2.goodFeaturesToTrack(old_gray, mask = None, **feature_params)
24 
25 # Create a mask image for drawing purposes
26 mask = np.zeros_like(old_frame)
27 
28 while(1):
29  ret,frame = cap.read()
30  frame_gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
31 
32  # calculate optical flow
33  p1, st, err = cv2.calcOpticalFlowPyrLK(old_gray, frame_gray, p0, None, **lk_params)
34 
35  # Select good points
36  good_new = p1[st==1]
37  good_old = p0[st==1]
38 
39  # draw the tracks
40  for i,(new,old) in enumerate(zip(good_new,good_old)):
41  a,b = new.ravel()
42  c,d = old.ravel()
43  mask = cv2.line(mask, (a,b),(c,d), color[i].tolist(), 2)
44  frame = cv2.circle(frame,(a,b),5,color[i].tolist(),-1)
45  img = cv2.add(frame,mask)
46 
47  cv2.imshow('frame',img)
48  k = cv2.waitKey(30) & 0xff
49  if k == 27:
50  break
51 
52  # Now update the previous frame and previous points
53  old_gray = frame_gray.copy()
54  p0 = good_new.reshape(-1,1,2)
55 
56 cv2.destroyAllWindows()
57 cap.release()
