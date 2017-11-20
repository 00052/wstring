#ifndef TMPBUFTABLE_H
#define TMPBUFTABLE_H
extern void resetTmpBufCount(int count);
extern void initTmpBuf(int count);
extern void clrTmpBuf();
extern void* allocTmpMemory(int size);
extern void setTmpBuffer(void*buf);
#endif // TMPBUFTABLE_H
