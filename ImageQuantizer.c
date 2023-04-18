#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#include "PortableGrayMap.h"
#include "ImageQuantizer.h"
#include "Reduction.h"

static size_t* histogram(const PortableGrayMap* image){
    if(!image)
        return NULL;
    size_t n = image->maxValue + 1;
    size_t *h = malloc(n*sizeof(size_t));
    if(!h)
        return NULL;

    for(size_t i = 0;i<n;i++)
        h[i]=0;

    for(size_t i=0; i<image->height;i++){
        for(size_t j = 0; j<image->width; j++){
            h[image->array[i][j]]++;
        }
    }

    return h;
}


PortableGrayMap* quantizeGrayImage(const PortableGrayMap* image, size_t numLevels){

    if(!image)
        return NULL;

    PortableGrayMap* pgm = malloc(sizeof(PortableGrayMap));
    if(!pgm){
        free(pgm);
        return NULL;
    }
    PortableGrayMapType *t =  malloc(sizeof(PortableGrayMapType));
    if(!t){
        free(t);
        return NULL;
    }
    *t = image->type;
    pgm->type = *t;
    pgm-> width = image ->width;
    pgm-> height = image ->height;
    pgm-> maxValue = image ->maxValue;
    pgm->array = malloc(image->height*sizeof(uint16_t*));
    for(int i=0;image->height;i++)
        pgm->array[i] = malloc(image->width*sizeof(uint16_t));
    if(!pgm->array){
        free(pgm->array);
        return NULL;
    }
    pgm->array = image->array;

    size_t *h = histogram(image);
    if(!h)
        return NULL;
    size_t n = image->maxValue + 1;
    size_t *thresholds = malloc(numLevels*sizeof(size_t));
    uint16_t *levels = malloc(numLevels * sizeof(uint16_t));

    if(!thresholds || !levels)
        return NULL;
    bool s = computeReduction(h,n,numLevels,thresholds,levels);
    if(s==1){
        for(size_t i=0;i<image->height;i++){
            for(size_t j=0;j<image->width;j++){
                size_t k = numLevels-1, tmp;
                while(k>0 && image->array[i][j] < thresholds[k])
                    tmp=levels[k];
                pgm->array[i][j]=tmp;
            }
        }
    }
    return pgm;
}
