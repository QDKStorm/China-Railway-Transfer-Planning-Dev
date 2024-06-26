<div align="center">
  <p>
      <img width="75%" src="./theme_picture.png" alt="YOLO Vision banner"></a>
  </p>
</div>

## <div align="center"> China Railway Transfer Planning</div>

China Railway Transfer Planning是为了解决12306官方应用查询中转时查询结果不全、方案不优、响应速度慢等问题而制作的应用程序。它基于离线的数据，利用图论方法求解路径，并提供了按时间筛选、同城换乘、夜发昼达等定制功能。

本仓库是CRTP的算法测试仓库，主要包含算法与数据相关的文件。此外，你还可以在[China-Railway-Transfer-Planning](https://github.com/QDKStorm/China-Railway-Transfer-Planning)仓库中看到这个项目图形化的文件，CRTP的应用本地也在该仓库中。

项目会定期更新最新的离线数据，但不会提供数据的来源。

项目还在进步中，欢迎提出建议！

### 用法

仓库中的`cpp`文件在C++14下正常编译运行，`py`文件在`python=3.9.18`下正常运行，主要依赖的库的版本如下：

```
tqdm==4.66.2
pandas==2.2.1
numpy==1.26.4
```

### 文件结构

- `BFS.cpp`和`BFS2.cpp`：寻路算法的源码，分别是两种不同的建图方案
- `city_list.txt`：车站列表
- `crawler.py`和`crawler.ipynb`：开发初期使用的爬虫文件，**并不是12306的爬虫**，已经弃用

- `data_process.ipynb`：预处理数据用的所有代码
- `station_id.json`：车站与其编号之间的对应关系，已经弃用
- `test_time.txt`和`train_time.txt`：分别是测试用和正式的列车时刻表

### 注意

- 本项目不提供12306官网的爬虫程序
