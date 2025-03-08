# 安装数据库
```bash
sudo apt-get install postgresql

# 使用 Homebrew 安装
brew install postgresql

# 启动 PostgreSQL 服务
brew services start postgresql
```

# 创建数据库

```
# 创建新数据库
createdb mydb

# 连接到数据库
psql mydb
```
# 基本操作

```
-- 创建表
CREATE TABLE users (
    id SERIAL PRIMARY KEY,
    name VARCHAR(100) NOT NULL,
    email VARCHAR(100) UNIQUE NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- 插入数据
INSERT INTO users (name, email) VALUES ('张三', 'zhangsan@example.com');

-- 查询数据
SELECT * FROM users WHERE name = '张三';

-- 更新数据
UPDATE users SET email = 'new_email@example.com' WHERE name = '张三';

-- 删除数据
DELETE FROM users WHERE name = '张三';

```
