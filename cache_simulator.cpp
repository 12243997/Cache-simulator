#include<iostream>
#include <fstream>
#include<cmath>
using namespace std;

const unsigned int CACHE_SIZE = 32 * 1024;
const unsigned int BLOCK_SIZE = 128;
const unsigned int WAYS = 1;
const unsigned int CACHE_LINES = CACHE_SIZE / BLOCK_SIZE;
const unsigned int NUM_SETS = CACHE_LINES / WAYS;
const unsigned int OFFSET_BITS = log2(BLOCK_SIZE);
const unsigned int INDEX_BITS = log2(NUM_SETS);

struct CacheLine {
  bool valid_bit;
  unsigned int tag;
  unsigned int last_used = 0;
};

void accessCache(unsigned int address, CacheLine cache [][WAYS], unsigned int &access_counter, unsigned int &hit_count, unsigned int &miss_count) {
  access_counter++;
  unsigned int offset = address & (BLOCK_SIZE - 1);
  unsigned int set_index = (address >> OFFSET_BITS) & (NUM_SETS - 1);
  unsigned int tag = address >> (OFFSET_BITS + INDEX_BITS);
  bool hit = false;
  
  for(int ways = 0; ways < WAYS; ++ways){
    
    if(cache[set_index][ways].valid_bit && cache[set_index][ways].tag == tag){
      hit_count++;
      hit = true;
      cache[set_index][ways].last_used = access_counter;
      break;
    }
  }
  if(hit) {
      cout << "Address: 0x" << hex << address
      << " | Tag: 0x" << tag
      << " | Index: " << dec << set_index
      << " | Offset: " << offset << " | Hit" << endl; return;
  } else {
    miss_count++;
    for(int ways = 0; ways < WAYS; ++ways){
      if(!cache[set_index][ways].valid_bit){
        cout << "Address: 0x" << hex << address
        << " | Tag: 0x" << tag
        << " | Index: " << dec << set_index
        << " | Offset: " << offset << " | Miss" << " | Inserted Way: " << ways << endl;
        cache[set_index][ways].tag = tag; cache[set_index][ways].valid_bit = true; 
        cache[set_index][ways].last_used = access_counter; return;
      }
    }
    //LRU수행
    int changed_way = 0;
    for(int ways = 1; ways < WAYS; ++ways){
      if(cache[set_index][changed_way].last_used > cache[set_index][ways].last_used){
        changed_way = ways;
      }
    }
    cout << "Address: 0x" << hex << address
     << " | Tag: 0x" << tag
     << " | Index: " << dec << set_index
     << " | Offset: " << offset
     << " | Miss"
     << " | Replaced Way: " << changed_way
     << endl;
    cache[set_index][changed_way].tag = tag;
    cache[set_index][changed_way].last_used = access_counter;
  }
}

int main(){

  ifstream traceFile("trace.txt");
  if(!traceFile.is_open()){
    cout << "Filed to open trace file." << endl; return 1;
  }

  unsigned int access_counter = 0;
  unsigned int hit_count = 0;
  unsigned int miss_count = 0;

  CacheLine cache[NUM_SETS][WAYS];
  //처음 cache에는 아무것도 들어있지 않음 -> valid = false;
  for(int i = 0; i < NUM_SETS; ++i){
    for(int j = 0; j < WAYS; ++j){
      cache[i][j].valid_bit = false;
    }
  }

  unsigned int address;
  while(traceFile >> hex >> address){
    accessCache(address, cache, access_counter, hit_count, miss_count);
  }

 cout << "Total Accesses : " << hit_count + miss_count << endl;
 cout << "Hits           : " << hit_count << endl;
 cout << "Misses         : " << miss_count << endl;
 cout << "Miss Rate      : " << (float)miss_count / (hit_count + miss_count) << endl;
}

/*
offset -> 6비트, index -> 7비트 - >총 13비트  1 0000 0000 0000
*/