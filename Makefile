CC = "g++"
PROJECT = output
SRC = main.cpp interconnect.cpp soft.cpp bram.cpp ip_hard.cpp vp.cpp

LIBS = `pkg-config opencv4 --cflags --libs`

$(PROJECT) : $(SRC)
	$(CC) -g -lsystemc $(SRC) -o $(PROJECT) $(LIBS) 
run:  
	./$(PROJECT)
	
clean:
	rm output
