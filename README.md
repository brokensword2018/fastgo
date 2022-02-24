### 使用方式
```
1. git clone https://github.com/brokensword2018/fastgo.git
2. 切换到fastgo目录 sudo ./install.sh
3. 配置 ~/.fastgo/path.txt 文件， 其中是可以搜索的目录。 
配置方式 <path level> 
path 表示可以搜索的目录 level表示该目录下几层子目录可以搜索配置文件，
配置文件中不要有~符号。 一行一个目录。
path.txt 例子如下。
/home/username  5
/data  4
4. fastgo -l  加载路径
```
使用go命令进行路径的跳转


### 依赖的库
- ftxui  通过cmake进行下载使用
- curl   项目子目录
- jsoncpp   项目子目录