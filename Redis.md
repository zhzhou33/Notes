+ cmd 输入 redis-server 开启数据库服务
+ cmd 输入 redis-cli 开启客户端，默认连接 127.0.0.1:6379
  + 远程连接方式：redis-cli -h 127.0.0.1 -p 6379
    + -a 可以指定密码
+ 常用操作
  + DEL <key>
    + key 存在时删除key
  + EXISTS <key>
    + 检查给定 key 是否存在
  + KEYS <pattern>
    + 查找所有符合给定模式 (pattren) 的 key
  + RENAME <key> <newkey>
    + 修改 key 的名称
    + 可能会产生冲突
    + 使用 RENAMENX <key> <newkey>
      + 仅当 newkey 不存在时，将key改名为 newkey
  + TYPE <key>
    + 返回key所存储的值的类型
  + SET <key> <value>
    + 设置指定 key 的值
  + GET <key>
    + 获取指定 key 的值
+ Redis 集合 Set
  + SADD <key> <member1> [member2]
    + 向集合添加一个或多个成员
  + SCARD <key>
    + 返回集合的成员数
  + SINTER <key1> [key2]
    + 返回给定所有集合的交集
  + SINTERSTORE <destination> <key1> [key2]
    + 返回给定所有集合的交集并存储在 destination 中
  + SISMEMBER <key> <member>
    + 判断 member 元素是否是集合 key 的成员
  + SMEMBERS <key>
    + 返回集合中的所有成员
    + 注意：redis是单线程的，如果集合中查找元素过多，则可能会阻塞，因此使用 SSCAN 更好
  + SMOVE <source> <destination> <member>
    + 将 member 元素从 source 集合移动到 destination 集合
  + SREM <key> <member1> [member2]
    + 移除集合中一个或多个成员
  + SUNION <key1> [key2]
    + 返回所有给定集合的并集
  + SUNIONSTORE <destination> <key1> [key2]
    + 所有给定集合的并集存储在 destination 集合中
  + SSCAN <key> <cursor> MATCH <pattren> COUNT <count>
    + 迭代集合 set 中的元素
+ SCAN 命令
  + SCAN <cursor> MATCH <pattern> COUNT <count>
    + 迭代数据库中的数据库键
    + count选项后面跟的数字并不是意味着每次返回的元素数量，而是scan命令每次遍历字典槽的数量 
    + 每次迭代默认 count = 10
    + 返回给是第一个是游标的位置，如果为 0 说明已经迭代完了
      + 第二个是查找结果
    + scan 0 match * count 5
      + 从 0 开始迭代查找 pattern = *