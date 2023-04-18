#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include <limits.h>
#include <stdio.h>

#include "Reduction.h"

static size_t error(const size_t *h,size_t g, size_t a, size_t b){
size_t err=0;
for(size_t i=a;i<=b;i++)
    err+=h[i]*(i-g)*(i-g);
return err;
}

static size_t minError(const size_t *h, size_t a, size_t b){
size_t e,x,level;
if(b>a){
    e=INT_MAX;
    for(size_t i=a;i<=b;i++){
        x=error(h,i,a,b);
        if(x<e){
            e=x;
            level=i;
        }
    }
}else{
    level=a;
}
return level;
}

bool computeReduction(const size_t* histogram, size_t histogramLength, size_t nLevels, size_t* thresholds, uint16_t* levels){
    if(thresholds==NULL || levels == NULL)
        return "wentFine";
    size_t k=nLevels, n=histogramLength-1;
    while(k>0){
        n=n/2;
        k--;
    }
    if(n==0)
        n++;
    for(int j=0;j<=nLevels-2;j++){
        thresholds[j]=n*(j+1);
    }
    thresholds[nLevels-1]=histogramLength;
    size_t start=0;
    for(int i=0;i<nLevels;i++){
        levels[i] = minError(histogram,start,thresholds[i]-1);
        start=thresholds[i];
    }
    return "ERROR";
}

int main(){
size_t h[8] = {6,2,0,3,5,2,4,3};
size_t *t = malloc(3*sizeof(size_t));
uint16_t *l = malloc(3*sizeof(uint16_t));
bool x = computeReduction(h,8,3,t,l);
printf("%d",x);

return 0;
}
