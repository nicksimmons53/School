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
    SMResult            *psmResult          pointer to the SMResult struct
Notes:
    None
Returns:
    null                                    if the memory was not allocated
    void                *allocNode->sbData  pointer to the user data in
                                            the allocated node
*************************************************************************/
void * userAllocate(StorageManager *pMgr
    , short shUserDataSize, short shNodeType, char sbUserData[], SMResult *psmResult) 
{
    AllocNode *pAllocNew = NULL;
    FreeNode *temp;
    void *pUserData;
    
    short *address;
    short allocatedSize = shUserDataSize + pMgr->shPrefixSize;
    
    // If there is 'free' memory available on the free list use that 
    if (pMgr->pFreeHeadM[shNodeType] != NULL) {
        // Assign the free mem to an alloc node and remove it from free list
        temp = pMgr->pFreeHeadM[shNodeType]->pNextFree;
        pAllocNew = (AllocNode *) pMgr->pFreeHeadM[shNodeType];
        pMgr->pFreeHeadM[shNodeType] = temp;
        
        // Set the alloc node variables 
        pAllocNew->shRefCount = 1;
        pAllocNew->shNodeType = shNodeType;
        pAllocNew->shAllocSize = allocatedSize;
        pAllocNew->cAF = 'A';
        memcpy(pAllocNew->sbData, sbUserData, shUserDataSize);
    }
    
    // If there is no space on the free list 'allocate' memory
    if (pAllocNew == NULL) {
        // Call utilAlloc to allocate memory
        pAllocNew = utilAlloc(pMgr, allocatedSize);

        if (pAllocNew == NULL)
            return NULL;
        
        // Set the alloc node variables
        pAllocNew->shRefCount = 1;
        pAllocNew->cAF = 'A';
        pAllocNew->shAllocSize = allocatedSize;
        pAllocNew->shNodeType = shNodeType;
        memcpy(pAllocNew->sbData, sbUserData, shUserDataSize);
    }

    // Set the address to the user data based on the allocated address
    address = (short *)pAllocNew + pMgr->shPrefixSize/2;
    pUserData = (void *)address;
    
    psmResult->rc = 0;
    return pUserData;
}

/*********************** userRemoveRef ***********************************
Purpose:
    Decrements a reference count for an AllocNode whose address is found 
    from the pointer to the user data for that node. 
Parameters:
    StorageManager      *pMgr           pointer to the storage manager 
                                        struct
    void                *pUserData      pointer to the user data within
                                        an allocated node 
    SMResult            *psmResult      pointer to the SMResult struct
Notes:
    If a reference for a node is reduced, and it reaches 0, all directly
    referenced nodes reference counts are decremented by 1. A recursive
    call for userRemoveRef.
Returns:
    None
*************************************************************************/
void userRemoveRef(StorageManager *pMgr, void *pUserData, SMResult *psmResult) {
    AllocNode *pAllocNode;
    AllocNode *temp;
    MetaAttr *pAttr;
    void **ppNode;
    short *pshDataDeref;
    short *pshPrefixAddress;
    short nodeType;
    int iDeref = 1;
    int i;

    // Find and set the address of the beginning of the allocated node
    pshPrefixAddress = (short *)pUserData - pMgr->shPrefixSize/2;
    pAllocNode = (AllocNode *)pshPrefixAddress;

    // Decrement the reference count for the node being specified
    pAllocNode->shRefCount -= iDeref;

    // If the nodes ref count is 0, 'free' it and update any linked nodes
    if (pAllocNode->shRefCount == 0) {
        // Set the node type, recursively call to update, 'free' the node
        nodeType = pAllocNode->shNodeType;

        // Loop through the meta data attr to find the location in the node
        for (i = pMgr->nodeTypeM[nodeType].shBeginMetaAttr; pMgr->metaAttrM[i].shNodeType == nodeType; i++) {
            pAttr = &(pMgr->metaAttrM[i]);
            switch (pAttr->cDataType) {
                case 'P':
                    ppNode = (void **) &(pAllocNode->sbData[pAttr->shOffset]);
                    if (*ppNode != NULL) {
                        pshDataDeref = (short *)*ppNode;
                        userRemoveRef(pMgr, pshDataDeref, psmResult);
                    }
            }
        }
        memFree(pMgr, pAllocNode, psmResult);
    }
}

/*********************** userAssoc ***************************************
Purpose:
    Sets a pointer in a user node to point to another node or NULL and
    updates reference counts accordingly.
Parameters:
    StorageManager      *pMgr           pointer to the storage manager
                                        struct
    void                *pUserDataFrom  a user data pointer to the 'from'
                                        node which contains the pointer 
                                        attr. This is the 'from' node.
    char                szAttrName[ ]   the name of the attr that is used
                                        to find the pointer in the 'from'
                                        node
    void                *pUserDataTo    a user data pointer to the 'to' 
                                        node
    SMResult            *psmResult      pointer to the SMResult struct
Notes:
    
Returns:
    None
*************************************************************************/
void userAssoc(StorageManager *pMgr, void *pUserDataFrom, char szAttrName[]
    , void *pUserDataTo, SMResult *psmResult) 
{
    int i;
    int iFlag = 0;
    short *pshDataFromAddress;
    short *pshDataDeref;
    short shNodeTypeFrom;
    AllocNode *pAllocNodeFrom;
    AllocNode *pTemp;
    void **ppNode;
    MetaAttr *pAttr;
    
    // Find the address to the from node 
    pshDataFromAddress = (short *)pUserDataFrom - pMgr->shPrefixSize/2;
    pAllocNodeFrom = (AllocNode *)pshDataFromAddress;
    shNodeTypeFrom = pAllocNodeFrom->shNodeType;
    
    // Loop through the metadata to find the location of the pointer in the alloc node
    for (i = pMgr->nodeTypeM[shNodeTypeFrom].shBeginMetaAttr; pMgr->metaAttrM[i].shNodeType == shNodeTypeFrom; i++) {
        // Assign the address of the metadata according to the offset
        pAttr = &(pMgr->metaAttrM[i]);

        // Check for a pointer 
        switch (pAttr->cDataType) {
            case 'P':
                // Assign the address of the pointer in the node
                ppNode = (void **) &(pAllocNodeFrom->sbData[pAttr->shOffset]);
                iFlag = 1;

                // If the attribute matches, associate the node and memcpy
                if (strcmp(szAttrName, pAttr->szAttrName) == 0) {
                    // If the node is already pointing, deref, otherwise increment
                    if (*ppNode != NULL) {
                        pshDataDeref = (short *)*ppNode;
                        pTemp = (AllocNode *)pshDataDeref;
                        userRemoveRef(pMgr, pshDataDeref, psmResult);
                        memcpy(ppNode, &pUserDataTo, sizeof(ppNode));
                    } else {
                        userAddRef(pMgr, pUserDataTo, psmResult);
                        memcpy(ppNode, &pUserDataTo, sizeof(ppNode));
                    }
                }
        }
    }

    if (iFlag == 0)
        psmResult->rc = RC_ASSOC_ATTR_NOT_PTR;
    psmResult->rc = 0;
}

/*********************** userAddRef **************************************
Purpose:
    Adds a reference to the specified node being passed.    
Parameters:
    StorageManager      *pMgr           pointer to the storage manager 
                                        struct
    void                *pUserDataTo    pointer to the user data pointer 
                                        to the 'to' node
    SMResult            *psmResult      pointer to the SMResult struct
Notes:
    None
Returns:
    None
*************************************************************************/
void userAddRef(StorageManager *pMgr, void *pUserDataTo, SMResult *psmResult) {
    AllocNode *pAllocNode;

    short *pshPrefixAddress;
    int iRef = 1;
    
    // Set the prefix address based on the user data address
    pshPrefixAddress = (short *)pUserDataTo - pMgr->shPrefixSize/2;
    pAllocNode = (AllocNode *)pshPrefixAddress;
    
    // Increment the alloc node reference count
    pAllocNode->shRefCount += iRef;
    psmResult->rc = 0;
}

/*********************** memFree *****************************************
Purpose:    
    Adding the passed AllocNode to the free list in StorageManager for 
    that specific node type.
Parameters:
    StorageManager      *pMgr           pointer to the storage manager 
                                        struct
    AllocNode           *pAlloc         pointer to the alloc node being
                                        added to the free list
    SMResult            *psmResult      pointer to the SMResult struct
Notes:
    Will return an error through psmResult if an error is encountered.
    The node being 'freed' is added to the front of the free list for that
    node type. 
Returns: 
    None
*************************************************************************/
void memFree(StorageManager *pMgr, AllocNode *pAlloc, SMResult *psmResult) {
    FreeNode *pFreeNode;
    
    // If the node is not allocated set error message and break
    if (pAlloc->cAF != 'A') {
        psmResult->rc = RC_CANT_FREE;
        return;
    }
    
    // Cast the alloc node as a free node and set the struct variables
    pFreeNode = (FreeNode *) pAlloc;
    pFreeNode->cAF = 'F';
    pFreeNode->shAllocSize = pAlloc->shAllocSize;
    
    // Insert the new free node onto the appropriate node type list
    pFreeNode->pNextFree = pMgr->pFreeHeadM[pAlloc->shNodeType];
    pMgr->pFreeHeadM[pAlloc->shNodeType] = pFreeNode;
    
    // Set psmResult to success
    psmResult->rc = 0;
}
