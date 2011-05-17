#include <algorithm>
#include <vector>
#include <stdlib.h>
#include <string.h>
#include "ts_locked_node.c++"
#include <pthread.h>
#ifndef __TS_BTREE_BUCKET
#define __TS_BTREE_BUCKET

template<
    typename K,
    typename V
>
class btree_node {
    public:
        K key;
        V value;
        btree_node<K,V> *left;
        btree_node<K,V> *right;
        explicit btree_node(K k, V val) {
            left = NULL;
            right = NULL;
            value = val;
            key = k;
        }
        ~btree_node() {
            delete(left);
            delete(right);
        }
        void insert(K k, V value) {
            if(k <= key) {
                if(left == NULL)
                    left = new btree_node(k, value);
                else
                    left->insert(k, value);
            }
            else if(right == NULL)
                right = new btree_node(k, value);
            else
                right->insert(k, value);
        }
        V find(K k) {
            if(k == key) return value;
            if(k < key)
                if(left != NULL)
                    return left->find(k);
            if(right != NULL)
                return right->find(k);
            return NULL;
        }
};

template<
    typename K,
    typename V
>
class ts_btree_bucket {
    private:
        typedef ts_locked_node<K,V,ts_btree_bucket<K,V> > node;
        volatile int capacity, size;
        btree_node<K,V> * root;
        pthread_rwlock_t rwlock;

    public:
        typedef V value_type;
        typedef K key_type;

        explicit ts_btree_bucket(int cap) {
            capacity = cap;
            root = NULL;
            size = 0;
            pthread_rwlock_init(&rwlock, NULL);
        }
        ~ts_btree_bucket() {
            if(root) delete(root);
            pthread_rwlock_destroy(&rwlock);
        }
        void insert(K k, V v) {
            pthread_rwlock_wrlock(&rwlock);
            size++;
            if(root == NULL) root = new btree_node<K,V>(k, v);
            else root->insert(k,v);
            pthread_rwlock_unlock(&rwlock);
        }
        bool remove(K key) {
            return false;
        }
        V find(K key) {
            V ret = NULL;
            pthread_rwlock_rdlock(&rwlock);
            if(root != NULL) ret = root->find(key);
            pthread_rwlock_unlock(&rwlock);
            return ret;
        }
        bool shouldBurst() {
            return size > capacity;
        }
        node *burst() {
            pthread_rwlock_wrlock(&rwlock);
            node *newnode = new node();
            inOrderBurst(root, newnode);
            pthread_rwlock_unlock(&rwlock);
            return newnode;
        }
    private:
        void inOrderBurst(btree_node<K,V> *bn, node *nn) {
            if(bn == NULL) return;
            inOrderBurst(bn->left, nn);
            nn->insert(bn->key, bn->value);
            inOrderBurst(bn->right, nn);
        }
};
#endif