
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void **tmpBuf_Table;
int tmpBuf_current=0;
int tmpBuf_count=5;

void resetTmpBufCount(int count);
void initTmpBuf(int count);
void clrTmpBuf();
void* allocTmpMemory(int size);
void setTmpBuffer(void*buf);

void resetTmpBufCount(int count)
{
    clrTmpBuf();
    free(tmpBuf_Table);
    initTmpBuf(count);
}

void initTmpBuf(int count)
{
    tmpBuf_Table=malloc(sizeof(void*)*count);
    memset(tmpBuf_Table,0,sizeof(void*)*count);
    tmpBuf_count=count;
    tmpBuf_current=0;
}

void clrTmpBuf()
{
    int i;
    for(i=0;i<tmpBuf_count;i++)
        free(tmpBuf_Table[i]);
    free(tmpBuf_Table);
    tmpBuf_current=0;
}

void* allocTmpMemory(int size)
{
    //printf("index=%d count=%d\n",tmpBuf_current,tmpBuf_count);

    void *ret=malloc(size);
    free(tmpBuf_Table[tmpBuf_current]);
    tmpBuf_Table[tmpBuf_current]=ret;
    tmpBuf_current=(tmpBuf_current+1)%tmpBuf_count;
    return ret;
}

void setTmpBuffer(void*buf)
{
    printf("current=%d\n",tmpBuf_current);
    free(tmpBuf_Table[tmpBuf_current]);
    tmpBuf_Table[tmpBuf_current]=buf;
    tmpBuf_current=(tmpBuf_current+1)%tmpBuf_count;
}
