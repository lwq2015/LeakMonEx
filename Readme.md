# GDI,Handle,Memory Leak检测工具

fork from: <https://github.com/nave432/LeakMon.git>

## 前因

最近在查GDI泄漏问题，找了很多工具，都不适用，最后找到了此源代码，还可以使用，就修改了一下。

优点：

- 代码简单
- 不仅仅有相关地址，还有堆栈信息，可以定位源代码。
- 可以修改相关源代码，满足自己的需求

缺点：

- 只能动态注入，无法自动启动并注入，这个不算是太大的问题
- 区分了 GDI， Handle, Memory， 一次只能检测一种类型，无法动态切换，需要重启被检测的程序
- 查看工具太简单，但可以用对比工具替代

相关工具：

1. [Dr. Memory](https://github.com/DynamoRIO/drmemory)，但对MFC支持不太好
2. [Deleaker](https://s3.us-east-2.amazonaws.com/download.softanics/deleaker/DeleakerSetup_2026.3.0.0.zip), VS扩展插件，可以试用
3. [gdiview](https://bjansen.github.io/scoop-apps/nirsoft/gdiview/), 可以观察具体的GDI变化

## 主要修改内容

1. 适配Windows10, 支持VS2019
2. 规范工程配置，规范相关代码书写
3. 修正注册表API库不正确（有可能是系统更新迁移所致）
4. Injector 添加搜索和进程路径列
5. 相关窗口可以缩放

## 使用简介

1. 启动 Injector
2. 找到待检查的进程并选中，进行注入 *Inject* 到进程，选择检测类型，并配置相关符号即可
3. 再多次 *Dump*，就可以生成相关日志，会提示是否用 *Dump Viewer* 打开，使用多次Dump，进行对比，就可以分析泄漏
4. *Reset* 可以重置已经记录的内容
5. *PDB* 可以配置调试信息
6. *Find* 可以搜索进程名称，可以重复搜索，找到需要检测的进程
