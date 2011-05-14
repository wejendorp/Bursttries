#include "../include/atomic_ops.h"
#include "ts_bucket_iterator.c++"
#include "ts_node.c++"
#include <algorithm>
#include <vector>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
//#include <utility> // Make_pair

#ifndef __TS_ARRAY_BUCKET
#define __TS_ARRAY_BUCKET

#define m_pair std::make_pair<K, V>

#define STABLE 0
#define BURSTING 1
#define atomic (volatile AO_t *)


template<
    typename K,
    typename V
>
class ts_array_bucket {
    private:
        typedef std::vector<std::pair<K, V> > pairvector;
        typedef std::allocator<pairvector> vector_alloc_t;
        vector_alloc_t vector_allocator;

        typedef typename pairvector::iterator iter;
        //typedef ts_array_bucket_iterator iterator;
        typedef ts_node<K,V,ts_array_bucket<K,V> > node;

        volatile ts_array_bucket * state;
        int capacity;
        //volatile int referencecount;
        pthread_rwlock_t lock;

        pairvector *contents;
        //iterator ts_array_bucket_iterator<ts_array_bucket<V,K> >;

        //friend class ts_array_bucket_iterator<ts_array_bucket<V,K> >;

    public:
        typedef V value_type;
        typedef K key_type;

        explicit ts_array_bucket(int cap) {
            contents = vector_allocator.allocate(1);
            vector_allocator.construct(contents, pairvector(0));
            //burstiterator = NULL;
            //AO_store(atomic &referencecount, 0);
            AO_store(atomic &capacity, cap);
            AO_store(atomic &state, STABLE);

            pthread_rwlock_init(&lock, NULL);
        }
        ~ts_array_bucket() {
            vector_allocator.destroy(contents);
            vector_allocator.deallocate(contents, 1);
            pthread_rwlock_destroy(&lock);
        }

        // Insert requires locking, for the same reasons as remove
        void insert(K key, V value) {
            iter it;
            // If bursting, insert at end to avoid relocations
            // Requires exclusive lock to guarantee insert position
            if(pthread_rwlock_wrlock(&lock)==0) {
                if (state != STABLE) {
                    it = iter_at(key, value);
                } else {
                    it = contents->end();
                }
                contents->insert(it, m_pair(key, value));
                pthread_rwlock_unlock(&lock);
            }
        }

        // Remove requires locking, because removing an element inside the
        // vector requires moving all subsequent elements back, leaving the
        // vector in an unstable state.
        bool remove(K key) {
            if(pthread_rwlock_wrlock(&lock)==0) {
                iter it = iter_at(key, NULL);
                if((*it).first != key)
                    return false;
                contents->erase(it);
                pthread_rwlock_unlock(&lock);
                return true;
            }
            return false;
        }
        V find(K key) {
            V val = NULL;
            if(pthread_rwlock_rdlock(&lock)==0) {
                iter it = iter_at(key, NULL);
                if((*it).first == key)
                    val = (*it).second;
                pthread_rwlock_unlock(&lock);
            }
            return val;
        }
        node *burst() {
            // Allow only one invocation of burst, leaving the node in a
            // read-only state
            if(AO_compare_and_swap(atomic &state, STABLE, BURSTING)) {
                node *newnode = new node();
                AO_store_release_write(atomic &state, newnode);
                // 

                return newnode;
            }
            return NULL;
        }

//        ts_array_bucket_iterator begin() {
//            return iterator(this, 0);
//        }
//        ts_array_bucket_iterator end() {
//            return iterator(this, 0);
//        }

    private:
        iter iter_at(K key, V value) {
            return std::lower_bound(contents->begin(), contents->end(),
                        m_pair(key, value));
        }
};
#endif