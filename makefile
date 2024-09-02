#Place this makefile in the same directory as your all .cpp and .h files
#at the command prompt
#make ./<TARGET>


# BUILD SWITCH

DEBUG:=1


#TODO;� specify the list of source files programmed for this project, SOURCES = ???
SOURCES = main.cpp card.cpp table.cpp shoe.cpp dealer.cpp player.cpp simulator.cpp HoldCalculator.cpp
SOURCES += logging.cpp
INCS = ???

####### Specify the target name ######
TARGET = BJKiller  # TODO;� specify the executable program name of this project, TARGET = ???

#-------Do Not Change below this line-------------
#Compile with g++
CC = g++

#use -g for gnu debugger and -std= for c++11 compiling
CXXFLAGS = -g -std=c++11

OBJS:= ${SOURCES:.cpp=.o}

###### Tabs Matter!#########
$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS)

.cpp.o:
	$(CC) -c $(CXXFLAGS) $(INCDIR) $<

$(INCS): $(OBJS)

#In order to get rid of all .o files create, at the command prompt
#make clean
clean:
	rm -f $(OBJS) $(TARGET) core
