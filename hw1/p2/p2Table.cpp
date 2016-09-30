#include "p2Table.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstdlib> 
#include <iomanip>

using namespace std;

// Implement member functions of class Row and Table here
ifstream& open_file(ifstream& in, const string& file){

   in.close(); 

   in.clear();

   in.open(file.c_str()); 

   return in; 
} 

Row::Row (size_t r_size) {

    row_size = r_size;

    _data = new int[r_size];

}

const int Row::operator[] (size_t i) const {

    return _data[i];

}

int& Row::operator[] (size_t i) {

    return _data[i];

}

void Row::printR() {

    for (int i = 0; i < row_size; ++i) {

        cout << setw(4) << right;

        if (_data[i] == -100) 

        	cout << " ";

        else 

        	cout << _data[i];

    }

    cout << endl;

}


bool
Table::read(const string& csvFile)
{
	ifstream fin;
	string line,str,s;
	vector<string> vs0,vs1;

   	open_file(fin, csvFile);
   
   	getline(fin, line);

   	istringstream record(line); // bind record to line we just read

   	while(record>>str)

   		vs0.push_back(str);

   	for(auto e0:vs0){

   		int comma=0;

   		int seq=0;

   		bool num = true;

		   		
   		for (auto c : e0) {


		 if (c == ','){

		 	comma+=1;

		 	if(comma!=seq){

		 		s.push_back('-');
		 		s.push_back('1');
		 		s.push_back('0');
		 		s.push_back('0');

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
		 	s.push_back('1');
		 	s.push_back('0');
		 	s.push_back('0');

			seq+=1;
		}

		size_row = seq;

		vs1.push_back(s);

		s.clear();

		
   	}

   	size_col = vs1.size();


   	//write in _data

   	for(auto e: vs1){
   		
   		int val;

   		Row temp_row(size_row);

   		istringstream towrite(e);
   		
   		int i =0;

   		while(towrite>>val){

   			temp_row[i] = val;

   			i+=1;

   		}

   		_rows.push_back(temp_row);

   		for(int i=0; i<size_row ;++i){

   			cout << setw(4) << right;

   			if (temp_row[i] == -100) 

        	cout << " ";

        	else 

        	cout << temp_row[i];
   		}

   		cout<<endl;
   	}

    size_col = _rows.size();

  
	return true; // TODO
}


const Row& Table::operator[] (size_t i) const {

    return _rows[i];

}

Row& Table::operator[] (size_t i) {

    return _rows[i];

}

void Table::print(){

	for(int i=0; i<size_col;++i){

		_rows[i].printR();

	}

}
  

int Table::sum(const int& in){

	int temp_sum=0;

	for(int i=0; i<size_col;++i){

        if (_rows[i][in]!= -100) 

			temp_sum+=_rows[i][in];		

	}

	return temp_sum;

}
int Table::max(const int& in){

	int temp_max=-100;
	int next;

	for(int i=0; i<size_col;++i){

		next=_rows[i][in];	

		if (next>temp_max)

			temp_max=next; 

	}

	return temp_max;

}
int Table::min(const int& in){

	int temp_min=101;
	int next;

	for(int i=0; i<size_col;++i){

		if(_rows[i][in]!=-100)

			next=_rows[i][in];	

		if (next<temp_min)

			temp_min=next; 

	}

	return temp_min;
}
int Table::count(const int& in){

	int temp_count=0;

	for(int i=0; i<size_col;++i){

        if (_rows[i][in]!= -100) 

			temp_count++;
	}

		return temp_count;
}

float Table::ave(const int& in){

	float temp_count = (float) count(in);

	float temp_sum = (float) sum(in);

	float temp_ave = temp_sum/temp_count;

	return temp_ave;

}

void Table::add(const Row& nrow){

	_rows.push_back(nrow);

	size_col = _rows.size();

}



size_t Table::getRowSize(){return size_row;}
size_t Table::getColSize(){return size_col;}





