CC=../gstlfilt/gfilt
CFLAGS=-O2 -DSEED=38957421 -DTESTSIZE=1000000 -DSEARCH=1

tests: test_map test_trie

test_map: test.c++
	$(CC) $(CFLAGS) test.c++ -DTRIE=0 -o test_map

test_trie: test.c++
	$(CC) $(CFLAGS) test.c++ -DTRIE=1 -o test_trie

massif: tests
	valgrind --tool=massif --massif-out-file=massif.map.out ./test_map
	valgrind --tool=massif --massif-out-file=massif.trie.out ./test_trie

massif_show:
	ms_print massif.map.out | head -n 30
	ms_print massif.trie.out | head -n 30
	rm -f ms_print.tmp.*

time: tests
	time -v ./test_map 2> time.log
	time -v ./test_trie 2>> time.log

clean:
	rm -f test_map test_trie *.o massif.map.out massif.trie.out