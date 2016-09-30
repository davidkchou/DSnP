#ifndef P2_TABLE_H
#define P2_TABLE_H

#include <vector>
#include <string>

using namespace std;

class Row
{
public:
   const int operator[] (size_t i) const;  // TODO
   int& operator[] (size_t i); // TODO
   Row(size_t rowSize);
   void printR();

private:
   int  *_data;
   int row_size;
};

class Table
{
public:
   const Row& operator[] (size_t i) const;
   Row& operator[] (size_t i);

   size_t getRowSize();
   size_t getColSize();

   bool read(const string&);
   void print();
   void add(const Row& nrow);


   int sum(const int& in);
   int max(const int& in);
   int min(const int& in);
   int count(const int& in);

   float ave(const int& in);



private:
   vector<Row>  _rows;
   size_t size_row;
   size_t size_col;
};

#endif // P2_TABLE_H
