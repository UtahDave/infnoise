#include <stdbool.h>
#include <stdint.h>
#include <sys/types.h>

// Required accuracy of estimated vs measured entropy in health monitor
#define INM_ACCURACY 1.03

// The FT240X has a 512 byte buffer.  Must be multiple of 64
// We also write this in one go to the Keccak sponge, which is at most 1600 bits
#define BUFLEN 512u

// This is how many previous bits are used to predict the next bit from the INM
#define PREDICTION_BITS 14u

// This is the maximum time we allow to pass to perform the I/O operations, since long
// delays can reduce entropy from the INM.
#define MAX_MICROSEC_FOR_SAMPLES 5000u

// This is the gain of each of the two op-amp stages in the INM
#define DESIGN_K 1.84

#define BITMODE_SYNCBB 0x4

// This defines which pins on the FT240X are used
#define COMP1 1u
#define COMP2 4u
#define SWEN1 2u
#define SWEN2 0u

// All data bus bits of the FT240X are outputs, except COMP1 and COMP2
#define MASK (0xffu & ~(1u << COMP1) & ~(1u << COMP2))

bool inmHealthCheckStart(uint8_t N, double K, bool debug);
void inmHealthCheckStop(void);
bool inmHealthCheckAddBit(bool evenBit, bool oddBit, bool even);
bool inmHealthCheckOkToUseData(void);
double inmHealthCheckEstimateK(void);
double inmHealthCheckEstimateEntropyPerBit(void);
uint32_t inmGetEntropyLevel(void);
void inmClearEntropyLevel(void);
bool inmEntropyOnTarget(uint32_t entropy, uint32_t bits);
void inmWriteEntropyStart(uint32_t bufLen, bool debug);
void inmWriteEntropyToPool(uint8_t *bytes, uint32_t length, uint32_t entropy);
void inmWaitForPoolToHaveRoom(void);
void inmDumpStats(void);
void startDaemon(bool daemon, bool pidFile, char *fileName);
bool isSuperUser(void);

extern double inmK, inmExpectedEntropyPerBit;
