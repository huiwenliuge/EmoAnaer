#include "DictCreate.hpp"

/*int main(void){
    FILE* trnfile=fopen("TestMat","r");
    FILE* ofp=fopen("TestDic","w");
    fcrtDic(trnfile,ofp);

    FILE* dictfile=fopen("TestDic","r");
    ChUTF8* tab;
    tab = fldDic(dictfile);

    FILE* onlyfortest=fopen("Only4Test","w");
    prtHash2File(tab,onlyfortest);

}*/

void fapdDic(FILE* infp,FILE* srcfp,FILE* outfp,int dim){
	char* tmp  =(char*)calloc(sizeof(char),421);  
	char* sta=tmp;
	int lenTmp,i;

    ChUTF8* hash=fldDic(srcfp,dim);
	ChUTF8 NewS;
    while(fscanf(infp,"%s\n",sta)!=-1){
		tmp=sta;
        while(tmp[0]!='\0'){
            lenTmp=retLenUTF8(tmp);
           if(!hIscontain(hash,tmp,lenTmp)){
                NewS = (ChUTF8)malloc(sizeof(struct chUTF8));
                NewS->schar=(char*)malloc(sizeof(char)*(lenTmp+1));
                memcpy(NewS->schar,tmp,lenTmp);
                NewS->schar[lenTmp]='\0';
//std::cout<<tmp<<":"<<lenTmp<<":"<<NewS->schar<<std::endl;
                NewS->vecftr=(double*)malloc(sizeof(double)*dim);
                for(i=0;i<dim;++i){
                    NewS->vecftr[i]=(rand()%101-50)/50.0;
                }
            
                hashput(hash,NewS);
           }
					
            tmp+=lenTmp;
        }
		memset(sta,'\0',421*sizeof(char));
    }
	
    prtHash2File(hash,outfp,dim);
    fflush(outfp);
    hashFree(hash);

}

void fcrtDic(FILE* infp,FILE* outfp,int dim){
    

    char* tmp=(char*)calloc(sizeof(char),421);
    int lenTmp,i;
	char* sta=tmp;
    ChUTF8* hash = tabCreate();

    while(fscanf(infp,"%s\n",sta)!=-1){
		tmp=sta;
        while(tmp[0]!='\0'){
            lenTmp=retLenUTF8(tmp);
            if(!hIscontain(hash,tmp,lenTmp)){
                ChUTF8 NewS = (ChUTF8)malloc(sizeof(struct chUTF8));
            
                NewS->schar=(char*)malloc(sizeof(char)*(lenTmp+1));
                memcpy(NewS->schar,tmp,lenTmp);
                NewS->schar[lenTmp]='\0';

                NewS->vecftr=(double*)malloc(sizeof(double)*dim);
                for(i=0;i<dim;++i){
                    NewS->vecftr[i]=(rand()%101-50)/50.0;
                }
            
                hashput(hash,NewS);
            }
            tmp+=lenTmp;
        }
        memset(sta,'\0',421*sizeof(char));
    }
    
   
    
    prtHash2File(hash,outfp,dim);
    fflush(outfp);
    
    hashFree(hash);
}

void hashput(ChUTF8* tab,ChUTF8 unit){

    int offset;
    offset=hFuncUTF8(unit->schar);
    unit->next=tab[offset];
    tab[offset]=unit;

}

void prtHash2File(ChUTF8* tab,FILE* fp,int dim){

    //default size 32*64*64
    int i,j;
    ChUTF8 tmp;
    fprintf(fp,"Standard_Dict_Form");
    for(i=0;i<32*64*64;++i){
        tmp=tab[i];
        while(tmp){
            fprintf(fp,"\n");
            fprintf(fp,"%s ",tmp->schar);
            for(j=0;j<dim;++j)
                fprintf(fp,"#%lf#",tmp->vecftr[j]);
            tmp=tmp->next;
        }
    }

}

void hashFree(ChUTF8* tab){

    int i;
    ChUTF8 tmp;

    for(i=0;i<32*64*64;++i){
        while(tab[i]){
            tmp=tab[i];
            tab[i]=tab[i]->next;
            free(tmp->schar);
            free(tmp->vecftr);
            free(tmp);
        }
    }
    free(tab);
}

ChUTF8 hashget(ChUTF8* tab,char* src){

    int offset=hFuncUTF8(src);
    int len=retLenUTF8(src);
    ChUTF8 tmp = tab[offset];
    while(tmp){
        if(strncmp(tmp->schar,src,len)==0)
            return tmp;
        tmp=tmp->next;
    }
    return NULL;
}

bool hIscontain(ChUTF8* tab,char* src,int len){

    int offset=hFuncUTF8(src);
    ChUTF8 tmp = tab[offset];
    while(tmp){
        if(strncmp(tmp->schar,src,len)==0)
            return true;
        tmp=tmp->next;
    }

    return false;

}

ChUTF8* fldDic(FILE* fp,int dim){
    ChUTF8* ret = tabCreate();
    char* tmp=(char*)calloc(sizeof(char),7);
    fscanf(fp,"%s\n",tmp);
    std::cout<<tmp<<std::endl;
    int lenTmp,i;
    double dtmp;
    while(fscanf(fp,"%s ",tmp)!=-1){
        lenTmp=retLenUTF8(tmp);
        //std::cout<<tmp<<std::endl;
        ChUTF8 NewS = (ChUTF8)malloc(sizeof(struct chUTF8));
        NewS->schar=(char*)malloc(sizeof(char)*(lenTmp+1));
        memcpy(NewS->schar,tmp,lenTmp);
        NewS->schar[lenTmp]='\0';
        NewS->vecftr=(double*)malloc(sizeof(double)*dim);
        for(i=0;i<dim;++i){
            fscanf(fp,"#%lf#",&dtmp);
            NewS->vecftr[i]=dtmp;
        }
        fgetc(fp);
        hashput(ret,NewS);
        memset(tmp,'\0',7*sizeof(char));
    }
    return ret;
}

int strLenUTF8(char* str){
    int ret=0;
    char* point;
    point = str;
    while(*str!='\0'){
        str+=retLenUTF8(str);
        ++ret;
    }
    return ret;
}

double** crtInvec(char* str,int invecdim,ChUTF8* tab){

    int senlen=strLenUTF8(str);
    double** ret = (double**)malloc(sizeof(double*)*senlen);
    int i,j;
    char* point=str;
    char* now;
    ChUTF8 tmp;
    for(i=0;i<senlen;++i){
        ret[i]=(double*)malloc(sizeof(double)*invecdim);
        tmp=hashget(tab,point);
        point+=retLenUTF8(point);
    //    std::cout<< tmp->schar << retLenUTF8(point) <<std::endl;
        if(tmp!=NULL){
            for(j=0;j<invecdim;++j){
                ret[i][j]=tmp->vecftr[j];
            }
        }else{
            now=point;            
            do{
                tmp=hashget(tab,now);
                now+=retLenUTF8(now);
            }while(tmp==NULL);
            printf("pre not found \n %s\n ",point);
            for(j=0;j<invecdim;++j){
                ret[i][j]=tmp->vecftr[j]+ret[i-1][j];
                ret[i][j]/=2.0;
            }
        }
    }

    return ret;

}

void upgradeDict(ChUTF8* tab,char* str,double** dInvec,int indim,double lnrt){

    int offset,i,j;
    ChUTF8 dicbody;
    char* now=str;
    j=0;
  //  dicbody=hashget(tab,str);
  //  std::cout<< "dic : "<<dicbody->vecftr[0] << " ";
    while(*now!='\0'){
        dicbody=hashget(tab,now);
    //    std::cout<< dicbody->schar <<std::endl;
        offset=retLenUTF8(now);
        now+=offset;
        for(i=0;i<indim;++i){
    //        std::cout<< dicbody->schar<< " ";
    //        std::cout<< dicbody->vecftr[0] << " ";
            dicbody->vecftr[i]-=dInvec[j][i]*lnrt;
        }
        ++j;
    }
 //   dicbody=hashget(tab,str);
 //   std::cout<< dicbody->vecftr[0] << std::endl;
    
}

void upDictbyTab(ChUTF8* tab,ChUTF8* ttab,int indim,double lnrt){

    int i,j;
    ChUTF8 tmp,adj;

    for(i=0;i<32*64*64;++i){
        tmp=ttab[i];
        while(tmp){
            adj=hashget(tab,tmp->schar);
            for(j=0;j<indim;++j){
                adj->vecftr[j]-=tmp->vecftr[j];
            }                      
            tmp=tmp->next;
        }
    }
}
