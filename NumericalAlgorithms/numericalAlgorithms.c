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
const double timeStepSize = 5000000;
const int timeSteps = 2000;
const int plotEveryKthStep = 1000;
const double a = pow(10,-5); //constant value of a and s

double x[N][3];
double v[N][3];

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
    double force[3];
    force[0] = 0.0;
    force[1] = 0.0;
    force[2] = 0.0;

    for (int j=0; j<N; j++) //looks through all other particles
    {
      if (i != j)
      {
        double xDist = x[i][0]-x[j][0];
        double yDist = x[i][1]-x[j][1];
        double zDist = x[i][2]-x[j][2];

        const double distance = sqrt(xDist * xDist + yDist * yDist + zDist * zDist);

        double f = (4*a*((12*pow(a,12))/pow(distance, 13) - (6*pow(a,6))/pow(distance, 7)));
       
        force[0] += (xDist)/distance * f;
        force[1] += (yDist)/distance * f;
        force[2] += (zDist)/distance * f;
        
        double newX;
        double newY;
        double newZ;

        if (xDist < 0)
        {
          newX = -1 + xDist;
        }
        else if (xDist > 0)
        {
          newX = 1 + xDist;
        }
        if (yDist < 0)
        {
          newY = -1 + yDist;
        }
        else if (yDist > 0)
        {
          newY = 1 + yDist;
        }
        if (zDist < 0)
        {
          newZ = -1 + zDist;
        }
        else if (zDist > 0)
        {
          newZ = 1 + zDist;
        }
        
        const double newDistance = sqrt(newX * newX + newY * newY + newZ * newZ);
        
        double newF = (4*a*((12*pow(a,12))/pow(newDistance, 13) - (6*pow(a,6))/pow(newDistance, 7)));

        force[0] += (newX)/newDistance * newF;
        force[1] += (newY)/newDistance * newF;
        force[2] += (newZ)/newDistance * newF;

        v[i][0] += timeStepSize * force[0];
        v[i][1] += timeStepSize * force[1];
        v[i][2] += timeStepSize * force[2];
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