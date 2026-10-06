#include<iostream>
#include<fstream>
using namespace std;

int main() {
  ofstream traceFile("trace.txt");
  if(!traceFile.is_open()) {
    cout << "Filed to open trace file." << endl; return 1;
  }

  unsigned int address = 0x10000000;
  for(int i = 0; i < 1024; ++i){
    traceFile << hex << address << endl;
    address += 4;
  }
  traceFile.close();
}