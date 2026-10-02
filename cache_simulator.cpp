#include<iostream>
using namespace std;
/* ****************************************************************************** 
  32-bit 주소 사용
cache의 조건 : address = 32bit, Cache size = 32KB, Block size = 64B, Direct Mapped

   ****************************************************************************** */
struct CacheLine {
  bool valid_bit;
  unsigned int tag;
};

int main(){

  CacheLine cache[512];
  //처음 cache에는 아무것도 들어있지 않음 -> valid = false;
  for(int i = 0; i < 512; ++i){
    cache[i].valid_bit = false;
  }
  int hit_count = 0;
  int miss_count = 0;
  unsigned int addresses[] = {
    0x12345678,
    0x1234567c,
    0x12345680,
    0x12345678
  };
  /*unsigned int offset = address & 0x3F;
  unsigned int index = (address >> 6) & 0x1FF;
  unsigned int tag = (address >> 15);
  cout << "Address = 0x" << hex << address << endl;
  cout << "Offset  = " << dec << offset << endl;
  cout << "Index   = " << dec << index << endl;
  cout << "tag     = " << dec << tag << endl;*/

  /*if (cache[index].valid_bit && cache[index].tag == tag) {
    cout << "HIT" << endl;
  }else { 
    cout << "Miss" << endl;
    cache[index].valid_bit = true;
    cache[index].tag = tag;
  }*/
 for(auto a : addresses){
  unsigned int offset = a & 0x3F;
  unsigned int index = (a >> 6) & 0x1FF;
  unsigned int tag = a >> 15;
  if(cache[index].valid_bit && cache[index].tag == tag){
    
    cout << "Address: 0x" << hex << a
     << " | Tag: 0x" << tag
     << " | Index: " << dec << index
     << " | Offset: " << offset << " | Hit" << endl;
    hit_count++;
  }else{
    cache[index].valid_bit = true;
    cache[index].tag = tag;
    cout << "Address: 0x" << hex << a
     << " | Tag: 0x" << tag
     << " | Index: " << dec << index
     << " | Offset: " << offset << " | Miss" << endl;
    miss_count++;
  }
 }
 cout << "Total Accesses : " << hit_count + miss_count << endl;
 cout << "Hits           : " << hit_count << endl;
 cout << "Misses         : " << miss_count << endl;
 cout << "Miss Rate      : " << (float)miss_count / (hit_count + miss_count) << endl;
}