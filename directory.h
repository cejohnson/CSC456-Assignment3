/*******************************************************
                        directory.h
                       Chris Johnson
                           2013
********************************************************/

#ifndef DIRECTORY_H
#define DIRECTORY_H

#include <cmath>
#include <iostream>

typedef unsigned long ulong;
typedef unsigned char uchar;
typedef unsigned int uint;

/****add new states, based on the protocol****/
enum directoryStates {
  EXCLUSIVE_MODIFIED_D = 0,
  SHARED_D,
  UNCACHED_D
};

class Cache;

class directoryBlock 
{
protected:
   ulong tag;
   ulong Flags;   // 0:invalid, 1:valid, 2:dirty 
   uchar *vector; //Full bit-vector
   int numProcs;
 
public:
   //Initialize directory block
   directoryBlock(int n)
   {
     tag = 0;
     Flags = UNCACHED_D;
     numProcs = n;
     vector = new uchar[n];
     for (int i = 0; i < n; i++) {
       vector[i] = 0;
     }
   }

   ulong getTag()         { return tag; }
   ulong getFlags()     { return Flags;}
   uchar getVector(int i)    { return vector[i]; }
   //uchar* getVector()         { return vector; }
   void clearVector()
   { 
     for (int i = 0; i < numProcs; i++) {
       vector[i] = 0;
     } 
     Flags = UNCACHED_D;
   }
   void setVector(int procNum, uchar value)     { vector[procNum] = value; }
   void setFlags(ulong flags)     {  Flags = flags;}
   void setTag(ulong a)   { tag = a; }
   void invalidate()      { tag = 0; clearVector(); }//useful function
   bool isUsed()         { return ((Flags) != UNCACHED_D); }
};

class Directory
{
protected:
   ulong size, lineSize, assoc, sets, log2Sets, log2Blk, tagMask, numLines;
   //ulong reads,readMisses,writes,writeMisses,writeBacks;
   //ulong invalidations, cacheToCacheTransfers;

   ulong numBlocks, numProcs;

   //******///
   //add coherence counters here///
   //******///

   directoryBlock **directory;
   ulong calcTag(ulong addr)     { return (addr >> (log2Blk) );}
   ulong calcIndex(ulong addr)  { return ((addr >> log2Blk) & tagMask);}
   ulong calcAddr4Tag(ulong tag)   { return (tag << (log2Blk));}
   
public:
    //ulong currentCycle;  
     
    Directory(int,int,int);
   ~Directory() { delete directory;}


   
   directoryBlock *findBlockToReplace(ulong addr);
   directoryBlock *fillBlock(ulong addr);
   directoryBlock * findBlock(ulong addr);

   ulong Read(ulong,ulong,Cache**);
   void ReadX(ulong,ulong,Cache**);
   void Upgrade(ulong,ulong,Cache**);

   void Reply();
   void ReplyD();

   void UnCache(ulong, ulong);
   
   /*ulong getRM(){return readMisses;} ulong getWM(){return writeMisses;} 
   ulong getReads(){return reads;}ulong getWrites(){return writes;}
   ulong getWB(){return writeBacks;}
   
   void writeBack(ulong)   {writeBacks++;}
   void Access(ulong,uchar);
   void printStats();*/

   //******///
   //add other functions to handle bus transactions///
   //******///

};

#endif
