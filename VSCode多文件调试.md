+ 对于单文件调试来说，只需要默认的 launch.json 即可正常运行

  + ```json
    {
      // 使用 IntelliSense 了解相关属性。 
      // 悬停以查看现有属性的描述。
      // 欲了解更多信息，请访问: https://go.microsoft.com/fwlink/?linkid=830387
      "version": "0.2.0",
      "configurations": [
          {
              "name": "g++ - 生成和调试活动文件",
              "type": "cppdbg",
              "request": "launch",
              "program": "${workspaceFolder}/${fileBasenameNoExtension}",
              //args 是传递给main函数的参数，int argc,char** argv
              "args": [],
              "stopAtEntry": false,
              //cwd 是进入到这个路径下
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
              //"preLaunchTask": "C/C++: g++ 生成活动文件",
              "preLaunchTask": "build",
              "miDebuggerPath": "/usr/bin/gdb"
          }
      ]
    }
    ```

  + 重要的参数

    + program  指定可执行调试文件入口地址路径
    + args 可以给程序传参
    + cwd 是进入到某个路径下，对于一些脚本操作来说比较重要
    + preLaunchTask 前置执行任务
      + 由 tasks.json 负责生成可执行文件
      + 参数即为绑定的任务标签

  + ```json
    {
        "options": {
            "cwd":"${fileDirname}"
        },
        "tasks": [
            {
                "type": "cppbuild",
                "label": "C/C++: g++ 生成活动文件",
                "command": "/usr/bin/g++",
                "args": [
                    "-g",
                    "*.cpp",
                    "-o",
                    "${fileDirname}/${fileBasenameNoExtension}"
                ],
                "options": {
                    "cwd": "${fileDirname}"
                },
                "problemMatcher": [
                    "$gcc"
                ],
                "group": {
                    "kind": "build",
                    "isDefault": true
                },
                "detail": "调试器生成的任务。"
            },
            {
                "type":"shell",
                "label": "cmake",
                "command":"cmake",
                "args": [
                    ".."
                ]
            },
            {
                "label":"make",
                "group": {
                    "kind": "build",
                    "isDefault": true
                },
                "command":"make",
                "args": []
            },
            {
                "label": "build",
                "dependsOn":[
                    //"cmake",
                    "make"
                ],
            }
        ],
        "version": "2.0.0"
    }
    ```

  + 入口标签为 build

    + dependsOn 可以指定其他标签，这里指定 make 标签
    + make 标签主要是在 cwd 路径下 执行 make 命令
      + cwd 文件开始已经指定了
        + 这里可以看出 cwd 的作用，就是在 cwd 指定的路径下执行某个命令
      + group 属性是分组，分为 build 编译生成、test 测试 和 none
        + 指定为 test 时，在命令面板搜索 Run Test Task时，只有这个任务会被显示出来
        + 好像没什么特别大的功能作用

+ 由此，VSCode可以直接调用Makefile，指定 -g 参数则可以调试了

  + task.json 的功能就是生成可执行文件供 launch.json 使用
    + 可以利用 make 或 cmake 编译链接