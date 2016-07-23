
#include <stdio.h>
#include <string.h>
#include "rdssds.h"


int main()
{
   sds x = sdsnew("foo");
   sdsprint(x);
   sdsfree(x);
   char ch[] = "test, hello sds.";
   int len = strlen(ch);
   sds x1 = sdsnewlen(ch, len);
   sdsprint(x1);
   int sdsLen = sdslen(x1);
   printf("[sds string print] sdsLen=%d\n", sdsLen);
   
   sds x2 = sdsdup(x1);  
   sdsfree(x1);
   sdsprint(x2);
   int sdsfreeLen = sdsavail(x2);
   printf("[sds string print] sdsfreeLen=%d\n", sdsfreeLen);
   // sdsgrowzero(x2, 100); 
   //len = sdslen(x2);
   //sdsfreeLen = sdsavail(x2);
   //printf("[sds string print] sdslen=%d,dsfreeLen=%d\n", len, sdsfreeLen);
   char  ch2[] = "I'm carlos.";
   len = strlen(ch2);
   x2 = sdscatlen(x2, ch2, len);
   sdsprint(x2);
   sdsfree(x2);
   
   return 0;
}
