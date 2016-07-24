#ifndef ADLIST_H_
#define ADLIST_H_

#include <sys/types.h>
#include <stdarg.h>

/* Node, List, and Iterator are the only data structures used currently. */

typedef struct listNode {
    struct listNode *prev;
    struct listNode *next;
    void *value;
} listNode;

typedef struct listIter {
    listNode *next;
    int direction; /* 迭代方向 */
} listIter;

/*
    列表头结构
 */
typedef struct list {
    listNode *head;           /* 头指针 */
    listNode *tail;           /* 尾指针 */
    void *(*dup)(void *ptr);  /* 拷贝函数 */
    void (*free)(void *ptr);  /* 释放函数 */
    int (*match)(void *ptr, void *key); /* 比较函数 */
    unsigned long len;
} list;

/* Functions implemented as macros */
#define listLength(l) ((l)->len)
#define listFirst(l) ((l)->head)
#define listLast(l) ((l)->tail)
#define listPrevNode(n) ((n)->prev)
#define listNextNode(n) ((n)->next)
#define listNodeValue(n) ((n)->value)

#define listSetDupMethod(l,m) ((l)->dup = (m))
#define listSetFreeMethod(l,m) ((l)->free = (m))
#define listSetMatchMethod(l,m) ((l)->match = (m))

#define listGetDupMethod(l) ((l)->dup)
#define listGetFree(l) ((l)->free)
#define listGetMatchMethod(l) ((l)->match)

/* Prototypes */
list *listCreate(void);    /* 创建一个列表头结构 */
void listRelease(list *list); /* 释放整个列表 */
list *listAddNodeHead(list *list, void *value); /* 在头处添加一个节点 */
list *listAddNodeTail(list *list, void *value); /* 在尾处添加一个结点 */
/* 将一个包含给定值的节点添加到某个节点的之前或之后 
   after=1时是放在 old_node之后 */
list *listInsertNode(list *list, listNode *old_node, void *value, int after); 

void listDelNode(list *list, listNode *node);   /* 删除给定节点 要释放这个节点 */
listIter *listGetIterator(list *list, int direction);
listNode *listNext(listIter *iter);  /* 获取下一个节点的迭代器 */
void listReleaseIterator(listIter *iter);
list *listDup(list *orig);        /* 拷贝一个链表 */
listNode *listSearchKey(list *list, void *key); /* 在列表中搜索一个节点 key  */
listNode *listIndex(list *list, long index); /* 按index 往前往后搜索 */
void listRewind(list *list, listIter *li);
void listRewindTail(list *list, listIter *li);  
void listRotate(list *list);  /* 把最后一个节点放到第一个节点处 */

/* Directions for iterators */
#define AL_START_HEAD 0
#define AL_START_TAIL 1

#endif
