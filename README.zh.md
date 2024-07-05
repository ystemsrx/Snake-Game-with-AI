[English](README.md)
# 带AI助手的贪吃蛇游戏
## 概述

该项目是一个基于命令行的贪吃蛇游戏，具有四个难度级别（简单、普通、专家、大师），并集成了语言模型以增强游戏互动。游戏使用C语言编写，语言模型和助手功能使用Python实现。

## 文件

- `main.c`：游戏的入口点，提供难度选择。
- `Easy.c`：实现简单难度的贪吃蛇游戏。
- `Normal.c`：实现普通难度的贪吃蛇游戏。
- `Expert.c`：实现专家难度的贪吃蛇游戏。
- `Master.c`：实现大师难度的贪吃蛇游戏。
- `Language_Model.py`：用于与语言模型交互的Python脚本。
- `AI_Assistant.py`：使用PyQt5显示模型响应的Python脚本。
- `Error_Fixing.bat`：用于处理文件恢复的批处理脚本。
- `Start_Python.bat`：用于启动Python脚本的批处理脚本。

## 设置和执行

1. 将所有文件（`main.c`、`Easy.c`、`Normal.c`、`Expert.c`、`Master.c`、`Language_Model.py`、`AI_Assistant.py`、`Error_Fixing.bat`、`Start_Python.bat`）放入同一个目录。

2. 编译主C文件：
   ```sh
   gcc -o main.exe main.c
   ```

3. 运行编译后的程序：
   - 双击 `main.exe` 或在命令行中运行：
     ```sh
     ./main.exe
     ```

4. 按照屏幕上的提示选择难度级别：
   - 1: 简单
   - 2: 普通
   - 3: 专家
   - 4: 大师

5. 选择的难度文件（例如 `Easy.c`）将被编译并启动游戏。

## 游戏说明

- 贪吃蛇游戏通过命令行进行。

![image](https://github.com/ystemsrx/Snake-Game-with-AI/assets/140463276/edfca1d7-aa57-4b8d-84dc-a02a75c79657)


- 使用 `W`、`A`、`S`、`D` 键控制蛇的移动。
- 游戏窗口通过刷新模拟蛇的运动。

## 语言模型集成

- 启动任何游戏模式时，`Language_Model.py` 将被触发。
- 项目使用 Ollama 库运行本地语言模型 `llama3` 进行实时互动。
- `Conversations.txt` 用于在C程序和Python脚本之间进行通信。

### 互动流程

1. 当游戏启动（例如在简单模式下）时，将创建 `Conversations.txt`。
2. 一条消息写入文件，例如：“玩家开始游戏，模式：简单。”
3. 文件被重命名为 `Conversation.txt`。
4. `Language_Model.py` 检测到 `Conversation.txt`，读取最后一行，将其发送到模型，并将响应写入 `Model_Response.txt`。
5. `AI_Assistant.py` 持续监视 `Model_Response.txt`，在屏幕右上角的PyQt5窗口中实时显示响应。
6. 在游戏过程中，模型会根据吃到食物或游戏结束等事件进行实时互动。

## AI助手设置

要使用AI助手，您需要安装Python和必要的库：

1. 从 [python.org](https://www.python.org/) 安装Python。

2. 安装Ollama库并拉取 `llama3` 模型：
   ```sh
   pip install ollama
   ollama pull llama3
   ```

3. 安装PyQt5用于图形用户界面：
   ```sh
   pip install PyQt5
   ```

![image](https://github.com/ystemsrx/Snake-Game-with-AI/assets/140463276/b067e557-3640-4134-b6ba-7b2bf1581167)

![image](https://github.com/ystemsrx/Snake-Game-with-AI/assets/140463276/38f7f3c0-0084-4b66-929c-b86cdcb5053c)


## 错误处理

- `Error_Fixing.bat`：确保 `Conversation.txt` 或 `Conversations.txt` 在被删除时能够恢复。复制为 `Temp.txt` 并在必要时重命名。
- `Language_Model.py`：包含错误恢复功能，处理 `Conversation.txt` 和 `Conversations.txt` 同时存在的情况，删除其中一个以保持稳定。

## 依赖项

- 用于编译C文件的GCC。
- Python 3.x。
- 用于运行语言模型的Ollama库。
- 用于GUI组件的PyQt5。

## 注意事项

- 确保所有必要的Python模型已下载并配置。
- 根据您的环境调整批处理文件和脚本中的路径。
