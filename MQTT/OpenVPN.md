

# OpenVPN 采集与传输小组

文档更新时间 2020年12月11日

# SSH 远程登录

SSH ( Secure Shell ) 用于远程登录云服务器 (Elastic Compute Service, 简称 ECS)。

Secure Shell ( SSH ) 是由 IETF ( The Internet Engineering Task Force )  制定的建立在应用层基础上的安全网络协议。它是专为远程登录会话 ( 甚至可以用Windows远程登录 Linux 服务器进行文件互传 ) 和其他网络服务提供安全性的协议，可有效弥补网络中的漏洞。通过 SSH，可以把所有传输的数据进行加密，也能够防止 DNS 欺骗和 IP 欺骗。还有一个额外的好处就是传输的数据是经过压缩的，所以可以加快传输的速度。目前已经成为 Linux 系统的标准配置。

一般默认 SSH 端口是 22 号。

## Windows 平台安装 OpenSSH
简单安装方式：

1. 系统要求：`Windows 10 1809` 及以上版本
2. 打开设置，选择应用，点击应用和功能项中按钮【可选功能】，添加功能，安装 OpenSSH 服务器

![image-20201211152448732](C:\Users\24568\AppData\Roaming\Typora\typora-user-images\image-20201211152448732.png)

参考：https://docs.microsoft.com/zh-cn/windows-server/administration/openssh/openssh_install_firstuse

Linux 平台（一般自带 OpenSSH）

## 远程登录

以管理员身份运行 `Windows PowerShell`（或者其他终端，如 `Windwos Terminal`），输入
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

### SSH 用法

```
ssh root@47.103.83.146 -p 22
```
参数说明
```
root 远程服务器用户名，可替换成其他
@ 后面 ip 是远程服务器的公网 ip
-p 22 是默认的，可以不填
```
![image-20201211153137092](C:\Users\24568\AppData\Roaming\Typora\typora-user-images\image-20201211153137092.png)

如有提问，则输入 yes，再输入密码（输入密码时候是不可见的）
登录成功：

```
欢迎使用 Alibaba Cloud Linux 2 等保合规镜像
```
![image-20201211153224668](C:\Users\24568\AppData\Roaming\Typora\typora-user-images\image-20201211153224668.png)

# OpenVPN 服务端的搭建

| 主机           | 操作系统              | 角色           |
| -------------- | --------------------- | -------------- |
| 阿里云服务器   | Alibaba Cloud Linux 2 | OpenVPN 服务器 |
| 本地笔记本电脑 | Windows 10            | OpenVPN 客户端 |

## * 安装 OpenVPN 和 easy-rsa

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
![image-20201211153349139](C:\Users\24568\AppData\Roaming\Typora\typora-user-images\image-20201211153349139.png)

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

![image-20201211153456877](C:\Users\24568\AppData\Roaming\Typora\typora-user-images\image-20201211153456877.png)


命令
## 初始化 pki （公钥基础设施）
```bash
./easyrsa init-pki
```
提示：

```bash
Note: using Easy-RSA configuration from: /usr/share/easy-rsa/3.0.8/vars

init-pki complete; you may now create a CA or requests.
Your newly created PKI dir is: /usr/share/easy-rsa/3.0.8/pki
```

## 创建 CA （第三方证书）

```
./easyrsa build-ca
```

`创建时后加 nopass 表示不加密 就不会提示 Enter PEM pass phrase，这里设置了密码为 123456，后面创建服务端和客户端时选择了无密码。`

提示：

```bash
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

```bash
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

```bash
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

创建  Diffie-Hellman，确保 key 穿越不安全网络的命令，比较漫长。

提示：

```
DH parameters of size 2048 created at /usr/share/easy-rsa/3.0.8/pki/dh.pem
```

## 创建客户端

```
./easyrsa gen-req client nopass
```

提示：

```bash
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
 cp /usr/share/easy-rsa/3.0.8/pki/issued/client.crt keys/ # 复制 client.crt
```

复制 server.conf 模板到 openvpn 文件夹

```bash
cp /usr/share/doc/openvpn-2.4.9/sample/sample-config-files/server.conf /etc/openvpn/
```

编辑 server.conf（部分设置）

```
proto tcp # 修改为 tcp 协议，或者用 udp
```

## 设置 ta.key

在 keys 文件夹下

```
openvpn --genkey --secret ta.key
```

这时再查看 keys 文件夹下：

```
ca.crt  client.crt  client.key  dh.pem  server.crt  server.key  ta.key
```

### iptables 设置 nat 规则和打开路由转发

```bash
iptables -t nat -A POSTROUTING -s 10.8.0.0/24 -j MASQUERADE
iptables -vnL -t nat
```

编辑

```bash
vim /etc/sysctl.conf
```

修改成：

```bash
net.ipv4.ip_forward = 1
```

保存

```
sysctl -p
```

## 启动 openvpn

```
openvpn --daemon --config server.conf
```

检查 OpenVPN 是否打开成功

```bash
netstat -lntup | grep 1194 # 检查 1194 端口是否打开
```

如果出现

```bash
tcp        0      0 0.0.0.0:1194            0.0.0.0:*               LISTEN      4161/openvpn
```

说明 OpenVPN 打开成功。

如果开启后没有打开1194 端口，说明开启服务失败，可能是配置文件有错，也有可能是权限不够，查询日志。

# 验证 CA 和 server.crt 文件正确性

![image-20201030151106397](C:\Users\24568\AppData\Roaming\Typora\typora-user-images\image-20201030151106397.png)

# 配置 server.conf

在 openvpn 目录下，新建

```
vim server.conf
```

配置：

```bash
local 0.0.0.0
port 1194
proto tcp
dev tun
ca keys/ca.crt
cert keys/server.crt
key keys/server.key
dh keys/dh.pem
server 10.1.0.0 255.255.255.0
ifconfig-pool-persist ipp.txt
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

# OpenVPN 客户端部署 Windows 端

Windows 平台 V2.4.6 下载地址

https://swupdate.openvpn.org/community/releases/openvpn-install-2.4.6-I602.exe

安装记得勾选上 `EasyRSA 2 Certificate Management Scripts`

将之前存放服务器在 `/root/users/client1` 中的四个文件下载到客户端，存放在`C:\Program Files\OpenVPN\config` 目录下

配置客户端 Config

```
##############################################
# Sample client-side OpenVPN 2.0 config file #
# for connecting to multi-client server.     #
#                                            #
# This configuration can be used by multiple #
# clients, however each client should have   #
# its own cert and key files.                #
#                                            #
# On Windows, you might want to rename this  #
# file so it has a .ovpn extension           #
##############################################

# Specify that we are a client and that we
# will be pulling certain config file directives
# from the server.
client

# Use the same setting as you are using on
# the server.
# On most systems, the VPN will not function
# unless you partially or fully disable
# the firewall for the TUN/TAP interface.
;dev tap
dev tun

# Windows needs the TAP-Win32 adapter name
# from the Network Connections panel
# if you have more than one.  On XP SP2,
# you may need to disable the firewall
# for the TAP adapter.
;dev-node MyTap

# Are we connecting to a TCP or
# UDP server?  Use the same setting as
# on the server.
proto tcp
;proto udp

# The hostname/IP and port of the server.
# You can have multiple remote entries
# to load balance between the servers.
remote 47.103.83.146 1194
;remote my-server-2 1194

# Choose a random host from the remote
# list for load-balancing.  Otherwise
# try hosts in the order specified.
;remote-random

# Keep trying indefinitely to resolve the
# host name of the OpenVPN server.  Very useful
# on machines which are not permanently connected
# to the internet such as laptops.
resolv-retry infinite

# Most clients don't need to bind to
# a specific local port number.
nobind

# Downgrade privileges after initialization (non-Windows only)
;user nobody
;group nobody

# Try to preserve some state across restarts.
persist-key
persist-tun

# If you are connecting through an
# HTTP proxy to reach the actual OpenVPN
# server, put the proxy server/IP and
# port number here.  See the man page
# if your proxy server requires
# authentication.
;http-proxy-retry # retry on connection failures
;http-proxy [proxy server] [proxy port #]

# Wireless networks often produce a lot
# of duplicate packets.  Set this flag
# to silence duplicate packet warnings.
;mute-replay-warnings

# SSL/TLS parms.
# See the server config file for more
# description.  It's best to use
# a separate .crt/.key file pair
# for each client.  A single ca
# file can be used for all clients.
ca ca.crt
cert client.crt
key client.key

# Verify server certificate by checking that the
# certicate has the correct key usage set.
# This is an important precaution to protect against
# a potential attack discussed here:
#  http://openvpn.net/howto.html#mitm
#
# To use this feature, you will need to generate
# your server certificates with the keyUsage set to
#   digitalSignature, keyEncipherment
# and the extendedKeyUsage to
#   serverAuth
# EasyRSA can do this for you.
remote-cert-tls server

# If a tls-auth key is used on the server
# then every client must also have the key.
tls-auth ta.key 1

# Select a cryptographic cipher.
# If the cipher option is used on the server
# then you must also specify it here.
# Note that v2.4 client/server will automatically
# negotiate AES-256-GCM in TLS mode.
# See also the ncp-cipher option in the manpage
cipher AES-256-CBC

# Enable compression on the VPN link.
# Don't enable this unless it is also
# enabled in the server config file.
comp-lzo

# Set log file verbosity.
verb 3

# Silence repeating messages
;mute 20

```

# 启动 OpenVPN GUI

![image-20201211160024075](C:\Users\24568\AppData\Roaming\Typora\typora-user-images\image-20201211160024075.png)

用管理员身份运行，链接成功

![连接成功](C:\Users\24568\AppData\Roaming\Typora\typora-user-images\image-20201211151403071.png)



# 路由器设置 OpenVPN