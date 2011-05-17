CC=g++
CFLAGS=-O2 -DTESTSIZE=1000000 -lgsl -lgslcblas -lrt
GSL_RNG_SEED=`date +'%y%m%d%H%M%S'`
GSL_RNG_TYPE=uni
ENV=

tests: test_map test_trie 
#gsl_test thread_test

test_map: tests/test.c++
	$(CC) $(CFLAGS) tests/test.c++ -o tests/bin/test_map

test_trie: test_trie_map test_trie_unsorted test_trie_sorted test_trie_btree

test_trie_unsorted: tests/test.c++
	$(CC) $(CFLAGS) tests/test.c++ -DTRIE -DBUCKETTYPE=seq_unsorted_bucket\
					-DBUCKETSIZE=32 -o tests/bin/test_trie_unsorted_32
	$(CC) $(CFLAGS) tests/test.c++ -DTRIE -DBUCKETTYPE=seq_unsorted_bucket\
					-DBUCKETSIZE=64 -o tests/bin/test_trie_unsorted_64
	$(CC) $(CFLAGS) tests/test.c++ -DTRIE -DBUCKETTYPE=seq_unsorted_bucket\
					-DBUCKETSIZE=128 -o tests/bin/test_trie_unsorted_128
	$(CC) $(CFLAGS) tests/test.c++ -DTRIE -DBUCKETTYPE=seq_unsorted_bucket\
					-DBUCKETSIZE=256 -o tests/bin/test_trie_unsorted_256
	$(CC) $(CFLAGS) tests/test.c++ -DTRIE -DBUCKETTYPE=seq_unsorted_bucket\
					-DBUCKETSIZE=512 -o tests/bin/test_trie_unsorted_512
test_trie_map:
	$(CC) $(CFLAGS) tests/test.c++ -DTRIE -DBUCKETTYPE=seq_map_bucket\
					-DBUCKETSIZE=128 -o tests/bin/test_trie_map_128
	$(CC) $(CFLAGS) tests/test.c++ -DTRIE -DBUCKETTYPE=seq_map_bucket\
					-DBUCKETSIZE=256 -o tests/bin/test_trie_map_256
	$(CC) $(CFLAGS) tests/test.c++ -DTRIE -DBUCKETTYPE=seq_map_bucket\
					-DBUCKETSIZE=512 -o tests/bin/test_trie_map_512
	$(CC) $(CFLAGS) tests/test.c++ -DTRIE -DBUCKETTYPE=seq_map_bucket\
					-DBUCKETSIZE=4096 -o tests/bin/test_trie_map_4096

test_trie_btree: tests/test.c++
	$(CC) $(CFLAGS) tests/test.c++ -DTRIE -DBUCKETTYPE=seq_btree_bucket\
					-DBUCKETSIZE=32 -o tests/bin/test_trie_btree_32
	$(CC) $(CFLAGS) tests/test.c++ -DTRIE -DBUCKETTYPE=seq_btree_bucket\
					-DBUCKETSIZE=64 -o tests/bin/test_trie_btree_64
	$(CC) $(CFLAGS) tests/test.c++ -DTRIE -DBUCKETTYPE=seq_btree_bucket\
					-DBUCKETSIZE=128 -o tests/bin/test_trie_btree_128
	$(CC) $(CFLAGS) tests/test.c++ -DTRIE -DBUCKETTYPE=seq_btree_bucket\
					-DBUCKETSIZE=256 -o tests/bin/test_trie_btree_256
	$(CC) $(CFLAGS) tests/test.c++ -DTRIE -DBUCKETTYPE=seq_btree_bucket\
					-DBUCKETSIZE=512 -o tests/bin/test_trie_btree_512

test_trie_sorted: tests/test.c++
	$(CC) $(CFLAGS) tests/test.c++ -DTRIE -DBUCKETTYPE=seq_sorted_bucket\
					-DBUCKETSIZE=64 -o tests/bin/test_trie_sorted_64
	$(CC) $(CFLAGS) tests/test.c++ -DTRIE -DBUCKETTYPE=seq_sorted_bucket\
					-DBUCKETSIZE=128 -o tests/bin/test_trie_sorted_128
	$(CC) $(CFLAGS) tests/test.c++ -DTRIE -DBUCKETTYPE=seq_sorted_bucket\
					-DBUCKETSIZE=256 -o tests/bin/test_trie_sorted_256
	$(CC) $(CFLAGS) tests/test.c++ -DTRIE -DBUCKETTYPE=seq_sorted_bucket\
					-DBUCKETSIZE=512 -o tests/bin/test_trie_sorted_512


run: tests
	$(ENV) find ./tests/bin/ -executable | /bin/sh

time: tests
	$(ENV) time -v ./tests/bin/test_map 2> logs/time.log
	echo "\n" >> logs/time.log
	$(ENV) time -v ./tests/bin/test_trie_unsorted 2>> logs/time.log
	echo "\n" >> logs/time.log
	$(ENV) time -v ./tests/bin/test_trie_sorted 2>> logs/time.log
	echo "\n" >> logs/time.log
	$(ENV) time -v ./tests/bin/test_trie_btree 2>> logs/time.log
	cat logs/time.log | most

clean:
	rm -r -f tests/bin/
	mkdir tests/bin

thread_test: tests/shakespeare_ts_test.c++
	$(CC) $(CFLAGS) -DBUCKETSIZE=64 tests/shakespeare_ts_test.c++ -o tests/bin/thread_test