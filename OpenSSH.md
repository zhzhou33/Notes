+ 服务器端

  + 使用 OpenSSH 工具：https://github.com/PowerShell/Win32-OpenSSH/releases
    + 这个工具自带服务端与客户端
    + 打开cmd，安装 sshd (服务端)
      + cd 到安装目录下
      + powershell.exe -ExecutionPolicy Bypass -File install-sshd.ps1
      + 执行安装sshd
      + 设置服务自动启动
        + sc config sshd start=auto
        + net start sshd
      + 也可以直接 win+r ：输入 services.msc 找到 OpenSSH server 的服务并启动
      + 添加环境变量 path = OpenSSH 文件夹路径
        + 这样就可以直接在 cmd 输入 ssh 命令了
        + 客户端也可以这样设置

+ 客户端

  + cmd 输入 ssh-keygen -t rsa
    + 注意 ssh 命令是否有效，无效则添加环境变量
    + 此命令生成公私钥对，输入后一直回车就可以了
    + 生成在 C:\Users\Administrator\\.ssh
    + 注意 里面有个 pub 是公钥，需要放在服务端
      + 把 客户端的公钥文件重命名成 authorized_keys
      + 然后复制到服务端的 .ssh文件目录下
        + 里面存放的是所有的授权公钥，以后直接在文件后面附加即可
    + 至此，客服端已经配置完毕

+ PS

  + 最重要的一步！

    + 打开 C:\ProgramData\ssh\ssh_config
    + 确保以下3条没有被注释 
    
      + PubkeyAuthentication yes 
      + AuthorizedKeysFile	.ssh/authorized_keys 
      + PasswordAuthentication no 
    + 确保以下2条有注释掉 

      + #Match Group administrators 
    + \#       AuthorizedKeysFile \__PROGRAMDATA\__/ssh/administrators_authorized_keys
    + 之后再powershell 重启服务：Restart-Service sshd
  + 回到客户端：
    + ssh userName@serverIP
    + 可以再 客户端的 .ssh 文件夹下新建一个 config 文件，内容如下保存
      + Host *
        User Administrator
        Port 22
        IdentityFile ~/.ssh/id_rsa
      + Host * 表示连接所有主机
      + 这样就可以直接 ssh 192.168.xxx.xxx 了，不用输 userName了
    + 例如：
      + Host *     
        + Port 2222 
      + Host remoteserver     
        + HostName remote.example.com     
        + User zhou     
        + Port 2112
    + Host *表示对所有主机生效，后面的Port 2222 表示所有主机的默认连接端口都是2222
      + 这里的缩进并不是必须的，只是为了直观好看
    + 后面的 Host remoteserver 表示，下面的设置只对 rempteserver 生效
      + 注意：remoteserver 只是一个别名，具体由 HostName 指定
    + User 和 Port 表示用户名和端口，Port会覆盖掉上面的 Host * 的端口配置？
    + 以后登陆：
      + ssh remoteserver
      + 等同于
      + ssh -p 2112 zhou@remote.example.com
    + 注意：Host 命令的值可以使用通配符
      + Host * 表示对所有的主机都有效
      + Host *.edu 表示只对一级域名为 .edu 的主机有效
      + 他们的设置都可以被单个主机的设置覆盖
  
+ 权限问题

  + 生成密钥之后，建议修改它们的权限，防止其他人读取

    + chmod 600 ~/.ssh/id_rsa
    + chmod 600 ~/.ssh/id_rsa.pub
  + 之后就把自己的公钥上传服务器，并且注意将服务器端的 authorized_keys 文件的权限设为 644，即只有文件所有者才能写，如果权限不对，SSH服务器可能会拒绝读取该文件
  
    + chmod 644 ~/.ssh/authorized_keys
  + ssh-copy-id 命令：自动上传公钥

    + OpenSSH 自带一个ssh-copy-id命令，可以自动将公钥拷贝到远程服务器的/.ssh/authorized_keys文件。
    + 如果/.ssh/authorized_keys文件不存在，ssh-copy-id命令会自动创建该文件。用户在本地计算机执行下面的命令，就可以把本地的公钥拷贝到服务器`$ ssh-copy-id -i key_file user@host`。
    +  上面命令中，-i参数用来指定公钥文件，user是所要登录的账户名，host是服务器地址。如果省略用户名，默认为当前的本机用户名。执行完该命令，公钥就会拷贝到服务器。注意，公钥文件可以不指定路径和.pub后缀名，ssh-copy-id会自动在~/.ssh目录里面寻找`$ ssh-copy-id -i id_rsa user@host`。
    上面命令中，公钥文件会自动匹配到~/.ssh/id_rsa.pub。ssh-copy-id会采用密码登录，系统会提示输入远程服务器的密码。
    + 注意，ssh-copy-id是直接将公钥添加到authorized_keys文件的末尾。如果authorized_keys文件的末尾不是一个换行符，会导致新的公钥添加到前一个公钥的末尾，两个公钥连在一起，使得它们都无法生效。所以，如果authorized_keys文件已经存在，使用ssh-copy-id命令之前，务必保证authorized_keys文件的末尾是换行符（假设该文件已经存在）
  + 关闭密码登录
    + 为了安全性，启用密钥登录之后，最好关闭服务器的密码登录。对于 OpenSSH，具体方法就是打开服务器 sshd 的配置文件/etc/ssh/sshd_config，将PasswordAuthentication这一项设为no `PasswordAuthentication no`，修改配置文件以后，不要忘了重新启动 sshd，否则不会生效。