#include "threads/thread.h"
#include "WaitQueue.h"

struct Resource {
    int SequenceNumber;
    struct thread* Holder;
    struct Resource* Next;
    struct WaitQueueElem* WaitingQueue;
};

Resource* Init();

Resource* AppendResource(struct Resource* resource);

Resource* AquireResource(struct thread* thread, struct Resource* resource);

bool ReleaseResource(struct thread* thread, struct Resoure* resource);

bool IsThreadMustWait(struct thread* thread, struct Resource* resource);

bool IsResourceHeld(struct Resource* resource);

bool AddThreadToWaitingQueue(struct thread* thread, struct Resource* resource);