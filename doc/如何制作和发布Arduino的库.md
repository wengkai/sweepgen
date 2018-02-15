本文是关于如何将自己写的Arduino库整理、发布成Arduino IDE的库管理器能管理、能提供给所有用户下载、升级的库。

如何将自己写的程序做成库，即一对.h、.cpp文件，网上已经有很多资料了。

# Arduino库1.5版本格式要求

https://github.com/arduino/Arduino/wiki/Arduino-IDE-1.5:-Library-specification

新的库格式是为了从1.6.2版的IDE开始所支持的自动的库管理器而设计的。用库管理器可以自动下载、更新互联网上的库。

1.5格式的最新版本是2.1版。

## 文件夹布局

### 源代码

源代码位于`src`目录下，如：

```C++
Servo/src/Servo.h
Servo/src/Servo.cpp
```

### 示例代码

示例代码必须位于`examples`目录下。

### 额外的文档

文档可以放在`extras`目录下。

## 元数据

###库属性

在库的根目录放`library.properties`文件。这个文件里的数据让库管理器可以自动搜索、安装这个库。

这个文件中的数据都是`key=value`的格式，内容都是UTF-8编码的。可用的字段有：

* name：库的名字，只能用字母、数字、空格、下划线、小数点和减号，不能以数字开头，前后不能有空格。
* version：版本号，必须是[semver](https://semver.org)格式的版本号，如1.2.0或1.2。
* author：作者的姓名和电邮
* maintainer：维护者的姓名和电邮
* sentence：一句描述库的作用的话。
* paragraph：一段描述库的作用的话。
* category：分类。请归于下列分类之一：
  * Display
  * Communication
  * Signal Input/Output
  * Sensors
  * Device Control
  * Timing
  * Data Storage
  * Data Processing
  * Other
* url：库的托管网址，如github的项目网址。
* architectures：逗号分隔地列出库所支持的硬件平台，`*`表示支持所有平台。
* dot_a_linkage：值为`true`表示支持编译为.a文件。
* includes：逗号分隔地列出要用户程序加的`#include`文件。如果没有这个变量，则`src/`下所有的`.h`文件都会被`#include`。
* precompiled：目前还没有启用。如果是`true`，就会使用`src/{build.mcu}`里的`.a`或`.so`文件。这里的`{build.mcu}`是预先编译的库文件的硬件平台，比如`cortex-m3`。
* ldflags：目前还没有启用。表示链接的选项，如`ldflags=-lm`。

### 关键字

在库根目录下的`keywords.txt`文件，用来给出这个库所产生的需要语法高亮的关键字。

这个文件的格式是每行为一个`<名字> <关键字类型>`的对，`#`开头的行是注释。

关键字类型有：

- KEYWORD1
- KEYWORD2
- KEYWORD3
- LITERAL1
- LITERAL2
- RESERVED_WORD
- RESERVED_WORD2
- DATA_TYPE
- PREPROCESSOR

如：

```
#######################################
# Syntax Coloring Map For Button
#######################################
 
#######################################
# Datatypes (KEYWORD1)
#######################################
 
Button KEYWORD1
 
#######################################
# Methods and Functions (KEYWORD2)
#######################################
 
isPressed    KEYWORD2
 
#######################################
# Constants (LITERAL1)
#######################################
```



# 发布Aruino库

https://github.com/arduino/Arduino/wiki/Library-Manager-FAQ

要发布的库最好按照1.5版库格式编写，一定要有`library.properties`文件。

## 发布

* 在GitHub这样的托管网站发布
* 给当前的版本打一个标记tag，并push这个tag（GitHub的[Tag](https://git-scm.com/book/en/v2/Git-Basics-Tagging)功能说明）。
* 在[Arduino的GitHub](https://github.com/arduino/Arduino/issues)上打一个issue，给出下载你的库的仓库的URL。
* 过了几天，Arduino团队的某人就会把你的库加入到库管理器的索引中，然后关闭issue。这之后，很快你的库就可以在库管理器中下载了。

## 更新

一旦你的库已经被发布在库管理器了，如果你有了更新，就需要修改`library.properties`文件中的`version`，然后再打一个tag。库管理器的索引每个小时会检查一次，因此会发现你的升级并发布的。





