# This file is a makefile file for building and running.
# Author: 12012613 Zhong Yuanji钟元吉
# Encode: UTF-8
# Version: GNU Make 4.3
# Date: 2022/11/20

# 在终端输入make即可编译并运行文件TestForData.c
CXX = gcc # 使用gcc编译C文件
CFLAGES = -c -Wall -mavx2

SRC_DIR = ./src
INC = -I./inc # 寻找inc所有文件
SRC = $(wildcard $(SRC_DIR)/*.c) # 寻找.c文件
OBJS = $(patsubst  %.c,  %.o, $(SRC)) # 代替代码
TARGET = TestForData # 生成文件名

# Linux
$(TARGET) : $(OBJS)
	$(CXX) -o $@ $(OBJS)
	./$(TARGET)
	rm -f $(SRC_DIR)/*.o
%.o : %.c
	$(CXX) $(CFLAGES) $< -o $@ $(INC)

.PHONY : clean
clean :
	rm -f $(TARGET) $(SRC_DIR)/*.o

# # Windows
# $(TARGET) : $(OBJS)
# 	$(CXX) -o $@ $(OBJS)
# 	.\$(TARGET)
# 	del .\src\*.o
# %.o : %.c
# 	$(CXX) $(CFLAGES) $< -o $@ $(INC)

# .PHONY : clean
# clean :
# 	del .\$(TARGET).exe .\src\*.o