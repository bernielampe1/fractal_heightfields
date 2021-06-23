#include <iostream>
#include <fstream.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

using namespace std;

struct point2d
{
  point2d():x(0), y(0) {}
  void set(const int xin, const int yin) {x=xin;y=yin;}
  int x,y;
};

void fractalize(double *ary, const unsigned s, const point2d pt1, const point2d pt2,
                const point2d pt3, const double q, const double n);
void writePGM(const double *ary, const unsigned s, const char *fname);

int main(int argc, char *argv[])
{
  double *ary = NULL;
  double n;
  double q = 2.0; 
  unsigned s = 0; 
  point2d pt1, pt2, pt3;

  srand(clock());

  if (argc == 4)
  {
    s = (unsigned)atoi(argv[1]);
    n = (double)atof(argv[2]);
  }
  else
  {
    cerr << "Usage: " << argv[0] << " <image size> <roughness [0-1]> <fname>" << endl;
    exit(0);
  }

  ary = new double[s * s];

  for(unsigned i = 0; i < s * s; i++) ary[i] = 0.0;

  pt1.set(0, 0); pt2.set(0, s - 1); pt3.set(s - 1, s -1);
  fractalize(ary, s, pt1, pt2, pt3, q, n);

  pt1.set(0, 0); pt2.set(s - 1, 1); pt3.set(s - 1, s - 1);
  fractalize(ary, s, pt1, pt2, pt3, q, n);

  writePGM(ary, s, argv[3]);

  delete [] ary;

  return(0);
}

void fractalize(double *ary, const unsigned s, const point2d pt1, const point2d pt2,
                const point2d pt3, const double q, const double n)
{
  point2d pt1_1, pt2_1, pt3_1;

  pt1_1.set((pt1.x + pt2.x) / 2, (pt1.y + pt2.y) / 2);
  double e = q * ((double)rand() / RAND_MAX) - q / 2.0;
  ary[pt1_1.y * s + pt1_1.x] = ((ary[pt1.y * s + pt1.x] +
                                 ary[pt2.y * s + pt2.x] +
                                 ary[pt3.y * s + pt3.x]) / 3.0) + e;;

  pt2_1.set((pt2.x + pt3.x) / 2, (pt2.y + pt3.y) / 2);
  e = q * ((double)rand() / RAND_MAX) - q / 2.0;
  ary[pt2_1.y * s + pt2_1.x] = ((ary[pt1.y * s + pt1.x] +
                                 ary[pt2.y * s + pt2.x] +
                                 ary[pt3.y * s + pt3.x]) / 3.0) + e;;

  pt3_1.set((pt1.x + pt3.x) / 2, (pt1.y + pt3.y) / 2);
  e = q * ((double)rand() / RAND_MAX) - q / 2.0;
  ary[pt3_1.y * s + pt3_1.x] = ((ary[pt1.y * s + pt1.x] +
                                 ary[pt2.y * s + pt2.x] +
                                 ary[pt3.y * s + pt3.x] / 3.0)) + e;;

  int a = (int)sqrt(pow(pt1.x - pt2.x, 2.0) + pow(pt1.y - pt2.y, 2.0));
  int b = (int)sqrt(pow(pt2.x - pt3.x, 2.0) + pow(pt2.y - pt3.y, 2.0));
  if (a * b / 2 < 3)
  {
    return;
  }
  else
  {
    fractalize(ary, s, pt1, pt1_1, pt3_1, q * n, n);
    fractalize(ary, s, pt1_1, pt2, pt2_1, q * n, n);
    fractalize(ary, s, pt2_1, pt3, pt3_1, q * n, n);
    fractalize(ary, s, pt1_1, pt2_1, pt3_1, q * n, n);
  }
}

void writePGM(const double *ary, const unsigned s, const char *fname)
{
  unsigned i;
  double max, min;
  ofstream outFile;
  double scaleFac;
  double val;

  for(max = min = ary[0], i = 1; i < s * s; i++)
  {
    if (ary[i] < min)
    {
      min = ary[i];
    }
    else if (ary[i] > max)
    {
      max = ary[i];
    }
  }

  scaleFac = 255.0 / (max - min);

  outFile.open(fname);
  outFile << "P2" << endl << s << " " << s << endl << "255" << endl;

  for(unsigned i = 0; i < s * s; i++)
  {
    val = int((ary[i] - min) * scaleFac + 0.5);
    outFile << val << " ";
  }

  outFile.close();
}

