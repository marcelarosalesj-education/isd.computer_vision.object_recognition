//#include <opencv2/contrib/contrib.hpp> // had to remove for opencv3
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp" // I added this to make it work with opencv3
//#include <cv.h>
#include <string>
#include <iostream>
#include <fstream>
#include <queue>
#include <stdio.h>
#include <stdlib.h>
#include <string>

using namespace cv;
using namespace std;
string file_name;
bool stop = false;
bool freeze = false;
Mat imgClick;

/*
    Prints 
    
    @params binarized image and  number of regions to look for.
    @return nothing.
*/
void mouseCoordinatesExampleCallback(int event, int x, int y, int flags, void* param)
{
    uchar* destination;
    vector<Point> points;
    int Px;
    int Py;
    int vR;
    int vG;
    int vB;
    float rpi;
    float gpi;
    float bpi;
    float vH;
    float vS;
    float vV;
    float Cmax;
    float Cmin;
    float dif;
    int conta=0;
    float Ahsv[50][3];
    float Hmax, Hmin, Smax, Smin, Vmax, Vmin;
    float Hmax2=0;
    float Hmin2=0;
    switch (event)
    {
        case CV_EVENT_LBUTTONDOWN:

            

            freeze = true;
            Px=x;
            Py=y;
            destination = (uchar*) imgClick.ptr<uchar>(Py);
            
            vB=destination[Px * 3];
            vG=destination[Px*3+1];
            vR=destination[Px*3+2];
            //Valores HSV
            rpi = vR / 255.0;
            gpi = vG / 255.0;
            bpi = vB / 255.0;
            
            Cmax = max(rpi, gpi);
            Cmax = max(Cmax, bpi);
            Cmin = min(rpi, gpi);
            Cmin = min(Cmin, bpi);
            dif = Cmax - Cmin;
            if(dif == 0){
                vH=0;
            } else if(Cmax == rpi){
                vH = 60.0 * (fmod(((gpi-bpi)/dif),6.0));
            } else if(Cmax == gpi){
                vH = 60.0 * (((bpi-rpi)/dif)+2.0);
            } else if(Cmax == bpi){
                vH = 60.0 * (((rpi-gpi)/dif)+4.0);
            }
            if(vH<0){
                vH=360+vH;
            }
            if(Cmax==0){
                vS = 0;
            } else {
                vS = dif/Cmax;
            }
            vV = Cmax;
            
            cout << vH << "  " << vS << "  " << vV<< endl;
            if(conta < 30){
                Ahsv[conta][0] = vH;
                Ahsv[conta][1] = vS;
                Ahsv[conta][2] = vV;
                conta++;
            }
            /*  Draw a point */
            points.push_back(Point(Px, Py));
            break;
        case CV_EVENT_MOUSEMOVE:
            break;
        case CV_EVENT_LBUTTONUP:
            break;
        case CV_EVENT_RBUTTONDOWN:
            freeze=true;
            cout << "boton derecho"<<endl;
        //flag=!flag;
            break;
        
    }
}

/*
    Displays the number of regions found.
    
    @params binarized image and  number of regions to look for.
    @return nothing.
*/
void regionGrowing(Mat img, short num) {
    int tempx, tempy;
    int lon = img.cols;
    int lat = img.rows;
    int area[num];
    // For two dimensional and multichannel images we first define their size: row and column count wise.
    // For instance, CV_8UC3 means we use unsigned char types that are 8 bit long and each pixel has three of these to form the three channels.
    // http://docs.opencv.org/doc/tutorials/core/mat_the_basic_image_container/mat_the_basic_image_container.html
    
    // This color matrix is to map what it has been found. It's a white matrix.
    Mat color(lat, lon, CV_8UC3, Scalar(255,255,255)); 
    int colB=30, colG=100,colR=200;
    //imshow("COLOR: ", color); 
    //cout << " CH of color: "<<color.channels()<<endl;
    bool found=false, finished=false;
    
    //Iterate up to all regions are found. 
    //WARNING: It is super important to give the right amoung of regions in the image.
    //Otherwise, this will loop :(
    for(int i=0; i<num; i++){
        area[i]=0;
        // Do while you're not finding a region.
        do {
            // Generate random coordinates
            tempx = rand() % lon;
            tempy = rand() % lat;
            //cout<<"random > "<< i+1<<" : ( "<< tempx<<" , "<<tempy<<" ) VALUE OF PIXEL >"<<(int)img.at<uchar>(Point(tempx,tempy))<<endl;
            //cout<<"valor en color ---->B:"<<(int)color.at<Vec3b>(Point(tempx,tempy))[0]<<" G:"<<(int)color.at<Vec3b>(Point(tempx,tempy))[1]<<" R:"<<(int)color.at<Vec3b>(Point(tempx,tempy))[2]<<endl;
            // http://docs.opencv.org/doc/user_guide/ug_mat.html "Note the ordering of x and y"
            
            //This if finds regions.
            //This means, img has a value that is black & color is white.
            if(img.at<uchar>(Point(tempx,tempy)) == 0 && color.at<Vec3b>(Point(tempx,tempy))[0]==255
                && color.at<Vec3b>(Point(tempx,tempy))[1]==255 && color.at<Vec3b>(Point(tempx,tempy))[2]==255){
                found=true;
                // Sets the seed and paint it of whatever color.
                color.at<Vec3b>(Point(tempx,tempy))[0]=colB;
                color.at<Vec3b>(Point(tempx,tempy))[1]=colG;
                color.at<Vec3b>(Point(tempx,tempy))[2]=colR;
                area[i]++;
                queue<int> fila;
                
                // Grow the seed up to discover the whole region.
                while(finished!=true){
                    // North
                    if(img.at<uchar>(Point(tempx,tempy-1)) == 0 && color.at<Vec3b>(Point(tempx,tempy-1))[0]==255
                        && color.at<Vec3b>(Point(tempx,tempy-1))[1]==255 && color.at<Vec3b>(Point(tempx,tempy-1))[2]==255){
                        fila.push(tempx); // mete x
                        fila.push(tempy-1); // mete y
                        color.at<Vec3b>(Point(tempx,tempy-1))[0]=colB;
                        color.at<Vec3b>(Point(tempx,tempy-1))[1]=colG;
                        color.at<Vec3b>(Point(tempx,tempy-1))[2]=colR;
                        area[i]++;

                    }
                    // West
                    if(img.at<uchar>(Point(tempx-1,tempy)) == 0 && color.at<Vec3b>(Point(tempx-1,tempy))[0]==255 
                        && color.at<Vec3b>(Point(tempx-1,tempy))[1]==255 && color.at<Vec3b>(Point(tempx-1,tempy))[2]==255){
                        fila.push(tempx-1); // mete x
                        fila.push(tempy); // mete y
                        color.at<Vec3b>(Point(tempx-1,tempy))[0]=colB;
                        color.at<Vec3b>(Point(tempx-1,tempy))[1]=colG;
                        color.at<Vec3b>(Point(tempx-1,tempy))[2]=colR;
                        area[i]++;
                    }
                    // South
                    if(img.at<uchar>(Point(tempx,tempy+1)) == 0 && color.at<Vec3b>(Point(tempx,tempy+1))[0]==255 
                        && color.at<Vec3b>(Point(tempx,tempy+1))[1]==255 && color.at<Vec3b>(Point(tempx,tempy+1))[2]==255){
                        fila.push(tempx); // mete x
                        fila.push(tempy+1); // mete y
                        color.at<Vec3b>(Point(tempx,tempy+1))[0]=colB;
                        color.at<Vec3b>(Point(tempx,tempy+1))[1]=colG;
                        color.at<Vec3b>(Point(tempx,tempy+1))[2]=colR;
                        area[i]++;
                    }
                    // East
                    if(img.at<uchar>(Point(tempx+1,tempy)) == 0 && color.at<Vec3b>(Point(tempx+1,tempy))[0]==255 
                        && color.at<Vec3b>(Point(tempx+1,tempy))[1]==255 && color.at<Vec3b>(Point(tempx+1,tempy))[2]==255){
                        fila.push(tempx+1); // mete x
                        fila.push(tempy); // mete y
                        color.at<Vec3b>(Point(tempx+1,tempy))[0]=colB;
                        color.at<Vec3b>(Point(tempx+1,tempy))[1]=colG;
                        color.at<Vec3b>(Point(tempx+1,tempy))[2]=colR;
                        area[i]++;
                    }
                    
                    // Growing Cicle keeps doing while queue is not empty.
                    // Because it means that it keeps finding pixels.
                    if (fila.empty()){
                        finished=true;
                    } else {
                        tempx = fila.front();
                        fila.pop(); // get next x
                        tempy = fila.front();
                        fila.pop(); // get next y
                    }
                } // End of while. It finished finding a region.

                // Color changes for next region. 
                if(i%3 == 0)
                    colB+=20;
                else if (i%3 == 1)
                    colG+=40;
                else if (i%3 == 2)
                    colR+=60;
                if(colB>=255) colB=0;
                if(colG>=255) colG=0;
                if(colR>=255) colR=0;

                // Reset finish variable, another region must be found.
                finished=false;
                
                imshow("Result: ", color);
                cout<<"Object Area = "<<area[i]<<" px."<<endl;

                waitKey(0);
            }
        } while(found != true); // New Region's Pixel found?

        // Clear for the next pixel finding. 
        found=false;
    }

    imwrite( "./img_out/"+file_name+"_result.jpg", color);
     
}

VideoCapture cap(1);

int main()
{
	Mat img;
    short num_seed;
    cout<<"How many regions to find?"<<endl;
        cin >> num_seed;

    while(stop == false){


        cap >> img;


        imshow("Cam", img);
        setMouseCallback("Cam", mouseCoordinatesExampleCallback);
        if(!freeze){
            imgClick = img;
            namedWindow("Click");
            setMouseCallback("Click", mouseCoordinatesExampleCallback);
            imshow("Click", imgClick);
        }



        threshold(img, img, 50,255,THRESH_BINARY);
        imshow("Binary image ", img); // Binarized the image, it only contain 0s and 255s
        cout << "CH : "<<img.channels()<<endl;
 
        char key = waitKey(5);
        switch (key) {
            case 'm': cout << "Region Growing Algorithm about to start \n" << endl; regionGrowing(img, num_seed); break;
            case 27: stop = true; break;
        }

    }
    
    cout<<"Goodbye!"<<endl;
    return 0;
}
