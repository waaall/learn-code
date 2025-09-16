模块化高性能推理服务器，使用 Gunicorn + FastAPI + HTTPX + ONNX Runtime 架构，并包含完整的日志配置。

## 项目结构

```
model_server/
├── app/
│   ├── __init__.py
│   ├── main.py              # FastAPI 应用主文件
│   ├── dependencies.py      # 依赖注入模块
│   ├── models.py           # 数据模型定义
│   ├── routers/            # 路由模块
│   │   ├── __init__.py
│   │   ├── predict.py      # 推理路由
│   │   └── health.py       # 健康检查路由
│   ├── services/           # 服务层
│   │   ├── __init__.py
│   │   ├── model_service.py # 模型服务
│   │   └── cache_service.py # 缓存服务
│   └── core/               # 核心配置
│       ├── __init__.py
│       ├── config.py       # 应用配置
│       ├── logging.py      # 日志配置
│       └── events.py       # 启动/关闭事件处理
├── requirements.txt
├── gunicorn_conf.py        # Gunicorn 配置
└── Dockerfile              # Docker 配置
```

### 安装依赖 (requirements.txt)

```txt
fastapi==0.104.1
uvicorn[standard]==0.24.0
gunicorn==21.2.0
onnxruntime==1.16.2
httpx==0.25.2
redis==5.0.1
python-multipart==0.0.6
python-jose==3.3.0
passlib==1.7.4
python-multipart==0.0.6
orjson==3.9.10
cachetools==5.3.2
python-dotenv==1.0.0
pydantic-settings==2.1.0
pydantic==2.5.0
numpy==1.24.4
pandas==2.0.3
pillow==10.1.0
```

## 核心配置模块

### app/core/config.py

```python
import os
from typing import List, Optional
from pydantic import AnyUrl, field_validator
from pydantic_settings import BaseSettings
from dotenv import load_dotenv

load_dotenv()  # 加载环境变量

class Settings(BaseSettings):
    # 应用配置
    PROJECT_NAME: str = "ML Model Server"
    VERSION: str = "1.0.0"
    API_PREFIX: str = "/api"
    DEBUG: bool = False
    
    # 服务器配置
    HOST: str = "0.0.0.0"
    PORT: int = 8000
    WORKERS: int = 4
    LOG_LEVEL: str = "info"
    
    # 模型配置
    MODEL_DIR: str = "models"
    MODEL_NAMES: List[str] = ["model1", "model2"]  # 你的模型名称列表
    
    # Redis配置
    REDIS_URL: str = "redis://localhost:6379/0"
    CACHE_ENABLED: bool = True
    CACHE_TTL: int = 300  # 缓存时间(秒)
    
    # 外部API配置
    EXTERNAL_API_URL: str = "https://api.example.com/predict"
    EXTERNAL_API_TIMEOUT: int = 30
    
    # 安全配置
    SECRET_KEY: str = "your-secret-key-here"  # 生产环境请使用更安全的密钥
    ALGORITHM: str = "HS256"
    ACCESS_TOKEN_EXPIRE_MINUTES: int = 30
    
    class Config:
        case_sensitive = True
        env_file = ".env"

settings = Settings()
```

### app/core/logging.py

```python
import logging
import sys
from logging.config import dictConfig
from pathlib import Path

from app.core.config import settings

# 确保日志目录存在
LOG_DIR = Path("logs")
LOG_DIR.mkdir(exist_ok=True)

# 日志配置
def setup_logging():
    log_config = {
        "version": 1,
        "disable_existing_loggers": False,
        "formatters": {
            "default": {
                "format": "%(asctime)s - %(name)s - %(levelname)s - %(message)s",
            },
            "access": {
                "format": "%(asctime)s - %(name)s - %(levelname)s - %(client_addr)s - %(request_line)s %(status_code)s",
            },
        },
        "handlers": {
            "default": {
                "formatter": "default",
                "class": "logging.StreamHandler",
                "stream": sys.stdout,
            },
            "file": {
                "formatter": "default",
                "class": "logging.handlers.RotatingFileHandler",
                "filename": LOG_DIR / "app.log",
                "maxBytes": 10485760,  # 10MB
                "backupCount": 5,
                "encoding": "utf8",
            },
            "access": {
                "formatter": "access",
                "class": "logging.StreamHandler",
                "stream": sys.stdout,
            },
            "access_file": {
                "formatter": "access",
                "class": "logging.handlers.RotatingFileHandler",
                "filename": LOG_DIR / "access.log",
                "maxBytes": 10485760,  # 10MB
                "backupCount": 5,
                "encoding": "utf8",
            },
            "error_file": {
                "formatter": "default",
                "class": "logging.handlers.RotatingFileHandler",
                "filename": LOG_DIR / "error.log",
                "maxBytes": 10485760,  # 10MB
                "backupCount": 5,
                "encoding": "utf8",
                "level": "ERROR",
            },
        },
        "loggers": {
            "": {  # 根日志记录器
                "handlers": ["default", "file", "error_file"],
                "level": settings.LOG_LEVEL.upper(),
                "propagate": False,
            },
            "uvicorn.error": {
                "level": "INFO",
                "handlers": ["default", "file"],
                "propagate": False,
            },
            "uvicorn.access": {
                "level": "INFO",
                "handlers": ["access", "access_file"],
                "propagate": False,
            },
            "app": {
                "handlers": ["default", "file", "error_file"],
                "level": settings.LOG_LEVEL.upper(),
                "propagate": False,
            },
        },
    }
    
    dictConfig(log_config)
    logger = logging.getLogger("app")
    logger.info("Logging configured successfully")
    
    return logger

# 创建应用日志记录器
logger = setup_logging()
```

### app/core/events.py

```python
import asyncio
import logging
from contextlib import asynccontextmanager

import onnxruntime as ort
from redis import asyncio as aioredis
import httpx

from app.core.config import settings
from app.core.logging import logger
from app.services.model_service import ModelService
from app.services.cache_service import CacheService

# 全局服务实例
model_service = None
cache_service = None
http_client = None

@asynccontextmanager
async def lifespan(app):
    # 启动事件
    global model_service, cache_service, http_client
    
    logger.info("Starting application...")
    
    try:
        # 初始化HTTP客户端
        http_client = httpx.AsyncClient(
            timeout=settings.EXTERNAL_API_TIMEOUT,
            limits=httpx.Limits(max_keepalive_connections=20, max_connections=100)
        )
        logger.info("HTTP client initialized")
        
        # 初始化缓存服务
        cache_service = CacheService(settings.REDIS_URL)
        await cache_service.initialize()
        logger.info("Cache service initialized")
        
        # 初始化模型服务
        model_service = ModelService(settings.MODEL_DIR, settings.MODEL_NAMES)
        await model_service.load_models()
        logger.info("Model service initialized")
        
        yield
        
    except Exception as e:
        logger.error(f"Error during startup: {e}")
        raise
    
    finally:
        # 关闭事件
        logger.info("Shutting down application...")
        
        # 关闭HTTP客户端
        if http_client:
            await http_client.aclose()
            logger.info("HTTP client closed")
        
        # 关闭缓存服务
        if cache_service:
            await cache_service.close()
            logger.info("Cache service closed")
        
        # 清理模型服务
        if model_service:
            model_service.unload_models()
            logger.info("Model service unloaded")
```

## 服务层模块

### app/services/cache_service.py

```python
import logging
from typing import Optional, Any
import orjson
from redis import asyncio as aioredis

from app.core.config import settings
from app.core.logging import logger

class CacheService:
    def __init__(self, redis_url: str):
        self.redis_url = redis_url
        self.client: Optional[aioredis.Redis] = None
    
    async def initialize(self):
        """初始化Redis连接"""
        try:
            self.client = aioredis.from_url(
                self.redis_url,
                encoding="utf-8",
                decode_responses=False,
                max_connections=20
            )
            await self.client.ping()
            logger.info("Redis connection established")
        except Exception as e:
            logger.error(f"Failed to connect to Redis: {e}")
            self.client = None
    
    async def close(self):
        """关闭Redis连接"""
        if self.client:
            await self.client.close()
            logger.info("Redis connection closed")
    
    async def get(self, key: str) -> Optional[Any]:
        """从缓存获取数据"""
        if not self.client or not settings.CACHE_ENABLED:
            return None
        
        try:
            data = await self.client.get(key)
            if data:
                return orjson.loads(data)
        except Exception as e:
            logger.error(f"Error getting cache for key {key}: {e}")
        return None
    
    async def set(self, key: str, value: Any, expire: int = None) -> bool:
        """设置缓存数据"""
        if not self.client or not settings.CACHE_ENABLED:
            return False
        
        try:
            expire = expire or settings.CACHE_TTL
            data = orjson.dumps(value)
            await self.client.setex(key, expire, data)
            return True
        except Exception as e:
            logger.error(f"Error setting cache for key {key}: {e}")
            return False
    
    async def delete(self, key: str) -> bool:
        """删除缓存数据"""
        if not self.client:
            return False
        
        try:
            await self.client.delete(key)
            return True
        except Exception as e:
            logger.error(f"Error deleting cache for key {key}: {e}")
            return False
```

app/services/model_service.py

```python
import os
import onnxruntime as ort
import numpy as np
import logging
from typing import Dict, Optional
from threading import RLock

from app.core.config import settings
from app.core.logging import logger


class ModelService:
    def __init__(self, model_dir: str, max_cache_size: int = 5):
        """
        :param model_dir: 模型文件所在目录
        :param max_cache_size: 最多常驻的模型数量（LRU 缓存）
        """
        self.model_dir = model_dir
        self.max_cache_size = max_cache_size
        self.models: Dict[str, ort.InferenceSession] = {}
        self.model_usage: Dict[str, int] = {}  # 用于简单的 LRU
        self.lock = RLock()

    def _make_session(self, model_path: str) -> ort.InferenceSession:
        """创建 ORT session"""
        session_options = ort.SessionOptions()
        session_options.intra_op_num_threads = 0  # 0 = 让 ORT 自适应
        session_options.inter_op_num_threads = 0
        session_options.graph_optimization_level = ort.GraphOptimizationLevel.ORT_ENABLE_ALL

        logger.info(f"Loading model: {model_path}")
        return ort.InferenceSession(
            model_path,   sess_options=session_options,
            providers=["CPUExecutionProvider"]  # 如果有 GPU，可以改成 ["CUDAExecutionProvider", "CPUExecutionProvider"]
        )

    def load_model(self, model_name: str) -> Optional[ort.InferenceSession]:
        """加载模型（带缓存机制）"""
        with self.lock:
            if model_name in self.models:             self.model_usage[model_name] += 1
                return self.models[model_name]
            model_path = os.path.join(self.model_dir, f"{model_name}.onnx")
            if not os.path.exists(model_path):               logger.error(f"Model file not found: {model_path}")
                return None
            if len(self.models) >= self.max_cache_size:
                # 简单的 LRU: 移除最少使用的模型
                to_remove = min(self.model_usage, key=self.model_usage.get)
                logger.info(f"Unloading model from cache: {to_remove}")
                self.models.pop(to_remove)
                self.model_usage.pop(to_remove)

            session = self._make_session(model_path)
            self.models[model_name] = session
            self.model_usage[model_name] = 1

            return session

    def get_model(self, model_name: str) -> Optional[ort.InferenceSession]:
        """获取已加载模型"""
        with self.lock:
            return self.models.get(model_name, None)

    async def predict(self, model_name: str, input_data: Dict[str, np.ndarray]) -> Optional[Dict[str, np.ndarray]]:
        """执行模型预测"""
        session = self.load_model(model_name)
        if not session:
            return None

        try:
            # ORT 内部自动调度线程，不需要 Python 线程池
            outputs = session.run(None, input_data)

            output_names = [output.name for output in session.get_outputs()]
            return {name: output for name, output in zip(output_names, outputs)}

        except Exception as e:
            logger.error(f"Error during prediction with model {model_name}: {e}")
            return None
```

## 路由模块

### app/routers/health.py

```python
from fastapi import APIRouter, Depends, HTTPException
from typing import Dict

from app.core.logging import logger
from app.services.model_service import ModelService
from app.services.cache_service import CacheService
from app.dependencies import get_model_service, get_cache_service

router = APIRouter(prefix="/health", tags=["health"])

@router.get("", response_model=Dict[str, str])
async def health_check(
    model_service: ModelService = Depends(get_model_service),
    cache_service: CacheService = Depends(get_cache_service)
):
    """健康检查端点"""
    try:
        # 检查模型服务状态
        models_loaded = len(model_service.get_available_models()) > 0
        
        # 检查缓存服务状态
        cache_connected = cache_service.client is not None
        
        status = "healthy" if models_loaded and cache_connected else "degraded"
        
        return {
            "status": status,
            "models_loaded": models_loaded,
            "cache_connected": cache_connected
        }
        
    except Exception as e:
        logger.error(f"Health check failed: {e}")
        raise HTTPException(status_code=503, detail="Service unavailable")
```

### app/routers/predict.py

```python
import time
import hashlib
from typing import Dict, List, Optional, Any
from fastapi import APIRouter, HTTPException, Depends, BackgroundTasks

from app.core.config import settings
from app.core.logging import logger
from app.services.model_service import ModelService
from app.services.cache_service import CacheService
from app.dependencies import get_model_service, get_cache_service

router = APIRouter(prefix="/inference", tags=["inference"])

def generate_cache_key(model_name: str, input_data: Dict) -> str:
    """生成缓存键"""
    data_str = f"{model_name}_{str(input_data)}"
    return hashlib.md5(data_str.encode()).hexdigest()

@router.post("/{model_name}")
async def predict(
    model_name: str,
    input_data: Dict[str, Any],
    background_tasks: BackgroundTasks,
    use_cache: bool = True,
    model_service: ModelService = Depends(get_model_service),
    cache_service: CacheService = Depends(get_cache_service)
):
    """执行模型预测"""
    start_time = time.time()
    
    # 检查模型是否存在
    if model_name not in model_service.get_available_models():
        raise HTTPException(status_code=404, detail=f"Model {model_name} not found")
    
    # 生成缓存键
    cache_key = generate_cache_key(model_name, input_data)
    
    # 尝试从缓存获取结果
    if use_cache:
        cached_result = await cache_service.get(cache_key)
        if cached_result:
            logger.info(f"Cache hit for model {model_name}")
            return {
                "result": cached_result,
                "cached": True,
                "processing_time": 0
            }
    
    # 执行预测
    try:
        # 预处理输入数据 (根据实际模型需求实现)
        processed_input = preprocess_input(input_data, model_name)
        
        # 运行模型推理
        result = await model_service.predict(model_name, processed_input)
        
        if result is None:
            raise HTTPException(status_code=500, detail="Prediction failed")
        
        # 后处理输出数据 (根据实际模型需求实现)
        processed_output = postprocess_output(result, model_name)
        
        # 计算处理时间
        processing_time = time.time() - start_time
        
        # 缓存结果
        if use_cache:
            background_tasks.add_task(
                cache_service.set, 
                cache_key, 
                processed_output, 
                settings.CACHE_TTL
            )
        
        return {
            "result": processed_output,
            "cached": False,
            "processing_time": processing_time
        }
        
    except Exception as e:
        logger.error(f"Prediction error for model {model_name}: {e}")
        raise HTTPException(status_code=500, detail=str(e))

@router.get("/models")
async def list_models(
    model_service: ModelService = Depends(get_model_service)
):
    """获取可用模型列表"""
    models = model_service.get_available_models()
    return {"models": models}

def preprocess_input(input_data: Dict, model_name: str) -> Dict[str, Any]:
    """预处理输入数据 (需要根据实际模型实现)"""
    # 这里应该根据具体模型实现数据预处理逻辑
    # 例如: 转换为numpy数组, 标准化, 调整形状等
    return input_data

def postprocess_output(output_data: Dict, model_name: str) -> Dict[str, Any]:
    """后处理输出数据 (需要根据实际模型实现)"""
    # 这里应该根据具体模型实现数据后处理逻辑
    # 例如: 转换为列表, 应用softmax, 提取关键信息等
    return output_data
```

## 依赖注入模块

### app/dependencies.py

```python
from fastapi import Depends, HTTPException, status
from typing import Annotated

from app.core.events import model_service, cache_service, http_client
from app.services.model_service import ModelService
from app.services.cache_service import CacheService
import httpx

def get_model_service() -> ModelService:
    """获取模型服务依赖"""
    if model_service is None:
        raise HTTPException(
            status_code=status.HTTP_503_SERVICE_UNAVAILABLE,
            detail="Model service not initialized"
        )
    return model_service

def get_cache_service() -> CacheService:
    """获取缓存服务依赖"""
    if cache_service is None:
        raise HTTPException(
            status_code=status.HTTP_503_SERVICE_UNAVAILABLE,
            detail="Cache service not initialized"
        )
    return cache_service

def get_http_client() -> httpx.AsyncClient:
    """获取HTTP客户端依赖"""
    if http_client is None:
        raise HTTPException(
            status_code=status.HTTP_503_SERVICE_UNAVAILABLE,
            detail="HTTP client not initialized"
        )
    return http_client
```

## 主应用模块

### app/main.py

```python
from fastapi import FastAPI
from fastapi.middleware.cors import CORSMiddleware

from app.core.config import settings
from app.core.logging import logger
from app.core.events import lifespan
from app.routers import health, inference

# 创建FastAPI应用
app = FastAPI(
    title=settings.PROJECT_NAME,
    version=settings.VERSION,
    debug=settings.DEBUG,
    lifespan=lifespan
)

# 添加CORS中间件
app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],  # 生产环境应该限制为特定域名
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

# 添加路由
app.include_router(health.router, prefix=settings.API_PREFIX)
app.include_router(inference.router, prefix=settings.API_PREFIX)

@app.get("/")
async def root():
    """根端点"""
    return {
        "message": "Welcome to ML Model Server",
        "version": settings.VERSION,
        "docs": "/docs"
    }

# 主程序入口
if __name__ == "__main__":
    import uvicorn
    uvicorn.run(
        "app.main:app",
        host=settings.HOST,
        port=settings.PORT,
        log_level=settings.LOG_LEVEL,
        reload=settings.DEBUG
    )
```

## Gunicorn 配置

### gunicorn_conf.py

```python
import multiprocessing
import os

from app.core.config import settings

# 服务器套接字
bind = f"{settings.HOST}:{settings.PORT}"

# 工作进程数
workers = settings.WORKERS
if workers == -1:
    workers = multiprocessing.cpu_count() * 2 + 1

# 工作进程类型
worker_class = "uvicorn.workers.UvicornWorker"

# 日志配置
loglevel = settings.LOG_LEVEL
accesslog = "logs/access.log"
errorlog = "logs/error.log"
capture_output = True

# 进程名称
proc_name = "model_server"

# 服务器钩子
def on_starting(server):
    """服务器启动时执行"""
    # 确保日志目录存在
    os.makedirs("logs", exist_ok=True)

def when_ready(server):
    """服务器准备就绪时执行"""
    pass

def on_exit(server):
    """服务器退出时执行"""
    pass
```

## Docker 配置

### Dockerfile

```dockerfile
FROM python:3.11-slim

WORKDIR /app

# 安装系统依赖
RUN apt-get update && apt-get install -y \
    gcc \
    && rm -rf /var/lib/apt/lists/*

# 复制依赖文件并安装
COPY requirements.txt .
RUN pip install --no-cache-dir -r requirements.txt

# 复制应用代码
COPY . .

# 创建日志目录
RUN mkdir -p logs

# 暴露端口
EXPOSE 8000

# 启动命令
CMD ["gunicorn", "-c", "gunicorn_conf.py", "app.main:app"]
```

## 使用说明

1. 安装依赖:
   ```bash
   pip install -r requirements.txt
   ```
2. 配置环境变量: 创建 .env 文件:
   ```
   # 应用配置
   DEBUG=False
   LOG_LEVEL=info
   
   # 服务器配置
   HOST=0.0.0.0
   PORT=8000
   WORKERS=4
   
   # 模型配置
   MODEL_DIR=models
   MODEL_NAMES=model1,model2
   
   # Redis配置
   REDIS_URL=redis://redis:6379/0
   CACHE_ENABLED=True
   CACHE_TTL=300
   
   # 外部API配置
   EXTERNAL_API_URL=https://api.example.com/predict
   EXTERNAL_API_TIMEOUT=30
   ```
3. 运行应用:
   · 开发模式:
     ```bash
     uvicorn app.main:app --reload --host 0.0.0.0 --port 8000
     ```
   · 生产模式:
     ```bash
     gunicorn -c gunicorn_conf.py app.main:app
     ```
4. 使用Docker运行:
   ```bash
   docker build -t model-server .
   docker run -p 8000:8000 --env-file .env model-server
   ```

## 性能优化建议

1. 模型优化:
   · 使用ONNX模型优化器优化模型
   · 考虑模型量化（FP16/INT8）以减少内存和提高速度
2. 缓存策略:
   · 根据请求模式调整缓存大小和TTL
   · 实现缓存预热机制
3. 并发处理:
   · 根据CPU核心数调整Gunicorn工作进程数
   · 使用异步处理提高I/O密集型操作的性能
4. 监控和日志:
   · 添加Prometheus指标端点
   · 实现分布式追踪
   · 设置日志轮转和归档策略

