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

const int N = 10; //number of particles
double timeStepSize = pow(10,-4); //start small, then change during runtime
const int timeSteps = 20000000;
const int plotEveryKthStep = 100;
const double a = pow(10,-5); //constant value of a and s - in assignment pow(10,-5)
const double s = pow(10,-5); 

//representation of the 27 dimensions
const int boxes[27][3] = {{0,0,0},{0,0,1},{0,0,-1},{0,1,0},{0,1,1},{0,1,-1},{0,-1,0},{0,-1,1},{0,-1,-1},
                            {1,0,0},{1,0,1},{1,0,-1},{1,1,0},{1,1,1},{1,1,-1},{1,-1,0},{1,-1,1},{1,-1,-1},
                            {-1,0,0},{-1,0,1},{-1,0,-1},{-1,1,0},{-1,1,1},{-1,1,-1},{-1,-1,0},{-1,-1,1},{-1,-1,-1}};

double x[N][3];
double v[N][3];

void setUp(int N) //support arbitrary number of particles
{ 
  //positions for the two particle experiments
  // x[0][0] = 0.1;
  // x[0][1] = 0.5;
  // x[0][2] = 0.5;

  // x[1][0] = 0.9;
  // x[1][1] = 0.5;
  // x[1][2] = 0.5;

  for (int i=0; i<N; i++)
  {
    //initial random positions
    x[i][0] = (long double)rand()/(long double)RAND_MAX;
    x[i][1] = (long double)rand()/(long double)RAND_MAX;
    x[i][2] = (long double)rand()/(long double)RAND_MAX;

    //initial velocities as zero
    v[i][0] = 0.0;
    v[i][1] = 0.0;
    v[i][2] = 0.0;

    //initial velocities with random values
    // v[i][0] = (((long double)rand()/(long double)RAND_MAX)*2 - 1)*pow(10,-5);
    // v[i][1] = (((long double)rand()/(long double)RAND_MAX)*2 - 1)*pow(10,-5);
    // v[i][2] = (((long double)rand()/(long double)RAND_MAX)*2 - 1)*pow(10,-5);
  }
}

void printCSVFile(int counter) 
{
  std::stringstream filename;
  filename << "results" << N << "/result-" << counter <<  ".csv";
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
  double shortestDistanceForTimestep = 1.0; 
  for (int i=0; i<N; i++) //chooses particle we are examining
  {
    for (int j=i+1; j<N; j++) //looks through all other particles
    {
      //initialises force array
      double force[3];
      force[0] = 0.0;
      force[1] = 0.0;
      force[2] = 0.0;

      //loop through all 27 images of the particles
      for (int k=0; k<27; k++)
      {
        //calculate distances in 3 dimensions
        double xDist = x[i][0]-(x[j][0] + boxes[k][0]);
        double yDist = x[i][1]-(x[j][1] + boxes[k][1]);
        double zDist = x[i][2]-(x[j][2] + boxes[k][2]);

        //calculate euclidean distance
        double distance = sqrt((xDist*xDist) + (yDist*yDist) + (zDist*zDist));
        //check if shortest distance for timestep
        if (distance < shortestDistanceForTimestep)
        {
          shortestDistanceForTimestep = distance;
        }
        //calculate force
        double f = (4*a*(((12*pow(s,12))/pow(distance, 13)) - ((6*pow(s,6))/pow(distance, 7))));
        //update force
        force[0] += xDist/distance * f;
        force[1] += yDist/distance * f;
        force[2] += zDist/distance * f;
      }

      //updates velocities of both particles      
      v[i][0] += timeStepSize * force[0];
      v[i][1] += timeStepSize * force[1];
      v[i][2] += timeStepSize * force[2];

      v[j][0] -= timeStepSize * force[0];
      v[j][1] -= timeStepSize * force[1];
      v[j][2] -= timeStepSize * force[2]; 
    }
  }
  for(int i = 0; i<N; i++)
  {
    //update positions
    x[i][0] += timeStepSize * v[i][0];
    x[i][1] += timeStepSize * v[i][1];
    x[i][2] += timeStepSize * v[i][2];
    
    //keep particles inside the box
    if(x[i][0]<=0 || x[i][0]>1)
    {
      x[i][0] -= floor(x[i][0]);
    }
    if(x[i][1]<=0 || x[i][1]>1)
    {
      x[i][1] -= floor(x[i][1]);
    }
    if(x[i][2]<=0 || x[i][2]>1)
    {
      x[i][2] -= floor(x[i][2]);
    }
  }

  //calculate new timestep size
  if (shortestDistanceForTimestep > 0.00015) //inaccurate below a certain distance
  {
    timeStepSize = pow(shortestDistanceForTimestep,3)*pow(10,12);
  }
  else
  {
    timeStepSize = shortestDistanceForTimestep;
  }
}

int main() 
{
  clock_t start = clock();

  setUp(N);
  printCSVFile(0);
  for (int i=0; i<timeSteps; i++) 
  {
    //printf("%d%\r",i*100/timeSteps);
    updateBody(N);
    if (i%plotEveryKthStep==0) 
    {
      printCSVFile(i/plotEveryKthStep+1); // Please switch off all IO for performance tests.
    }
  }

  clock_t end = clock();

  double timeTaken = double(end)-double(start);

  //timing measurements
  std::stringstream filename;
  filename << "Part2/time" << N; 
  std::ofstream out( filename.str().c_str() );

  out << timeTaken/timeSteps << std::endl;

  return 0;
}