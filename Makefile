CC = gcc

CFLAGS = -g -Wall

TARGET = msh379

all: $(TARGET)

$(TARGET): $(TARGET).c util.c commands.c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c util.c commands.c

clean:
	$(RM) -rf $(TARGET) commands.o util.o $(TARGET).dSYM submit.tar

tar:
	tar -zcvf submit.tar $(TARGET).c msh379.h commands.c commands.h Makefile project_report.pdf