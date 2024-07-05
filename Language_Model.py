import ollama
import time
import os
import subprocess
from threading import Thread

# 初始化聊天
messages = [{'role': 'system', 'content': 'You are a Snake Game assistant, receiving game information from the player and providing feedback. When the player eats food, eats Bonus Food, collides with their own body causing a length reduction, or hits an obstacle leading to Game Over, you will be notified. The game has four difficulty levels: Easy, Normal, Expert, and Master. When the score reaches 30 points, the game is won, and you can provide different feedback based on the current score. The prompts you receive are from the system, but your conversation is with the player. Keep this in mind. Your tone should be casual, and you can make jokes, use puns, or roast the player. Think of yourself as the brother or teasing friend of the player, creating a relaxed and fun atmosphere. Feel free to use emojis. You must make your reply short and call the player bro.'}]

def initialize_files():
    with open('Model_Response.txt', 'w', encoding='utf-8') as file:
        pass  # 清空文件内容

def get_last_lines():
    with open('Conversation.txt', 'r', encoding='utf-8') as file:
        lines = file.readlines()
    return lines

def append_to_conversation(content):
    with open('Conversation.txt', 'a', encoding='utf-8') as file:
        file.write(content + '\n\n')

def rename_file(old_name, new_name):
    if os.path.exists(old_name):
        os.rename(old_name, new_name)

def error_recovery():
    while True:
        if os.path.exists('Conversation.txt') and os.path.exists('Conversations.txt'):
            size_conversation = os.path.getsize('Conversation.txt')
            size_conversations = os.path.getsize('Conversations.txt')
            if size_conversation < size_conversations:
                os.remove('Conversation.txt')
            elif size_conversation > size_conversations:
                os.remove('Conversations.txt')
            else:
                os.remove('Conversation.txt')
        time.sleep(0.01)

def main():
    # 以无头模式启动 Error_Fixing.bat
    subprocess.Popen(['Error_Fixing.bat'], creationflags=subprocess.CREATE_NO_WINDOW)

    initialize_files()
    
    # 启动错误补救线程
    recovery_thread = Thread(target=error_recovery, daemon=True)
    recovery_thread.start()
    
    while True:
        time.sleep(0.5)
        if not os.path.exists('Conversation.txt'):
            continue
        
        lines = get_last_lines()
        
        # 找到最后有字的一行
        last_non_empty_line_index = len(lines) - 1
        while last_non_empty_line_index >= 0 and not lines[last_non_empty_line_index].strip():
            last_non_empty_line_index -= 1
        
        if last_non_empty_line_index >= 0:
            user_input = lines[last_non_empty_line_index].strip()
            if user_input.lower() in ['exit', 'quit']:
                break
            
            # 将用户输入添加到消息列表
            messages.append({'role': 'user', 'content': user_input})
            
            # 获取模型响应
            stream = ollama.chat(
                model='llama3',
                messages=messages,
                stream=True,
            )
            
            # 输出和保存模型响应
            response = ''
            with open('Model_Response.txt', 'a', encoding='utf-8') as file:
                for chunk in stream:
                    part = chunk['message']['content']
                    response += part
                    print(part, end='', flush=True)
                    file.write(part)
                    file.flush()  # 立即保存到文件
            
            # 将模型响应添加到消息列表
            messages.append({'role': 'assistant', 'content': response})

            # 在每次响应结束后添加两个换行符
            with open('Model_Response.txt', 'a', encoding='utf-8') as file:
                file.write('\n\n')
                file.flush()

            # 重命名文件
            time.sleep(0.1)
            rename_file('Conversation.txt', 'Conversations.txt')

if __name__ == "__main__":
    main()
