#include "RNNet.hpp"

RNNet::RNNet(int featurenum,int hidlayernum,const char* pathRoot){

    featureNUM=featurenum;
    hidlayerNUM=hidlayernum;
    ccdict=NULL;
    tdict=NULL;
    sent=NULL;
    
    DLDin2hid=(double**)malloc(sizeof(double*)*hidlayerNUM);
    DLDk2out =(double**)malloc(sizeof(double*)*EMOTIONNUM);
    DLDrh2h  =(double**)malloc(sizeof(double*)*hidlayerNUM);
    DLDro2h  =(double**)malloc(sizeof(double*)*hidlayerNUM);

    int i;
    for(i=0;i<hidlayerNUM;++i){
        DLDin2hid[i]=(double*)calloc(sizeof(double),featureNUM);
        DLDrh2h[i]=(double*)calloc(sizeof(double),hidlayerNUM);
        DLDro2h[i]=(double*)calloc(sizeof(double),EMOTIONNUM);
    }
    for(i=0;i<EMOTIONNUM;++i)
        DLDk2out[i] =(double*)calloc(sizeof(double),hidlayerNUM);

    EMOLIST=(char**)malloc(sizeof(char*)*EMOTIONNUM);
    for(i=0;i<EMOTIONNUM;++i)
        EMOLIST[i]=(char*)calloc(sizeof(char),11);
    strcpy(EMOLIST[0],"ANGRY");
    strcpy(EMOLIST[1],"ANXIOUS");
    strcpy(EMOLIST[2],"AWKWARD");
    strcpy(EMOLIST[3],"HAPPY");
    strcpy(EMOLIST[4],"MOVE");
    strcpy(EMOLIST[5],"NOVEL");
    strcpy(EMOLIST[6],"SAD");
    
    char* pdic=(char*)calloc(sizeof(char),41);
    int pathrootlen=strlen(pathRoot);
    strcpy(pdic,pathRoot);
    strcpy(pdic+pathrootlen,"Dictionary/CROSS");
    FetchDict(pdic);
    FetchPara(pdic);
    //
    //RNNForward_CROSSENTROPY_Nthread("../",2);
    //
    
    char* path=(char*)calloc(sizeof(char),41);
    strcpy(path,"tmp/CROSS");
    _save_var(path);
    
}

void RNNet::Save2txt(const char* pathRoot){
    // char* pdic=(char*)calloc(sizeof(char),41);
    // char* ppar=(char*)calloc(sizeof(char),41);
    // int pathrootlen = strlen(pathRoot);
    // strcpy(pdic,pathRoot);
    // strcpy(ppar,pathRoot);
    // //Dictionary/CROSS
    // strcpy(pdic+pathrootlen,"Dictionary/CROSSDICT");
    // strcpy(ppar+pathrootlen,"Dictionary/CROSSPARA");

    // FILE* fp = fopen(pdic,"w");
    // prtHash2File(ccdict,fp,featureNUM);
    // prtPara(ppar);
    char* path=(char*)calloc(sizeof(char),41);
    int pathrootlen = strlen(pathRoot);
    strcpy(path,pathRoot);
    strcpy(path+pathrootlen,"Dictionary/CROSS");
    _save_var(path);
}

void RNNet::_save_var(const char* pathRoot){
    char* pdic=(char*)calloc(sizeof(char),41);
    char* ppar=(char*)calloc(sizeof(char),41);
    int pathrootlen = strlen(pathRoot);
    strcpy(pdic,pathRoot);
    strcpy(ppar,pathRoot);
    strcpy(pdic+pathrootlen,"DICT");
    strcpy(ppar+pathrootlen,"PARA");
    
    FILE* fp = fopen(pdic,"w");
    prtHash2File(ccdict,fp,featureNUM);
    prtPara(ppar);
}

void RNNet::FirstVar(const char* filePath){
    //
    std::cout << "Indeveloping\n";
}

Sentence RNNet::FetchSent(const char* pathRoot){

    //Sentence ret;
    char** filelist=(char**)malloc(sizeof(char*)*EMOTIONNUM);
    int i,j,len,pathrootlen;
    FILE* sentfp;
    char* line=(char*)calloc(sizeof(char),421);
    totnum = 0;
    pathrootlen=strlen(pathRoot);
    for(i=0;i<EMOTIONNUM;++i){
        filelist[i]=(char*)calloc(sizeof(char),101);
        strcpy(filelist[i],pathRoot);
        strcpy(filelist[i]+pathrootlen,EMOLIST[i]);
        //std::cout<< filelist[i] <<std::endl;

        sentfp=fopen(filelist[i],"r");
        // const double* ans=(double*)calloc(sizeof(double),EMOTIONNUM);
        // ans[i]=1.0;

        while(fscanf(sentfp,"%s\n",line)!=-1){

            len=strLenUTF8(line);
            Sentence sentunit=(Sentence)malloc(sizeof(struct sentence));
            sentunit->num = totnum;
            totnum++;
            sentunit->schar=line;
            sentunit->tlen=len;
            sentunit->inVec=crtInvec(line,featureNUM,ccdict);
            sentunit->hidLay=(double**)malloc(sizeof(double*)*len);
            sentunit->kLayer=(double**)malloc(sizeof(double*)*len);
            sentunit->outLay=(double**)malloc(sizeof(double*)*len);
            sentunit->DLDo  =(double**)malloc(sizeof(double*)*len);
            sentunit->DLDh  =(double**)malloc(sizeof(double*)*len);
            sentunit->DLDx  =(double**)malloc(sizeof(double*)*len);
            sentunit->energy=(double**)malloc(sizeof(double*)*len);
            sentunit->stdAns=(double*)calloc(sizeof(double),EMOTIONNUM);
            sentunit->eng=(double*)calloc(sizeof(double),EMOTIONNUM);

            for(j=0;j<len;++j){
                // for(int k=0;k<featureNUM;++k){
                //     std::cout<<sentunit->inVec[j][k];
                // }
                //std::cout<<std::endl;
                sentunit->energy[j]=(double*)calloc(sizeof(double),EMOTIONNUM);
                sentunit->hidLay[j]=(double*)calloc(sizeof(double),hidlayerNUM);
                sentunit->kLayer[j]=(double*)calloc(sizeof(double),hidlayerNUM);
                sentunit->outLay[j]=(double*)calloc(sizeof(double),EMOTIONNUM);
                sentunit->DLDo[j]  =(double*)calloc(sizeof(double),EMOTIONNUM);
                sentunit->DLDh[j]  =(double*)calloc(sizeof(double),hidlayerNUM);
                sentunit->DLDx[j]  =(double*)calloc(sizeof(double),featureNUM);
            }

            sentunit->stdAns[i]=1.0;
            sentunit->next=sent;
            sent=sentunit;

            
            line=(char*)calloc(sizeof(char),421);
        }
        fclose(sentfp);

    }

    for(i=0;i<EMOTIONNUM;++i)
        free(filelist[i]);
    //fclose(sentfp);
    free(filelist);
    std::cout<< totnum <<std::endl;
    return sent;

}

ChUTF8* RNNet::FetchDict(const char* pathRoot){

    char* path=(char*)calloc(sizeof(char),41);
    strcpy(path,pathRoot);
    strcpy(path+strlen(pathRoot),"DICT");
    
    ChUTF8* ret;
    std::cout<< path<<std::endl;
    FILE* dicfp=fopen(path,"r");
    FILE* dictmp=fopen("dictmp","w");
    ret=fldDic(dicfp,featureNUM);
    prtHash2File(ret,dictmp,featureNUM);
    free(path);
    ccdict=ret;
    return ret;

}

void RNNet::prtPara(const char* path){
    FILE* paratmp=fopen(path,"w");
    
    int i,j;
    fprintf(paratmp,"%s %d\n","Para_Mat_W",hidlayerNUM);
    for(i=0;i<EMOTIONNUM;++i){
        for(j=0;j<hidlayerNUM;++j)
            fprintf(paratmp,"#%lf#",k2out[i][j]);
        fputc('\n',paratmp);
    }

    fprintf(paratmp,"Para_Mat_B %d\n",hidlayerNUM);
    for(i=0;i<hidlayerNUM;++i){
        for(j=0;j<EMOTIONNUM;++j)
            fprintf(paratmp,"#%lf#",ro2h[i][j]);
        fputc('\n',paratmp);
    }

    fprintf(paratmp,"Para_Mat_T\n");
    for(i=0;i<hidlayerNUM;++i){
        for(j=0;j<hidlayerNUM;++j)
            fprintf(paratmp,"#%lf#",rh2h[i][j]);
        fputc('\n',paratmp);
    }

    fprintf(paratmp,"Para_Mat_U");
    for(i=0;i<hidlayerNUM;++i){
        fputc('\n',paratmp);
        for(j=0;j<featureNUM;++j)
            fprintf(paratmp,"#%lf#",in2hid[i][j]);
    }
    
    fclose(paratmp);
}

void RNNet::FetchPara(const char* pathRoot){
    char* path=(char*)calloc(sizeof(char),41);
    strcpy(path,pathRoot);
    strcpy(path+strlen(pathRoot),"PARA");

    //std::cout<< path<<std::endl;
    FILE* parafp=fopen(path,"r");
    //FILE* paratmp=fopen("paratmp","w");

    char* line=(char*)calloc(sizeof(char),81);
    int i,j;
    fscanf(parafp,"%s %d\n",line,&i);
    //std::cout<<line<<" "<<"line"<<std::endl;
    
    k2out=(double**)malloc(sizeof(double*)*EMOTIONNUM);
    
    for(i=0;i<EMOTIONNUM;++i){
        k2out[i]=(double*)malloc(sizeof(double)*hidlayerNUM);   
        for(j=0;j<hidlayerNUM;++j){
            fscanf(parafp,"#%lf#",k2out[i]+j);
      //      std::cout<<k2out[i][j]<<std::endl;
        }
        fgetc(parafp);
    }
    fscanf(parafp,"%s %d\n",line,&i);
    std::cout<<line<<std::endl;

    ro2h=(double**)malloc(sizeof(double*)*hidlayerNUM);
    for(i=0;i<hidlayerNUM;++i){
        ro2h[i]=(double*)malloc(sizeof(double)*EMOTIONNUM);
        for(j=0;j<EMOTIONNUM;++j){
            fscanf(parafp,"#%lf#",ro2h[i]+j);
        }
        fgetc(parafp);
    }

    fscanf(parafp,"%s\n",line);
    std::cout<<line<<std::endl;

    rh2h=(double**)malloc(sizeof(double*)*hidlayerNUM);
    for(i=0;i<hidlayerNUM;++i){
        rh2h[i]=(double*)malloc(sizeof(double)*hidlayerNUM);
        for(j=0;j<hidlayerNUM;++j){
            fscanf(parafp,"#%lf#",rh2h[i]+j);
        }
        fgetc(parafp);
    } 
    fscanf(parafp,"%s\n",line);
    std::cout<<line<<std::endl;

    in2hid=(double**)malloc(sizeof(double*)*hidlayerNUM);
    for(i=0;i<hidlayerNUM;++i){
        in2hid[i]=(double*)malloc(sizeof(double)*featureNUM);
        for(j=0;j<featureNUM;++j){
            fscanf(parafp,"#%lf#",in2hid[i]+j);
        }
        fgetc(parafp);
    }

    //prtPara("paratmp");
    free(path);
    free(line);
    fclose(parafp);
}

double RNNet::RNNForward_CROSSENTROPY(const char* pathRoot){

    int pathrootlen=strlen(pathRoot);
    char* psen=(char*)calloc(sizeof(char),41);
    strcpy(psen,pathRoot);
    strcpy(psen+pathrootlen,"Material/");
    FreeSen();
    Sentence sentmp=FetchSent(psen);
    free(psen);
    
    while(sentmp){
        _Forward_CROSSENTROPY(sentmp);
        sentmp=sentmp->next;
    }

    return 0;
} 

double RNNet::RNNForward_CROSSENTROPY_Nthread(const char* pathRoot,int Nthread){
    
    char* psen=(char*)calloc(sizeof(char),41);
    int pathrootlen=strlen(pathRoot);   
    strcpy(psen,pathRoot);
    strcpy(psen+pathrootlen,"Material/");
    FreeSen();
    Sentence sentmp=FetchSent(psen);
    free(psen);
    flag = totnum-1;
    
    std::thread th[Nthread];
    int n;
    // for(n=0;n<Nthread;++n){
    //     th[n]=std::thread(init_thread,n);
    // }
    // for(n=0;n<Nthread;++n)
    //     th[n].join();
    for(n=0;n<Nthread;++n){
        th[n]=std::thread(_Forward_CROSSENTROPY_SingleTask,\
            this,&sentmp,n);
    }
    for(n=0;n<Nthread;++n)
        th[n].join();
    return 0;
}

void RNNet::RNNBackward_CROSSENTROPY(double lnrate){

    // int i;
    // for(i=0;i<hidlayerNUM;++i){
    //     DLDin2hid[i]=(double*)calloc(sizeof(double),featureNUM);
    //     DLDrh2h[i]=(double*)calloc(sizeof(double),hidlayerNUM);
    //     DLDro2h[i]=(double*)calloc(sizeof(double),EMOTIONNUM);
    // }
    // for(i=0;i<EMOTIONNUM;++i)
    //     DLDk2out[i] =(double*)calloc(sizeof(double),hidlayerNUM);
    lnrt=lnrate;
    int i;
    for(i=0;i<hidlayerNUM;++i){
        memset(DLDin2hid[i],'\0',sizeof(double)*featureNUM);
        memset(DLDrh2h[i],'\0',sizeof(double)*hidlayerNUM);
        memset(DLDro2h[i],'\0',sizeof(double)*EMOTIONNUM);
    }
    for(i=0;i<EMOTIONNUM;++i)
        memset(DLDk2out[i],'\0',sizeof(double)*hidlayerNUM);

    Sentence sentmp = sent;

    while(sentmp){
        _Backward_CROSSENTROPY(sentmp);
        sentmp=sentmp->next;
    }
    
    int j;
    for(i=0;i<hidlayerNUM;++i){
        for(j=0;j<featureNUM;++j){
            in2hid[i][j]-=DLDin2hid[i][j]*lnrt;
        }
        for(j=0;j<hidlayerNUM;++j){
            rh2h[i][j]-=DLDrh2h[i][j]*lnrt;
        }
        for(j=0;j<EMOTIONNUM;++j){
            ro2h[i][j]-=DLDro2h[i][j]*lnrt;
        }
    }
    for(i=0;i<EMOTIONNUM;++i){
        for(j=0;j<hidlayerNUM;++j){
            k2out[i][j]-=DLDk2out[i][j]*lnrt;
        }
    }

}

void RNNet::RNNBackward_CROSSENTROPY_Nthread(double lnrts,int Nthread){
    lnrt=lnrts;
    int i;
    for(i=0;i<hidlayerNUM;++i){
        memset(DLDin2hid[i],'\0',sizeof(double)*featureNUM);
        memset(DLDrh2h[i],'\0',sizeof(double)*hidlayerNUM);
        memset(DLDro2h[i],'\0',sizeof(double)*EMOTIONNUM);
    }
    for(i=0;i<EMOTIONNUM;++i)
        memset(DLDk2out[i],'\0',sizeof(double)*hidlayerNUM);

    Sentence stmp = sent;
    
    flag = totnum-1;
    std::thread th[Nthread];
    int n;
    // for(n=0;n<Nthread;++n){
    //     th[n]=std::thread(init_thread,n);
    // }
    // for(n=0;n<Nthread;++n)
    //     th[n].join();
    for(n=0;n<Nthread;++n){
       th[n]=std::thread(_Backward_CROSSENTROPY_SingleTask,\
               this,&stmp,n);
    }


    for(n=0;n<Nthread;++n)
        th[n].join();

    int j;
    for(i=0;i<hidlayerNUM;++i){
        for(j=0;j<featureNUM;++j){
            in2hid[i][j]-=DLDin2hid[i][j]*lnrt;
        }
        for(j=0;j<hidlayerNUM;++j){
            rh2h[i][j]-=DLDrh2h[i][j]*lnrt;
        }
        for(j=0;j<EMOTIONNUM;++j){
            ro2h[i][j]-=DLDro2h[i][j]*lnrt;
        }
    }
    for(i=0;i<EMOTIONNUM;++i){
        for(j=0;j<hidlayerNUM;++j){
            k2out[i][j]-=DLDk2out[i][j]*lnrt;
        }
    }
}

double RNNet::_Forward_CROSSENTROPY(Sentence sen){

    int t,i,j;
    
    for(i=0;i<hidlayerNUM;++i){
        sen->hidLay[0][i]=0.0;
        for(j=0;j<featureNUM;++j)
            sen->hidLay[0][i]+=in2hid[i][j]*sen->inVec[0][j];
        sen->kLayer[0][i]=sigm(sen->hidLay[0][i]);
    }
    
    for(i=0;i<EMOTIONNUM;++i){
        sen->outLay[0][i]=0.0;
        for(j=0;j<hidlayerNUM;++j)
            sen->outLay[0][i]+=k2out[i][j]*sen->kLayer[0][j];
        sen->eng[i]=0.0;
    }
    softmax_add(sen,sen->outLay[0],0);
    // sen->loss=0.0;
    // for(i=0;i<EMOTIONNUM;++i)
    //     sen->loss+=-sen->stdAns[i]*log(sen->energy[i]);

    for(t=1;t<sen->tlen;++t){

        for(i=0;i<hidlayerNUM;++i){
            sen->hidLay[t][i]=0.0;
            for(j=0;j<featureNUM;++j)
                sen->hidLay[t][i]+=in2hid[i][j]*sen->inVec[t][j];
            for(j=0;j<hidlayerNUM;++j)
                sen->hidLay[t][i]+=rh2h[i][j]*sen->kLayer[t-1][j];
            for(j=0;j<EMOTIONNUM;++j)
                sen->hidLay[t][i]+=ro2h[i][j]*sen->outLay[t-1][j];
            sen->kLayer[t][i]=sigm(sen->hidLay[t][i]);
        }

        for(i=0;i<EMOTIONNUM;++i){
            sen->outLay[t][i]=0.0;
            for(j=0;j<hidlayerNUM;++j)
                sen->outLay[t][i]+=k2out[i][j]*sen->kLayer[t][j];  
        }
        softmax_add(sen,sen->outLay[t],t);

    }

    for(i=0;i<sen->tlen;++i)
        for(j=0;j<EMOTIONNUM;++j)
            sen->eng[j]+=sen->energy[i][j];
    for(i=0;i<EMOTIONNUM;++i)
        sen->eng[i]/=sen->tlen;
    
    sen->loss=0.0;
    for(i=0;i<EMOTIONNUM;++i)
        sen->loss+=-sen->stdAns[i]*log(sen->eng[i]);
    
    return sen->loss;
}

void RNNet::_Backward_CROSSENTROPY(Sentence sen){
    int t,i,j,k;
    t=sen->tlen-1;
    for(i=0;i<EMOTIONNUM;++i)
        sen->DLDo[t][i]=-(sen->stdAns[i]-sen->energy[t][i]);

    for(t=sen->tlen-2;t>-1;--t){
        for(i=0;i<EMOTIONNUM;++i){
            sen->DLDo[t][i]=-(sen->stdAns[i]-sen->energy[t][i]);
            for(k=0;k<EMOTIONNUM;++k){
                for(j=0;j<hidlayerNUM;++j){
                    sen->DLDo[t][i]+=sen->DLDo[t+1][k]\
                            *k2out[k][j]*dsigm(sen->hidLay[t+1][j])\
                            *ro2h[j][i];
                }
            }
        }
    }
    
    t=sen->tlen-1;
    for(i=0;i<hidlayerNUM;++i){
        sen->DLDh[t][i]=0.0;
        for(j=0;j<EMOTIONNUM;++j){
            sen->DLDh[t][i]+=sen->DLDo[t][j]*k2out[j][i]\
                    *dsigm(sen->hidLay[t][i]);
        }
    }
    for(t=sen->tlen-2;t>-1;--t){
        for(i=0;i<hidlayerNUM;++i){
            sen->DLDh[t][i]=0.0;
            for(j=0;j<EMOTIONNUM;++j){
                sen->DLDh[t][i]+=sen->DLDo[t][j]*k2out[j][i]\
                        *dsigm(sen->hidLay[t][i]);
            }
            for(j=0;j<hidlayerNUM;++j){
                sen->DLDh[t][i]+=sen->DLDh[t+1][j]*rh2h[j][i]\
                        *dsigm(sen->hidLay[t+1][i]);
            }
        }
    }
    
    t=sen->tlen-1;
    for(i=0;i<featureNUM;++i){
        sen->DLDx[t][i]=0.0;
        for(j=0;j<hidlayerNUM;++j){
            sen->DLDx[t][i]+=sen->DLDh[t][j]*in2hid[j][i];
        }
    }
    for(t=sen->tlen-2;t>-1;--t){
        for(i=0;i<featureNUM;++i){
            sen->DLDx[t][i]=0.0;
            for(j=0;j<hidlayerNUM;++j){
                sen->DLDx[t][i]+=sen->DLDh[t][j]*in2hid[j][i];
                for(k=0;k<hidlayerNUM;++k){
                    sen->DLDx[t][i]+=sen->DLDh[t+1][j]*rh2h[j][k]\
                        *dsigm(sen->hidLay[t][k])*in2hid[k][i];
                }
            }
            // for(j=0;j<hidlayerNUM;++j){
            //     for(k=0;k<hidlayerNUM;++k){
            //         sen->DLDx[t][i]+=sen->DLDh[t+1][j]*rh2h[j][k]\
            //             *dsigm(sen->hidLay[t][k])*in2hid[k][i];
            //     }
            // }
        }
    }
    
    double plnrt = lnrt/sen->tlen;
    upgradeDict(ccdict,sen->schar,sen->DLDx,featureNUM,plnrt);    
    
    double pder;
    
    for(i=0;i<EMOTIONNUM;++i){
        for(j=0;j<hidlayerNUM;++j){
            pder=0.0;
            for(t=0;t<sen->tlen;++t){
                pder+=sen->DLDo[t][i]*sen->kLayer[t][j];
            }
            pder/=sen->tlen;
            DLDk2out[i][j]=pder/sen->tlen;         
        }
    }
    
    for(i=0;i<hidlayerNUM;++i){
        for(j=0;j<featureNUM;++j){
            pder=0.0;
            for(t=0;t<sen->tlen;++t){
                pder+=sen->DLDh[t][i]*sen->inVec[t][j];
            }
            pder/=sen->tlen;
            DLDin2hid[i][j]+=pder;
        }
    }

    for(i=0;i<hidlayerNUM;++i){
        for(j=0;j<hidlayerNUM;++j){
            pder=0.0;
            for(t=1;t<sen->tlen;++t){
                pder+=sen->DLDh[t][i]*sen->kLayer[t-1][j];
            }
            pder/=sen->tlen;
            DLDrh2h[i][j]+=pder;
        }
    }

    for(i=0;i<hidlayerNUM;++i){
        for(j=0;j<EMOTIONNUM;++j){
            pder=0.0;
            for(t=1;t<sen->tlen;++t){
                pder+=sen->DLDh[t][i]*sen->outLay[t-1][j];
            }
            pder/=sen->tlen;
            DLDro2h[i][j]+=pder;
        }
    }
    
}

Sentence gonext(Sentence sen,long target){
    long start = sen->num;
    Sentence ret=sen;
    for(;start>target;--start)
        sen = sen->next;
    return sen;
}

double RNNet::_Forward_CROSSENTROPY_SingleTask(RNNet* rnn,Sentence* sen,int n){
    Sentence tsen = *sen;
    long num = tsen->num;
    std::mutex g_mutex;
    while( rnn->flag > -1){
        g_mutex.lock();
        num = rnn->flag;
        //printf("%ld\n",num);
        rnn->flag--;
        g_mutex.unlock();
        tsen = gonext(tsen,num);
        //std::cout<<tsen->schar<<std::endl;
        //rnn->_Forward_CROSSENTROPY(tsen);
    }
    return 0;
}

void RNNet::_Backward_CROSSENTROPY_SingleTask(RNNet* rnn,Sentence* sen,int n){
    Sentence tsen = *sen;
    std::mutex g_mutex;
    long num = tsen->num;
    while(rnn->flag > -1){
        g_mutex.lock();
        num = rnn->flag;
        //printf("%ld\n",num);
        rnn->flag--;
        g_mutex.unlock();
        tsen = gonext(tsen,num);
        //rnn->_Backward_CROSSENTROPY(tsen);
    }
}

void RNNet::softmax_add(Sentence sen,double* src,int t){
    double normal=0.0;
    int i;
    for(i=0;i<EMOTIONNUM;++i)
        normal+=exp(src[i]);
    
    for(i=0;i<EMOTIONNUM;++i){
        sen->energy[t][i]=exp(src[i])/normal;
     //   std::cout<<sen->energy[t][i]<<std::endl;
    }
}

int RNNet::FreeSen(){
    if(sent==NULL) return 0;
    Sentence tmp;
    int i,j;
    while(sent){
        tmp = sent;
        sent=sent->next;
        free(tmp->schar);
        for(i=0;i<tmp->tlen;++i){
            free(tmp->inVec[i]);
            free(tmp->hidLay[i]);
            free(tmp->outLay[i]);
            free(tmp->kLayer[i]);
            free(tmp->energy[i]);
            free(tmp->DLDh[i]);
            free(tmp->DLDo[i]);
            free(tmp->DLDx[i]);
        }
        free(tmp->inVec);
        free(tmp->hidLay);
        free(tmp->kLayer);
        free(tmp->outLay);
        free(tmp->energy);
        free(tmp->eng);
        free(tmp->stdAns);
        free(tmp->DLDh);
        free(tmp->DLDo);
        free(tmp->DLDx);
        free(tmp);
    }
    return 1;
}

void RNNet::FreeAll(){
    FreeSen();
    if(ccdict!=NULL) hashFree(ccdict);
    int i,j;
    for(i=0;i<EMOTIONNUM;++i){
        free(k2out[i]);
        free(DLDk2out[i]);
    }
    for(i=0;i<hidlayerNUM;++i){
        free(in2hid[i]);
        free(ro2h[i]);
        free(rh2h[i]);
        free(DLDro2h[i]);
        free(DLDrh2h[i]);
        free(DLDin2hid[i]);
    }
}

void RNNet::prtSent(const char* path){
    Sentence tmp=sent;

    FILE* fp = fopen(path,"w");
    int i,j;

    while(tmp){
        fprintf(fp,"{\n");
        fprintf(fp,"%s %d\ninVec:{\n",tmp->schar,tmp->tlen);
        for(i=0;i<tmp->tlen;++i){
            for(j=0;j<featureNUM;++j)
                fprintf(fp,"%f ",tmp->inVec[i][j]);
            fprintf(fp,"\n");
        }
        fprintf(fp,"}\nhidLay:{\n");
        for(i=0;i<tmp->tlen;++i){
            for(j=0;j<hidlayerNUM;++j)
                fprintf(fp,"%f ",tmp->hidLay[i][j]);
            fprintf(fp,"\n");
        }
        fprintf(fp,"}\nkLayer:{\n");
        for(i=0;i<tmp->tlen;++i){
            for(j=0;j<hidlayerNUM;++j)
                fprintf(fp,"%f ",tmp->kLayer[i][j]);
            fprintf(fp,"\n");
        }
        fprintf(fp,"}\noutLay:{\n");
        for(i=0;i<tmp->tlen;++i){
            for(j=0;j<EMOTIONNUM;++j)
                fprintf(fp,"%f ",tmp->outLay[i][j]);
            fprintf(fp,"\n");
        }
        fprintf(fp,"}\neng:{\n");
        for(j=0;j<EMOTIONNUM;++j)
            fprintf(fp,"%f ",tmp->eng[j]);
        fprintf(fp,"\n}\nstdAns:{\n");
        for(j=0;j<EMOTIONNUM;++j)
            fprintf(fp,"%f ",tmp->stdAns[j]);
        fprintf(fp,"\n}\nloss:%f\n\n",tmp->loss);
        tmp=tmp->next;
    }

}

void RNNet::init_thread(int n){
    std::cout<<"thread\n";
}