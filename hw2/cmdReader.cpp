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

void moveCursorR(char*&, char*&);
void moveCursorL(char*&, const char&);
void backspace(char*&, char*&,const char&);
void insertTab(char*&, char*&);


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
         case BACK_SPACE_KEY : /* vTODO */ backspace(_readBufPtr,_readBufEnd,_readBuf[0]); break;
         case DELETE_KEY     : deleteChar(); break;
         case NEWLINE_KEY    : addHistory();
                               cout << char(NEWLINE_KEY);
                               resetBufAndPrintPrompt(); break;
         case ARROW_UP_KEY   : moveToHistory(_historyIdx - 1); break;
         case ARROW_DOWN_KEY : moveToHistory(_historyIdx + 1); break;
         case ARROW_RIGHT_KEY: /* vTODO */ moveCursorR(_readBufPtr,_readBufEnd); break;
         case ARROW_LEFT_KEY : /* vTODO */ moveCursorL(_readBufPtr,_readBuf[0]); break;
         case PG_UP_KEY      : moveToHistory(_historyIdx - PG_OFFSET); break;
         case PG_DOWN_KEY    : moveToHistory(_historyIdx + PG_OFFSET); break;
         case TAB_KEY        : /* vTODO */ insertTab(_readBufPtr,_readBufEnd); break;
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


// This function moves _readBufPtr to the "ptr" pointer
// It is used by left/right arrowkeys, home/end, etc.
//
// Suggested steps:
// 1. Make sure ptr is within [_readBuf, _readBufEnd].
//    If not, make a beep sound and return false. (DON'T MOVE)
// 2. Move the cursor to the left or right, depending on ptr
// 3. Update _readBufPtr accordingly. The content of the _readBuf[] will
//    not be changed
//
// [Note] This function can also be called by other member functions below
//        to move the _readBufPtr to proper position.
bool
CmdParser::moveBufPtr(char* const ptr)
{
   //vTODO...
  while(_readBufPtr!=ptr){

    if(ptr==_readBuf){
    
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

// [Notes]
// 1. Delete the char at _readBufPtr
// 2. mybeep() and return false if at _readBufEnd
// 3. Move the remaining string left for one character
// 4. The cursor should stay at the same position
// 5. Remember to update _readBufEnd accordingly.
// 6. Don't leave the tailing character.
// 7. Call "moveBufPtr(...)" if needed.
//
// For example,
//
// cmd> This is the command
//              ^                (^ is the cursor position)
//
// After calling deleteChar()---
//
// cmd> This is he command
//              ^
//
bool
CmdParser::deleteChar()
{
   // vTODO...

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
   }

   else

    mybeep();

   return true;
}

// 1. Insert character 'ch' for "repeat" times at _readBufPtr
// 2. Move the remaining string right for "repeat" characters
// 3. The cursor should move right for "repeats" positions afterwards
// 4. Default value for "repeat" is 1. You should assert that (repeat >= 1).
//
// For example,
//
// cmd> This is the command
//              ^                (^ is the cursor position)
//
// After calling insertChar('k', 3) ---
//
// cmd> This is kkkthe command
//                 ^
//
void
CmdParser::insertChar(char ch, int repeat)
{
   // vTODO...
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

}

// 1. Delete the line that is currently shown on the screen
// 2. Reset _readBufPtr and _readBufEnd to _readBuf
// 3. Make sure *_readBufEnd = 0
//
// For example,
//
// cmd> This is the command
//              ^                (^ is the cursor position)
//
// After calling deleteLine() ---
//
// cmd>
//      ^
//
void
CmdParser::deleteLine()
{
     // vTODO...

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
    }


}


// This functions moves _historyIdx to index and display _history[index]
// on the screen.
//
// Need to consider:
// If moving up... (i.e. index < _historyIdx)
// 1. If already at top (i.e. _historyIdx == 0), beep and do nothing.
// 2. If at bottom, temporarily record _readBuf to history.
//    (Do not remove spaces, and set _tempCmdStored to "true")
// 3. If index < 0, let index = 0.
//
// If moving down... (i.e. index > _historyIdx)
// 1. If already at bottom, beep and do nothing
// 2. If index >= _history.size(), let index = _history.size() - 1.
//
// Assign _historyIdx to index at the end.
//
// [Note] index should not = _historyIdx
//
void
CmdParser::moveToHistory(int index)
{
  if(!_tempCmdStored){

   if(index<0 || index>_history.size())

      mybeep();

   else{

      if(_historyIdx == _history.size()){

        _history.push_back(_readBuf);

        _tempCmdStored = true;

      }
        _historyIdx = index;
      
        retrieveHistory();
    }
  }

  else{

     if(index<0 || index>(_history.size()-1))

        mybeep();

      else{

        _historyIdx = index;
        
        if(_historyIdx == (_history.size()-1)){
            
            _history.pop_back();
            
            _tempCmdStored = false;

            _readBufPtr = _readBuf;

              while(_readBufPtr!=_readBufEnd){

                *_readBufPtr='\0';

                _readBufPtr++;
              }
          }

        retrieveHistory();

        

      }
    }
}

// This function adds the string in _readBuf to the _history.
// The size of _history may or may not change. Depending on whether 
// there is a temp history string.
//
// 1. Remove ' ' at the beginning and end of _readBuf
// 2. If not a null string, add string to _history.
//    Be sure you are adding to the right entry of _history.
// 3. If it is a null string, don't add anything to _history.
// 4. Make sure to clean up "temp recorded string" (added earlier by up/pgUp,
//    and reset _tempCmdStored to false
// 5. Reset _historyIdx to _history.size() // for future insertion
//
void
CmdParser::addHistory()
{
   // TODO...
    if(_tempCmdStored){

      _history.pop_back();

      _tempCmdStored = true;
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


// 1. Replace current line with _history[_historyIdx] on the screen
// 2. Set _readBufPtr and _readBufEnd to end of line
//
// [Note] Do not change _history.size().
//
void
CmdParser::retrieveHistory()
{
   deleteLine();
   
   strcpy(_readBuf, _history[_historyIdx].c_str());

   cout << _readBuf;

   _readBufPtr = _readBufEnd = _readBuf + _history[_historyIdx].size();
}

//My Functions 
void backspace(char*& rbp,char*& rbe,const char& rb){

  if(rbp!=&rb){

  char* temp_ptr = rbp;

  char* pre_ptr = temp_ptr;
  
  pre_ptr--;
  
  int count=0;
  
  cout<<'\b';
  
  while(temp_ptr!=rbe){
  
    *pre_ptr = * temp_ptr;
  
    cout<<*pre_ptr;
    
    temp_ptr++;
    
    pre_ptr++;
    
    count++;
  }


  rbp--; 
  
  rbe--;

  *rbe='\0';

  
  cout<<" ";
  
  for(int i=0;i<(count+1);i++)
      cout<<'\b';
  }
  
  else 
    mybeep();


}


void moveCursorR(char*& rbp, char*& rbe){

  if(rbp!=rbe){
  
    cout<<*rbp;
  
    rbp++;
}
  
  else
    mybeep();
}

void moveCursorL(char*& rbp, const char& rb){

  if(rbp!= &rb){
  
    cout<<'\b';
  
    rbp--;
  }
  
  else
    mybeep();
}

void insertTab(char*& rbp, char*& rbe){

  if(rbp == rbe){

      for(int i=0; i<TAB_POSITION; i++){

        *rbp =' ';

        rbe++;

        cout<< *rbp;

        rbp++;
      }
  }

  else{

    for(int i=0; i<TAB_POSITION; i++){
      char temp_c;
      char next_c;
      char* temp_ptr;
      int count=0;

      temp_c = ' ';

      temp_ptr = rbp;

      rbe++;

      while(rbp!=rbe){

        next_c = *rbp;

        *rbp = temp_c;

        cout<<*rbp;

        rbp++;

        temp_c = next_c;

        count++;

      }

      rbp = temp_ptr;
        

      for(int i=1;i<count;i++)
        cout<<'\b';

      rbp = temp_ptr;

      rbp++;

    }

  }


}
