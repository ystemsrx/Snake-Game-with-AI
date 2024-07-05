[中文版](README.zh.md)
# Snake Game with AI assistant

## Overview

This project consists of a command-line based Snake game with four difficulty levels (Easy, Normal, Expert, Master), integrated with a language model to enhance gameplay interaction. The game is written in C, and the language model and assistance features are implemented in Python.

## Files

- `main.c`: Main entry point for the game, provides difficulty selection.
- `Easy.c`: Implementation of the Snake game on Easy difficulty.
- `Normal.c`: Implementation of the Snake game on Normal difficulty.
- `Expert.c`: Implementation of the Snake game on Expert difficulty.
- `Master.c`: Implementation of the Snake game on Master difficulty.
- `Language_Model.py`: Python script to interact with the language model.
- `AI_Assistant.py`: Python script to display model responses using PyQt5.
- `Error_Fixing.bat`: Batch script to handle file recovery.
- `Start_Python.bat`: Batch script to start Python scripts.

## Setup and Execution

1. Place all files (`main.c`, `Easy.c`, `Normal.c`, `Expert.c`, `Master.c`, `Language_Model.py`, `AI_Assistant.py`, `Error_Fixing.bat`, `Start_Python.bat`) into a single directory.

2. Compile the main C file:
   ```sh
   gcc -o main.exe main.c
   ```

3. Run the compiled program:
   - Double-click `main.exe` or run it from the command line:
     ```sh
     ./main.exe
     ```

4. Follow the on-screen prompts to select the difficulty level:
   - 1: Easy
   - 2: Normal
   - 3: Expert
   - 4: Master

5. The selected difficulty file (e.g., `Easy.c`) will be compiled and the game will start.

## Game Instructions

- The Snake game is played via the command line.
- Use the `W`, `A`, `S`, `D` keys to control the snake's movement.
- The game window refreshes to simulate the snake's motion.

## Language Model Integration

- Upon starting any game mode, `Language_Model.py` will be triggered.
- The project uses the Ollama library to run the local language model `llama3` for real-time interaction.
- `Conversations.txt` is used to communicate between the C program and Python script.

### Interaction Flow

1. When the game starts (e.g., in Easy mode), `Conversations.txt` is created.
2. A message is written to the file, e.g., "The player starts the game, gamemode: Easy."
3. The file is renamed to `Conversation.txt`.
4. `Language_Model.py` detects `Conversation.txt`, reads the last line, sends it to the model, and writes the response to `Model_Response.txt`.
5. `AI_Assistant.py` continuously monitors `Model_Response.txt`, displays the response in a PyQt5 window at the top right of the screen, updating in real-time.
6. Interaction continues with the model responding to game events like eating food or game over.

## AI Assistant Setup

To use the AI assistant, you need to install Python and the necessary libraries:

1. Install Python from [python.org](https://www.python.org/).

2. Install the Ollama library and pull the `llama3` model:
   ```sh
   pip install ollama
   ollama pull llama3
   ```

3. Install PyQt5 for the graphical user interface:
   ```sh
   pip install PyQt5
   ```

## Error Handling

- `Error_Fixing.bat`: Ensures `Conversation.txt` or `Conversations.txt` is restored if deleted. Copies to `Temp.txt` and renames it back if necessary.
- `Language_Model.py`: Contains error recovery to handle instances where both `Conversation.txt` and `Conversations.txt` exist, deleting the latter to maintain stability.

## Dependencies

- GCC for compiling C files.
- Python 3.x.
- Ollama library for running language models.
- PyQt5 for GUI components.

## Notes

- Ensure all necessary Python models are downloaded and configured.
- Adjust the paths in batch files and scripts as necessary for your environment.
