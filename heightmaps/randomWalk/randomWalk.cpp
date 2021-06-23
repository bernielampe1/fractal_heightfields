#include <fstream>
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <time.h>

using namespace std;

struct point2d
{
  point2d():x(0), y(0) { }

  int x, y;
};

void bresanham(const point2d first, const point2d second, const unsigned cols, unsigned char *pic);
void writePPM(const char *fname, const unsigned num, point2d *ary);

int main(int argc, char *argv[])
{
  double ang;
  point2d *ary;
  point2d lastPt;
  point2d nextPt;
  unsigned numSteps;
  double stepLength;

  if (argc != 4)
  {
    cerr << "Usage: " << argv[0] << " <num steps> <step length> <fname>" << endl;
    exit(1);
  }

  srand(clock());

  numSteps = atoi(argv[1]);
  stepLength = atof(argv[2]);

  ary = new point2d[numSteps];

  for(unsigned i = 0; i < numSteps; i++)
  {
    ang = 2 * M_PI * rand() / RAND_MAX;
    nextPt.x = (int)rint(lastPt.x + stepLength * cos(ang));
    nextPt.y = (int)rint(lastPt.y + stepLength * sin(ang));
    ary[i] = nextPt;
    lastPt = nextPt;
  }
  
  writePPM(argv[3], numSteps, ary);

  return(0);
}

void writePPM(const char *fname, const unsigned num, point2d *ary)
{
  unsigned cols, rows;
  point2d maxPt;
  point2d minPt;
  ofstream outFile;
  unsigned char *pic;

  minPt = maxPt = ary[0];
  for (unsigned i = 1; i < num; i++)
  {
    if (minPt.x > ary[i].x)
    {
      minPt.x = ary[i].x;
    }
    else if (maxPt.x < ary[i].x)
    {
      maxPt.x = ary[i].x;
    }

    if (minPt.y > ary[i].y)
    {
      minPt.y = ary[i].y;
    }
    else if (maxPt.y < ary[i].y)
    {
      maxPt.y = ary[i].y;
    }
  }

  if (minPt.x < 0)
  {
    for(unsigned i = 0; i < num; i++)
    {
      ary[i].x += abs(minPt.x);
    }
  }

  if (minPt.y < 0)
  {
    for(unsigned i = 0; i < num; i++)
    {
      ary[i].y += abs(minPt.y);
    }
  }

  rows = abs(maxPt.y - minPt.y);
  cols = abs(maxPt.x - minPt.x);

  pic = new unsigned char[rows * cols];

  for(unsigned i = 0; i < num - 1; i++)
  {
    bresanham(ary[i], ary[i+1], cols, pic);
  }

  outFile.open(fname);
  outFile << "P3" << endl << cols << " " << rows << endl << "255" << endl;
  for(unsigned i = 0; i < rows * cols; i++)
  {
    outFile << (unsigned)pic[i] << " " << (unsigned)pic[i] << " " << (unsigned)pic[i] << "   ";
  }
  outFile.close();
}

void bresanham(const point2d first, const point2d second, const unsigned cols, unsigned char *pic)
{
  int x1 = first.x;
  int y1 = first.y;

  int x2 = second.x;
  int y2 = second.y;

  if (x1 > x2)
  {
    x1 = second.x;
    y1 = second.y;
    x2 = first.x;
    y2 = first.y;
  }

  int dx = abs(x2 - x1);
  int dy = abs(y2 - y1);
  int inc_dec = ((y2 >= y1) ? 1 : -1);

  if (dx > dy)
  {
    int two_dy = 2 * dy;
    int two_dy_dx = 2 * (dy - dx);
    int p = 2 * dy - dx;
    int x = x1;
    int y = y1;

    pic[y * cols + x] = (unsigned char)255;

    while(x < x2)
    {
      x++;
      if (p < 0)
      {
        p += two_dy;
      }
      else
      {
        y += inc_dec;
        p += two_dy_dx;
      }

      pic[y * cols + x] = (unsigned char)255;
    }
  }
  else
  {
    int two_dx = 2 * dx;
    int two_dx_dy = 2 * (dx - dy);
    int p = 2 * dx - dy;
    int x = x1;
    int y = y1;

    pic[y * cols + x] = (unsigned char)255;

    while (y != y2)
    {
      y += inc_dec;

      if (p < 0)
      {
        p += two_dx;
      }
      else
      {
        x++;
        p += two_dx_dy;
      }

      pic[y * cols + x] = (unsigned char)255;
    }
  }
}

