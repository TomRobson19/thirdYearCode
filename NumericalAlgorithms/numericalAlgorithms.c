// Compile this file with
//
// g++ -O3 numericalAlgorithms.c -o argon
//
// Run it with
//
// ./argon
//
// Based on spaceboddies.c, (C) 2015 Tobias Weinzierl

#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <math.h>

const int N = 2; //number of particles
const long double timeStepSize = 5000000;
const int timeSteps = 2000;
const int plotEveryKthStep = 1000;
const long double a = pow(10,-5); //constant value of a and s

long double x[N][3];
long double v[N][3];

void setUp(int N) //support arbitrary number of particles
{ 

  x[0][0] = 0.4;
  x[0][1] = 0.0;
  x[0][2] = 0.0;

  x[1][0] = 0.6;
  x[1][1] = 0.0;
  x[1][2] = 0.0;

  for (int i=0; i<N; i++)
  {
    // x[i][0] = (long double)rand()/(long double)RAND_MAX;
    // x[i][1] = (long double)rand()/(long double)RAND_MAX;
    // x[i][2] = (long double)rand()/(long double)RAND_MAX;

    v[i][0] = 0.0;
    v[i][1] = 0.0;
    v[i][2] = 0.0;
  }
}

void printCSVFile(int counter) 
{
  std::stringstream filename;
  filename << "results/result-" << counter <<  ".csv";
  std::ofstream out( filename.str().c_str() );

  out << "x, y, z" << std::endl;

  for (int i=0; i<N; i++) 
  {
    out << x[i][0]
        << ","
        << x[i][1]
        << ","
        << x[i][2]
        << std::endl;
  }
}

void updateBody(int N) 
{  
  for (int i=0; i<N; i++) //chooses particle we are examining
  {
    long double force[3];
    force[0] = 0.0;
    force[1] = 0.0;
    force[2] = 0.0;

    for (int j=0; j<N; j++) //looks through all other particles
    {
      if (i != j)
      {
        const long double distance = sqrt(
          (x[i][0]-x[j][0]) * (x[i][0]-x[j][0]) +
          (x[i][1]-x[j][1]) * (x[i][1]-x[j][1]) +
          (x[i][2]-x[j][2]) * (x[i][2]-x[j][2])
        );
        printf("distance = %LE \n",distance);

        long double f = (4*a*((12*powl(a,12))/powl(distance, 13) - (6*powl(a,6))/powl(distance, 7)));
        
        printf("Force = %LE \n",f);
        printf("F = %LE \n",(12*powl(a,12))/powl(distance, 13));
        printf("F = %LE \n",(6*powl(a,6))/powl(distance, 7));

        for (int k=0; k<3; k++) //calculate force and velocity in each plane
        {
          force[k] += (x[i][k]-x[j][k]) * f;
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
  }
}

int main() 
{
  setUp(N);
  printCSVFile(0);
  for (int i=0; i<timeSteps; i++) 
  {
    updateBody(N);
    if (i%plotEveryKthStep==0) 
    {
      printCSVFile(i/plotEveryKthStep+1); // Please switch off all IO if you do performance tests.
    }
  }
  return 0;
}