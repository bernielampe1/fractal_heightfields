#include <iostream>
#include <fstream.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

using namespace std;

void fractalize(double *ary, const unsigned s, const unsigned lr, const unsigned lc,
                const unsigned ur, const unsigned uc, const double r, const double n);
void writePPM(const double *ary, const unsigned s, const char *fname);

int main(int argc, char *argv[])
{
  double *ary = NULL;
  double n;
  double r = 2.0; 
  unsigned s = 0; 

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
  fractalize(ary, s, 0, 0, s - 1, s - 1, r, n);
  writePPM(ary, s, argv[3]);

  if (ary != NULL)
  {
    delete [] ary;
  }

  return(0);
}

void fractalize(double *ary, const unsigned s, const unsigned lr, const unsigned lc,
                const unsigned ur, const unsigned uc, const double r, const double n)
{
  unsigned mr = (ur + lr) / 2;
  unsigned mc = (uc + lc) / 2;
  double e = r * ((double)rand() / RAND_MAX) - r / 2.0;

  ary[mr * s + mc] = ((ary[lr * s + lc] +
                       ary[lr * s + uc] +
                       ary[ur * s + lc] +
                       ary[ur * s + uc]) / 4.0) + e;

  if (ur - lr <= 1 &&  uc - lc <= 1)
  {
    return;
  }
  else
  {
    fractalize(ary, s, lr, lc, mr, mc, r * n, n);
    fractalize(ary, s, lr, mc, mr, uc, r * n, n);
    fractalize(ary, s, mr, lc, ur, mc, r * n, n);
    fractalize(ary, s, mr, mc, ur, uc, r * n, n);
  }
}

void writePPM(const double *ary, const unsigned s, const char *fname)
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
  outFile << "P3" << endl << s << " " << s << endl << "255" << endl;

  for(unsigned i = 0; i < s * s; i++)
  {
    val = int((ary[i] - min) * scaleFac + 0.5);
    outFile << val << " " << val << " " << val << " ";
  }

  outFile.close();
}

