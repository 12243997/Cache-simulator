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
- Cache hit/miss detection and statistics
- Configurable cache size, block size, and associativity
- LRU replacement policy using access timestamps

## Experimental Results

### Effect of Block Size
To analyze the effect of spatial locality, the simulator was tested with sequential memory accesses while varying the cache block size

#### Configuration
- Cache Size: 32 KB
- Associativity: Direct-mapped (1-way)
- Memory Accesses: 1024
- Access Pattern: Sequential integer accesses (4 bytes apart)

#### Results

| Block Size | Hits | Misses | Miss Rate |
|------------|------|--------|-----------|
| 16 B | 768 | 256 | 25% |
| 32 B | 896 | 128 | 12.5% |
| 64 B | 960 | 64 | 6.25% |
| 128 B | 992 | 32 | 3.125% |

![Block Size vs Miss Rate](result/figures/block_size_miss_rate.png)

#### Analysis
Since the trace accesses consecutive memory addresses, it exhibits strong spatial locality.
When a cache block is loaded on a miss, subsequent accesses to nearby integers within the same block result in cache hits.
As the block size increases, more consecutive integers are fetched by a single cache miss, reducing the overall miss rate.

## Future Work
- Associativity analysis
- Write-through / Write-back policies
- Write-allocate / No-write-allocate policies
- Multi-level cache (L1 / L2)
- AMAT calculation