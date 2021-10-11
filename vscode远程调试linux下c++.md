+ 先创建项目文件夹

+ 检查更新以及安装必要的工具

  + sudo apt-get update
  + sudo apt-get dist-upgrade
  + sudo apt-get install build-essential gdb
    + 安装gdb自动安装g++
    + whereis g++
    + whereis gdb

+ ssh连接linux主机，并打开之前创建的项目文件夹

+ 在项目文件下创建一个测试程序 helloworld.cpp

  + 提示安装 C/C++ 扩展，直接安装，安装到服务器

+ 创建 tasks.json

  + 主菜单选择 终端 - > 配置默认生成任务
  + 在下拉列表，选择 g++ build active file，选择 /usr/bin 下

+ ```json
  {
  	// 有关 tasks.json 格式的文档，请参见
  	// https://go.microsoft.com/fwlink/?LinkId=733558
  	"version": "2.0.0",
  	"tasks": [
  		{
  			"type": "shell",
  			"label": "g++ build active file",
  			"command": "/usr/bin/g++",
  			"args": [
  				"-std=c++11",
  				"-g",
  				"${file}",
  				"-o",
  				"${fileDirname}/${fileBasenameNoExtension}"
  			],
  			"options": {
  				"cwd": "/usr/bin"
  			},
  			"problemMatcher": [
  				"$gcc"
  			],
  			"group": {
  				"kind": "build",
  				"isDefault": true
  			}
  		},
  		{ //删除二进制文件
  			"type": "shell",
  			"label": "delete output file",
  			"command": "rm",
  			"args": [
  				"${fileDirname}/${fileBasenameNoExtension}"
  			],
  			"presentation": {
  				"reveal": "silent", //删除过程不切换终端（专注程序输出）
  			}
  		}
  	]
  }
  ```

+ 主菜单选择  运行 - > 添加配置 - > 选择 C++（GDB/LLDB）

+ ```json
  {
      // 使用 IntelliSense 了解相关属性。 
      // 悬停以查看现有属性的描述。
      // 欲了解更多信息，请访问: https://go.microsoft.com/fwlink/?linkid=830387
      "version": "0.2.0",
      "configurations": [
          {
              "name": "g++ build and debug active file",
              "type": "cppdbg",
              "request": "launch",
              "program": "${fileDirname}/${fileBasenameNoExtension}",
              "args": [],
              "stopAtEntry": false,
              "cwd": "${workspaceFolder}",
              "environment": [],
              "externalConsole": false,
              "MIMode": "gdb",
              "setupCommands": [
                  {
                      "description": "为 gdb 启用整齐打印",
                      "text": "-enable-pretty-printing",
                      "ignoreFailures": true
                  }
              ],
              "preLaunchTask": "g++ build active file",
              "postDebugTask": "delete output file",
              "miDebuggerPath": "/usr/bin/gdb"
          }
      ]
  }
  ```

+ 对于 launch.json 文件来说

  + externalConsole：false 让输出在 vscode 内置终端

  + miDebuggerPath ：

    + ```
      "miDebuggerArgs": "-q -ex quit; wait() { fg >/dev/null; }; /bin/gdb -q --interpreter=mi"
      ```

    + 可以让终端不显示调试启动信息
  
+ 如果出现引入库报错的情况，可以使用命令 

  + g++ -v -E -x c++ -
  + 将输出的头文件放入 c_cpp_properties.json 中的 include 路径后面即可
  
+ 调试时如果出现 libc-start.c 错误，还需要去下载这个库

  + cd /
  + sudo mkdir build
  + cd build
  + sudo mkdir glibc-LK5gWL   //文件名对应报错的文件名
  + cd glibc-LK5gWL
  +  sudo wget http://ftp.gnu.org/gnu/glibc/glibc-2.23.tar.gz 
    + 需要对应的版本号
  +  sudo tar -zxvf glibc-2.23.tar.gz 
  +  其实就是没有找到相关库，需要在对应的地方放进对应的库文件。
    (版本下错了也没关系，自己重命名为报错的类型也是可以的)。 
  
+ coderunner 中文乱码

  + ```json
    "cpp": "cd $dir && g++ -fexec-charset=GBK -std=c++17 $fileName -o $fileNameWithoutExt && $dir$fileNameWithoutExt",
    ```

  + 因为vscode的编码是 utf-8，而cmd为GBK格式，所以需要转换一下