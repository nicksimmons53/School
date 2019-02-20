#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include "cs3723p1.h"

/*********************** userAllocate ************************************
Purpose:
    Allocates memory for a node from our heap, copies the user data into
    that node and will return a pointer to that user data in the node.
Parameters:
    StorageManager      *pMgr               pointer to the storage manager
                                            struct
    short               shUserDataSize      user data size being copied to
                                            the newly allocated node
    short               shNodeType          subscript in the
                                            StorageManager NodeType array
    char                sbUserData[ ]       user data being copied into
                                            the newly allocated node
    SMResult            *psmResult
Notes:
Returns:
    null                                    if the memory was not allocated
    void                *allocNode->sbData  pointer to the user data in
                                            the allocated node
*************************************************************************/
void * userAllocate(StorageManager *pMgr
    , short shUserDataSize, short shNodeType, char sbUserData[], SMResult *psmResult) {
    AllocNode *pAllocNew;
    FreeNode *temp;
    void *pUserData;

    short allocatedSize = shUserDataSize + pMgr->shPrefixSize;

    if (pMgr->pFreeHeadM[shNodeType]->cAF == 'F') {
        pAllocNew = (AllocNode *) pMgr->pFreeHeadM[shNodeType];

        pAllocNew->shRefCount = 1;
        pAllocNew->shNodeType = shNodeType;
        pAllocNew->shAllocSize = allocatedSize;
        pAllocNew->cAF = 'A';
        memcpy(pAllocNew->sbData, sbUserData, shUserDataSize);

        temp = pMgr->pFreeHeadM[shNodeType]->pNextFree;
        pMgr->pFreeHeadM[shNodeType] = temp;

        psmResult->rc = 0;
    } else {
        pAllocNew = utilAlloc(pMgr, allocatedSize);
        if (pAllocNew == NULL)
            return NULL;
    }

    pUserData = pAllocNew + pMgr->shPrefixSize;
    return pUserData;
}

void userRemoveRef(StorageManager *pMgr, void *pUserData, SMResult *psmResult) {

}

void userAssoc(StorageManager *pMgr, void *pUserDataFrom, char szAttrName[]
    , void *pUserDataTo, SMResult *psmResult) {

}

void userAddRef(StorageManager *pMgr, void *pUserDataTo, SMResult *psmResult) {

}

void memFree(StorageManager *pMgr, AllocNode *pAlloc, SMResult *psmResult) {

}
