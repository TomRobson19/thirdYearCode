/*
        double newX;
        double newY;
        double newZ;

        if(xDist > 0)
        {
          newX = -1+xDist;
        }
        else if(xDist < 0)
        {
          newX = 1+xDist;
        }
        else
        {
          newX = 0.0;
        }
        if(yDist > 0)
        {
          newY = -1+yDist;
        }
        else if(yDist < 0)
        {
          newY = 1+yDist;
        }
        else
        {
          newY = 0.0;
        }
        if(zDist > 0)
        {
          newZ = -1+zDist;
        }
        else if(zDist < 0)
        {
          newZ = 1+zDist;
        }
        else
        {
          newZ = 0.0;
        }
        
        double newDistance = sqrt((newX*newX) + (newY*newY) + (newZ*newZ));
        
        double newF = (4*a*(((12*pow(s,12))/pow(newDistance, 13)) - ((6*pow(s,6))/pow(newDistance, 7))));

        if (newDistance < shortestDistance)
        {
          shortestDistance = newDistance;
        }


        force[0] += newX/newDistance * newF;
        force[1] += newY/newDistance * newF;
        force[2] += newZ/newDistance * newF;
        */