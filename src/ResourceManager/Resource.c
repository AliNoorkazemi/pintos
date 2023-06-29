#include "threads/thread.h"
#include "Resource.h"

struct Resource* AppendResource(int id){
    if (resource_haed == NULL)
    {
        resource_haed->Holder = NULL;
        resource_haed->Next = NULL;
        resource_haed->SequenceNumber = ResourceCount;
        ResourceCount++;
        resource_haed->WaitingQueue = NULL;
        resource_haed->Id = id;
    }else {
        struct Resource res = resource_haed;

        while (res->Next != NULL)
        {
            res = res->Next;
        }
        
        struct Resource resource;
        resource->Holder = NULL:
        resource->Id = id:
        resource->SequenceNumber = ResourceCount;
        ResourceCount++;
        res->Next = resource;
    }
}

bool IsResourceHeld(struct Resource* resource){
    return resource->Holder != NULL;
}

bool IsThreadMustWait(struct thread* thread, struct Resource* resource){
    if (IsResourceHeld(&resource))
    {
        return true;
    }

    if (thread->resources == NULL)
    {
        return false;
    }

    struct Resource threadRes = thread->resources;
    unsigned int score = 0;

    while (threadRes != NULL)
    {
        if (threadRes->SequenceNumber > score)
        {
            score = threadRes->SequenceNumber
        }

        threadRes = threadRes->Next;
    }
    
    if (score > resource->SequenceNumber)
    {
        return true;
    }else {
        return false;
    }   
}

void AddThreadToWaitingQueue(struct thread* thread, struct Resource* resource){
    if (resource->WaitingQueueHead == NULL)
    {
        struct WaitingQueueElem waitQueueElem;
        waitQueueElem->Thread = thread;
        waitQueueElem->Next = NULL;

        resource->WaitingQueueHead = waitQueueElem;
    }else{
        struct WaitingQueueElem elem = resource->WaitingQueueHead;

        while (elem->Next != NULL)
        {
            elem = elem->Next;
        }
        
        struct WaitingQueueElem newElem;
        newElem->Thread = thread;
        newElem->Next = NULL;
        elem->Next = newElem;
    }
}