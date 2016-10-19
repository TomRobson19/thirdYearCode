// Translate this file with
//
// g++ -O3 spaceboddies.c -o spaceboddies
//
// Run it with
//
// ./spaceboddies
//
// Open Paraview (www.paraview.org) and do the following:
// - Select File/open and select all the results files. Press the Apply button.
// - Click into the left visualisation window (usually titled Layout #1).
// - Click the result-* item in the window Pipeline Browser. Ensure that your Layout #1 and the item result-* is marked.
// - Select Filters/Alphabetical/TableToPoints. Press Apply button.
// - Switch the representation (on top) from Surface into Points.
// - Press the play button and adopt colours and point sizes.
// - For some Paraview versions, you have to mark your TableToPoints item (usually called TableToPoints1) and explicitly select that X Column is x, Y Column is y, and Z Column is z.
// - What is pretty cool is the Filter TemporalParticlesToPathlines. If you set Mask Points to 1, you see a part of the traiactory.
//
// (C) 2015 Tobias Weinzierl

#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <math.h>

const int N = 2; //number of particles
const double timeStepSize = pow(10,12);
const int timeSteps = 2000;
const int plotEveryKthStep = 1;
const double a = pow(10,-5); //constant value of a and s

double x[N][3];
double v[N][3];

void setUp(int N) { //support arbitrary number of particles

  x[0][0] = 0.4;
  x[0][1] = 0.0;
  x[0][2] = 0.0;

  x[1][0] = 0.6;
  x[1][1] = 0.0;
  x[1][2] = 0.0;

  for (int i=0; i<N; i++)
  {
    // x[i][0] = (double)rand()/(double)RAND_MAX;
    // x[i][1] = (double)rand()/(double)RAND_MAX;
    // x[i][2] = (double)rand()/(double)RAND_MAX;

    v[i][0] = 0.0;
    v[i][1] = 0.0;
    v[i][2] = 0.0;
  }
}


void printCSVFile(int counter) {
  std::stringstream filename;
  filename << "result-" << counter <<  ".csv";
  std::ofstream out( filename.str().c_str() );

  out << "x, y, z" << std::endl;

  for (int i=0; i<N; i++) {
    out << x[i][0]
        << ","
        << x[i][1]
        << ","
        << x[i][2]
        << std::endl;
  }
}



void updateBody(int N) {
  
  for (int i=0; i<N; i++)
  {
    double force[3];
    force[0] = 0.0;
    force[1] = 0.0;
    force[2] = 0.0;

    for (int j=0; j<N; j++) {
      if (i != j)
      {
        const double distance = sqrt(
          (x[i][0]-x[j][0]) * (x[i][0]-x[j][0]) +
          (x[i][1]-x[j][1]) * (x[i][1]-x[j][1]) +
          (x[i][2]-x[j][2]) * (x[i][2]-x[j][2])
        );

        double f = (4*a*((12*pow(a,12))/pow(distance, 13) - (6*pow(a,6))/pow(distance, 7)));

        for (int k=0; k<3; k++) //calculate force and velocity in each plane
        {
          force[k] += (x[j][k]-x[i][k]) * f / distance;
          v[i][k] += timeStepSize * force[k];
        }
      } 
    }

    for (int l=0; l<3; l++)
    {
      x[i][l] += timeStepSize * v[i][l];

      if (x[i][l] < 0.0 || x[i][l] > 1.0)
      {
        x[i][l] -= floor(x[i][l])/1;
      }
    }
    
    
    //need to keep within box

  }
}


int main() {

  setUp(N);
  printCSVFile(0);

  for (int i=0; i<timeSteps; i++) {
    updateBody(N);
    if (i%plotEveryKthStep==0) {
      printCSVFile(i/plotEveryKthStep+1); // Please switch off all IO if you do performance tests.
    }
  }

  return 0;
}
