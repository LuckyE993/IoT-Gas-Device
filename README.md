# 项目简介

该项目能够检测工厂中四种常见气体（氧气、一氧化碳、硫化氢、甲烷），同时检测温度和湿度并显示在7寸屏幕上，并且能够将所获得的传感器数据上传至指定服务器。

**STM32相关的实现请参考STM32内[README](STM32F429\README.md)文件，ESP32相关的实现请参考ESP32内[README](ESP32\README.md)文件。**

# 详细功能说明

1. **气体浓度采集**：四合一气体传感器用于采集区域内四种常见气体的浓度。
2. **数据展示**：7寸LCD屏幕显示采集到的气体传感器数据。
3. **数据上传**：将气体与温湿度数据上传至局域网内的服务器。
4. **智能配网**：ESP32在无网络的情况下实现智能配网。
5. **数据传输**：通过封装成JSON格式的数据包发送传感器数据及IP地址等自辨信息。

# 开发环境

## 硬件要求

- 野火STM32F429IGT6核心板V1
- 野火7寸电容屏模块
- 四合一气体传感器
- DHT11温湿度传感器
- ESP32-WROOM-32-U
- RGB灯珠模块

## 软件要求

- Keil MDK build 528
- ESP-IDF 5.1
- Windows 10 专业版 23H2
- Ubuntu 18.04

# 安装使用步骤

1. **硬件连线**：按照 [IO接线表](STM32F429\Docs\IO接线表.xlsx) 进行硬件连接。
2. **克隆项目并编译STM32（Windows平台）**：
   1. 克隆本项目：
      ```sh
      git clone https://github.com/yourusername/project-name.git
      ```
   2. 打开项目文件：
      - 在Windows平台，打开 [STM32F429\Project\RVMDK（uv5）\BH-F429.uvprojx](STM32F429\Project\RVMDK（uv5）\BH-F429.uvprojx)。
   3. 编译并运行工程文件，完成后将程序下载到STM32开发板。
3. **克隆项目并编译ESP32（Linux平台）**：
   1. 克隆本项目：
      ```sh
      git clone https://github.com/yourusername/project-name.git
      ```
   2. 打开ESP32文件夹：
      - 在Linux平台，打开 `ESP32` 文件夹，使用ESP-IDF编译工具链进行编译。
   3. 将编译后的程序下载到ESP32中：
      - 预先配置目标WIFI SSID与密码：编辑 [ESP32\inc\wifi.h](ESP32\inc\wifi.h)。
      - 配置要发送的目标URL或HOST地址：编辑 [ESP32\src\server.c](ESP32\src\server.c)。
      - 如无法连接WIFI，程序将自动启动Smart Config，请参考乐鑫官方ESP Touch V1进行Smart Config智能配网。配网完成后需重新进行上电。
4. **查看传感器信息**：
   - 使用相关的HTTP调试工具查看传感器的相关信息。
