# 伪目标声明后
.PHONY: clean

# 编译器
CC = gcc

# 编译选项
#-I./include
CFLAGS = -Wall

# 源代码文件
SRCS = $(wildcard src/*.c)
OBJS = $(SRCS:.c=.o)

# 目标可执行文件
TARGET = MinProxy

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f $(OBJS) 