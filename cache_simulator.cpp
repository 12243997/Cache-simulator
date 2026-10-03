#include<iostream>
using namespace std;
/* ****************************************************************************** 
  32-bit 주소 사용
cache의 조건 : address = 32bit, Cache size = 32KB, Block size = 64B, Direct Mapped

   ****************************************************************************** */
unsigned int hit_count = 0;
unsigned int miss_count = 0;

struct CacheLine {
  bool valid_bit;
  unsigned int tag;
};

void accessCache(unsigned int address, CacheLine cache[], unsigned int &hit_count, unsigned int &miss_count) {
  unsigned int offset = address & 0x3F;
  unsigned int index = (address >> 6) & 0x1ff;
  unsigned int tag = address >> 15;

  if(cache[index].valid_bit && cache[index].tag == tag){
    cout << "Address: 0x" << hex << address
     << " | Tag: 0x" << tag
     << " | Index: " << dec << index
     << " | Offset: " << offset << " | Hit" << endl;
     hit_count++;
  }else {
    cout << "Address: 0x" << hex << address
     << " | Tag: 0x" << tag
     << " | Index: " << dec << index
     << " | Offset: " << offset << " | Miss" << endl;
     miss_count++;
     cache[index].valid_bit = true;
     cache[index].tag = tag;
  }
}
int main(){

  CacheLine cache[512];
  //처음 cache에는 아무것도 들어있지 않음 -> valid = false;
  for(int i = 0; i < 512; ++i){
    cache[i].valid_bit = false;
  }
  
  unsigned int addresses[] = {
    0x12345678,
    0x1234567c,
    0x12345680,
    0x12345678
  };
  for(auto a : addresses){
    accessCache(a, cache, hit_count, miss_count);
  }
 cout << "Total Accesses : " << hit_count + miss_count << endl;
 cout << "Hits           : " << hit_count << endl;
 cout << "Misses         : " << miss_count << endl;
 cout << "Miss Rate      : " << (float)miss_count / (hit_count + miss_count) << endl;
}