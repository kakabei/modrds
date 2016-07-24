
#include <stdio.h>
#include <string.h>
#include "adlist.h"


int main()
{
    list * ls = listCreate();
    listIter *iter;
    listNode *node;

    char val1[] = "value1";
    char val2[] = "value2";
    char val3[] = "value3";

    listAddNodeHead(ls, val1);
    listAddNodeHead(ls, val2);
    listAddNodeHead(ls, val3);
    iter = listGetIterator(ls, AL_START_HEAD);

    while((node = listNext(iter)) != NULL) {
        printf("value = %s\n", (char*)node->value);
    }
    listReleaseIterator(iter);
	return 0;
}
