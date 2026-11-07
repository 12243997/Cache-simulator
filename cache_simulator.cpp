#include<iostream>
#include <fstream>
#include<cmath>
using namespace std;

const unsigned int BLOCK_SIZE = 64; //6개의 bit로 표현 가능 0 - 5

// L1 Cache
const unsigned int L1_CACHE_SIZE = 32 * 1024; // 32KB
const unsigned int L1_WAYS = 4;
const unsigned int L1_CACHE_LINES = L1_CACHE_SIZE / BLOCK_SIZE;
const unsigned int L1_NUM_SETS = L1_CACHE_LINES / L1_WAYS;

// L2 Cache
const unsigned int L2_CACHE_SIZE = 256 * 1024; // 256KB
const unsigned int L2_WAYS = 8;
const unsigned int L2_CACHE_LINES = L2_CACHE_SIZE / BLOCK_SIZE;
const unsigned int L2_NUM_SETS = L2_CACHE_LINES / L2_WAYS;

// address
const unsigned int OFFSET_BITS = log2(BLOCK_SIZE);
const unsigned int L1_INDEX_BITS = log2(L1_NUM_SETS);
const unsigned int L2_INDEX_BITS = log2(L2_NUM_SETS);

// AMAT
const unsigned int L1_HIT_TIME = 1;
const unsigned int L2_HIT_TIME = 10;
const unsigned int MEMORY_PENALTY = 100;

const bool WRITE_BACK = true;
const bool WRITE_ALLOCATE = true;

struct CacheLine {
  bool valid_bit;
  bool dirty_bit;
  unsigned int tag;
  unsigned int last_used = 0;
};

struct Cache {
  CacheLine* lines; //실제 CacheLine들이 저장되는 고악ㄴ
  unsigned int num_sets; // set 개수
  unsigned int ways; //set 당 way 개수
  unsigned int index_bits; //주소에서 index를 몇 bit 사용할지
};

// &을 return 하여 CacheLine 자체를 반환 -> accessCache에서 cacheline의 valid bit 값을 실제로 바꿈
CacheLine& getLine(Cache &cache, unsigned int set_index, unsigned int way) {
  return cache.lines[set_index * cache.ways + way];
}

int lookupCache(unsigned int address, Cache &cache, unsigned int &access_counter) {
  access_counter++;

  unsigned int set_index = (address >> OFFSET_BITS) & (cache.num_sets - 1);
  unsigned int tag = address >> (OFFSET_BITS + cache.index_bits);

  for(int way = 0; way < cache.ways; ++way) {
    // hit 일 때
    if(getLine(cache, set_index, way).valid_bit && getLine(cache, set_index, way).tag == tag) {
      getLine(cache, set_index, way).last_used = access_counter;

      return way;
    }
  }
  return -1;
}

int insertCache(unsigned int address, Cache& cache, unsigned int access_counter) {
  unsigned int set_index = (address >> OFFSET_BITS) & (cache.num_sets - 1);
  unsigned int tag = address >> (OFFSET_BITS + cache.index_bits);

  for(int way = 0; way < cache.ways; ++way) {
    if(!getLine(cache, set_index, way).valid_bit) {
      getLine(cache, set_index, way).valid_bit = true;
      getLine(cache, set_index, way).dirty_bit = false;
      getLine(cache, set_index, way).tag = tag;
      getLine(cache, set_index, way).last_used = access_counter; return way;
    }
  }

  //LRU 실행
  int changed_way = 0;

  for(int way = 1; way < cache.ways; ++way) {
    if(getLine(cache, set_index, changed_way).last_used > getLine(cache, set_index, way).last_used) {
      changed_way = way;
    }
  }
  getLine(cache, set_index, changed_way).valid_bit = true;
  getLine(cache, set_index, changed_way).dirty_bit = false;
  getLine(cache, set_index, changed_way).tag = tag;
  getLine(cache, set_index, changed_way).last_used = access_counter;
  
  return changed_way;
}
int main(){

  ifstream traceFile("trace.txt");
  if(!traceFile.is_open()){
    cout << "Filed to open trace file." << endl; return 1;
  }

  unsigned int l1_hit_count = 0;
  unsigned int l1_miss_count = 0;

  unsigned int l2_hit_count = 0;
  unsigned int l2_miss_count = 0;

  unsigned int memory_access = 0; 
  unsigned int memory_write = 0;
  unsigned int access_counter = 0;
  
  

  CacheLine l1_lines[L1_NUM_SETS * L1_WAYS];
  CacheLine l2_lines[L2_NUM_SETS * L2_WAYS];

  Cache l1 = {l1_lines, L1_NUM_SETS, L1_WAYS, L1_INDEX_BITS};
  Cache l2 = {l2_lines, L2_NUM_SETS, L2_WAYS, L2_INDEX_BITS};

  for (int i = 0; i < L1_NUM_SETS * L1_WAYS; ++i) {
    l1_lines[i].valid_bit = false;
    l1_lines[i].dirty_bit = false;
    l1_lines[i].last_used = 0;
  }

  for (int i = 0; i < L2_NUM_SETS * L2_WAYS; ++i) {
    l2_lines[i].valid_bit = false;
    l2_lines[i].dirty_bit = false;
    l2_lines[i].last_used = 0;
  }

  char operation;
  unsigned int address;
  while(traceFile >> operation >> hex >> address){
    
    int l1_hit_way = lookupCache(address, l1, access_counter);
    unsigned int l1_set_index = (address >> OFFSET_BITS) & (l1.num_sets - 1);

    if(l1_hit_way != -1) {
      l1_hit_count++;

      if(operation == 'W' && WRITE_BACK) {
        getLine(l1, l1_set_index, l1_hit_way).dirty_bit = true;
      }
    } else {
      l1_miss_count++;

      int l2_hit_way = lookupCache(address, l2, access_counter);

      if(l2_hit_way != -1) {
        l2_hit_count++;
        int inserted_way = insertCache(address, l1, access_counter);

        if(operation == 'W' && WRITE_ALLOCATE && WRITE_BACK) {
          getLine(l1, l1_set_index, inserted_way).dirty_bit = true;
        }
      } else {
        l2_miss_count++;
        memory_access++;

        insertCache(address, l2, access_counter);

        int inserted_way = insertCache(address, l1, access_counter);

        if(operation == 'W' && WRITE_ALLOCATE && WRITE_BACK) {
          getLine(l1, l1_set_index, inserted_way).dirty_bit = true;
        }
        cout << "Write Miss -> L1 Insert"
     << " | Way: " << inserted_way
     << " | Dirty: "
     << getLine(l1, l1_set_index, inserted_way).dirty_bit
     << endl;
      }
    }
  }

  float l1_miss_rate = (float)l1_miss_count / (l1_hit_count + l1_miss_count);

  float l2_local_miss_rate = 0.0;
  if(l2_hit_count + l2_miss_count > 0) {
    l2_local_miss_rate = (float)l2_miss_count /(l2_hit_count + l2_miss_count);
  }
  float amat = L1_HIT_TIME + l1_miss_rate * (L2_HIT_TIME + l2_local_miss_rate * MEMORY_PENALTY);

  cout << "\n===== Cache Simulation Results =====" << endl;

  cout << "\n[L1 Cache]" << endl;
  cout << "Total Accesses : " << l1_hit_count + l1_miss_count << endl;
  cout << "Hits           : " << l1_hit_count << endl;
  cout << "Misses         : " << l1_miss_count << endl;
  cout << "Miss Rate      : " << (float)l1_miss_count / (l1_hit_count + l1_miss_count) << endl;

  cout << "\n[L2 Cache]" << endl;
  cout << "Total Accesses : " << l2_hit_count + l2_miss_count << endl;
  cout << "Hits           : " << l2_hit_count << endl;
  cout << "Misses         : " << l2_miss_count << endl;
  cout << "Miss Rate      : " << (float)l2_miss_count / (l2_hit_count + l2_miss_count) << endl;
  cout << "Global Miss Rate : " << (float)l2_miss_count / (l1_hit_count + l1_miss_count) << endl;
  cout << "\n[Memory]" << endl;
  cout << "Memory Accesses : " << memory_access << endl;

  cout << "\n[Performance]" << endl;
  cout << "AMAT : " << amat << " cycles" << endl;

  
}

/*
offset -> 6비트, index -> 7비트 - >총 13비트  1 0000 0000 0000
*/