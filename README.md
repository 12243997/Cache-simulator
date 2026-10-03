# Cache Simulator
A simple cache simulator implemented in C++ for studyingcache memory organization.

## Current Configuration

- 32-bit address
- Cache size : 32 KB
- Block size : 64 B
- Set-associative cache
- Configurable associativity ('WAYS')

## Implemented

- Address decomposition (Tag / Index / Block Offset)
- Memory trace file input
- Cache hit/miss detection
- Hit/miss statistics
- Configurable cache size, block size, and associativity
- Set-associative cache
- LRU replacement policy using access timestamps

## Current LRU Implementation

Each cache line stores the access count of its most recent use.

On a cache miss :
1. An invalid way is used if one is available
2. If all ways are valid, the way with the smallest 'last_used' value is replaced.

The implementation has been tested with a 4-way set-associative cache.

## Future Work
- Compare miss rates for different associativities
- Write-through / Write-back policies
- Write-allocate / No-write-allocate policies
- Multi-level cache (L1 / L2)
- AMAT calculation
- Performance analysis and visualization