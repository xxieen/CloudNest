# 源文件列表
SRCS := $(wildcard *.c)
# 目标文件列表
OBJS := $(patsubst %.c, %.o, $(SRCS))

# 编译器
CC := gcc
# 编译选项
CFLAGS := -Wall -Wextra -g

# 目标：依赖列表
# 生成可执行文件
server: $(OBJS)
	$(CC) $^ -o $@ $(CFLAGS)
	$(RM) $(OBJS)  
# 通配符规则：.c -> .o
%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

# 清理规则：删除目标文件和可执行文件
clean:
	$(RM) $(OBJS) server

# 重建规则：清理并重新构建可执行文件
rebuild: clean server

# 声明伪目标，避免与同名文件冲突
.PHONY: clean rebuild

