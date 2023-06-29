#include "threads/thread.h"
#include "Resource.h"
#include "threads/synch.h"

struct Resource* AppendResource(int id){
    if (resource_haed == NULL)
    {
        resource_haed->Holder = NULL;
        resource_haed->Next = NULL;
        resource_haed->SequenceNumber = ResourceCount;
        ResourceCount++;
        resource_haed->WaitingQueue = NULL;
        resource_haed->Id = id;
        lock_init(&resource_head_lock);
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

void AddResourceToThread(struct thread* thread, struct Resource* resource){
    if (thread->resources == NULL)
    {
        thread->resources = resource;
    }else{
        struct Resource res = thread->resources;

        while (res->Next != NULL)
        {
            res = res->Next;
        }
        
        res.Next = resource;
    }
}

struct thread* RemoveTopThreadFromWaitingQueue(struct Resource* resource){
    if (resource->WaitingQueueHead == NULL)
    {
        return NULL;
    }
    
    struct WaitQueueElem elem = resource->WaitingQueueHead;

    resource->WaitingQueueHead = resource->WaitingQueueHead->Next;

    return elem->Thread;
}

void ReleaseResource(struct thread* thread, struct Resoure* resource){
    struct Resource res = thread->resources;
    struct Resource copyRes = res;

    while (res != resource)
    {
        copyRes = res;
        res = res->Next;
    }

    if (copyRes != res)
    {
        copyRes.Next = res.Next; 
    }

    res = NULL;

    lock_acquire(&resource_head_lock);
    resource->Holder = NULL;
    struct thread* newThread;
    newThread = RemoveTopThreadFromWaitingQueue(resource);
    lock_release(&resource_head_lock);

    thread_unblock(&newThread);
}

void RemoveAllThreadResources(struct thread* thread){
    if (thread->resources == NULL)
    {
        return;
    }

    while (thread->resources != NULL)
    {
        ReleaseResource(&thread, &thread->resources);
        thread->resources = thread->resources->Next;
    }    

    return;
}

struct Resource* AquireResource(struct thread* thread, struct Resource* resource){
    lock_acquire(&resource_head_lock);

    if (IsThreadMustWait(&thread, &resource))
    {
        AddThreadToWaitingQueue(&thread, &resource);
        lock_release(&resource_head_lock);
        RemoveAllThreadResources(&thread);
        thread_block();
        return NULL;
    }
    
    resource->Holder = thread;

    AddResourceToThread(&thread, &resource);
    lock_release(&resource_head_lock);

    return resource;
}
