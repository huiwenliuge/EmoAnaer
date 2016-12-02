#include "RNNet.hpp"

int main(void){
    RNNet rnn(256,32,"../");
    
    rnn.RNNForward_CROSSENTROPY_Nthread("../",2);    
    rnn.prtSent("SENTN");
    rnn.RNNBackward_CROSSENTROPY_Nthread(0.001,2);
    for(int n=0;n<10;++n){
        rnn.RNNForward_CROSSENTROPY_Nthread("../",2);
        rnn.RNNBackward_CROSSENTROPY_Nthread(0.001,2);
    }
    rnn.prtSent("TSENT");
}