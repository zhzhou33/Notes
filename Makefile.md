+ 如果想取消某一条命令本身的打印，那么可以在这个命令的前面加上 @ 符号
+ := 立即展开赋值
+ = 延迟展开赋值
+ ?= 条件赋值（如果已经赋值，则跳过）
+ += 追加赋值，拼接
+ $@  当前规则中的目标
+ $^  当前规则中的所有依赖
+ $<  当前依赖中的第一个依赖
+ $$  当前执行的进程的进程编号
+ $*  模式识别中 % 匹配的部分
+ $(shell pwd)
  + 将 shell 命令的结果返回(得到当前目录，可以赋给常量)
  + PATH_DIR = $(shell pwd)
  + gcc *.c -I ./...
    + -I ：指定头文件所在目录，相对路径或绝对路径
    + -L ：指定链接的动态库或静态库路径
```makefile
# 变量替换
$(TGT):  $(SRCS: .c=.o) 	// .c = .o 表示所有的 .c 生成的 .o 
   $(CC)  $^ -o $@
注意：$(VAR:a=b)：表示把变量 VAR 中所有以 a 字串结尾的 a 替换成 b 字串
    所以 $(SRCS:.c=.o) 表示将SRCS中所有以 .c 结尾的文件都替换成 .o 结尾
    当然也可以使用  $(SRCS:%.c=%.o)
```

+ 头文件的更新，应该使得所有依赖于它的文件被重新生成，否则修改头文件不能重新生成make，会提示最新！(因此需要把头文件也写入规则，make 看到这个文件就会去判断是不是最新的？)
  + gcc -MM 源文件
    + gcc -MM test.c ：会生成所有依赖于 test.c 文件依赖关系
      + test.o : test.c

```makefile
%.d: %.c
	$(CC) -MM $< > $@ # 将所有的 .c 依赖关系生成后 重定向输出到 $@ (.d) 中
					  # 所以 .d 文件中存储的都是 .c 的依赖关系
# 下面一行规则用来告诉 make，将上面模式规则中的命令执行的结果包含进当前文件
include $(SRCS:.c=.d)	#包含进来的意思就是把 .d 文件的规则写在当前位置的意思
sinclude $(SRSC:.c=.d)	# 一样的作用，建议这么写
```

+ .PHONY: clean
  + 伪目标：不是一个真正的目标，仅仅是为了执行所在规则下面的命令，不应该让make来判断它是否存在或是否应该被生成。

```makefile
# 主 Makefile
TGT = a.out
# 指定子目录(生成 a.out 只会依赖于这些目录)(指定源文件所在目录，不需头文件)
SUB_DIR = main test
# 指定一个子目标，将子目录下的源文件链接打包
export SUB_TGT = built-in.o	# export 声明为全局

export PATH_DIR = $(shell pwd)		# 先指定当前所在目录
export HEAD_DIR = $(PATH_DIR)/head	# 指定头文件目录
export CC = gcc					# 指定编译器
export CFLAGS = -I$(HEAD_DIR) -Wall# 指定编译选项，-I后面加不加空格都可以，-Wall指定警告选项
export LD = ld						# 指定链接器
export LDFLAGS = 					# 指定链接选项，静态动态库
# 终极目标所在规则：
$(TGT): $(SUB_DIR)
	$(CC) $(CFLAGS) $(^:=/$(SUB_TGT))
	
# 下面的规则用来告诉 make 需要进入到生成 a.out 所依赖的那些子目录
# 不是一个真正的目标，仅仅是为了执行所在规则下面的命令，不应该让make来判断它是否存在或是否应该被生成
# 终极目标的依赖是一个目录，因此我们需要说明怎么进入这个目录里面去
$(SUB_DIR):
	make -C $@				# -C 选项可以让 make 进入到后面指定的目录中

clean:
	rm -vf $(TGT) 
	for dir in $(SUB_DIR); do \
		make -C $$dir clean;\
	done

.PHONY: clean $(SUB_DIR)
```

```makefile
# 子 Makefile：用来告诉 make 如何生成当前目录下的子目标
SRCS = test.c
SUB_DIR = foo
# 下面的规则用来说明如何生成当前目录下的子目标(是由当前目录下的 .c 生成的 .o 和当前目录下的子目录下的子目标临时打包生成)
$(SUB_TGT): $(SRCS:.c=.o) $(SUB_DIR)	# 打包链接生成 built-in.o
	$(LD) $(LDFLAGS) $(SRCS:.c=.o) $(SUB_DIR:=/$(SUB_TGT))	\
							-r -o $@	# -r 表示临时打包	

%.o: %.c
	$(CC) $(CFLAGS) $< -c
# 头文件 head 更新的时候也应该使得所有依赖它的文件被重新生成
%.d: %.c
	$(CC) $(CFLAGS) $< -MM > $@
sinclude $(SRCS:.c=.d)

$(SUB_DIR):
	make -C $@
clean:
	rm -vf $(SUB_TGT) $(SRCS:.c=.o) $(SRCS:.c:.d)
	for dir in $(SUB_DIR); do \
		make -C $$dir clean;\
	done
	
.PHONY: clean
```

+ 在命令前面加上 - ：会使当前命令有错则跳过，继续执行

+ 静态模式

  + $(objects): %.o : %.c  ：表示 ($(objects): %.o) :  %.c
  + 其中 % 匹配的都是一样的，都是 objects 中以 .o 结尾的属性

  ```makefile
  %.d: %.cpp
  	$(CC) $< -MM > $@.$$$$ -I Headers;\
  sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g'\ < $@.$$$$ > $@;\
  # 上面的 sed 是一个匹配替换命令。以自定义  ，分割
  	s 表示 sed 文件处理方式为替换文件内容，s，a，b 表示将 a 替换成 b
  # \($*\)\.o[ :]*
  	表示保存文件的内容，$* 表示文件名去除后缀名以外的文件名，同时可以使用 \1 来表示
  # \1.o $@ : 
  	\1.o 表示的就是前面匹配的内容后缀替换成 .o，$@ 也就是 .d 文件， : 作为文件内容
  # g
  	g 表示获得内存缓冲区内容，并替代当前模板快中的文本
  
  # 除此之外，还由一种写法
  sed 's/$*.o: /$*.o $*.d: /' $@.$$$$ > $@;\
  rm -vf $@.$$$$
  # 注意：这个 $* 就是匹配之前 % 匹配的部分
  ```

  + < $@$$$$ ：表示使用之前的临时变量作为输入
  + \1：就代表被匹配到的第一个模式，模式，就是正则用 （）括起来的内容

  \# 将 $@.$$$$ 这个临时文件中的 $*.o 替换成 $*.o $*.d

  \# 注意，这个 $* 的意思是匹配之前 % 匹配的部分
  
+ 注意：对于生成文件到指定目录

  + ```makefile
    # 错误使用
    $(OBJ_DIR)/$(SRCS:.cpp=.o)
    # 其中SRCS:.cpp=.o 仅仅只是做了一个替换的功能，最终生成的其实是 obj/main.o area.o
    # 正确使用，前缀函数
    OBJ_O   = $(addprefix $(OBJ_DIR)/,$(SRCS:.cpp=.o))
    # 或者:前缀函数+模式字符替换函数
    OBJ_O   = $(addprefix $(OBJ_DIR)/,$(patsubst %.cpp,%.o,$(SRCS)))
    ```

+ 函数

  + wildcard：扩展通配符

    + 在变量定义和函数引用时，通配符将失效，这种情况下，可以使用 wildcard

    + ```makefile
      # 用法
      $(wildcard PATTERN ...)
      # 获取工作目录的所有 .c 文件列表
      $(wildcard *.c) 
      # 获取工作目录的所有 .c 文件和 子目录 sub 中的所有 .c 文件
      $(wildcard *.c ./sub/*.c)
      # 配合循环: foreach 表示遍历 $(SRC_DIR) 的所有子目录，同时把子目录下的 .c 遍历出来
      SRC_C = $(foreach dir, $(SRC_DIR), $(wildcard $(dir)/*.c))
      ```

  + addprefix：加前缀函数

    + ```makefile
      # 用法, fixstring 表示要添加的固定前缀，逗号后面可以是一个或多个要添加前缀的子字符串，多个字符串以空格隔开
      $(addprefix ,fixstring,string1 string2 ...)
      # 返回加过前缀的文件名序列
      # 例如
      $(addprefix src/,a.c b.c)
      返回值是 "src/a.c src/b.c"
      ```

  + patsubst：模式字符串替换函数

    + ```makefile
      # 格式
      $(patsubst <pattern>,<replacement>,<text>)
      # 功能
      查找 <text> 中的单词(单词以 空格、Tab、回车、换行 分割)，是否符合模式 <pattern>
      如果匹配，则以 <replacement> 替换
      #  <pattern> 可以包括通配符 %，表示任意长度的字串
      函数返回被替换过后的字符串
      # 例如
      $(patsubst %.c,%.o,a.c b.c)
      返回值是 "a.o b.o"
      ```

  + dir：取目录函数

    + 从文件名序列 <names> 中取出目录部分。

      + 目录部分指最后一个反斜杠 / 之前的部分
      + 如果没有反斜杠，则返回 ./

    + ```makefile
      # 格式
      $(dir <names...>)
      # 示例
      $(dir src/foo.c hackc)
      返回值是 "src/ ./""
      ```

  + notdir：取文件函数

    + 从文件名序列 <names> 中取出非目录部分

      + 非目录部分指最后一个反斜杠 / 之后的部分

    + ```makefile
      # 格式
      $(notdir <names...>)
      # 示例
      $(notdir src/foo.c hacks)
      返回值是 "foo.c hacks"
      ```

  + 相关示例

  + ```makefile
    # 自动遍历 src 目录下的所有 .c 文件,生成位置在 src/*.c
    SRC_DIR = src
    OBJ_DIR = obj
    DP_DIR = dep
    SRC_C = $(foreach dir, $(SRC_DIR), $(wildcard $(dir)/*.c))
    # 生成所有的 .o 文件,生成位置在 obj/*.o
    OBJ_O = $(addprefix $(OBJ_DIR))/,$(patsubst %.c,%.o,$(notdir $(SRC_C)))
    # 生成所有的 .d 依赖,生成位置在 dep/*.d
    SRC_D = $(addprefix $(DP_DIR)/,$(patsubst %.c,%.d,$(notdit $(SRC_C))))
    ```

+ vpath 和 VPATH 只是在 make 寻找依赖关系时才有用，对于 gcc 等命令行来说还需要手动添加头文件目录

+ 关于在make下创建目录的问题

  + 把目录作为依赖。写法是在填依赖关系时加一个 |
    + out/foo.o : foo.c | out
    +  也可以写成两行
      + out/foo.o : foo.c
      + out/foo.o : |out
  + 目录前修饰了 | ， 这样，只有在 out 不存在时，才会提前 (在构建 out/foo.o 前)  去构建，当 out 已经存在，则 out/foo.o 则不依赖 out 的时间戳
