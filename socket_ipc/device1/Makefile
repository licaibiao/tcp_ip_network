#COMPILE_CROSS:= arm-hisiv300-linux-

CC := $(COMPILE_CROSS)gcc
GG := $(COMPILE_CROSS)g++
 
INC := -I ../common

COMMON_C  := $(wildcard ../common/*.c)

COMMON_OBJ	  := $(patsubst %.c,%.o,$(COMMON_C))

CFLAGS += -lpthread

client_1:$(COMMON_OBJ) device1_client1.o
	$(CC) $^ $(CFLAGS) -o $@ 

client_2:$(COMMON_OBJ) device1_client2.o
	$(CC) $^ $(CFLAGS) -o $@ 

$(COMMON_OBJ):%.o:%.c
	echo $(COMMON_OBJ)
	$(CC) $(CFLAGS) -c $< -o $@ $(INC)	
 
device1_client1.o:%.o:%.c
	echo $(MAIN_OBJ)
	$(CC) $(CFLAGS) -c $< -o $@ $(INC)	

device1_client2.o:%.o:%.c
	echo $(MAIN_OBJ)
	$(CC) $(CFLAGS) -c $< -o $@ $(INC)	
 
clean:
	$(RM) ../common/*.o
	$(RM) ./*.o
	$(RM) client_1 client_2
