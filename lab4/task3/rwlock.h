#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    pthread_mutex_t lock;
    pthread_cond_t readers_proceed;
    pthread_cond_t writer_proceed;
    int num_readers;
    int writer_active;
    int pending_writers;
} my_rwlock_t;

int my_rwlock_init(my_rwlock_t *rw) {
    pthread_mutex_init(&rw->lock, NULL);
    pthread_cond_init(&rw->readers_proceed, NULL);
    pthread_cond_init(&rw->writer_proceed, NULL);
    rw->num_readers = 0;
    rw->writer_active = 0;
    rw->pending_writers = 0;
    return 0;
}

int my_rwlock_rdlock(my_rwlock_t *rw) {
    pthread_mutex_lock(&rw->lock);
    // wait if a writer is active or if writers are waiting
    while (rw->writer_active || rw->pending_writers > 0) {
        pthread_cond_wait(&rw->readers_proceed, &rw->lock);
    }
    rw->num_readers++;
    pthread_mutex_unlock(&rw->lock);

    return 0;
}

int my_rwlock_wrlock(my_rwlock_t *rw) {
    pthread_mutex_lock(&rw->lock);
    rw->pending_writers++;
    // wait until there are no readers and no active writer
    while (rw->writer_active || rw->num_readers > 0) {
        pthread_cond_wait(&rw->writer_proceed, &rw->lock);
    }
    rw->pending_writers--;
    rw->writer_active = 1;
    pthread_mutex_unlock(&rw->lock);

    return 0;
}

int my_rwlock_unlock(my_rwlock_t *rw) {
    pthread_mutex_lock(&rw->lock);
    if (rw->writer_active) {
        rw->writer_active = 0;
    } else {
        rw->num_readers--;
    }

    // wake a waiting writer if no readers are left
    if (rw->pending_writers > 0 && rw->num_readers == 0) {
        pthread_cond_signal(&rw->writer_proceed);
    } else if (rw->pending_writers == 0) {
        // if no writers are waiting, wake all readers
        pthread_cond_broadcast(&rw->readers_proceed);
    }
    pthread_mutex_unlock(&rw->lock);

    return 0;
}

void compResults(char *msg, int rc) {
    if (rc != 0) {
        printf("%s failed with %d\n", msg, rc);
    }
}

int my_rwlock_destroy(my_rwlock_t *rw) {
    pthread_mutex_destroy(&rw->lock);
    pthread_cond_destroy(&rw->readers_proceed);
    pthread_cond_destroy(&rw->writer_proceed);
    return 0;
}