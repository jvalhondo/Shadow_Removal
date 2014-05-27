
#include "shadow_removal.h"

using namespace std;
using namespace cv;


extern CvRect obtenerBoundBox(IplImage* mascNueva)
{
	CvSize imgSize = cvGetSize(mascNueva);
	int x,y,x2,y2=0;
	int x1,y1,x12,y12;
	int i,j;
	// para encontrar las cuatro coordenadas que delimitan el boundBox es necesario
	// recorrer la imagen de cuatro maneras.

	// el punto m�s alto se obtiene recorriendo la imagen por l�neas de arriba a abajo, el primer valor
	// distinto de cero encontrado proporciona la coordenada x de la esquina superior izquierda.
	int flag = 0;
	for (i = 0; i < imgSize.height; i++ )
	{
		for (j = 0; j < imgSize.width; j++ )
		{

			CvScalar aux = cvGet2D(mascNueva,i,j);	
			if(aux.val[0]!=0)
			{
				// esquina superior izquierda (X)
				x = i;
				flag = 1;
				break;
			}	
		}
		if (flag==1)
			break;
	}
	// el punto m�s bajo se obtiene recorriendo la imagen por l�neas de abajo a arriba, el primer valor
	// distinto de cero encontrado proporciona la coordenada x de la esquina inferior derecha.
	flag = 0;
	for (i = imgSize.height-1; i >= 0; i--)
	{
		for (j = 0; j < imgSize.width; j++ )
		{
			CvScalar aux = cvGet2D(mascNueva,i,j);
			if(aux.val[0]!=0)
			{
				// esquina inferior derecha (Y)
				x2 = i;
				flag = 1;
				break;
			}	
		}
		if (flag ==1)
			break;
	}
	// el punto m�s a la izquierda se obtiene recorriendo la imagen por columnas de izquierda a derecha, el primer valor
	// distinto de cero encontrado proporciona la coordenada y de la esquina superior izquierda.
	flag = 0;
	for (j = 0; j < imgSize.width; j++ )
	{
		for (i = 0; i < imgSize.height; i++ )
		{
			CvScalar aux = cvGet2D(mascNueva,i,j);
			if(aux.val[0]!=0)
			{
				// esquina superior izquierda (Y)
				y = j;
				flag = 1;
				break;
			}	
		}
		if (flag ==1)
			break;
	}

	// el punto m�s a la derecha se obtiene recorriendo la imagen por columnas de derecha a izquierda, el primer valor
	// distinto de cero encontrado proporciona la coordenada y de la esquina inferior derecha.
	flag = 0;
	for (j = imgSize.width-1; j >= 0; j--)
	{
		for (i = 0; i < imgSize.height; i++ )
		{
			CvScalar aux = cvGet2D(mascNueva,i,j);
			if(aux.val[0]!=0)
			{
				// esquina inferior derecha (Y)
				y2 = j;
				flag  = 1;
				break;
			}	
		}
		if (flag ==1)
			break;
	}
	CvRect rect = cvRect(y,x,y2-y,x2-x);
	return rect;
}

extern IplImage* calcMedia(IplImage* imgPeque1)
{
	IplImage *imgMedia = cvCreateImage(cvGetSize(imgPeque1),imgPeque1->depth, 1);

	CvSize imgSize = cvGetSize(imgPeque1);

	// se guarda la imagen de m�scara con zero padding
	CvScalar s1;	//
	CvScalar s2;	// 1-----2-----3
	CvScalar s3;	// |	 |	   |
	CvScalar s4;	// |	 |	   |
	CvScalar s5;	// 4 ----5-----6
	CvScalar s6;	// |	 |	   |
	CvScalar s7;	// |	 |	   |
	CvScalar s8;	// 7-----8-----9
	CvScalar s9;
	double val[9];	

	for ( int i = 0; i < imgSize.height; i++ )
	{
		for ( int j = 0; j < imgSize.width; j++ )
		{
			if ((i == 0) & (j==0))
			{
				s1 = cvGet2D(imgPeque1,i,j);	
				s2 = cvGet2D(imgPeque1,i,j);		
				s3 = cvGet2D(imgPeque1,i,j+1);	
				s4 = cvGet2D(imgPeque1,i,j);		
				s5 = cvGet2D(imgPeque1,i,j);		
				s6 = cvGet2D(imgPeque1,i,j+1);		
				s7 = cvGet2D(imgPeque1,i+1,j);	
				s8 = cvGet2D(imgPeque1,i+1,j);		
				s9 = cvGet2D(imgPeque1,i+1,j+1);	
			}


			if((i == 0) & (j!=0) & (j !=imgSize.width-1))
			{
				s1= cvGet2D(imgPeque1,i,j-1);	
				s2 = cvGet2D(imgPeque1,i,j);		
				s3 = cvGet2D(imgPeque1,i,j+1);	
				s4 = cvGet2D(imgPeque1,i,j-1);		
				s5 = cvGet2D(imgPeque1,i,j);		
				s6 = cvGet2D(imgPeque1,i,j+1);		
				s7 = cvGet2D(imgPeque1,i+1,j-1);	
				s8 = cvGet2D(imgPeque1,i+1,j);		
				s9 = cvGet2D(imgPeque1,i+1,j+1);
			}

			if((i == 0) & (j == imgSize.width-1))
			{
				s1= cvGet2D(imgPeque1,i,j-1);	
				s2 = cvGet2D(imgPeque1,i,j);		
				s3 = cvGet2D(imgPeque1,i,j);	
				s4 = cvGet2D(imgPeque1,i,j-1);		
				s5 = cvGet2D(imgPeque1,i,j);		
				s6 = cvGet2D(imgPeque1,i,j);		
				s7 = cvGet2D(imgPeque1,i+1,j-1);	
				s8 = cvGet2D(imgPeque1,i+1,j);		
				s9 = cvGet2D(imgPeque1,i+1,j);

			}

			if((i == imgSize.height-1) & (j ==imgSize.width-1))
			{
				s1= cvGet2D(imgPeque1,i-1,j-1);	
				s2 = cvGet2D(imgPeque1,i-1,j);		
				s3 = cvGet2D(imgPeque1,i-1,j);	
				s4 = cvGet2D(imgPeque1,i,j-1);		
				s5 = cvGet2D(imgPeque1,i,j);		
				s6 = cvGet2D(imgPeque1,i,j);		
				s7 = cvGet2D(imgPeque1,i,j-1);	
				s8 = cvGet2D(imgPeque1,i,j);		
				s9 = cvGet2D(imgPeque1,i,j);
			}
			if((i == imgSize.height-1) & (j !=imgSize.width-1) & (j != 0))
			{
				s1= cvGet2D(imgPeque1,i-1,j-1);	
				s2 = cvGet2D(imgPeque1,i-1,j);		
				s3 = cvGet2D(imgPeque1,i-1,j+1);	
				s4 = cvGet2D(imgPeque1,i,j-1);		
				s5 = cvGet2D(imgPeque1,i,j);		
				s6 = cvGet2D(imgPeque1,i,j+1);		
				s7 = cvGet2D(imgPeque1,i,j-1);	
				s8 = cvGet2D(imgPeque1,i,j);		
				s9 = cvGet2D(imgPeque1,i,j+1);
			}
			if((j == 0) & (i!=0) & (i != imgSize.height-1))
			{
				s1= cvGet2D(imgPeque1,i-1,j);	
				s2 = cvGet2D(imgPeque1,i-1,j);		
				s3 = cvGet2D(imgPeque1,i-1,j+1);	
				s4 = cvGet2D(imgPeque1,i,j);		
				s5 = cvGet2D(imgPeque1,i,j);		
				s6 = cvGet2D(imgPeque1,i,j+1);		
				s7 = cvGet2D(imgPeque1,i+1,j);	
				s8 = cvGet2D(imgPeque1,i+1,j);		
				s9 = cvGet2D(imgPeque1,i+1,j+1);

			}
			if((j == 0) & (i == imgSize.height-1))
			{
				s1= cvGet2D(imgPeque1,i-1,j);	
				s2 = cvGet2D(imgPeque1,i-1,j);		
				s3 = cvGet2D(imgPeque1,i-1,j+1);	
				s4 = cvGet2D(imgPeque1,i,j);		
				s5 = cvGet2D(imgPeque1,i,j);		
				s6 = cvGet2D(imgPeque1,i,j+1);		
				s7 = cvGet2D(imgPeque1,i,j);	
				s8 = cvGet2D(imgPeque1,i,j);		
				s9 = cvGet2D(imgPeque1,i,j+1);

			}
			if((j == imgSize.width-1) & (i != imgSize.height-1)& (i != 0))
			{
				s1= cvGet2D(imgPeque1,i-1,j-1);	
				s2 = cvGet2D(imgPeque1,i-1,j);		
				s3 = cvGet2D(imgPeque1,i-1,j);	
				s4 = cvGet2D(imgPeque1,i,j-1);		
				s5 = cvGet2D(imgPeque1,i,j);		
				s6 = cvGet2D(imgPeque1,i,j);		
				s7 = cvGet2D(imgPeque1,i+1,j-1);	
				s8 = cvGet2D(imgPeque1,i+1,j);		
				s9 = cvGet2D(imgPeque1,i+1,j);
			}
			if ((i != 0) & (j!=0) & (j != imgSize.width-1) & (i != imgSize.height-1))
			{
				s1= cvGet2D(imgPeque1,i-1,j-1);	
				s2 = cvGet2D(imgPeque1,i-1,j);		
				s3 = cvGet2D(imgPeque1,i-1,j+1);	
				s4 = cvGet2D(imgPeque1,i,j-1);		
				s5 = cvGet2D(imgPeque1,i,j);		
				s6 = cvGet2D(imgPeque1,i,j+1);		
				s7 = cvGet2D(imgPeque1,i+1,j-1);	
				s8 = cvGet2D(imgPeque1,i+1,j);		
				s9 = cvGet2D(imgPeque1,i+1,j+1);	
			}
			double media = (s1.val[0]+s2.val[0]+s3.val[0]+s4.val[0]+s5.val[0]+s6.val[0]+s7.val[0]+s8.val[0]+s9.val[0])/9;
			cvSet2D(imgMedia,i,j,cvScalar(media,0,0));
		}
	}
	return imgMedia;
}

extern CvRect obtenerInterseccion(CvRect r1, CvRect r2)
{
	CvRect intersection;
	intersection.x = (r1.x < r2.x) ? r2.x : r1.x;
	intersection.y = (r1.y < r2.y) ? r2.y : r1.y;
	intersection.width = (r1.x + r1.width < r2.x + r2.width) ?
		r1.x + r1.width : r2.x + r2.width;
	intersection.width -= intersection.x;
	intersection.height = (r1.y + r1.height < r2.y + r2.height) ?
		r1.y + r1.height : r2.y + r2.height;
	intersection.height -= intersection.y;    

	// check for non-overlapping regions
	if ((intersection.width <= 0) || (intersection.height <= 0)) {
		intersection = cvRect(0, 0, 0, 0);
	} 

	return intersection;


}

// M�todo de eliminaci�n de sombras basado en el paper "Moving Shadow Detection and Removal for Traffic Sequences"
extern IplImage* shadowRemoval2(IplImage *original, IplImage *fondo, IplImage * mascara, double umbral, CvRect boundbox, double area, int tamSe1, int tamSe2,bool Et1o2)
{

	//Declaracion imagen de bordes
	IplImage *Et;
	//Im�genes "ampliadas" para permitir operaciones morfol�gicas
	int n = 10;
	double porcentajeArea = 0.03;
	IplImage *EtG, *maskG, *maskG2;

	//tama�o de los elementos estructurales empleados para la obtenci�n de DBt
	int tamSeMask;
	tamSeMask = 5;

	// Imagen reborde del objeto
	IplImage *DBt, *IEt, *IEt2, *IHt,*IVt, *IHtR, *IVtR, *REt ,*HREt, *VREt, *FR,*FR2, *ISt, *St, *Final, *Final2;
	//Se crean las im�genes ampliadas
	EtG = cvCreateImage(cvSize(original->width+2*n,original->height+2*n),IPL_DEPTH_8U,1);
	maskG = cvCreateImage(cvSize(original->width+2*n,original->height+2*n),IPL_DEPTH_8U,1);
	maskG2 = cvCreateImage(cvSize(original->width+2*n,original->height+2*n),IPL_DEPTH_8U,1);
	REt = cvCreateImage(cvSize(original->width+2*n,original->height+2*n),IPL_DEPTH_8U,1);
	FR = cvCreateImage(cvSize(original->width+2*n,original->height+2*n),IPL_DEPTH_8U,1);
	FR2 = cvCreateImage(cvSize(original->width+2*n,original->height+2*n),IPL_DEPTH_8U,1);
	ISt = cvCreateImage(cvSize(original->width+2*n,original->height+2*n),IPL_DEPTH_8U,1);
	St = cvCreateImage(cvSize(original->width+2*n,original->height+2*n),IPL_DEPTH_8U,1);
	Final = cvCreateImage(cvSize(original->width,original->height),IPL_DEPTH_8U,1);
	Final2 = cvCloneImage(Final);
	/////////////// FIN DECLARACI�N DE VARIABLES //////////////////////////

	//1. OBTENCI�N DE LA IMAGEN DE BORDES BINARIZADA (OTSU)
	// Se obtiene la imagen gradiente de la imagen original (teniendo en cuenta el fondo
	// y el par�metro umbral que controla la binarizaci�n
	Et = calculoGradiente(original,fondo,umbral,Et1o2);

	//Se copian las im�genes en una 2*n p�xeles mayor en cada dimension
	cvZero(EtG);
	cvSetImageROI(EtG,cvRect(n,n,original->width,original->height));
	cvCopy(Et,EtG,0);
	cvResetImageROI(EtG);

	//2. C�LCULO M�SCARA FG REFINADA
	//Se aplican operaciones morfol�gicas a la m�scara para obtener una m�scara 
	//sin agujeros y lo m�s compacta posible
	cvZero(maskG);
	cvSetImageROI(maskG,cvRect(n,n,original->width,original->height));
	cvCopy(mascara,maskG,0);
	cvResetImageROI(maskG);
	maskG = refinadoMascara(maskG,tamSeMask);
	cvCopy(maskG,maskG2);

	//3. C�LCULO DE LA IMAGEN DBt (REBORDE EXTERIOR DEL OBJETO)
	//Se obtiene la imagen DBt que representa el reborde exterior del objeto
	//Su grosor depender� del tama�o de las m�scaras
	//tamSe1 = 1;
	//tamSe2 = 2;
	//	 
	//if(boundbox.y+ original->height>120)
	//{ 
	//	tamSe1 = 1; // 3x3
	//	tamSe2 = 3; // 7x7
	//}

	DBt = calculoDBt(maskG2,tamSe1,tamSe2);
	//4. OBTENCI�N DE LA IMAGEN DE BORDES INTERIORES DEL OBJETO
	//obtenci�n de la imagen de bordes interiores
	IEt = calculoIEt(DBt,Et,maskG,n);
	IEt2 = cvCloneImage(IEt);
	//5. PROCESADO DE FILAS Y COLUMNAS DE LA IMAGEN DE BORDES INTERIORES
	//Procesado de filas y columnas de la imagen de bordes interioes
	//Obtenci�n de IHt e IVt respectivamente
	IHt = procesadoHorizontal(IEt);
	IVt = procesadoVertical(IEt);

	//6. ELIMINACI�N DE REGIONES PEQUE�AS (LIMIPIEZA DE RUIDO)
	//Hay que aplicar un proceso de refinado eliminando los blobs que
	// tengan un tama�o menor al 5% del total
	IHtR = eliminacionRuido(IHt,porcentajeArea,area);
	IVtR = eliminacionRuido(IVt,porcentajeArea,area);

	// se obtiene la imagen REt resultado de aplicar la operaci�n AND a las dos 
	// im�genes previamente obtenidas 
	cvAnd(IHtR,IVtR,REt,0);


	// 7. NUEVO PROCESADO FILAS Y COLUMNAS
	HREt = procesadoHorizontal(REt);
	VREt = procesadoVertical(REt);

	CvFont font;
	cvInitFont( &font, CV_FONT_HERSHEY_PLAIN, 0.7, 0.7, 0, 1, CV_AA);

	//Entra VREt y HREt
	// 8. PROCESO DE RE-ETIQUETADO DE LAS REGIONES OBTENIDAS EN BASE A CERCAN�A
	CBlobResult blobsVaux;
	CBlobResult blobsH;
	CBlobResult blobsV;
	blobsVaux = CBlobResult(VREt, NULL, 0 );
	blobsH = CBlobResult(HREt, NULL, 0 );

	CBlob aux;
	char indice[2];

	//Ordeno los blobs obtenidos del procesamiento vertical para poder 
	//unir/no unir los m�s cercanos.
	for(int k=0;k<blobsVaux.GetNumBlobs();k++)
	{
		blobsVaux.GetNthBlob(CBlobGetMinX(),blobsVaux.GetNumBlobs()-1-k,aux);
		blobsV.AddBlob(&aux);
	}

	int td = calculoUmbraltd(blobsH,blobsV,0.1);
	vector <IplImage*> imagenesH;
	vector <IplImage*> imagenesV;
	vector <IplImage*> imagenesIntersect;
	imagenesH = relabelH(blobsH,td,cvGetSize(VREt));
	imagenesV = relabelV(blobsV,td,cvGetSize(VREt));

	//9. UNA VEZ RE-ETIQUETADOS SE OBTIENE LOS OBJETOS NO-SOMBRA EXISTENTES
	CBlobResult b_result;
	for(int i=0;i<imagenesH.size();i++)
	{
		for(int j=0;j<imagenesV.size();j++)
		{	
			IplImage* inter = cvCreateImage(cvGetSize(VREt),8,1);
			cvAnd(imagenesH[i],imagenesV[j],inter,0);
			b_result = CBlobResult(inter, NULL,0);
			if(b_result.GetNumBlobs()!=0) //existe intersecci�n
			{
				imagenesIntersect.push_back(inter);
			}
			else
			{
				cvReleaseImage(&inter);
			}
		}
	}
	for(int t=0;t<imagenesIntersect.size();t++)
	{
		//Sobre cada imagen de intersecci�n se aplican el procesado por l�neas y por columnas
		HREt = procesadoHorizontal(imagenesIntersect[t]);
		VREt = procesadoVertical(imagenesIntersect[t]);
		cvOr(HREt,VREt,FR,0);
		cvCopy(FR,imagenesIntersect[t],0);
		if(t>0)
			cvAdd(imagenesIntersect[t],imagenesIntersect[t-1],imagenesIntersect[t],0);
	}

	//10. SE EXTRAE LA REGI�N SEMILLA DE SOMBRA Y SE ELIMINA DE OBJETOS
	///////////////// Im�genes Intersect ////////////
	if(imagenesIntersect.size()>0)
		cvCopy(imagenesIntersect[imagenesIntersect.size()-1],FR,0);
	else
		cvZero(FR);

	cvZero(IEt);
	cvNot(DBt,DBt);
	cvSub(maskG2,DBt,IEt);
	cvSub(IEt,FR,ISt);

	//Se obtienen los blobs de ISt para obtener la "seed Region" de sombra
	b_result = CBlobResult(ISt, NULL,0);
	//cojo las regiones con un tama�o mayor que el 1% del total 
	b_result.Filter(b_result ,B_EXCLUDE,CBlobGetArea(),B_LESS,0.005*area,0);
	CBlobResult blobsShadow;
	double_stl_vector areas;
	if(b_result.GetNumBlobs()>0)
	{  
		areas  = b_result.GetSTLResult(CBlobGetArea());
		cvZero(ISt);
		cvZero(St);
		CvRect bboxShadow;

		if(b_result.GetNumBlobs()>1)
		{
			cvZero(St);
		}
		for(int k=0; k<b_result.GetNumBlobs();k++)
		{
			bboxShadow = b_result.GetBlob(k)->GetBoundingBox();
			bboxShadow.x = bboxShadow.x-7;
			bboxShadow.y = bboxShadow.y-7;
			bboxShadow.width = bboxShadow.width+14;
			bboxShadow.height = bboxShadow.height+14;

			cvSetImageROI(DBt,bboxShadow);
			cvSetImageROI(St,bboxShadow);
			cvCopy(DBt,St,0);
			cvResetImageROI(DBt);
			cvResetImageROI(St);
			b_result.GetBlob(k)->FillBlob(ISt,CV_RGB(255,255,255));	
			cvAdd(St,ISt,St,0);
		}
		cvSub(maskG2,St,FR2,0);
		cvOr(FR,FR2,FR,0);

	}

	//Vac�o los vectores de im�genes
	for(int t=0;t<imagenesIntersect.size();t++)
	{
		cvReleaseImage(&imagenesIntersect[t]);
	}
	imagenesIntersect.clear();

	for(int t=0;t<imagenesH.size();t++)
	{
		cvReleaseImage(&imagenesH[t]);
	}
	imagenesH.clear();
	for(int t=0;t<imagenesV.size();t++)
	{
		cvReleaseImage(&imagenesV[t]);
	}
	imagenesV.clear();

	cvSetImageROI(FR,cvRect(n,n,original->width,original->height));
	cvZero(Final);
	cvCopy(FR,Final,0);
	cvResetImageROI(FR);

	cvReleaseImage(&EtG);
	cvReleaseImage(&maskG);
	cvReleaseImage(&maskG2);
	cvReleaseImage(&DBt);
	cvReleaseImage(&IEt);
	cvReleaseImage(&IHt);
	cvReleaseImage(&IVt);
	cvReleaseImage(&REt);
	cvReleaseImage(&HREt);
	cvReleaseImage(&VREt);
	cvReleaseImage(&FR);

	////Se elimina ruido (viene bien para videos pobremente codificados)
	b_result = CBlobResult(Final, NULL,0);
	//cojo las regiones con un tama�o mayor que el 1% del total 
	b_result.Filter(b_result ,B_EXCLUDE,CBlobGetArea(),B_LESS,0.01*area,0);

	double totalBlanco;
	double totalBlancoEt;
	double vF, vEt,ratioEtF;
	cvZero(Final);

	for(int k=0; k<b_result.GetNumBlobs();k++)
	{
		totalBlanco = 0;
		totalBlancoEt = 0;
		cvZero(Final2);
		b_result.GetBlob(k)->FillBlob(Final2,CV_RGB(255,255,255));

		CvRect bboxF = b_result.GetBlob(k)->GetBoundingBox();

		//Se analiza el contenido de bordes del blob. los p�xeles blancos deben ser m�s del 10%
		for(int i=bboxF.y;i<bboxF.height+bboxF.y;i++)
		{
			const int nChannels = Final2->nChannels;
			for(int j=bboxF.x;j<bboxF.width+bboxF.x;j++)
			{
				const int p = Final2->widthStep*i+j*nChannels;
				vF = (uchar)Final2->imageData[p];		
				if(vF==255) //blanco
				{
					totalBlanco = totalBlanco + 1;
					vEt = (uchar)IEt2->imageData[p];
					if(vEt == 255)
						totalBlancoEt = totalBlancoEt + 1;
				}
			}
		}

		ratioEtF = totalBlancoEt/totalBlanco;
		if(ratioEtF > 0.03)
			b_result.GetBlob(k)->FillBlob(Final,CV_RGB(255,255,255));
		else
		{	
			//cvShowImage("FinalTrozo",Final2);
			//cvShowImage("IEtTrozo",IEt2);
			//cvWaitKey(0);
			//cvDestroyWindow("FinalTrozo");
			//cvDestroyWindow("EtTrozo");
		}
	}
	cvReleaseImage(&Final2);
	cvReleaseImage(&Et);
	cvReleaseImage(&IEt2);
	return Final;
}
IplImage *calculoGradiente(IplImage *original, IplImage *fondo, double umbral, bool Et1o2) 
{
	//Declaraci�n de im�genes para la obtenci�n de la imagen de bordes del objeto
	// H o V horizontal y vertical
	// grad imagen de tipo single de 16 bits
	// bordesX_Y255 imagen de tipo entera, 8 bits (0-255)
	// FO_edge imagen binaria de bordes del objeto (FG)
	// BI_edge imagen binaria de bordes del fondo (BG)

	//imagenes de bordes para el objeto
	IplImage *gradFG_H, *gradFG_V, *bordesFG_H255, *bordesFG_V255, *FO_edge; 

	//imagenes de bordes para el fondo
	IplImage *gradBG_H, *gradBG_V, *bordesBG_H255, *bordesBG_V255, *BI_edge;

	//Im�genes de bordes definitiva (Et) binaria e imagen auxiliar
	// MI_edge = max(FO_edge, BI_edge)
	// Et = MI_edge-BI_edge;
	IplImage *Et, *MI_edge;

	gradFG_H =  cvCreateImage(cvGetSize(original),IPL_DEPTH_16S,1);
	gradFG_V =  cvCreateImage(cvGetSize(original),IPL_DEPTH_16S,1);
	gradBG_H =  cvCreateImage(cvGetSize(original),IPL_DEPTH_16S,1);
	gradBG_V =  cvCreateImage(cvGetSize(original),IPL_DEPTH_16S,1);

	bordesFG_H255 = cvCreateImage(cvGetSize(original),IPL_DEPTH_8U,1);
	bordesFG_V255 = cvCreateImage(cvGetSize(original),IPL_DEPTH_8U,1);
	bordesBG_H255 = cvCreateImage(cvGetSize(original),IPL_DEPTH_8U,1);
	bordesBG_V255 = cvCreateImage(cvGetSize(original),IPL_DEPTH_8U,1);

	FO_edge = cvCreateImage(cvGetSize(original),IPL_DEPTH_8U,1);
	BI_edge = cvCreateImage(cvGetSize(original),IPL_DEPTH_8U,1);
	MI_edge = cvCreateImage(cvGetSize(original),IPL_DEPTH_8U,1);
	Et = cvCreateImage(cvGetSize(original),IPL_DEPTH_8U,1);

	//Se calcula el gradiente horizontal y vertical empleando operadores de Sobel
	// tanto de la imagen original como de la imagen de fondo
	//HORIZONTAL
	cvSobel(original,gradFG_H,0,1,3);
	cvSobel(fondo,gradBG_H,0,1,3);
	cvConvertScaleAbs(gradFG_H,bordesFG_H255,1,0);
	cvConvertScaleAbs(gradBG_H,bordesBG_H255,1,0);
	cvAbsDiff(bordesFG_H255, bordesBG_H255,bordesFG_H255);

	//VERTICAL
	cvSobel(original,gradFG_V,1,0,3);
	cvSobel(fondo,gradBG_V,1,0,3);
	cvConvertScaleAbs(gradFG_V,bordesFG_V255,1,0);
	cvConvertScaleAbs(gradBG_V,bordesBG_V255,1,0);
	cvAbsDiff(bordesFG_V255, bordesBG_V255,bordesFG_V255);

	cvAdd(bordesFG_H255,bordesFG_V255,FO_edge,NULL);
	cvAdd(bordesBG_H255,bordesBG_V255,BI_edge,NULL);

	//Se obtiene la imagen de bordes sobre la que se va a trabajar, para ello se calcula en primer lugar
	if(Et1o2)
	{
		for (int i=0;i<original->height;i++)
		{
			for(int j=0;j<original->width;j++)
			{
				const int nChannels = original->nChannels;
				const int p = original->widthStep*i+j*nChannels;
				double vt1 = (uchar)FO_edge->imageData[p];
				double vt2 = (uchar)BI_edge->imageData[p];
				MI_edge->imageData[p] = max(vt1,vt2);
			}
		}

		cvAbsDiff(MI_edge,BI_edge,Et);
	}
	else
	{
		cvCopy(FO_edge,Et,0);
	}

	//Se umbralizan las im�genes gradiente obteniendo el umbral por el m�todo adaptativo de Otsu
	double threshold =  getUmbral_Otsu(Et);
	if(threshold<30)
	{
		cvThreshold(Et,Et,100,255,CV_THRESH_BINARY);
	}
	else
	{
		cvThreshold(Et,Et,threshold*umbral,255,CV_THRESH_BINARY);
	}

	cvReleaseImage(&MI_edge);
	cvReleaseImage(&BI_edge);
	cvReleaseImage(&FO_edge);
	cvReleaseImage(&gradFG_H);
	cvReleaseImage(&gradFG_V);
	cvReleaseImage(&gradBG_H);
	cvReleaseImage(&gradBG_V);
	cvReleaseImage(&bordesFG_H255);
	cvReleaseImage(&bordesFG_V255);
	cvReleaseImage(&bordesBG_H255);
	cvReleaseImage(&bordesBG_V255);

	return Et;

}

IplImage *refinadoMascara(IplImage *maskG,int tamSeMask)
{
	IplConvKernel* seMask = cvCreateStructuringElementEx(2*tamSeMask+1,2*tamSeMask+1, tamSeMask,tamSeMask,CV_SHAPE_ELLIPSE,0);
	cvDilate(maskG,maskG,seMask,1);
	cvErode(maskG,maskG,seMask,1);

	CBlobResult maskBlobs = CBlobResult(maskG,NULL,0);
	CBlob *current;
	cvZero(maskG);

	for(int contBlobs = 0; contBlobs<maskBlobs.GetNumBlobs();contBlobs++)
	{
		current = maskBlobs.GetBlob(contBlobs);
		current->FillBlob(maskG,CV_RGB(255,255,255));	
	}
	cvReleaseStructuringElement(&seMask);
	maskBlobs.~CBlobResult();
	return maskG;	


}

IplImage *calculoDBt(IplImage *maskG,int tamSe1,int tamSe2)
{
	IplConvKernel* se1 = cvCreateStructuringElementEx(2*tamSe1+1,2*tamSe1+1, tamSe1, tamSe1,CV_SHAPE_ELLIPSE,0);
	IplConvKernel* se2 = cvCreateStructuringElementEx(2*tamSe2+1,2*tamSe2+1, tamSe2, tamSe2,CV_SHAPE_ELLIPSE,0);

	IplImage *auxiliar = cvCreateImage(cvGetSize(maskG),maskG->depth,maskG->nChannels);
	IplImage *DBt = cvCreateImage(cvGetSize(maskG),maskG->depth,maskG->nChannels);
	cvCopy(maskG,DBt,0); 
	cvErode(DBt,auxiliar,se1,1);
	cvAbsDiff(DBt,auxiliar,DBt);
	cvDilate(DBt,auxiliar,se2,1);
	cvAnd(maskG,auxiliar,DBt);
	cvReleaseImage(&auxiliar);
	cvReleaseStructuringElement(&se1);
	cvReleaseStructuringElement(&se2);
	return DBt;
}

IplImage *calculoIEt(IplImage *DBt, IplImage *Et, IplImage *maskG, int n)
{
	IplImage *auxiliar, *EtG;
	auxiliar = cvCreateImage(cvGetSize(maskG),maskG->depth,maskG->nChannels);
	EtG = cvCreateImage(cvGetSize(maskG),maskG->depth,maskG->nChannels);
	cvZero(EtG);
	cvZero(auxiliar);

	cvSetImageROI(EtG,cvRect(n,n,Et->width,Et->height));
	cvCopy(Et,EtG,0);
	cvResetImageROI(EtG);

	cvNot(DBt,DBt);

	cvAnd(EtG,DBt,auxiliar,maskG);
	return auxiliar;

}

IplImage *procesadoHorizontal(IplImage *auxiliar)
{
	IplImage *IHt;
	IHt = cvCreateImage(cvGetSize(auxiliar),auxiliar->depth,1);
	cvZero(IHt);

	int primer = 10000;
	int ultimo = 0;
	int flag;

	for(int i=0;i<auxiliar->height;i++)
	{ 
		const int nChannels = auxiliar->nChannels;
		flag = 1;
		for(int j = 0;j<auxiliar->width;j++)
		{
			const int p = auxiliar->widthStep*i+j*nChannels;
			double v = (uchar)auxiliar->imageData[p];		
			if((v==255) && (flag==1))
			{
				primer = j;
				flag = 0;
			}
			else if((v==255) && (flag==0))
			{
				ultimo = j;
			}
		}
		if(primer!=10000)
		{
			for(int j = primer;j<ultimo+1;j++)
			{		
				IHt->imageData[auxiliar->widthStep*i+j*nChannels] = 255;
			}
		}
		primer=10000;
		ultimo = 0;     
	}
	return IHt;

}

IplImage *procesadoVertical(IplImage *auxiliar)
{
	IplImage *IVt;
	IVt = cvCreateImage(cvGetSize(auxiliar),auxiliar->depth,1);
	cvZero(IVt);

	int primer = 10000;
	int ultimo = 0;
	int flag;

	for(int j = 0;j<auxiliar->width;j++)
	{ 
		const int nChannels = auxiliar->nChannels;
		flag = 1;
		for(int i=0;i<auxiliar->height;i++)
		{
			const int p = auxiliar->widthStep*i+j*nChannels;
			double v = (uchar)auxiliar->imageData[p];		
			if((v==255) && (flag==1))
			{
				primer = i;
				flag = 0;
			}
			else if((v==255) && (flag==0))
			{
				ultimo = i;
			}
		}
		if(primer!=10000)
		{
			for(int i = primer;i<ultimo+1;i++)
			{		
				IVt->imageData[auxiliar->widthStep*i+j*nChannels] = 255;
			}
		}
		primer=10000;
		ultimo = 0;     
	}
	return IVt;

}

IplImage *eliminacionRuido(IplImage *I,double porcentajeArea,double area)
{
	CBlobResult blobs;
	CBlob *currentBlob;

	blobs = CBlobResult(I,NULL,0);
	int totalBlobs = blobs.GetNumBlobs();
	double_stl_vector areas;


	areas            = blobs.GetSTLResult(CBlobGetArea());

	blobs.Filter(blobs,B_EXCLUDE,CBlobGetArea(),B_LESS,area*porcentajeArea,0);

	cvZero(I);

	for(int contBlobs = 0; contBlobs<blobs.GetNumBlobs();contBlobs++)
	{
		currentBlob = blobs.GetBlob(contBlobs);
		currentBlob->FillBlob(I,CV_RGB(255,255,255));	
	}
	return I;

}
vector <IplImage*> relabelH(CBlobResult blobs, int td, CvSize size)
{
	CBlob *currentBlob;
	CBlob *nextBlob;
	int fin1; //final del current blob
	int inicio2; //inicio del next blob
	//distancia ser� inicio2-fin1
	int dist;
	CvRect bbox1;
	CvRect bbox2;
	vector <vector<int>> uniones;
	vector<int> u;
	int contador = 0;
	int merges = 0;

	for(int k=0;k<blobs.GetNumBlobs();k++)
	{
		if(k<blobs.GetNumBlobs()-1)
		{
			currentBlob = blobs.GetBlob(k);
			nextBlob = blobs.GetBlob(k+1);
			bbox1 = currentBlob->GetBoundingBox();
			bbox2 = nextBlob->GetBoundingBox();
			fin1 = bbox1.y + bbox1.height;
			inicio2 = bbox2.y;
			dist = inicio2-fin1;
			if(dist<td)
			{
				if(merges==0)
					u.push_back(k);
				u.push_back(k+1);
				merges++;
			}
			else //no hay uni�n
			{
				if(merges!=0)
				{
					uniones.push_back(u);
					u.clear();
				}
				else
				{
					u.push_back(k);
					uniones.push_back(u);
					u.clear();
				}
				merges = 0;
				contador++;
			}
		}
		else
		{
			if(merges==0)
			{
				currentBlob = blobs.GetBlob(k);
				u.push_back(k);
				uniones.push_back(u);
			}
			else
			{
				uniones.push_back(u);
			}
		}
	}
	vector <IplImage*> todosLosBlobs;
	int idx;
	for(int k=0;k<uniones.size();k++)
	{
		IplImage *aux = cvCreateImage(size,8,1);
		cvZero(aux);
		for(int t=0;t<uniones[k].size();t++)
		{
			idx = uniones[k][t];
			blobs.GetBlob(idx)->FillBlob(aux,cvScalar(255,255,255),0,0);
		}
		todosLosBlobs.push_back(aux);
	}
	return  todosLosBlobs;
}
vector <IplImage*> relabelV(CBlobResult blobs, int td, CvSize size)
{
	// tengo los blobs de la imagen ordenados de izquierda a derecha, 
	// calculo la distancia entre los blobs adyacentes
	int numeroBlobs = blobs.GetNumBlobs();
	int merges = 0;
	CBlob *currentBlob;
	CBlob *nextBlob;
	int fin1; //final del current blob
	int inicio2; //inicio del next blob
	//distancia ser� inicio2-fin1
	int dist;
	CvRect bbox1;
	CvRect bbox2;
	vector <vector<int>> uniones;
	vector<int> u;
	int contador = 0;

	for(int k=0;k<blobs.GetNumBlobs();k++)
	{
		if(k<blobs.GetNumBlobs()-1)
		{
			currentBlob = blobs.GetBlob(k);
			nextBlob = blobs.GetBlob(k+1);
			bbox1 = currentBlob->GetBoundingBox();
			bbox2 = nextBlob->GetBoundingBox();
			fin1 = bbox1.x + bbox1.width;
			inicio2 = bbox2.x;
			dist = inicio2-fin1;
			if(dist<td)
			{
				if(merges==0)
					u.push_back(k);
				u.push_back(k+1);
				merges++;
			}
			else //no hay uni�n
			{
				if(merges!=0)
				{
					uniones.push_back(u);
					u.clear();
				}
				else
				{
					u.push_back(k);
					uniones.push_back(u);
					u.clear();
				}
				merges = 0;
				contador++;
			}
		}
		else
		{
			if(merges==0)
			{
				currentBlob = blobs.GetBlob(k);
				u.push_back(k);
				uniones.push_back(u);
			}
			else
			{
				uniones.push_back(u);
			}
		}
	}

	vector <IplImage*> todosLosBlobs;
	int idx;
	for(int k=0;k<uniones.size();k++)
	{
		IplImage *aux = cvCreateImage(size,8,1);
		cvZero(aux);
		for(int t=0;t<uniones[k].size();t++)
		{
			idx = uniones[k][t];
			blobs.GetBlob(idx)->FillBlob(aux,cvScalar(255,255,255),0,0);
		}
		todosLosBlobs.push_back(aux);
	}
	return  todosLosBlobs;

}
int calculoUmbraltd(CBlobResult blobsH,CBlobResult blobsV, double alfa)
{
	CBlob bestrecho;
	CBlob bbajo;
	CvRect bboxV,bboxH;
	//Se coge el m�s estrecho de blobsV
	blobsV.GetNthBlob(CBlobGetDiffX(),blobsV.GetNumBlobs()-1,bestrecho);
	//Se coge el m�s bajo de blobsH
	blobsH.GetNthBlob(CBlobGetDiffY(),blobsH.GetNumBlobs()-1,bbajo);

	bboxV = bestrecho.GetBoundingBox();
	bboxH = bbajo.GetBoundingBox();

	int td = alfa*min(bboxV.width,bboxH.height);
	if(td<15)
		td = 15;
	return td;

}
