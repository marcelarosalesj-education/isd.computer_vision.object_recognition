# simpleObjectRecognition  

Setting:  
A web camera pointing to a solid-colored background with a couple of solid-color objects on it.  
(It can be a cardboard of one color and the figures of a different one)  

Input:  
Digital image of the scene.  

Process:  
1) Changing of color space (to HSV)  
2) Color quantization (using kmeans or a predefined threshold value or the arithmetic mean of some values given by the user at that moment).  
3) Get Hu moments of the object.  
4) Compare with a database previously calculated that is containing the relationship ObjectName - HuMoments.  
5) Throw the result. Is it correct?  

Output:  
Object value  
