#COMPILE_CROSS:= arm-hisiv300-linux-

TARGET = server

CC := $(COMPILE_CROSS)gcc
GG := $(COMPILE_CROSS)g++
 
INC := -I ../common

COMMON_C  := $(wildcard ../common/*.c)
MAIN_C	  := $(wildcard ./*.c)

COMMON_OBJ	  := $(patsubst %.c,%.o,$(COMMON_C))
MAIN_OBJ	  := $(patsubst %.c,%.o,$(MAIN_C))

CFLAGS += -lpthread

$(TARGET):$(COMMON_OBJ) $(MAIN_OBJ) 
	$(CC) $^ $(CFLAGS) -o $@ 
	
$(COMMON_OBJ):%.o:%.c
	echo $(COMMON_OBJ)
	$(CC) $(CFLAGS) -c $< -o $@ $(INC)	
 
$(MAIN_OBJ):%.o:%.c
	echo $(MAIN_OBJ)
	$(CC) $(CFLAGS) -c $< -o $@ $(INC)	
 
clean:
	$(RM) ../common/*.o
	$(RM) ./*.o
	$(RM) $(TARGET)
