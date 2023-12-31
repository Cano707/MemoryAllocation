/* ===========================================================================
 *
 * memfit.C --
 *
 *
 * Wolfram Schenck <wolfram.schenck@fh-bielefeld.de>
 *
 *
 * 1.0 / 17. Nov 15 (ws)
 * - from scratch
 *
 *
 * ===========================================================================
 */

#include "memfitHelpers.H"


#define MEM_SIZE         1000
#define MEM_MIN_ADDRESS  100
#define ALLOC_MAX        100

// ---------------------------------------------------------------------------

enum allocAlg_t {
  FIRST_FIT, NEXT_FIT, BEST_FIT, WORST_FIT
};

// ---------------------------------------------------------------------------

int
simulateAllocs( unsigned seedVal, allocAlg_t allocAlg, unsigned alloc_max )
{
  // init memory management
  initHeap( MEM_MIN_ADDRESS, MEM_SIZE );

  // init random generator
  srand(seedVal);

  // loop until allocation fails
  // (store number of successful loops in count)
  unsigned count = 0;
  do {
    // random selection: free existing block or allocate new block
    // ATTENTION: Probability of freeing memory: p = 0.5
    if( ((rand() % 2) == 1) && (allocList.size() > 0) )
    {
      randomFree();
    }
    else
    {
      unsigned address;
      // size of requested memory
      unsigned memRequest = (rand() % alloc_max) + 1;
      switch( allocAlg )
      {
        case FIRST_FIT:
          address = firstFitAlloc( memRequest );
          break;
        case NEXT_FIT:
          address = nextFitAlloc( memRequest );
          break;
        case BEST_FIT:
          address = bestFitAlloc( memRequest );
          break;
        case WORST_FIT:
        default:
          address = worstFitAlloc( memRequest );
          break;
      }
      if( !address )
        break;
    }

    //printList( "freeList", freeList );
    //printList( "allocList", allocList );

    count++;
  } while(1);

  return count;
}

// ---------------------------------------------------------------------------

int
main( int argc, char **argv )
{
  unsigned alloc_max;
  if (argc < 2) {
    alloc_max = ALLOC_MAX;
  } else {
    alloc_max = atoi(argv[1]);
    if(alloc_max == 0) {
      alloc_max = ALLOC_MAX;
    }
  }

  // use array count to store results for the four different mem. alloc. methods
  unsigned count[4] = {0,0,0,0};
  unsigned singleCount;

  unsigned itCount = 0;
  // loop over different seed values and collect for each seed value and
  // for each mem. alloc. method the number of successful allocations
  for( unsigned seedVal = 0; seedVal < 100000; seedVal += 1000 )
  {
    count[0] += (singleCount = simulateAllocs( seedVal, FIRST_FIT, alloc_max));
    printf( "FIRST_FIT: Memory allocation failure after %u successful memory operations.\n",
            singleCount );

    count[1] += (singleCount = simulateAllocs( seedVal, NEXT_FIT, alloc_max ));
    printf( "NEXT_FIT : Memory allocation failure after %u successful memory operations.\n",
            singleCount );

    count[2] += (singleCount = simulateAllocs( seedVal, BEST_FIT, alloc_max ));
    printf( "BEST_FIT : Memory allocation failure after %u successful memory operations.\n",
            singleCount );

    count[3] += (singleCount = simulateAllocs( seedVal, WORST_FIT, alloc_max ));
    printf( "WORST_FIT: Memory allocation failure after %u successful memory operations.\n",
            singleCount );

    printf( "\n" );

    itCount++;
  }

  // print results to console
  printf( "SUMMARY\n=======\n\n" );
  printf( "FIRST_FIT: %u\n", count[0] );
  printf( "NEXT_FIT : %u\n", count[1] );
  printf( "BEST_FIT : %u\n", count[2] );
  printf( "WORST_FIT: %u\n", count[3] );

  printf( "\nTrials   : %u\n", itCount );

  writeOutFile(count, alloc_max, itCount);
  
  return 0;
}
