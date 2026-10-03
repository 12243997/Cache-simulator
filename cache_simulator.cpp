#include<iostream>
#include <fstream>
using namespace std;

struct CacheLine {
  bool valid_bit;
  unsigned int tag;
};

void accessCache(unsigned int address, CacheLine cache [][2], unsigned int &hit_count, unsigned int &miss_count) {
  unsigned int offset = address & 0x3F;
  unsigned int index = (address >> 6) & 0xff;
  unsigned int tag = address >> 14;
  bool hit = false;
  
  
  for(int way = 0; way < 2; ++way){
    
    if(cache[index][way].valid_bit && cache[index][way].tag == tag){
      hit_count++;
      hit = true;
      break;
    }
  }
  if(hit) {
      cout << "Address: 0x" << hex << address
      << " | Tag: 0x" << tag
      << " | Index: " << dec << index
      << " | Offset: " << offset << " | Hit" << endl; return;
  } else {
    miss_count++;
    if(!cache[index][0].valid_bit){
      cout << "Address: 0x" << hex << address
      << " | Tag: 0x" << tag
      << " | Index: " << dec << index
      << " | Offset: " << offset << " | Miss" << endl;
      cache[index][0].tag = tag; cache[index][0].valid_bit = true; return;
    }
    if(!cache[index][1].valid_bit){
      cout << "Address: 0x" << hex << address
      << " | Tag: 0x" << tag
      << " | Index: " << dec << index
      << " | Offset: " << offset << " | Miss" << endl;
      cache[index][1].tag = tag; cache[index][1].valid_bit = true; return;
    }
    if(cache[index][0].valid_bit && cache[index][1].valid_bit) {
      //LRU 수행
    }
  }
  
}
int main(){

  ifstream traceFile("trace.txt");
  if(!traceFile.is_open()){
    cout << "Filed to open trace file." << endl; return 1;
  }

  unsigned int hit_count = 0;
  unsigned int miss_count = 0;

  CacheLine cache[256][2];
  //처음 cache에는 아무것도 들어있지 않음 -> valid = false;
  for(int i = 0; i < 256; ++i){
    for(int j = 0; j < 2; ++j){
      cache[i][j].valid_bit = false;
    }
  }

  unsigned int address;
  while(traceFile >> hex >> address){
    accessCache(address, cache, hit_count, miss_count);
  }

 cout << "Total Accesses : " << hit_count + miss_count << endl;
 cout << "Hits           : " << hit_count << endl;
 cout << "Misses         : " << miss_count << endl;
 cout << "Miss Rate      : " << (float)miss_count / (hit_count + miss_count) << endl;
}