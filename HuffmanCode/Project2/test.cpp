#include <iostream>
#include <queue>
#include <map>
#include <climits>
#include "bstream.h"

using namespace std;

int main()
{
  string name = "a.txt";
  ofbstream outfile;
  outfile.open(name.c_str());
  if (outfile.fail()) error("Can't open output file!");
  int num = 134;
  outfile << num;
  char c = 'C';

  cout << num <<" "  << c << " "<< int(c)<< endl;
  outfile.put('C');
  //Writes a single bit
  outfile.writeBit(0);
  outfile.writeBit(1);
  outfile.close();
  ifbstream infile;
  infile.open(name.c_str());
  if (infile.fail()) error("Can't open input file!");
  infile >> num;
  cout << "read " << num;
  char dd = infile.get();
  cout << " " << dd << " and "
    //reads a single bit
    << infile.readBit() ;
  cout << infile.readBit() << endl;
  infile.close();
  getchar();
  getchar();
  return 0;
}
