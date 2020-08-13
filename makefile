CC			= g++

#DEBUG		= -g -D DEBUG

LFLAGS		= -Wall $(DEBUG)
CFLAGS		= -Wall -MMD $(DEBUG)

TARGET		= flux

SRCS		= $(wildcard src/*.cpp src/*/*.cpp)
OBJS		= $(subst src/,build/,$(SRCS:.cpp=.o))
BUILD_DIRS 	= $(sort $(dir $(OBJS)))


DEFINES		= 

INCLUDES	= 

LIBS		= 

LINKS		= -lsfml-graphics -lsfml-window -lsfml-system


build: $(BUILD_DIRS) $(TARGET)

$(BUILD_DIRS):
	mkdir -p $@

$(TARGET): $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o $(TARGET) $(LIBS) $(LINKS)

build/%.o: src/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDES) $(DEFINES)

clean:
	rm -Rv build/ $(TARGET)
	rm -fv $(TARGET)

rebuild: clean build

cleandepend:
	rm -fv $(OBJS:.o=.d)

-include $(OBJS:.o=.d)
