#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define NUM_TIMESTEPS 100
#define NUM_PARTICLES 2
#define NUM_DIMENSIONS 3
#define DOMAIN_SIZE 10
#define SOFTENING 0.0f
#define CONST_G 1.0f

void update_acceleration(double **accelerationArray,
			 double **positionArray,
			 double *massArray) {

  double distances[NUM_DIMENSIONS];
  double inv_r;

  /* This is going to be a lot of looping! */
  for(int i = 0; i < NUM_PARTICLES; i++) {
    for(int j = 0; j < NUM_PARTICLES; j++) {

      if(i == j) continue;

      /* For each particle, calculate the distances to each other particles and
	 the sum of the gravitational accelerations

	 -Gm / r**2

      */
      inv_r = 0.0f;
      for(int k = 0; k < NUM_DIMENSIONS; k++) {
	distances[k] = positionArray[i][k] - positionArray[j][k];
	inv_r = inv_r + pow(distances[k], 2);
      }
      inv_r = inv_r + pow(SOFTENING, 2);
      inv_r = pow(inv_r, -1.5f);

      for(int k = 0; k < NUM_DIMENSIONS; k++) {
	accelerationArray[i][k] += 1.0 * (CONST_G * (distances[k] * inv_r) * massArray[j]);
      }

      /* printf("acceleration array is: (%0.2f, %0.2f, %0.2f)\n", */
      /*  	     accelerationArray[i][0], */
      /*  	     accelerationArray[i][1], */
      /*  	     accelerationArray[i][2]); */

      /* printf("distance array is: (%0.2f, %0.2f, %0.2f)\n", */
      /*  	     distances[0], */
      /*  	     distances[1], */
      /*  	     distances[2]); */
    }
  }
}


int main(int argc, char *argv[]) {
  /* This is partly based on the tutorial here:
     https://medium.com/swlh/create-your-own-n-body-simulation-with-python-f417234885e9

     TB TODO: Use command-line args etc...

     This will be a 2D simulation (at least initially)

   */

  printf("doing a n-body simulation...\n");

  /* we need:

   * an array of particle masses (constant)
   * an array of particle positions (before)
   * an array of particle positions (after)
   * an array of particle velocities (before + after)

   # For each time step:
   * calculate the acceleration due to gravity
   * use the leap-frog scheme (kick - drift - kick) to update the positions and velocities.

   */

  /* First step: allocate memory and assign values */
  double *mass = (double *) calloc(NUM_PARTICLES, sizeof(double));
  for(int i = 0; i < NUM_PARTICLES; i++) {
    mass[i] = 1.0f;
  }

  double **position = (double **) malloc(NUM_PARTICLES * sizeof(double*));
  double **acceleration = (double **) malloc(NUM_PARTICLES * sizeof(double*));
  double **velocity = (double **) malloc(NUM_PARTICLES * sizeof(double*));

  for(int i = 0; i < NUM_PARTICLES; i++) {
    position[i] = (double *) calloc(NUM_DIMENSIONS, sizeof(double));
    velocity[i] = (double *) calloc(NUM_DIMENSIONS, sizeof(double));
    acceleration[i] = (double *) calloc(NUM_DIMENSIONS, sizeof(double));
  }

  for(int i = 0; i < NUM_PARTICLES; i++) {
    for(int j = 0; j < NUM_DIMENSIONS; j++) {
      /* randomise the initial positions,
	 acceleration and velocities start at 0.0 */
      position[i][j] = (double) DOMAIN_SIZE * rand() / RAND_MAX;
    }

    printf("intial position is: (%0.2f, %0.2f, %0.2f)\n",
	   position[i][0], position[i][1], position[i][2]);
  }

  /* begin the time loop */
  for(int i = 0; i < NUM_TIMESTEPS; i++) {

    /* apply the first 'kick' and drift */
    for(int i = 0; i < NUM_PARTICLES; i++) {
      for(int j = 0; j < NUM_DIMENSIONS; j++) {
	velocity[i][j] = velocity[i][j] + 0.5 * acceleration[i][j];
	position[i][j] = position[i][j] + velocity[i][j];
      }
    }

    /* calculate the gravitational acceleration
       update the accelerations */
    update_acceleration(acceleration, position, mass);

    /* second kick */
    for(int i = 0; i < NUM_PARTICLES; i++) {
      for(int j = 0; j < NUM_DIMENSIONS; j++) {
	velocity[i][j] = velocity[i][j] + 0.5 * acceleration[i][j];
      }
    }

    /* Let's see where a given body is! */
    printf("body 1 is at position: (%03f, %03f, %03f)\n",
    	   position[0][0], position[0][1], position[0][2]);
    printf("body 2 is at position: (%03f, %03f, %03f)\n",
    	   position[1][0], position[1][1], position[1][2]);

    double vel_1 = pow(pow(velocity[0][0], 2) + pow(velocity[0][1], 2) + pow(velocity[0][2], 2), 0.5f);
    double vel_2 = pow(pow(velocity[1][0], 2) + pow(velocity[1][1], 2) + pow(velocity[1][2], 2), 0.5f);
    
    printf("body 1 velocity: (%03f, %03f, %03f) (speed: %03f)\n",
    	   velocity[0][0], velocity[0][1], velocity[0][2], vel_1);
    printf("body 2 velocity: (%03f, %03f, %03f) (speed: %03f)\n",
    	   velocity[1][0], velocity[1][1], velocity[1][2], vel_2);

    double temp_dist = 0.0f;
    for(int i=0; i<NUM_DIMENSIONS; i++) {
      temp_dist += pow(position[0][i] - position[1][i], 2);
    }

    printf("body 1 acceleration: (%03f, %03f, %03f)\n",
    	   acceleration[0][0], acceleration[0][1], acceleration[0][2]);
    printf("body 2 acceleration: (%03f, %03f, %03f)\n",
	   acceleration[1][0], acceleration[1][1], acceleration[1][2]);

    printf("\tdistance between particle 1 and 2 is %0.2f\n",
	   pow(temp_dist, 0.5f));
        
    /* TB TODO: Calculate the potential and kinetic energy to determine
       if they are being conserved */

    /* TB TODO: write out the results for this time stamp for
       visualisation purposes (probably Python) */

  }

  /* Clean-up allocated memory */
  free(mass);
  free(position);
  free(velocity);
  free(acceleration);

  return(EXIT_SUCCESS);
}
