/****************************************************************************
  FileName     [ cmdReader.cpp ]
  PackageName  [ cmd ]
  Synopsis     [ Define command line reader member functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2007-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/
#include <cassert>
#include <cstring>
#include <vector>
#include <string>
#include "cmdParser.h"

using namespace std;

//----------------------------------------------------------------------
//    Extrenal funcitons
//----------------------------------------------------------------------
void mybeep();
char mygetc(istream&);
ParseChar getChar(istream&);

//----------------------------------------------------------------------
//    Member Function for class Parser
//----------------------------------------------------------------------
void
CmdParser::readCmd()
{
   if (_dofile.is_open()) {
      readCmdInt(_dofile);
      _dofile.close();
   }
   else
      readCmdInt(cin);
}

void
CmdParser::readCmdInt(istream& istr)
{

   resetBufAndPrintPrompt();

   while (1) {
      ParseChar pch = getChar(istr);
      if (pch == INPUT_END_KEY) break;
      switch (pch) {
         case LINE_BEGIN_KEY :
         case HOME_KEY       : moveBufPtr(_readBuf); break;
         case LINE_END_KEY   :
         case END_KEY        : moveBufPtr(_readBufEnd); break;
         case BACK_SPACE_KEY : if(moveBufPtr(_readBufPtr-1))
                               deleteChar();
                               break;
         case DELETE_KEY     : deleteChar(); break;
         case NEWLINE_KEY    : addHistory();
                               cout << char(NEWLINE_KEY);
                               resetBufAndPrintPrompt(); break;
         case ARROW_UP_KEY   : moveToHistory(_historyIdx - 1); break;
         case ARROW_DOWN_KEY : moveToHistory(_historyIdx + 1); break;
         case ARROW_RIGHT_KEY: moveBufPtr(_readBufPtr+1); break;
         case ARROW_LEFT_KEY : moveBufPtr(_readBufPtr-1); break;
         case PG_UP_KEY      : moveToHistory(_historyIdx - PG_OFFSET); break;
         case PG_DOWN_KEY    : moveToHistory(_historyIdx + PG_OFFSET); break;
         case TAB_KEY        : {int count=0;
         					   char* temp_p=_readBufPtr;
         					   while(temp_p!=_readBuf){
         					   		temp_p--;
         					   		count++;
         					   }
         					   insertChar(' ', TAB_POSITION-(count%TAB_POSITION)); break;}
         case INSERT_KEY     : // not yet supported; fall through to UNDEFINE
         case UNDEFINED_KEY:   mybeep(); break;
         default:  // printable character
            insertChar(char(pch)); break;
      }
      #ifdef TA_KB_SETTING
      taTestOnly();
      #endif
   }
}

bool
CmdParser::moveBufPtr(char* const ptr)
{
  if(ptr>_readBufEnd || ptr<_readBuf){
  
    mybeep();
    
    return false;
  }
  
  else{
  
  while(_readBufPtr!=ptr){

    if(ptr<_readBufPtr){
    
      _readBufPtr--;
    
      cout<<'\b';
    }
    
    else{
    
      cout<<*_readBufPtr;
    
      _readBufPtr++;
    }
  }
   return true;
 }
}

bool
CmdParser::deleteChar()
{
   if(_readBufPtr!=_readBufEnd){

         char* temp_ptr=_readBufPtr;

         char* next_ptr=temp_ptr;

         int count=1;

         next_ptr++;

        while(next_ptr!=_readBufEnd){

         *temp_ptr=*next_ptr;

         cout<<*temp_ptr;

         temp_ptr++;

         next_ptr++;

         count++;

        }

        _readBufEnd--;

        *_readBufEnd = '\0';


        cout<<" ";

        for(int i=1;i<(count+1);i++)

          cout<<'\b';
      	/*
        if(_tempCmdStored){
          
          _history.pop_back();

          _tempCmdStored = false;
        }
        */
   }

   else

    mybeep();

   return true;
}

void
CmdParser::insertChar(char ch, int repeat)
{
    assert(repeat >= 1);

    for(int i=0; i<repeat; i++){

       if(_readBufPtr == _readBufEnd){
      
          *_readBufPtr = ch;

          _readBufEnd++;

          cout<< *_readBufPtr;

          _readBufPtr++;
      }

      else{

        char temp_c;
        char next_c;
        char* temp_ptr;
        int count=0;

        temp_c = ch;

        temp_ptr = _readBufPtr;

        _readBufEnd++;

        while(_readBufPtr!=_readBufEnd){

          next_c = *_readBufPtr;

          *_readBufPtr = temp_c;

          cout<<*_readBufPtr;

          _readBufPtr++;

          temp_c = next_c;

          count++;
        }

        _readBufPtr = temp_ptr;
          

        for(int i=1;i<count;i++)

          cout<<'\b';

        _readBufPtr = temp_ptr;

        _readBufPtr++;
      }  
    }
    /*
    if(_tempCmdStored){
        
        _history.pop_back();

        _tempCmdStored = false;
      }
      */
}

void
CmdParser::deleteLine()
{
    //cout<<"in delete line";
    while(_readBufPtr!=_readBuf){

      cout<<'\b';

      _readBufPtr--;
    }

    while(_readBufPtr!=_readBufEnd){

      cout<<' ';
      _readBufPtr++;
    }

    while(_readBufPtr!=_readBuf){

      cout<<'\b';

      _readBufPtr--;

      *(_readBufPtr+1) = '\0';
    }
}

void
CmdParser::moveToHistory(int index)
{
  if(!_tempCmdStored){

   if(index<0){
     
      if(_historyIdx==0)
     
       mybeep();
     
      else{
     
        index = 0;
     
        _historyIdx = index;
     
        retrieveHistory();
      }
    }
   
   else if(index>_history.size()){
   
      if(_historyIdx==_history.size())
   
       mybeep();
   
      else{
   
        index = _history.size();

        _historyIdx = index;
        
        retrieveHistory();
      }
    }

   else{

        _history.push_back(_readBuf);

        _tempCmdStored = true;

        _historyIdx = index;
      
        retrieveHistory();
    }
  }

  else{

     if(index<0){
        
        if(_historyIdx==0)
        
          mybeep();
        
        else{
        
          index = 0;
        
          _historyIdx = index;
        
          retrieveHistory();
        }
      }

      else if (index>(_history.size()-1)){
      
        if(_historyIdx==(_history.size()-1))
      
          mybeep();
      
        else{
      
          index = _history.size()-1;
      
          _historyIdx = index;
      
          retrieveHistory();
        }
      }

      else{
      	if(index == (_history.size()-1)){
      		_history.pop_back();
      		_tempCmdStored = false;
      	}

        _historyIdx = index;
          
        retrieveHistory();
      }
    }
}

void
CmdParser::addHistory()
{
    if(_tempCmdStored){

      _history.pop_back();

      _tempCmdStored = false;
    }
    //check space
     while(*_readBuf==' '){
        
       char* next_ptr;
          
       _readBufPtr = _readBuf;
          
       next_ptr = _readBufPtr;
          
       next_ptr++;
          
       while(_readBufPtr!=_readBufEnd){
          
         *_readBufPtr=*next_ptr;
          
         _readBufPtr++;
          
         next_ptr++;
        }
          
        _readBufEnd--;
          
        *_readBufEnd = '\0';
        }
   
        while(*(_readBufEnd-1)==' '){
          
          _readBufEnd--;
          
          *_readBufEnd = '\0';
        }
      
   if(_readBuf!=_readBufEnd)  
      
      _history.push_back(_readBuf);

    _readBufPtr = _readBuf;

    while(_readBufPtr!=_readBufEnd){

      *_readBufPtr='\0';

      _readBufPtr++;
    }

    _readBufPtr = _readBufEnd = _readBuf;

    _historyIdx = _history.size();
}

void
CmdParser::retrieveHistory()
{
   deleteLine();
   
   strcpy(_readBuf, _history[_historyIdx].c_str());

   cout << _readBuf;

   _readBufPtr = _readBufEnd = _readBuf + _history[_historyIdx].size();
}