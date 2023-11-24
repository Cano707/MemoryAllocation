/* ===========================================================================
 *
 * yourMemfitHelpers.C --
 *
 *
 * 1.0 / 17. Nov 15 (ws)
 * - from scratch
 *
 *
 * ===========================================================================
 */

#include "memfitHelpers.H"
#include <limits.h>
#include <algorithm>
#include <numeric>
#include <unistd.h>


#define OUTPUT_FILE "memfit_output.csv"


int writeOutFile(const unsigned fits[4], const unsigned alloc_max, const unsigned trials){
  // check if file exists
  int nexists = access(OUTPUT_FILE, F_OK);
  // apend to file
  FILE * fp = fopen(OUTPUT_FILE, "a");

  int deltas[4] = {0, (int) (fits[1]  - fits[0]), (int) (fits[2]  - fits[0]), (int) (fits[3]  - fits[0])};
  double rel[4] = {0, (double) deltas[1] / fits[0], (double) deltas[2] / fits[0], (double) deltas[3] / fits[0]};
  if (!fp)
    return -1;

  if (nexists)
    fprintf(fp, "TRIALS,ALLOCMAX,FF,NF,DNF,RNF,BF,DBF,RBF,WF,DWF,RWF\n");

  // insert new row into line
  fprintf(fp, "%u,%u,%u,%u,%i,%.2lf,%u,%i,%.2lf,%u,%i,%.2lf\n", 
  trials, alloc_max, 
  fits[0], 
  fits[1], deltas[1], rel[1],
  fits[2], deltas[2], rel[2],
  fits[3], deltas[3], rel[3]
  );
  return fclose(fp);
}

unsigned firstFitAlloc( unsigned blockSize )
{
  unsigned address = 0, idx = 0, size = freeList.size(), best_idx = size;

  for(; idx < size; idx++) {
    if(freeList[idx].size >= blockSize) {
      best_idx = idx;
      break;
    }
  }
  
  address = allocBlock(best_idx, blockSize);
  return address;
}


unsigned
nextFitAlloc( unsigned blockSize )
{
  static unsigned last_idx = 0;
  unsigned address = 0, size = freeList.size();

  unsigned counter = 0;
  while(counter < size) {
    unsigned idx = (counter + last_idx) % size; 
    if(freeList[idx].size >= blockSize) {
      address = allocBlock(idx, blockSize);
      last_idx = idx;
      break;
    }
    counter++;
  }

  return address;
}


unsigned
bestFitAlloc( unsigned blockSize )
{
  unsigned size = freeList.size();
  
  unsigned address = 0, idx = 0, diff = UINT_MAX, best_idx = size;
 
  for(; idx < size; idx++) {
    if(freeList[idx].size >= blockSize && (freeList[idx].size - blockSize) < diff) {
      diff = freeList[idx].size - blockSize;
      best_idx = idx;
    }
  }

  address = allocBlock(best_idx, blockSize);

  return address;
}


unsigned
worstFitAlloc( unsigned blockSize )
{
  unsigned size = freeList.size();
  
  unsigned address = 0, idx = 0, best_idx = size, prev_size = 0;
 
  for(; idx < size; idx++) {
    if(freeList[idx].size >= blockSize && freeList[idx].size > prev_size) {
      prev_size = freeList[idx].size;
      best_idx = idx;
    }
  }

  address = allocBlock(best_idx, blockSize);

  return address;
}


