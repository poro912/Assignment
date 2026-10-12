OBJS = main.o word_tree.o word_tree_control.o IOSystem.o strings.o 

# ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
word_tree : $(OBJS)
	g++ $(OBJS) -o word_tree

main.o : main.h main.cc word_tree.h IOSystem.h
	g++ -c main.cc

word_tree.o : word_tree.h word_tree.cc word_tree_control.o
	g++ -c word_tree.cc

word_tree_control.o : word_tree.h word_tree_control.cc strings.o
	g++ -c word_tree_control.cc

IOSystem.o : IOSystem.h IOSystem.cc
	g++ -c IOSystem.cc

strings.o : strings.h strings.cc
	g++ -c strings.cc

#ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
.PHONY: clean all
clean : 
	rm -f $(OBJS)
