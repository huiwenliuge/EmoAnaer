#ifndef RNNET_INC
#define RNNET_INC

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <cmath>
#include <thread>
#include <mutex>
#include <chrono>
#include <atomic>
#include "DictCreate.hpp"

#define EMOTIONNUM 7



struct sentence{
    long num;
    char* schar;
    int tlen;
    double** inVec;
    double** hidLay;
    double** kLayer;
    double** outLay;
    double** energy;
    double* eng;
    double* stdAns;
    double loss;
    double** DLDo;
    double** DLDh;
    double** DLDx;
    struct sentence* next;

};
typedef struct sentence* Sentence;

class RNNet{

    public:
        RNNet(int featurenum,int hidlayernum,const char* pathRoot);
        ~RNNet(){
            FreeAll();
        };

        void FirstVar(const char* filePath);
        void Save2txt(const char* PathRoot);
        void RestoreVar(const char* filePath);
        double RNNForward_CROSSENTROPY(const char* pathRoot);
        double RNNForward_CROSSENTROPY_Nthread(const char* pathRoot,int Nthread);
        void RNNBackward_CROSSENTROPY(double lnrt);
        void RNNBackward_CROSSENTROPY_Nthread(double lnrt,int Nthread);

        void prtSent(const char* path);
        void prtPara(const char* path);

        ChUTF8* ccdict;
        ChUTF8* tdict;
        double** in2hid;
        double** k2out;
        double** rh2h;
        double** ro2h;

        double sigm(double x){
            return 1.0/(1.0+exp(-x));
        }

        double dsigm(double x){
            return sigm(x)*(1.0-sigm(x));
        }
        
        //static std::atomic<Sentence> asent;
    private:
        int featureNUM;
        int hidlayerNUM;
        int DictSize;
        double lnrt;
        long flag;
        long totnum;
        double** DLDin2hid;
        double** DLDk2out;
        double** DLDrh2h;
        double** DLDro2h;
        
        Sentence sent;
        Sentence FetchSent(const char* pathRoot);
        ChUTF8* FetchDict(const char* pathRoot);
        void FetchPara(const char* pathRoot);
        
        static void init_thread(int n);
        
        double _Forward_CROSSENTROPY(Sentence sen);
        static double _Forward_CROSSENTROPY_SingleTask(RNNet* rnn,Sentence* sen,int n);
        void _Backward_CROSSENTROPY(Sentence sen);
        static void _Backward_CROSSENTROPY_SingleTask(RNNet* rnn,Sentence* sen,int n);

        void softmax_add(Sentence sen,double* src,int t);
        int FreeSen();

        char** EMOLIST;

        void _save_var(const char* pathRoot);

        void FreeAll();


};

#endif