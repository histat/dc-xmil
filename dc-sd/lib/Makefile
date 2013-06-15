RONIN = /usr/local/ronin

CC = sh-elf-gcc -ml -m4-single-only
AR = sh-elf-ar

RM	= rm -f
MKDIR = mkdir

NAME = sdcard-test
TARGET = $(NAME).elf
STRIPELF = $(NAME).strip.elf

LIBS = -L$(RONIN)/lib -lronin -lz -lm

CFLAGS = -Wall -O4 -fomit-frame-pointer -I$(RONIN)/include -I$(RONIN)/include -I.
LDFLAGS = -Wl,-Ttext,0x8c010000 -nostartfiles $(RONIN)/lib/crt0.o

OBJS = sci.o crc.o cc932.o ff.o sd.o

$(TARGET): $(OBJS) main.o
	$(CC) $(LDFLAGS) -o $(TARGET) $(OBJS) main.o $(LIBS)
	sh-elf-strip -o $(STRIPELF) $(TARGET)

libsd.a: $(OBJS)
	$(AR) rs $@ $(OBJS)

clean:
	$(RM) $(OBJS) $(TARGET) $(STRIPELF)
