+ 对于一个新建的文件夹，首先使用
  + git init
    + 来初始化该文件夹为一个本地仓库
    + 此时还没有生成默认的 master 分支
    + 需要 commit 之后才会生成本地仓库 master 分支
  + git remote add origin 远程master地址
    + 建立本地与远程仓库的连接
  + git add .
    + 新建任意文件，add
  + git commit -m "new"
    + 此时就有本地默认分支 master 了
    + 如果需要新建分支，使用
      + git branch dev
      + 然后切换分支 dev
      + git checkout dev
    + 也可以两步合并一步
      + git checkout -b dev
    + 注意：这个新建的分支是基于当前所在分支的，也就是说新建的分支内容跟当前分支一样，提交的历史记录也是一样的应该
      + 纯净分支命令
        + git checkout --orphan dev
          + 该命令会创建名为 dev 的分支，并且该分支下有前一个分支的所有文件，也就是执行这条命令的分支内容，但是这个新建的分支没有历史记录
        + git rm -rf .
          + 删除所有内容
      + 如果没有文件提交的话，分支是看不到的
  + git push
    + git push <远程主机名> <本地分支名>:<远程分支名>
    + 如果本地分支名与远程分支名相同，则可以省略冒号
      + git push <远程主机名> <本地分支名>
      + git push origin master
        + 将本地master分支推送到origin主机的master分支
      + 强制推送
        + git push --force origin master
      + 删除主机的分支
        + git push origin --delete dev
      + 删除本地的分支
        + git branch -D dev
        + git branch -d dev
    + git push 把本地分支推送到远程分支
      + 本地分支必须要存在，远程可以不存在，自动创建
        + 如果远程存在了，可能需要 git pull 同步版本信息
    + git push -u origin master:dev
      + -u 命令指定流的 upstream 和 downstream ，指定一次就行，本地分支只能映射一个远程分支
        + 对于 git pull 指令来说，必须要指定这种映射关系，在 .git/config 文件中可以看到这种映射关系
    + 对于已经指定流的映射，根据默认的 git push 配置，可以简化命令
      + git push 代指 git push origin master:dev
        + 根据之前制定的流映射关系，同时需要根据默认的配置关系
          + upstream  根据流映射自动push
          + simple  根据流映射，但是必须是同名的分支，即本地分支与远程分支同名，才可以自动push
        + 通过 git config --global push.default upstream 指定
          + 默认的是 simple，即必须是同名分支才可以简写
  + git pull
    + git pull <远程主机名> <远程分支名>:<本地分支名>
      + 将远程主机的分支拉取过来，与本地分支合并，如果本地没有，则新建
    + 出现 rejected(non-fast-forward)
      + git pull --rebase origin master
    + 出现 refusing to merge unrelated hsitories
      + git pull --allow-unrelated-histories
  
+ 克隆某一个分支
  
  + git clone <remote_repo> -b <branch>
  
+ 对于 git clone 下来的文件夹
  + 默认会有一个本地 master 分支，没有的话 git commit 就可以显示了
  + 之后可以创建新分支，也可以直接将 master 分支 push 到指定分支上
  
+ 撤销 git add

  + git reset
  + git reset HEAD

+ 撤销 commit
  + git reset --soft HEAD^	撤销上一次的commit，不撤销 add
  
  + git reset --hard HEAD^    撤销commit且撤销 add
    
    + 版本回退
    
    + HEAD^ 表示上一个版本，即上一次的commit，也可以写成 HEAD~1(波浪)
    + HEAD~2    撤销两次 commit
    
  + git commit --amend    修改注释
  
+ 版本控制

  + --mixed![img](https://img-blog.csdn.net/20180703134315399)
    + --mixed是reset的默认参数
      + HEAD是当前分支版本顶端的别名，也就是在当前分支最近的一次提交
      + INDEX也称作 Staging area，暂存区。是指一整套即将被下一个提交的文件集合，也是将成为当前HEAD的那个commit
      + Working Copy代码正在工作的文件源码
    + --mixed重置HEAD到另外一个commit，并且重置index(暂存区)和HEAD相匹配
      + 即回退到某次 commit的节点，但是是git add 之前的时候
  + --soft![img](https://img-blog.csdn.net/20180703134238194)
    + 回退到某次commit，但是是在 git add 之后的状态
  + --hard
    + 会改变本地的 Working Copy，可能会丢失数据
    + 使用 git reflog 查找提交记录 hash 值，然后恢复，如果没有，则无法恢复！
    + 如果希望彻底丢掉本地修改但是不希望更改branch指向的commit，可以执行
      + git reset --hard
      + git reset --hard HEAD
  + git checkout HEAD^
    + 类似于 git resert --hard，但是git checkout 不会改变当前 branch 的标签指向，而 reset会变动 branch 标签![img](https://img-blog.csdn.net/20180902113022436?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM0OTA4OTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
  + git restore  针对于 git commit 之前的操作进行撤销回退
    + git restore <file> 使得在工作空间但是不在暂存区的文件，撤销更改，内容恢复到没修改之前
      + 首先，要是一个修改的文件，即前提是同步好的文件，之后在工作空间被修改
      + 其次，修改后没有 git add
      + 执行指令，撤销本地的修改，与暂存区同步，就是 git add 之后的区
    + git restore --staged <file> 将暂存区的文件从暂存区撤出，但不会更改文件的内容