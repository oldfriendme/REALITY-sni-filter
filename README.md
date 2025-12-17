# REALITY-sni-filter

#### 适用与REALITY协议的高效sni过滤器。
以替代默认的[dokodemo-door](https://github.com/XTLS/Xray-examples/tree/main/VLESS-TCP-REALITY%20(without%20being%20stolen)) 的双重sniffing 带来的性能高损耗。

xray服务端配置文件参考xray_server_simple.json

<br>

### Install
```
make
```

### Usage
```
sni-filter -L=tcp://0.0.0.0:443 -F=tcp://127.0.0.1:443 -S=www.example.com
sni-filter -L=tcp://0.0.0.0:443 -F=unix:///run/unix.socket -S=www.example.com
```

### 功能
可过滤不匹配sni的REALITY数据,解决源站流量通过cdn被偷跑问题。

### 开发计划
没bug的话就写成一键脚本了.
