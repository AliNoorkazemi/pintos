#include "threads/thread.h"
#include "WaitQueue.h"
#include "threads/synch.h"

static unsigned int ResourceCount = 0;

struct Resource {
    int Id;
    int SequenceNumber;
    struct thread* Holder;
    struct Resource* Next;
    struct WaitQueueElem* WaitingQueueHead;
};

static struct Resource* resource_haed;

static struct Lock resource_head_lock;

struct Resource* AppendResource(struct Resource* resource);

struct Resource* AquireResource(struct thread* thread, struct Resource* resource);

bool ReleaseResource(struct thread* thread, struct Resoure* resource);

bool IsThreadMustWait(struct thread* thread, struct Resource* resource);

bool IsResourceHeld(struct Resource* resource);

void AddThreadToWaitingQueue(struct thread* thread, struct Resource* resource);

void AddResourceToThread(struct thread* thread, struct Resource* resource);