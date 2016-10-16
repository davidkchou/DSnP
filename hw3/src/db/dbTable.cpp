/****************************************************************************
  FileName     [ dbTable.cpp ]
  PackageName  [ db ]
  Synopsis     [ Define database Table member functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2015-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#include <iomanip>
#include <string>
#include <cctype>
#include <cassert>
#include <set>
#include <algorithm>
#include <vector>
#include <iostream>
#include <sstream>
#include "dbTable.h"

using namespace std;

/*****************************************/
/*          Global Functions             */
/*****************************************/
ostream& operator << (ostream& os, const DBRow& r)
{
   // vODO: to print out a row.
   // - Data are seperated by a space. No trailing space at the end.
   // - Null cells are printed as '.'
   for(int i=0;i<r.size();i++){
    if(r[i]!=INT_MAX)
      os<<r[i]<<' ';
    else
      os<<". ";
   }
   os<<endl;
   return os;
   
}

ostream& operator << (ostream& os, const DBTable& t)
{
   // vODO: to print out a table
   // - Data are seperated by setw(6) and aligned right.
   // - Null cells should be left blank (printed as ' ').
   for(int j=0; j<t.nRows();j++){
     for(int i=0;i<t[j].size();i++){
        os<<setw(6)<<right;
        if(t[j][i]!=INT_MAX)
          os<<t[j][i];
        else
          os<<" ";
      }
      os<<endl;
     }
   return os;

}

ifstream& operator >> (ifstream& ifs, DBTable& t)
{
   // vODO: to read in data from csv file and store them in a table
   // - You can assume all the data of the table are in a single line
   string s0;
   while(ifs>>s0){
      vector<int> tempRow;
      string s,ss;
      int comma=0;
      int seq=0;
      bool num = true;

      for (auto c : s0) {
       if (c == ','){
        comma+=1;

        if(comma!=seq){
          s.push_back('-');
          s.push_back('-');
          seq+=1;
        }

        s.push_back(' ');
        num=true;
       }
    
      else{
        while(num){
          seq+=1;
          num = false ;
        }
        s.push_back(c);
      }
     }
    if(seq==comma){
      s.push_back('-');
      s.push_back('-');
      seq+=1;
    }
// write in 
      istringstream is(s);
      while(is>>ss){
        if(ss=="--")
          tempRow.push_back(INT_MAX);
        else
          tempRow.push_back(stoi(ss));
      }
      DBRow tempDBRow(tempRow);
      t.addRow(tempDBRow);
      s0.clear();
     }
   
   return ifs;
   
}

/*****************************************/
/*   Member Functions for class DBRow    */
/*****************************************/
void
DBRow::removeCell(size_t c)
{
   // vODO
   for(int i=c+1; i<_data.size(); i++)
     _data[i-1]=_data[i];
  _data.pop_back();
}

/*****************************************/
/*   Member Functions for struct DBSort  */
/*****************************************/
bool
DBSort::operator() (const DBRow& r1, const DBRow& r2) const
{
   // vTODO: called as a functional object that compares the data in r1 and r2
   //       based on the order defined in _sortOrder
  for(int i=0;i<_sortOrder.size();i++){
    if(r1[_sortOrder[i]]>r2[_sortOrder[i]]){
      return true;
      break;
    }
  }
   return false;
}

/*****************************************/
/*   Member Functions for class DBTable  */
/*****************************************/
void
DBTable::reset()
{
   // TODO ??
   vector<DBRow> tmp;
   tmp.swap(_table);

}

void
DBTable::addCol(const vector<int>& d)
{
  
   // vODO: add a column to the right of the table. Data are in 'd'.
   for(int i=0; i<d.size();i++)
      _table[i].addData(d[i]);
   
}

void
DBTable::delRow(int c)
{
   // vODO: delete row #c. Note #0 is the first row.
   for(int i=c+1;i<nRows();i++){
     for(int j=0; j<nCols();j++){
        _table[i-1][j]=_table[i][j];
     }
   }
    _table.pop_back();
}

void
DBTable::delCol(int c)
{
   // delete col #c. Note #0 is the first row.
   for (size_t i = 0, n = _table.size(); i < n; ++i)
      _table[i].removeCell(c);
}

// For the following getXXX() functions...  (except for getCount())
// - Ignore null cells
// - If all the cells in column #c are null, return NAN
// - Return "float" because NAN is a float.
float
DBTable::getMax(size_t c) const
{
   // vODO: get the max data in column #c
   float temp_max=INT_MIN;
   bool nan = true;
   for(int i=0;i<nRows();i++){
      if(_table[i][c]>temp_max && _table[i][c]!=INT_MAX){
        temp_max=_table[i][c];
        nan=false;
      }
   }
   return (nan ? 0.1:temp_max);
}

float
DBTable::getMin(size_t c) const
{
   // vODO: get the min data in column #c
   float temp_min=INT_MAX;
   for(int i=0;i<nRows();i++){
      if(_table[i][c]<temp_min){
        temp_min=_table[i][c];
      }
   }
   return (temp_min==INT_MAX ? 0.1:temp_min);
}

float 
DBTable::getSum(size_t c) const
{
   // vODO: compute the sum of data in column #c
   float temp_sum=0;
   bool nan=true;
   for(int i=0;i<nRows();i++){
    if(_table[i][c]!=INT_MAX){
       temp_sum+=_table[i][c];
       nan=false;
    }      
   }
   return (nan ? 0.1:temp_sum);
}

int
DBTable::getCount(size_t c) const
{
   // vODO: compute the number of distinct data in column #c
   // - Ignore null cells
   int temp_count=0;
   for(int i=0;i<nRows();i++){
    if(_table[i][c]!=INT_MAX)
      temp_count++;
   }
   return temp_count;
}

float
DBTable::getAve(size_t c) const
{
   // vODO: compute the average of data in column #c
   bool nan = true;
   if(getCount(c)!=0)
      nan = false;
   return (nan ? 0.1:getSum(c)/getCount(c));
}

void
DBTable::sort(const struct DBSort& s)
{
   // vTODO: sort the data according to the order of columns in 's'
  for(int i=0; i<nRows(); i++){
    for(int j=0; j<nCols(); j++)
      if(s(_table[i],_table[j])){
        for(int k=0; k<nCols(); k++){
          int temp_int = _table[i][k];
          _table[i][k] = _table[j][k];
          _table[j][k] = temp_int;
        }
      } 
  }
}

void
DBTable::printCol(size_t c) const
{
   // vODO: to print out a column.
   // - Data are seperated by a space. No trailing space at the end.
   // - Null cells are printed as '.'
  if(c>=nCols())
        cout<<"NAN";
  else{
   for(int i=0;i<nRows();i++){
      if(_table[i][c]==INT_MAX)
        cout<<". ";
      else
        cout<<_table[i][c]<<" ";
   }
 }
   cout<<endl;
}

void
DBTable::printSummary() const
{
   size_t nr = nRows(), nc = nCols(), nv = 0;
   for (size_t i = 0; i < nr; ++i)
      for (size_t j = 0; j < nc; ++j)
         if (_table[i][j] != INT_MAX) ++nv;
   cout << "(#rows, #cols, #data) = (" << nr << ", " << nc << ", "
        << nv << ")" << endl;
}

