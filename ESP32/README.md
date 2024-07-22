# 功能描述

ESP32完成与STM32传感器进行通讯，获取传感器信息并发送到指定的服务器上。

首次使用需使用Smart Config对网络进行配置，配网完成后会存储在ESP32 FLASH 中。当需要更换WiFi时需要将GPIO13与3.3V短接，然后使用Smart Config进行配置网络。

# 系统示意图

![ESP32时序图.png](pics\ESP32时序图.png)

# 接口

## 串口

## WiFi

## 服务器