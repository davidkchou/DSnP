/****************************************************************************
  FileName     [ test.cpp ]
  PackageName  [ test ]
  Synopsis     [ Test program for simple database db ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2015-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/
#include <iostream>
#include <cstdlib>
#include <iomanip>     
#include "dbTable.h"

using namespace std;

extern DBTable dbtbl;

class CmdParser;
CmdParser* cmdMgr = 0; // for linking purpose

int
main(int argc, char** argv)
{
   if (argc != 2) {  // testdb <cvsfile>
      cerr << "Error: using testdb <csvfile>!!" << endl;
      exit(-1);
   }

   ifstream inf(argv[1]);


   if (!inf) {
      cerr << "Error: cannot open file \"" << argv[1] << "\"!!\n";
      exit(-1);
   }

   if (dbtbl) {
      cout << "Table is resetting..." << endl;
      dbtbl.reset();
   }
   if ((inf >> dbtbl)) {
      cerr << "Error in reading csv file!!" << endl;
      exit(-1);
   }

   cout << "========================" << endl;
   cout << " Print table " << endl;
   cout << "========================" << endl;
   cout << dbtbl << endl;
   

   // TODO
   // Insert what you want to test here by calling DBTable's member functions
   vector<int> vR(2,1);
   vector<int> vC(5,5);
   DBRow row(vR);
   cout<<"addRow"<<endl;
   dbtbl.addRow(row);
   cout<<dbtbl;
   cout<<"addCol"<<endl;
   dbtbl.addCol(vC);
   cout<<dbtbl;
   cout<<"delRow"<<endl;
   dbtbl.delRow(1);
   cout<<dbtbl;
   cout<<"delCol"<<endl;
   dbtbl.delCol(0);
   cout<<dbtbl;
   cout<<"getMax: "<<dbtbl.getMax(0)<<endl;
   cout<<"getMin: "<<dbtbl.getMin(0)<<endl;
   cout<<"getSum: "<<dbtbl.getSum(0)<<endl;
   cout<<"getCnt: "<<dbtbl.getCount(0)<<endl;
   cout<<"getAvg: "<<setprecision(2)<<dbtbl.getAve(0)<<endl;
   cout<<"prtCol"<<endl;
   dbtbl.printCol(2);

   return 0;
}
