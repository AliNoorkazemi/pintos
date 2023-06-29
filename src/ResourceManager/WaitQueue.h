#include "threads/thread.h"

struct WaitQueueElem {
    struct thread* Thread;
    struct WaitQueueElem* Next;
};