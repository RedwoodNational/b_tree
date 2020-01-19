CC=g++
CFLAGS=-pedantic -Wall -Werror -Wno-sign-compare -Wno-long-long -lm


all:wordlist

wordlist: driver.o  b_tree.o b_tree_node.o
	$(CC) $(CFLAGS)  driver.o b_tree.o b_tree_node.o -o wordlist

driver.o: driver.cpp b_tree.h
	$(CC) $(CFLAGS) -c driver.cpp

b_tree.o: b_tree.h b_tree_node.h stack.h queue.h b_tree.cpp
	$(CC) $(CFLAGS) -c b_tree.cpp

b_tree_node.o: b_tree_node.h b_tree_node.cpp
	$(CC) $(CFLAGS) -c b_tree_node.cpp

clean:
	rm wordlist driver.o b_tree.o b_tree_node.o
