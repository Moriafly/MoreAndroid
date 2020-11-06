

# SSH 远程登录手册

SSH ( Secure Shell ) 用于远程登录云服务器 (Elastic Compute Service, 简称 *ECS*)

## Windows 平台安装 OpenSSH
简单安装方式：

1. 系统要求：Windows 10 1809 及以上版本
2. 打开设置，选择应用，点击应用和功能项中按钮【可选功能】，添加功能，安装 OpenSSH 服务器![image-20200927093928002](C:\Users\24568\AppData\Roaming\Typora\typora-user-images\image-20200927093928002.png)
参考：https://docs.microsoft.com/zh-cn/windows-server/administration/openssh/openssh_install_firstuse

Linux 平台（一般自带 OpenSSH）

## 远程登录

以管理员身份运行 Windows PowerShell（或者其他终端，如 Windwos Terminal），输入
```
Add-WindowsCapability -Online -Name OpenSSH-Client
```
如果出现
```
Path          :
Online        : True
RestartNeeded : False
```
则表明 SSH 可用
```
ssh root@47.103.83.146 -p 22
```
说明
```
root 远程服务器用户名，可替换成其他
@ 后面 ip 是远程服务器的公网 ip
-p 22 是默认的，可以不填
```
如有提问，则输入 yes，再输入密码
登录成功
```
欢迎使用 Alibaba Cloud Linux 2 等保合规镜像
```
# OpenVPN 服务端的搭建

| 主机           | 操作系统              | 角色           |
| -------------- | --------------------- | -------------- |
| 阿里云服务器   | Alibaba Cloud Linux 2 | OpenVPN 服务器 |
| 本地笔记本电脑 | Windows 10            | OpenVPN 客户端 |

## 安装 OpenVPN 和 easy-rsa

安装 EPEL(Extra Packages for Enterprise Linux)

```bash
sudo yum install epel-release
```

若 extras 源未启用，则

```
sudo yum --enablerepo=extras install epel-release
```

检查 EPEL 源是否安装成功

```
sudo yum repolist
```

![image-20201016104205880](C:\Users\24568\Documents\GitHub\mqtt-learning\image-20201016104205880.png)

安装 openvpn 2.4.9 版本

```bash
sudo yum install openvpn
```

安装 easy-rsa 3.0.8 版本

easy-rsa 主要用于生成证书，服务端和客户端证书

```bash
sudo yum install easy-rsa
```

## 配置 vars 文件

进入 easy-rsa/3.0.8 文件夹

```
/usr/share/easy-rsa/3.0.8
```
下面有三个
```
easyrsa  openssl-easyrsa.cnf  x509-types
```

第一个 easyrsa 是命令

复制 vars.example 文件到 3.0.8 下 vars（自动新建的）

```
cp /usr/share/doc/easy-rsa-3.0.8/vars.example vars
```

编辑 vars

```
vim vars
```
修改其中的某些选项（不改也可以）


命令
## 初始化 pki
```bash
./easyrsa init-pki
```
提示：

```bash
Note: using Easy-RSA configuration from: /usr/share/easy-rsa/3.0.8/vars

init-pki complete; you may now create a CA or requests.
Your newly created PKI dir is: /usr/share/easy-rsa/3.0.8/pki
```

## 创建 ca

```
./easyrsa build-ca
```

提示：

```
Note: using Easy-RSA configuration from: /usr/share/easy-rsa/3.0.8/vars
Using SSL: openssl OpenSSL 1.0.2k-fips  26 Jan 2017

Enter New CA Key Passphrase:
Re-Enter New CA Key Passphrase:
Generating RSA private key, 2048 bit long modulus
..................+++
...........................+++
e is 65537 (0x10001)
You are about to be asked to enter information that will be incorporated
into your certificate request.
What you are about to enter is what is called a Distinguished Name or a DN.
There are quite a few fields but you can leave some blank
For some fields there will be a default value,
If you enter '.', the field will be left blank.
-----
Common Name (eg: your user, host, or server name) [Easy-RSA CA]:

CA creation complete and you may now import and sign cert requests.
Your new CA certificate file for publishing is at:
/usr/share/easy-rsa/3.0.8/pki/ca.crt
```

需要给 ca 文件设置密码（必须）

## 创建服务端

```
./easyrsa gen-req server nopass
```

提示：

```
Note: using Easy-RSA configuration from: /usr/share/easy-rsa/3.0.8/vars
Using SSL: openssl OpenSSL 1.0.2k-fips  26 Jan 2017
Generating a 2048 bit RSA private key
.......+++
.....+++
writing new private key to '/usr/share/easy-rsa/3.0.8/pki/easy-rsa-3572.62QGhX/tmp.yLVXua'
-----
You are about to be asked to enter information that will be incorporated
into your certificate request.
What you are about to enter is what is called a Distinguished Name or a DN.
There are quite a few fields but you can leave some blank
For some fields there will be a default value,
If you enter '.', the field will be left blank.
-----
Common Name (eg: your user, host, or server name) [server]:

Keypair and certificate request completed. Your files are:
req: /usr/share/easy-rsa/3.0.8/pki/reqs/server.req
key: /usr/share/easy-rsa/3.0.8/pki/private/server.key
```

## 服务端签名

```
./easyrsa sign server server
```

提示：

```
Note: using Easy-RSA configuration from: /usr/share/easy-rsa/3.0.8/vars
Using SSL: openssl OpenSSL 1.0.2k-fips  26 Jan 2017


You are about to sign the following certificate.
Please check over the details shown below for accuracy. Note that this request
has not been cryptographically verified. Please be sure it came from a trusted
source or that you have verified the request checksum with the sender.

Request subject, to be signed as a server certificate for 825 days:

subject=
    commonName                = server


Type the word 'yes' to continue, or any other input to abort.
  Confirm request details: yes
Using configuration from /usr/share/easy-rsa/3.0.8/pki/easy-rsa-3599.JlUmcU/tmp.VwivPG
Enter pass phrase for /usr/share/easy-rsa/3.0.8/pki/private/ca.key:
Check that the request matches the signature
Signature ok
The Subject's Distinguished Name is as follows
commonName            :ASN.1 12:'server'
Certificate is to be certified until Feb  9 01:15:33 2023 GMT (825 days)

Write out database with 1 new entries
Data Base Updated

Certificate created at: /usr/share/easy-rsa/3.0.8/pki/issued/server.crt
```

## 创建 dh 文件

```
./easyrsa gen-dh
```

比较漫长。

提示：

```
DH parameters of size 2048 created at /usr/share/easy-rsa/3.0.8/pki/dh.pem
```

## 创建客户端

```
./easyrsa gen-req client nopass
```

提示：

```
Note: using Easy-RSA configuration from: /usr/share/easy-rsa/3.0.8/vars
Using SSL: openssl OpenSSL 1.0.2k-fips  26 Jan 2017
Generating a 2048 bit RSA private key
...................+++
...............+++
writing new private key to '/usr/share/easy-rsa/3.0.8/pki/easy-rsa-3687.g8xLKm/tmp.KkmFBT'
-----
You are about to be asked to enter information that will be incorporated
into your certificate request.
What you are about to enter is what is called a Distinguished Name or a DN.
There are quite a few fields but you can leave some blank
For some fields there will be a default value,
If you enter '.', the field will be left blank.
-----
Common Name (eg: your user, host, or server name) [client]:

Keypair and certificate request completed. Your files are:
req: /usr/share/easy-rsa/3.0.8/pki/reqs/client.req
key: /usr/share/easy-rsa/3.0.8/pki/private/client.key
```

## 客户端签名

```
./easyrsa sign client client
```

## OpenVPN 配置

复制创建好的文件到 /etc/openvpn/keys 目录下

`keys 目录是自己创建的，用来存放相关 key`

```bash
 cp /usr/share/easy-rsa/3.0.8/pki/ca.crt keys/ # 复制 ca 文件
 cp /usr/share/easy-rsa/3.0.8/pki/dh.pem keys/ # 复制 dh 文件
 cp /usr/share/easy-rsa/3.0.8/pki/private/server.key keys/ # 复制 server.key
 cp /usr/share/easy-rsa/3.0.8/pki/issued/server.crt keys/ # 复制 server.crt
 cp /usr/share/easy-rsa/3.0.8/pki/private/client.key keys/ # 复制 client.key
 
```





# OpenVPN 服务端部署

创建 PKI（公钥基础设施）

```
./easyrsa init-pki
```

创建 CA（第三方证书）

```
./easyrsa build-ca
```

1. 创建时nopass表示不加密 就不会提示Enter PEM pass phrase
2. 例：[root@check1 3.0.3]# ./easyrsa build-ca nopass



```bash
cp /etc/openvpn/easy-rsa/3.0.8/pki/ca.crt /etc/openvpn/server/
cp /etc/openvpn/easy-rsa/3.0.8/pki/issued/server.crt /etc/openvpn/server/
cp /etc/openvpn/easy-rsa/3.0.8/pki/private/server.key /etc/openvpn/server/
cp /etc/openvpn/easy-rsa/3.0.8/pki/dh.pem /etc/openvpn/server/
cp /etc/openvpn/easy-rsa/3.0.8/pki/ta.key /etc/openvpn/server/
```



- 配置证书密钥

  ```bash
  cd /etc/openvpn/easy-rsa/
  ./easyrsa init-pki
  ./easyrsa build-ca nopass
  ./easyrsa build-server-full server nopass
  ./easyrsa build-client-full client1 nopass
  ./easyrsa build-client-full client2 nopass
  ./easyrsa gen-dh
  openvpn --genkey --secret ta.key
  ```

![image-20201016110053615](C:\Users\24568\Documents\GitHub\mqtt-learning\image-20201016110053615.png)

创建服务端证书

![image-20201016110202904](C:\Users\24568\Documents\GitHub\mqtt-learning\image-20201016110202904.png)

签约服务端证书

然后输入build-ca时设置的那个密码

\#输入yes确认信息

![image-20201016110326148](C:\Users\24568\Documents\GitHub\mqtt-learning\image-20201016110326148.png)

创建  Diffie-Hellman，确保 key 穿越不安全网络的命令

`时间有点长`

![image-20201016110443199](C:\Users\24568\Documents\GitHub\mqtt-learning\image-20201016110443199.png)

common name:

```
myName
```



# 创建客户端证书

第一步：进入root目录新建client文件夹，文件夹可随意命名，然后拷贝前面解压得到的easy-ras文件夹到client文件夹,进入下列目录

```bash
# mkdir client
# cp /etc/openvpn/easy-rsa client/
# cd client/easy-rsa/easyrsa3/
```

第二步：初始化

[root@along easyrsa3]# ./easyrsa init-pki //需输入yes 确定

③ 创建客户端key及生成证书（记住生成是自己客户端登录输入的密码）

[root@along easyrsa3]# ./easyrsa gen-req along //名字自己定义

![img](https://bbsmax.ikafan.com/static/L3Byb3h5L2h0dHBzL2ltYWdlczIwMTcuY25ibG9ncy5jb20vYmxvZy8xMjE2NDk2LzIwMTgwMS8xMjE2NDk2LTIwMTgwMTIzMTI1MTI0MTE1LTE3MTMzODI2NTMucG5n.jpg)

④ 将的到的qingliu.req导入然后签约证书

a. 进入到/etc/openvpn/easy-rsa/easyrsa3/

[root@along easyrsa3]# cd /etc/openvpn/easy-rsa/easyrsa3/

b. 导入req

[root@along easyrsa3]# ./easyrsa import-req /root/client/easy-rsa/easyrsa3/pki/reqs/along.req along

![img](https://bbsmax.ikafan.com/static/L3Byb3h5L2h0dHBzL2ltYWdlczIwMTcuY25ibG9ncy5jb20vYmxvZy8xMjE2NDk2LzIwMTgwMS8xMjE2NDk2LTIwMTgwMTIzMTI1MTI0Mzk3LTEzNTA4NzI5MzYucG5n.jpg)

c. 签约证书

[root@along easyrsa3]# ./easyrsa sign client along

![img](https://bbsmax.ikafan.com/static/L3Byb3h5L2h0dHBzL2ltYWdlczIwMTcuY25ibG9ncy5jb20vYmxvZy8xMjE2NDk2LzIwMTgwMS8xMjE2NDk2LTIwMTgwMTIzMTI1MTI0OTI4LTE1MzY2MDU5NTYucG5n.jpg)

//这里生成client所以必须为client，along要与之前导入名字一致

上面签约证书跟server类似，就不截图了，但是期间还是要输入CA的密码

### 5、把服务器端必要文件放到etc/openvpn/ 目录下

ca的证书、服务端的证书、秘钥

[root@along ~]# cp /etc/openvpn/easy-rsa/easyrsa3/pki/**ca.crt** /etc/openvpn/

[root@along ~]# cp /etc/openvpn/easy-rsa/easyrsa3/pki/private/**server.key** /etc/openvpn/



[root@along ~]# cp /etc/openvpn/easy-rsa/easyrsa3/pki/issued/**server.crt** /etc/openvpn/



[root@along ~]# cp /etc/openvpn/easy-rsa/easyrsa3/pki/**dh.pem** /etc/openvpn/

### 6、把客户端必要文件放到root/openvpn/ 目录下

客户端的证书、秘钥

[root@along ~]# cp /etc/openvpn/easy-rsa/easyrsa3/pki/ca.crt /root/client/

[root@along ~]# cp /etc/openvpn/easy-rsa/easyrsa3/pki/issued/along.crt /root/client/

[root@along ~]# cp /root/client/easy-rsa/easyrsa3/pki/private/along.key /root/client

### 7、为服务端编写配置文件

（1）当你安装好了openvpn时候，他会提供一个server配置的文件例子，在/usr/share/doc/openvpn-2.3.2/sample/sample-config-files 下会有一个server.conf文件，我们将这个文件复制到/etc/openvpn

[root@along ~]# rpm -ql openvpn |grep server.conf

![img](https://bbsmax.ikafan.com/static/L3Byb3h5L2h0dHBzL2ltYWdlczIwMTcuY25ibG9ncy5jb20vYmxvZy8xMjE2NDk2LzIwMTgwMS8xMjE2NDk2LTIwMTgwMTIzMTI1MTI1MTYyLTQ3NDI3MzE1My5wbmc=.jpg)

[root@along ~]# cp /usr/share/doc/openvpn-2.4.4/sample/sample-config-files/server.conf /etc/openvpn

（2）修改配置文件

[root@along ~]# vim /etc/openvpn/server.conf

[root@along ~]# grep '^[^#|;]' /etc/openvpn/server.conf 修改的地方如下：

```
local 0.0.0.0     #监听地址port 1194     #监听端口proto tcp     #监听协议dev tun     #采用路由隧道模式ca /etc/openvpn/ca.crt      #ca证书路径cert /etc/openvpn/server.crt       #服务器证书key /etc/openvpn/server.key  # This file should be kept secret 服务器秘钥dh /etc/openvpn/dh.pem     #密钥交换协议文件server 10.8.0.0 255.255.255.0     #给客户端分配地址池，注意：不能和VPN服务器内网网段有相同ifconfig-pool-persist ipp.txtpush "redirect-gateway def1 bypass-dhcp"      #给网关push "dhcp-option DNS 8.8.8.8"        #dhcp分配dnsclient-to-client       #客户端之间互相通信keepalive 10 120       #存活时间，10秒ping一次,120 如未收到响应则视为断线comp-lzo      #传输数据压缩max-clients 100     #最多允许 100 客户端连接user openvpn       #用户group openvpn      #用户组persist-keypersist-tunstatus /var/log/openvpn/openvpn-status.loglog         /var/log/openvpn/openvpn.logverb 3
```





每个项目都会由一大堆介绍,上述修改，openvpn提供的server.conf已经全部提供，我们只需要去掉前面的注释#，然后修改我们自己的有关配置

（3）配置后的设置

[root@along ~]# mkdir /var/log/openvpn

[root@along ~]# chown -R openvpn.openvpn /var/log/openvpn/

[root@along ~]# chown -R openvpn.openvpn /etc/openvpn/*

### 8、iptables 设置 nat 规则和打开路由转发

[root@along ~]# iptables -t nat -A POSTROUTING -s 10.8.0.0/24 -j MASQUERADE

[root@along ~]# iptables -vnL -t nat

[root@along ~]# vim /etc/sysctl.conf //打开路由转发

net.ipv4.ip_forward = 1

[root@along ~]# sysctl -p

### 9、开启openvpn 服务

[root@along ~]# openvpn /etc/openvpn/server.conf 开启服务

[root@along ~]# ss -nutl |grep 1194

![img](https://bbsmax.ikafan.com/static/L3Byb3h5L2h0dHBzL2ltYWdlczIwMTcuY25ibG9ncy5jb20vYmxvZy8xMjE2NDk2LzIwMTgwMS8xMjE2NDk2LTIwMTgwMTIzMTI1MTI1MzUwLTEzNDQwODY5NzIucG5n.jpg)

如果开启后没有打开1194 端口，说明开启服务失败，可能是配置文件有错，也有可能是权限不够，自己查询日志解决。

# OpenVPN 客户端部署 Windows 端

Windows 平台 V2.4.6 下载地址

https://swupdate.openvpn.org/community/releases/openvpn-install-2.4.6-I602.exe

安装记得勾选上 `EasyRSA 2 Certificate Management Scripts`



将之前存放服务器在 `/root/users/client1` 中的四个文件下载到客户端，存放在`C:\Program Files\OpenVPN\config` 目录下



```
client
dev tun
proto udp
remote 10.0.8.28 1194  # 主要这里修改成自己 server ip  端口
resolv-retry infinite
nobind
persist-key
persist-tun
ca ca.crt               # 这里需要证书 ca.crt
cert vpn.crt
key  vpn.key
auth md5
cipher AES-256-CBC
tls-auth ta.key 1
comp-lzo
verb 3
```



# 结果

![image-20201023153420626](C:\Users\24568\AppData\Roaming\Typora\typora-user-images\image-20201023153420626.png)

进入路径

```
cd /etc/openvpn/easy-rsa/3.0.8
```

#### CA 证书

3.0.8/pki/ca.crt

#### 服务端证书

3.0.8/pki/issued/server.crt

#### 客户端证书

3.0.8/pki/issued/client1.crt

#### 客户端秘钥

3.0.8/ta.key

#### DH 文件

3.0.8/pki/dh.pem

# 配置 vars

主要是证书相关

# 验证 CA 和 server.crt 文件正确性

![image-20201030151106397](C:\Users\24568\AppData\Roaming\Typora\typora-user-images\image-20201030151106397.png)

# 配置 server.conf

在 openvpn 目录下，新建

```
vim server.conf
```

配置：

```bash
# OpenVPN Port, Protocol and the Tun
port 1194
proto tcp
dev tun

# OpenVPN Server Certificate - CA, server key and certificate
ca /etc/openvpn/server/ca.crt
cert /etc/openvpn/server/hakase-server.crt
key /etc/openvpn/server/hakase-server.key

#DH and CRL key
dh /etc/openvpn/server/dh.pem
#注意本文没有跳过了丢消证书的检测
#crl-verify /etc/openvpn/server/crl.pem

# Network Configuration - Internal network
# Redirect all Connection through OpenVPN Server
server 10.10.1.0 255.255.255.0
push "redirect-gateway def1"

# Using the DNS from https://dns.watch
push "dhcp-option DNS 84.200.69.80"
push "dhcp-option DNS 84.200.70.40"

#Enable multiple client to connect with same Certificate key
duplicate-cn

# TLS Security
cipher AES-256-CBC
tls-version-min 1.2
tls-cipher TLS-DHE-RSA-WITH-AES-256-GCM-SHA384:TLS-DHE-RSA-WITH-AES-256-CBC-SHA256:TLS-DHE-RSA-WITH-AES-128-GCM-SHA256:TLS-DHE-RSA-WITH-AES-128-CBC-SHA256
auth SHA512
auth-nocache

# Other Configuration
keepalive 20 60
persist-key
persist-tun
comp-lzo yes
daemon
user nobody
group nobody

# OpenVPN Log
log-append /var/log/openvpn.log
verb 3
```



# 启动 Service

```
service openvpn restart
```





# 下载文件到 Windows 客户端

在 Windows 电脑创建一个文件夹（我这在 C 盘下创建了一个名为 vpn 的文件夹），打开 Windows PowerShell （不是进入服务器），输入

```
scp -r root@118.190.204.203:/etc/openvpn/easy-rsa/3.0.8 C:/vpn
```

再输入密码，进行下载









