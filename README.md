# ObjectRecognition  

This started as a course final project a year ago. I want to keep improving it and using it as a playground for my own Computer Vision learning.  


The easiest way to make this work:
```
docker pull spmallick/opencv-docker:opencv
xhost +local:docker
docker run --device=/dev/video0:/dev/video0 --device=/dev/video1:/dev/video1 -v /tmp/.X11-unix:/tmp/.X11-unix -v ~/Repos/ObjectRecognition:/work -e DISPLAY=$DISPLAY -p 5000:5000 -p 8888:8888 -it spmallick/opencv-docker:opencv /bin/bash
```
