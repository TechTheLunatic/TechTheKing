#ifndef TRAITEMENT_H
#define TRAITEMENT_H

#include <list>
#include "detectionEllipse.h"

CvMat* initHomogMatrix(int vue);
std::list<Point> homogListe(std::list<Point> listeCentres, CvMat* homogMatrix, int vue);
float correctionTranchePion(float y, int vue);
IplImage* extractionComposante(IplImage *img);
void ouverture(IplImage *imgBin, IplImage *imgBinOp, int cfg_morph_cols, int cfg_morph_rows);
IplImage* nvgHSV(const IplImage *imageRGB);
IplImage* transformImg(IplImage *img, CvMat* homogMatrix);
void afficherPions(IplImage *img, std::list<Point> listeCentres);
void afficherListeCentres(IplImage *img, std::list<Point> listeCentres);

#endif
