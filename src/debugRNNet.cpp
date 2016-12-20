#include "RNNet.hpp"

int main(void){
    RNNet rnn(256,32,"../");
    const char *pa = "../";
    rnn.RNNForward_CROSSENTROPY_Nthread("../",2);    
    rnn.prtSent("SENTN");
    rnn.RNNBackward_CROSSENTROPY_Nthread(0.1,2);
    for(int n=0;n<0;++n){
        std::cout << "n: "<< n <<std::endl;
        rnn.RNNForward_CROSSENTROPY_Nthread("../",2);
        rnn.RNNBackward_CROSSENTROPY_Nthread(0.1,2);
        //rnn.prtSent("SENTN");
    }
    rnn.RNNForward_CROSSENTROPY_Nthread("../",2);
    rnn.prtSent("TSENT");
    //rnn.Save2txt("../");
}
