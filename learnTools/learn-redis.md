
- [redis-github](https://github.com/redis/redis/)
- [redis为什么慢？](https://zhuanlan.zhihu.com/p/346302897)


## Redis简介

简单来说，Redis（Remote Dictionary Server）是一个开源的、基于内存的、键值型数据结构存储系统。它通常被用作数据库、缓存和消息代理。

几个关键特性：

1. 基于内存 (In-Memory):
   · 这是Redis最核心的特点。所有数据主要存储在RAM（内存）中，这使得其数据读写速度极快，通常能达到微秒级别。相比之下，传统磁盘数据库（如MySQL）的读写速度在毫秒级别。
   · 代价是内存成本比硬盘高，所以单个Redis实例不能存储超大规模的数据（除非使用集群）。同时，为了防止服务器重启或宕机导致数据丢失，Redis也提供了持久化机制。

2. 键值存储 (Key-Value Store):
   · 数据以键值对的形式存储。每个键（Key）都关联一个值（Value）。
   · 键是简单的字符串。
   · 值可以是多种数据结构，而不仅仅是字符串。这是Redis强大之处。

3. 丰富的数据结构 (Data Structures):
   · String（字符串）: 最基本类型，可以存文本、数字，甚至是二进制数据（如图片）。
   · Hash（哈希）: 类似于编程语言中的Map或dict，适合存储对象（如用户信息：name, age, email）。
   · List（列表）: 有序的字符串列表，支持从两端插入或弹出元素，可实现栈（Stack）或队列（Queue）的功能。
   · Set（集合）: 无序的、不重复的字符串集合，支持交集、并集等操作。
   · Sorted Set（有序集合）: 类似于Set，但每个成员都关联一个分数（Score），可以根据分数进行排序。非常适合排行榜、延时队列等场景。
   · 还有其他如 Bitmaps、HyperLogLogs、Streams 等高级结构。

4. 持久化 (Persistence):
   · 虽然数据主要在内存，但Redis提供了两种方式将数据写入硬盘，防止数据丢失：
     · RDB (Redis Database): 在指定时间间隔生成数据集的快照（Snapshot）。性能好，但可能会丢失最后一次快照之后的数据。
     · AOF (Append Only File): 记录每次写操作命令，并在重启时重新执行这些命令来恢复数据。数据更安全，但文件更大，恢复速度较慢。
   · 通常可以结合使用。

5. 其他特性:
   · 高性能:
   · 原子操作:
   · 发布/订阅 (Pub/Sub):
   · Lua 脚本:
   · 事务:
   · 高可用和分区（哨兵和集群）:



### 如何使用Redis作为服务器在内存中的缓冲区？

“缓冲区”在这里通常指的是缓存（Cache），其核心目的是通过将频繁访问的数据放在快速的内存中，减少对慢速后端（如主数据库）的访问，从而显著提升应用性能、降低后端负载。

以下是将其用作缓冲区的典型步骤和最佳实践：

1. 核心工作模式：缓存模式 (Cache Pattern)

最常见的模式是 “旁路缓存 (Cache-Aside)” 或 “懒加载 (Lazy Loading)”。流程如下：

· 读取数据 (Read):
  1. 应用程序收到读取请求（例如，获取商品信息）。
  2. 应用程序首先查询Redis，看看数据是否存在。
  3. 如果存在（缓存命中，Cache Hit），直接从Redis返回数据，流程结束。速度极快。
  4. 如果不存在（缓存未命中，Cache Miss），应用程序去查询主数据库（如MySQL）。
  5. 从主数据库取得数据后，将数据写入Redis（为下一次请求做准备），然后返回数据。
  伪代码示例（读取）：
  ```python
  def get_product(product_id):
      # 1. 先尝试从Redis缓存获取
      data = redis.get(f"product:{product_id}")
      if data is not None:
          return deserialize(data)  # 缓存命中
  
      # 2. 缓存未命中，从数据库获取
      data = db.query("SELECT * FROM products WHERE id = ?", product_id)
      if data is None:
          return None
  
      # 3. 将数据写入缓存，设置过期时间（例如3600秒）
      redis.setex(f"product:{product_id}", 3600, serialize(data))
      return data
  ```
· 写入数据 (Write):
  6. 应用程序更新或创建数据。
  7. 应用程序首先写入主数据库，确保数据最终落盘。
  8. 然后，使Redis中对应的缓存数据失效（删除Redis中的键）。这是关键！这可以防止数据库和缓存出现数据不一致。
  9. 下一次读取请求时，自然会触发“缓存未命中”，然后从数据库加载最新数据到缓存。
  伪代码示例（写入）：
  ```python
  def update_product(product_id, new_data):
      # 1. 先更新主数据库
      db.execute("UPDATE products SET ... WHERE id = ?", product_id, new_data)
  
      # 2. 然后删除（使失效）Redis中的缓存
      redis.delete(f"product:{product_id}")
  ```

10. 关键配置与最佳实践

· 设置过期时间 (Expiration/TTL):
  · 一定要为缓存键设置一个过期时间（例如 SETEX 命令或 EXPIRE 命令）。
  · 这可以防止数据永久占用内存，即使在上面的“写入失效”逻辑有BUG时，数据最终也会自动消失，然后从数据库加载新的。这是一种安全网。
· 缓存淘汰策略 (Eviction Policies):
  · 当内存耗尽时，Redis需要根据策略删除一些键。maxmemory-policy 配置项至关重要。
  · 常用策略：allkeys-lru（从所有键中淘汰最近最少使用的）或 volatile-lru（只从设置了过期时间的键中淘汰LRU）。这确保了最常用的数据留在内存中。
· 处理“缓存穿透” (Cache Penetration):
  · 问题：频繁查询一个数据库中根本不存在的数据（如不存在的用户ID），每次都会穿透缓存打到数据库。
  · 方案：即使查询不到数据，也在Redis缓存一个空值或特殊标记，并设置一个较短的过期时间。
· 处理“缓存击穿” (Cache Breakdown):
  · 问题：某个热点键在过期瞬间，有大量请求同时到来，全部未命中，同时去访问数据库。
  · 方案：使用互斥锁（Mutex Lock）。第一个请求未命中时，加锁去数据库加载数据，其他请求等待锁释放后直接从缓存读取。Redis的 SETNX 命令可用于实现分布式锁。
· 处理“缓存雪崩” (Cache Avalanche):
  · 问题：大量缓存键在同一时间点过期，导致所有对应请求都穿透到数据库。
  · 方案：为缓存过期时间设置一个随机波动值（例如，基础3600秒，随机加上0-300秒），避免同时失效。

3. 架构示意图

```
+-------------+    Cache Miss    +------------+
|             |----------------->|            |
| Application |                  |  Database  |
|             |<-----------------|  (MySQL)   |
+-------------+   Write/Result   +------------+
       |  ^
       |  | Cache Hit
       |  |
       |  |  Read/Write/Delete
       v  |
    +------------+
    |   Redis    |
    | (Buffer)   |
    +------------+
```

### 数据结构的特点和选择

Redis 提供了丰富的数据结构（string、hash、list、set、zset、stream 等），不同业务场景要选择不同的数据结构，否则要么浪费内存，要么写查询逻辑很复杂。


#### 选择 Redis 数据结构的思路

1. **数据是单值还是多字段？**
    - 单个简单值 → `string`
    - 有多个字段（类似 JSON/对象） → `hash`

2. **是否需要顺序？**
    - 按插入顺序访问 → `list`
    - 按排序规则访问 → `zset`（有序集合）

3. **是否需要去重？**
    - 集合运算（交集/并集/差集） → `set`
    - 允许重复 + 排序 → `zset`

4. **是否是日志/消息流？**
    - 持续追加、消费 → `stream` 或 `list`

5. **是否需要 TTL（过期）？**
    - 所有结构都可以设置 key 的过期时间。



#### 1. `string`（最常用，KV 存储）

- **适合场景**：
    - 缓存单个值（如 token、配置项、用户分数）。
    - 分布式锁（setnx）。

- **示例**：
```bash
set user:1000:name "Alice"
get user:1000:name
```


#### 2. `hash`（存储对象）

- **适合场景**：
    - 需要存储一个对象/字典，多字段读写。
    - 用户资料、会话信息。

- **示例**：
```bash
hset user:1000 name "Alice" age 18
hget user:1000 name
hgetall user:1000
```


#### 3. `list`（有序链表）

- **适合场景**：
    - 消息队列（简单版，配合 `lpush/rpop`）。
    - 按时间顺序保存数据（如日志）。

- **示例**：
```bash
lpush msg:queue "task1" "task2"
rpop msg:queue
```

#### 4. `set`（无序去重集合）

- **适合场景**：
    - 去重存储（如活跃用户 ID）。
    - 需要集合运算（交集、并集）。

- **示例**：
```bash
sadd active:users 1000 1001
sismember active:users 1000
smembers active:users
```

#### 5. `zset`（有序集合）

- **适合场景**：
    - 排行榜（按分数排序）。
    - 按时间戳做滑动窗口统计。

- **示例**：
```bash
zadd leaderboard 100 "user:1000"
zrange leaderboard 0 -1 withscores
```

#### 6. `stream`（日志流）

- **适合场景**：
    - 高可靠消息队列。
    - 多消费者订阅、消费数据。
- **示例**：
```bash
xadd events * user 1000 action "login"
xread block 0 streams events $
```


### 总结
使用Redis作为内存缓冲区（缓存）是一个非常有效的性能优化手段。其核心流程是：

1. 读请求，先查缓存，命中则返回，未命中则查数据库并回填缓存。
2. 写请求，先更新数据库，然后使对应的缓存失效。
3. 务必为缓存设置合理的过期时间和内存淘汰策略。
4. 了解并防范缓存穿透、击穿、雪崩等经典问题。


## 搭建一个redis单机服务器
```bash
docker pull redis:8.2.1
```


```conf
################################
# 基础配置
################################

允许所有机器访问
bind 0.0.0.0

# 监听端口
port 6379

# 关闭保护模式
protected-mode no

# 数据目录（docker-compose 里挂载到宿主机）
dir /data

# 开启密码访问
requirepass balabala

################################
# 日志配置
################################

# 日志级别：debug | verbose | notice | warning
# loglevel debug

# 日志文件，空字符串表示输出到 stdout
# logfile ""

# 开启键过期事件通知（调试时能看到 key 过期）
# notify-keyspace-events Ex

################################
# 持久化配置
################################

# 开启 AOF 持久化（调试时能保证数据不丢）
appendonly yes
appendfsync everysec

################################
# 慢查询日志配置
################################

# 记录所有命令（单位：微秒，0 表示所有命令都会被记录）
slowlog-log-slower-than 50

# 最多保留多少条慢查询日志
slowlog-max-len 1024
```

### 