//Opencv headers
#include "<cv.h>"
#include "<highgui.h>"
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/legacy/legacy.hpp>
#include <opencv2/legacy/compat.hpp>
#include <opencv2/video/background_segm.hpp>

#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <fstream>
#include <iostream>
#include <sstream>


/***************************** Inline Functions ******************************/

extern CvRect obtenerBoundBox(IplImage* mascNueva);

extern IplImage* calcMedia(IplImage* imgPeque1);

extern CvPoint *obtenerPuntosConvexHull(CvSeq *p);

extern CvRect obtenerInterseccion(CvRect r1, CvRect r2);

static double getUmbral_Otsu( IplImage* image);

extern IplImage *shadowRemoval2(IplImage *original,IplImage *fondo, IplImage * mascara, double umbral, CvRect boundbox,double area, int tamSe1, int tamSe2, bool Et1o2);

IplImage *calculoGradiente(IplImage *original, IplImage *fondo, double umbral, bool Et1o2);

IplImage *refinadoMascara(IplImage *maskG,int tamSeMask);

IplImage *calculoDBt(IplImage *maskG,int tamSe1,int tamSe2);

IplImage *calculoIEt(IplImage *DBt, IplImage *Et, IplImage *maskG, int n);

IplImage *procesadoHorizontal(IplImage *auxiliar);

IplImage *procesadoVertical(IplImage *auxiliar);

IplImage *eliminacionRuido(IplImage *IHt,double porcentajeArea,double area);

std::vector <IplImage*> relabelH(CBlobResult blobs, int td, CvSize size);
std::vector <IplImage*>  relabelV(CBlobResult blobs, int td, CvSize size);
int calculoUmbraltd(CBlobResult blobsH,CBlobResult blobsVaux, double alfa);
