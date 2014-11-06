CC			= g++

#DEBUG		= -g -D DEBUG

CFLAGS		= -Wall -MMD $(DEBUG) $(NOT_DEBUG)
#CFLAGS		= -Wall $(DEBUG) $(NOT_DEBUG)
LFLAGS		= -Wall $(DEBUG) $(NOT_DEBUG)

TARGET		= BIN

SRCS		= \
$(wildcard src/*.cpp src/*/*.cpp)

OBJS		= $(addprefix build/, $(notdir $(SRCS:.cpp=.o)))

DEFINES		= 

INCLUDES	= 

LIBS		= 

LINKS		= -lsfml-window -lsfml-audio -lsfml-graphics -lsfml-system



build: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LFLAGS) -o $(TARGET) $(OBJS) $(LIBS) $(LINKS) 

build/%.o: src/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDES) $(DEFINES)
build/%.o: src/*/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDES) $(DEFINES)

clean:
	rm -fv $(OBJS) $(OBJS:.o=.d) $(TARGET)

rebuild: clean build

valgrind: $(TARGET)
	(valgrind --show-reachable=yes --leak-check=full -v ./$(TARGET))

run:
	./$(TARGET)

depend: $(SRCS)
	....

cleandepend:
	rm -fv $(OBJS:.o=.d)

-include $(OBJS:.o=.d)