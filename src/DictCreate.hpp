#ifndef DIC_INC
#define DIC_INC

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <fstream>

struct chUTF8{
    char* schar; 
    double* vecftr;
    struct chUTF8* next;  
};

typedef struct chUTF8* ChUTF8;

inline ChUTF8* tabCreate(){
    ChUTF8* ret = (ChUTF8*)calloc(sizeof(ChUTF8),32*64*64);
    return ret;
}


inline int retLenUTF8(const char* head){
    int ch=(unsigned char)*head;
    if(ch<0xC0)
        return 1;
    else if(ch<0xE0)
        return 2;
    else if(ch<0xF0)
        return 3;
    else if(ch<0xF8)
        return 4;
    else if(ch<0xFC)
        return 5;
    else
        return 6;
}

//default table size 32*64*64
inline int hFuncUTF8(const char* head){
    int lenUTF8=retLenUTF8(head);

    if(lenUTF8 < 3){
        int ch=(unsigned char)head[0];
        ch=ch%64;
        return ch*32*64;

    }else{
        int ch1 = (unsigned char)head[0];
        int ch2 = (unsigned char)head[1];
        int ch3 = (unsigned char)head[2];

        int ret,l1,l2,l3;
        l1 = ch1%32;
        l2 = ch2%64;
        l3 = ch3%64;
        ret = l1*64*64+l2*64+l3;
        return ret;
    }
} 

//a default table of dict
ChUTF8* fldDic(FILE* fp,int dim);

void fapdDic(FILE* infp,FILE* srcfp,FILE* outfp,int dim);

void hashput(ChUTF8* tab,ChUTF8 unit);

ChUTF8 hashget(ChUTF8* tab,char* src);

void prtHash2File(ChUTF8* tab,FILE* fp,int dim);

void hashFree(ChUTF8* tab);

bool hIscontain(ChUTF8* tab,char* src,int len);

void fcrtDic(FILE* infp,FILE* outfp,int dim);

int strLenUTF8(char* str);

double** crtInvec(char* str,int invecdim,ChUTF8* tab);

void upgradeDict(ChUTF8* tab,char* str,double** dInvec,int indim,double lnrt);

void upDictbyTab(ChUTF8* tab,ChUTF8* ttab,int indim,double lnrt);

#endif
