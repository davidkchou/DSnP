#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdlib> 
#include <iomanip>
#include "p2Table.h"

using namespace std;




int main()
{
   Table table;
   

   // TODO: read in the csv file
   
   string csvFile;
   
   cout << "Please enter the file name: ";
   
   cin >> csvFile;
   
   if (table.read(csvFile))
   
      cout << "File \"" << csvFile << "\" was read in successfully." << endl;
   
   else exit(-1); // csvFile does not exist.


   // TODO read and execute commands

   
   vector<string> history;

   while (true) {

      enum cmd {PRINT,SUM,AVE,MAX,MIN,COUNT,ADD,EXIT,UN}; 

      string input;

      int input_case;
      int arg=-1;
      
      cin>>input;

      if(input=="PRINT")      input_case=PRINT;

      else if(input=="SUM")   {
         input_case=SUM;
         cin >> arg;
      }

      else if(input=="AVE") {
         input_case=AVE;
         cin >> arg;

      }

      else if(input=="MAX") {
         input_case=MAX;
         cin >> arg;

      }

      else if(input=="MIN")  {
         input_case=MIN;
         cin >> arg;
      }
      
      
      else if(input=="COUNT") {
         input_case=COUNT;
         cin >> arg;
      }
      
      else if(input=="ADD")   {
         input_case=ADD;         
      }

      else if(input=="EXIT") input_case=EXIT;

      
      else  input_case = UN;
      
      switch(input_case){
         
         case PRINT:
         table.print();

         history.push_back(input);

         break;
         
         case SUM:

         if (arg >= 0 && arg < table.getRowSize()) {

            int sum = table.sum(arg);

            cout << "The summation of data in column #" << arg << " is " << sum << "." << endl;
         } 

         break;

         case AVE:

         if (arg >= 0 && arg < table.getRowSize()) {

            float avg = table.ave(arg);

            cout << "The average of data in column #" << arg << " is " << fixed << setprecision(1) << avg << "." << endl;
         } 

         break;

         case MAX:
         
         if (arg >= 0 && arg < table.getRowSize()) {

            int max = table.max(arg);
            
            cout << "The maximum of data in column #" << arg << " is " << max << "." << endl;
         } 

         break;

         case MIN:
         
         if (arg >= 0 && arg < table.getRowSize()) {
         
            int min = table.min(arg);
         
            cout << "The minimum of data in column #" << arg << " is " << min << "." << endl;
         } 

         break;

         case COUNT:
         
         if (arg >= 0 && arg < table.getRowSize()) {
         
            int cnt = table.count(arg);
         
            cout << "The distinct count of data in column #" << arg << " is " << cnt << "." << endl;
         } 

         break;
         
         case ADD:
         {

            Row temp_row(table.getRowSize());

            string dump;

            int nval = 0;

            bool good = true;

            for(int j=0;j<table.getRowSize();++j){

               cin>>dump;

               if(dump=="-"){

                  temp_row[j]=-100;

               }

               else{

                  istringstream temp_is(dump);

                  temp_is>>nval;
                  
                  if(nval>100||nval<-99)
                  {
                     cout<<"Input out of range(-99/100)"<<endl;

                     good = false;
                  }

                  temp_row[j]=nval;
               } 
            }
            if(good){

               table.add(temp_row);
            
               table.print();
            }
         }

         break;
         
         case EXIT: exit(0);
         
         break;

         case UN: cout << input <<": command not found" << endl;
        
         break;



      }
   
     
   }

   return 0;
}
