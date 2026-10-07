#include<iostream>
#include<fstream>
using namespace std;

int main(){
  ofstream traceFile("trace.txt");

  if(!traceFile.is_open()) {
    cout << "Failed to open trace file." << endl; return 1;
  }

  unsigned int base_address = 0x10000000;
  unsigned int stride = 0x8000;
  
  for (int repeat = 0; repeat < 200; ++repeat) {
    for(int i = 0; i < 5; ++i) {
      unsigned int address = base_address + i * stride;
      traceFile << hex << address << endl;
    }
  }
  traceFile.close();
  return 0;
}