@echo off
:loop
REM 检查是否存在Conversation.txt或Conversations.txt
if exist Conversation.txt (
    REM 如果存在Conversation.txt，将其复制为Temp.txt
    copy /y Conversation.txt Temp.txt
) else if exist Conversations.txt (
    REM 如果存在Conversations.txt，将其复制为Temp.txt
    copy /y Conversations.txt Temp.txt
) else if not exist Conversation.txt if not exist Conversations.txt (
    REM 如果两个文件都不存在，将Temp.txt重命名为Conversations.txt
    if exist Temp.txt ren Temp.txt Conversations.txt
)

REM 等待1秒后继续循环
timeout /t 1 >nul
goto loop
