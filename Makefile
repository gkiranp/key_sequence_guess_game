
deafult: all

all: 
	g++ --std=c++11 -pthread keypress_seq_guess.cpp -o keypress_seq_guess

clean:
	rm -fr keypress_seq_guess
