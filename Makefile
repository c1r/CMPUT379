CC = gcc

CFLAGS = -g -Wall

TARGET = msh379

all: $(TARGET)

$(TARGET): $(TARGET).c util.c commands.c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c util.c commands.c

clean:
	find . -type f ! -name "*.c" ! -name "*.h" ! -name "Makefile" ! -name "*.pdf" -exec rm {} \;

tar:
	tar -zcvf submit.tar $(TARGET).c msh379.h commands.c commands.h util.h util.c Makefile project_report.pdf