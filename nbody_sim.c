#include <stdio.h>
#include <stdlib.h>

#define NUM_TIMESTEPS 100
#define NUM_PARTICLES 50
#define NUM_DIMENSIONS 2
#define DOMAIN_SIZE 10


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

  double **position_before = (double **) malloc(NUM_PARTICLES * sizeof(double*));
  double **position_after = (double **) malloc(NUM_PARTICLES * sizeof(double*));
  double **velocity_before = (double **) malloc(NUM_PARTICLES * sizeof(double*));
  double **velocity_after = (double **) malloc(NUM_PARTICLES * sizeof(double*));

  for(int i = 0; i < NUM_PARTICLES; i++) {
    position_before[i] = (double *) calloc(NUM_DIMENSIONS, sizeof(double));
    position_after[i] = (double *) calloc(NUM_DIMENSIONS, sizeof(double));
    velocity_before[i] = (double *) calloc(NUM_DIMENSIONS, sizeof(double));
    velocity_after[i] = (double *) calloc(NUM_DIMENSIONS, sizeof(double));
  }

  for(int i = 0; i < NUM_PARTICLES; i++) {
    for(int j = 0; j < NUM_DIMENSIONS; j++) {
      /* randomise the initial positions */
      position_before[i][j] = (double) DOMAIN_SIZE * rand() / RAND_MAX;
      position_after[i][j] = (double) DOMAIN_SIZE * rand() / RAND_MAX;
    }
  }
  
  /* loop over time points, functions to update the positions. */
  /* some sort of write-out? how to save a file? */
  
  /* Clean-up allocated memory */
  free(mass);
  free(position_before);
  free(position_after);
  free(velocity_before);
  free(velocity_after);
  
  return(EXIT_SUCCESS);
}
