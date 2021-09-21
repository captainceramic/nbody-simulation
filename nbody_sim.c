#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define NUM_TIMESTEPS 10000
#define NUM_PARTICLES 20
#define NUM_DIMENSIONS 3
#define DOMAIN_SIZE 10
#define SOFTENING 0.05f
#define CONST_G 0.00001f

void update_acceleration(double **accelerationArray,
			 double **positionArray,
			 double *massArray) {

  double distances[NUM_DIMENSIONS];
  double inv_r = 0.0f;
  
  /* This is going to be a lot of looping! */
  for(int i = 0; i < NUM_PARTICLES; i++) {
    for(int j = 0; j < NUM_PARTICLES; j++) {

      /* For each particle, calculate the distances to each other particles and
	 the sum of the gravitational accelerations */
      inv_r = 0.0f;
      for(int k = 0; k < NUM_DIMENSIONS; k++) {
	distances[k] = positionArray[i][k] - positionArray[j][k];
	inv_r = inv_r + pow(distances[k], 2);
      }
      inv_r += pow(SOFTENING, 2);
      inv_r = pow(inv_r, -1.5f);

      for(int k = 0; k < NUM_DIMENSIONS; k++) {
	accelerationArray[i][k] += CONST_G * (distances[k] * inv_r) * massArray[j];
      }
    }
  }
}


int main(int argc, char *argv[]) {
  /* This is partly based on the tutorial here:
     https://medium.com/swlh/create-your-own-n-body-simulation-with-python-f417234885e9

     I'm going to approach this in a pretty old-school way,
     just assigning memory using arrays rather doing it dynamically.

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
  }
  
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
    printf("body 5 is at position: (%03f, %03f, %03f)\n",
	   position[4][0], position[4][1], position[4][2]);

  }

  /* some sort of write-out? how to save a file? */
  
  /* Clean-up allocated memory */
  free(mass);
  free(position);
  free(velocity);
  free(acceleration);
  
  return(EXIT_SUCCESS);
}
