#include<cmath>
#include<fstream>
#include<iostream>
#include<string>
#include<time.h>
#include<stdlib.h>
#include<string.h>

using namespace std;

void writePGM(const int rows, const int cols, const double *data,
              const string fname);

void diamondSquare(const int rows, const int cols, const double factor,
                   double *terrain);

int main(int argc, char **argv)
{
  if (argc != 4 && argc != 5)
  {
    cerr << "Usage: " << argv[0] << " <size> <factor> <out.pgm> [seed]" << endl;
    return(1);
  }

  int size = atoi(argv[1]);
  double factor = atof(argv[2]);
  string outfile = string(argv[3]);

  if (argc == 5)
  {
    srand(atoi(argv[4]));
  }
  else
  {
    srand(time(NULL));
  }

  factor = pow(2.0, -factor);
  size = int(pow(2.0, ceil(log2(size)))) + 1;
  int rows = size;
  int cols = size;

  double *terrain = new double[rows * cols];
  memset(terrain, 0, rows * cols);

  diamondSquare(rows, cols, factor, terrain);
  writePGM(rows, cols, terrain, outfile);

  delete [] terrain;

  return(0);
}

void writePGM(const int rows, const int cols, const double *data,
              const string fname)
{
  double min = data[0];
  double max = data[0];
  for(int i = 0; i < rows * cols; i++)
  {
    if (data[i] < min) min = data[i];
    if (data[i] > max) max = data[i];
  }

  double s = 255.0 / (max - min);

  ofstream outfile;
  outfile.open(fname.c_str());
  outfile << "P5" << endl << cols << " " << rows << endl << "255" << endl;
  for(int i = 0; i < rows * cols; i++)
  {
    outfile << (unsigned char)((data[i] - min) * s + 0.5);
  }
  outfile.close();
}

void diamondSquare(const int rows, const int cols, const double factor,
                   double *terrain)
{
  double range = 1.0;
  double e, avg;
  int rp, cp;

  for(int s = rows - 1; s > 0; s /= 2)
  {
    /* pass through the array and perform the diamond step for each square */
    for(int r = s / 2; r < rows; r += s)
    {
      for(int c = s / 2; c < cols; c+= s)
      {
        avg = 0;
        avg += terrain[(r - s/2) * cols + (c - s/2)];
        avg += terrain[(r - s/2) * cols + (c + s/2)];
        avg += terrain[(r + s/2) * cols + (c - s/2)];
        avg += terrain[(r + s/2) * cols + (c + s/2)];
        avg /= 4.0;
        e = 2.0 * range * ((double)rand() / RAND_MAX) - range;
        terrain[r * cols + c] = avg + e;
      }
    }

    /* pass through the array and perform the square step for each diamond */
    for(int r = s / 2; r < rows; r += s)
    {
      for(int c = s / 2; c < cols; c += s)
      {
        /* point 1 */
        avg = 0;

        rp = r;
        cp = c;
        avg += terrain[rp * cols + cp];

        rp = (r - s >= 0)? r - s : rows - 1 - s/2;
        cp = c;
        avg += terrain[rp * cols + cp];

        rp = r - s/2;
        cp = c - s/2;
        avg += terrain[rp * cols + cp];

        rp = r - s/2;
        cp = c + s/2;
        avg += terrain[rp * cols + cp];

        avg /= 4.0;
        e = 2.0 * range * ((double)rand() / RAND_MAX) - range;
        terrain[(r - s/2) * cols + c] = avg + e;

        /* point 2 */
        avg = 0;

        rp = r;
        cp = c;
        avg += terrain[rp * cols + cp];

        rp = r;
        cp = (c + s < cols) ? c + s: s/2;
        avg += terrain[rp * cols + cp];

        rp = r - s/2;
        cp = c + s/2;
        avg += terrain[rp * cols + cp];

        rp = r + s/2;
        cp = c + s/2;
        avg += terrain[rp * cols + cp];

        avg /= 4.0;
        e = 2.0 * range * ((double)rand() / RAND_MAX) - range;
        terrain[r * cols + (c + s/2)] = avg + e;

        /* point 3 */
        avg = 0;

        rp = r;
        cp = c;
        avg += terrain[rp * cols + cp];

        rp = (r + s < rows) ? r + s : s/2;
        cp = c;
        avg += terrain[rp * cols + cp];

        rp = r + s/2;
        cp = c + s/2;
        avg += terrain[rp * cols + cp];

        rp = r + s/2;
        cp = c - s/2;
        avg += terrain[rp * cols + cp];

        avg /= 4.0;
        e = 2.0 * range * ((double)rand() / RAND_MAX) - range;
        terrain[(r + s/2) * cols + c] = avg + e;

        /* point 4 */
        avg = 0;

        rp = r;
        cp = c;
        avg += terrain[rp * cols + cp];

        rp = r;
        cp = (c - s >= 0) ? c - s : cols - 1 - s/2;
        avg += terrain[rp * cols + cp];

        rp = r - s/2;
        cp = c - s/2;
        avg += terrain[rp * cols + cp];

        rp = r + s/2;
        cp = c - s/2;
        avg += terrain[rp * cols + cp];

        avg /= 4.0;
        e = 2.0 * range * ((double)rand() / RAND_MAX) - range;
        terrain[r * cols + (c - s/2)] = avg + e;
      }
    }

    /* reduce the random number range */
    range *= factor;
  }
}

