
- [redis-github](https://github.com/redis/redis/)
- [redis为什么慢？](https://zhuanlan.zhihu.com/p/346302897)

## Redis简介

Redis（Remote Dictionary Server）是开源的内存键值数据库，常被用作数据库、缓存和消息代理。

几个关键特性：

1. 基于内存 (In-Memory)
   - 所有数据主要存储在 RAM 中，读写速度通常可达微秒级；传统磁盘数据库（如 MySQL）的读写速度一般在毫秒级。
   - 代价是内存成本高，单实例容量有限（除非使用集群）；为防止重启或宕机导致数据丢失，需结合持久化机制。

2. 键值存储 (Key-Value Store)
   - 数据以键值对形式存储，键为字符串。
   - 值支持多种数据结构，而不仅是字符串。

3. 丰富的数据结构 (Data Structures)
   - String：最基本类型，可存储文本、数字或二进制数据。
   - Hash：类似 Map/dict，适合存储对象（如用户信息：name、age、email）。
   - List：有序的字符串列表，支持两端插入或弹出，可实现栈或队列。
   - Set：无序且去重的字符串集合，支持交集、并集等操作。
   - Sorted Set：在集合元素上附带分数并按分数排序，适合排行榜、延时队列。
   - 其他：Bitmaps、HyperLogLog、Streams 等。

4. 持久化 (Persistence)
   - RDB (Redis Database)：按指定时间间隔生成数据集快照，性能好，但可能丢失最后一次快照后的数据。
   - AOF (Append Only File)：记录每次写操作并在重启时重放，数据更安全但文件更大、恢复速度较慢。
   - 可结合使用。

5. 其他特性
   - 高性能
   - 原子操作
   - 发布/订阅 (Pub/Sub)
   - Lua 脚本
   - 事务
   - 高可用与分区（哨兵和集群）

### 如何使用 Redis 作为服务器在内存中的缓冲区？

“缓冲区”通常指缓存（Cache），将频繁访问的数据放入内存以减少对后端（如主数据库）的访问，从而提升性能并降低负载。

以下是典型步骤和最佳实践：

1. 核心工作模式：旁路缓存（Cache-Aside/Lazy Loading）
   - 读取数据 (Read)
     1. 应用收到请求后先查询 Redis。
     2. 若命中（Cache Hit），直接返回。
     3. 若未命中（Cache Miss），查询主数据库（如 MySQL）。
     4. 从数据库取到数据后写入 Redis（设置过期时间），再返回结果。
   - 读取示例：
   ```python
   def get_product(product_id):
       data = redis.get(f"product:{product_id}")
       if data is not None:
           return deserialize(data)

       data = db.query("SELECT * FROM products WHERE id = ?", product_id)
       if data is None:
           return None

       redis.setex(f"product:{product_id}", 3600, serialize(data))
       return data
   ```
   - 写入数据 (Write)
     1. 先写主数据库，确保数据落盘。
     2. 删除或失效对应的 Redis 缓存键，避免数据不一致。
     3. 后续读取会触发未命中并回填最新数据。
   - 写入示例：
   ```python
   def update_product(product_id, new_data):
       db.execute("UPDATE products SET ... WHERE id = ?", product_id, new_data)
       redis.delete(f"product:{product_id}")
   ```

2. 关键配置与最佳实践
   - 设置过期时间：通过 `SETEX` 或 `EXPIRE` 给缓存键设置 TTL，防止占用内存并兜底处理失效逻辑异常。
   - 缓存淘汰策略：`maxmemory-policy` 常用 allkeys-lru 或 volatile-lru，确保常用数据留在内存。
   - 缓存穿透：对不存在的数据也写入一个空值或标记，并设置短 TTL。
   - 缓存击穿：热点键过期瞬间使用互斥锁（如 `SETNX` 实现分布式锁）串行加载。
   - 缓存雪崩：过期时间增加随机抖动（例如基础 3600 秒加 0-300 秒），避免集中失效。

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

Redis 提供了丰富的数据结构（string、hash、list、set、zset、stream 等），不同业务场景要选择合适的结构，否则会浪费内存或导致逻辑复杂。

#### 选择 Redis 数据结构的思路

1. 数据是单值还是多字段？
   - 单个简单值 -> `string`
   - 多字段（类似 JSON/对象） -> `hash`
2. 是否需要顺序？
   - 按插入顺序访问 -> `list`
   - 按排序规则访问 -> `zset`（有序集合）
3. 是否需要去重？
   - 集合运算（交集/并集/差集） -> `set`
   - 允许重复且需排序 -> `zset`
4. 是否是日志/消息流？
   - 持续追加、消费 -> `stream` 或 `list`
5. 是否需要 TTL（过期）？
   - 所有结构都可以为键设置过期时间。

#### 1. `string`（最常用，KV 存储）

- 适合场景：
  - 缓存单个值（如 token、配置项、用户分数）。
  - 分布式锁（`SETNX`）。
- 示例：
```bash
set user:1000:name "Alice"
get user:1000:name
```

#### 2. `hash`（存储对象）

- 适合场景：
  - 存储对象/字典，多字段读写。
  - 用户资料、会话信息。
- 示例：
```bash
hset user:1000 name "Alice" age 18
hget user:1000 name
hgetall user:1000
```

#### 3. `list`（有序链表）

- 适合场景：
  - 简单消息队列（`LPUSH/RPOP`）。
  - 按时间顺序保存数据（如日志）。
- 示例：
```bash
lpush msg:queue "task1" "task2"
rpop msg:queue
```

#### 4. `set`（无序去重集合）

- 适合场景：
  - 去重存储（如活跃用户 ID）。
  - 需要集合运算（交集、并集）。
- 示例：
```bash
sadd active:users 1000 1001
sismember active:users 1000
smembers active:users
```

#### 5. `zset`（有序集合）

- 适合场景：
  - 排行榜（按分数排序）。
  - 按时间戳做滑动窗口统计。
- 示例：
```bash
zadd leaderboard 100 "user:1000"
zrange leaderboard 0 -1 withscores
```

#### 6. `stream`（日志流）

- 适合场景：
  - 高可靠消息队列。
  - 多消费者订阅、消费数据。
- 示例：
```bash
xadd events * user 1000 action "login"
xread block 0 streams events $
```

### 总结

使用 Redis 作为内存缓冲区（缓存）的核心流程：

1. 读请求先查缓存，命中返回，未命中查询数据库并回填缓存。
2. 写请求先更新数据库，再使对应缓存失效。
3. 设置合理的过期时间和内存淘汰策略。
4. 防范缓存穿透、击穿、雪崩等问题。

## 搭建一个 redis 单机服务器

```bash
docker pull redis:8.2.1
```

```conf
################################
# 基础配置
################################

# 允许所有机器访问
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
