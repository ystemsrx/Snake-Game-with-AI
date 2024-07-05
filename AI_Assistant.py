import sys
import os
import time
from PyQt5.QtWidgets import QApplication, QWidget, QVBoxLayout, QLabel, QTextEdit
from PyQt5.QtCore import QFileSystemWatcher, Qt, QTimer
from PyQt5.QtGui import QFont, QTextCursor

class FileWatcher(QWidget):
    def __init__(self, filepath):
        super().__init__()
        self.filepath = filepath

        self.initUI()
        self.initFileWatcher()
        self.buffer = ""
        self.buffer_timer = QTimer(self)
        self.buffer_timer.timeout.connect(self.process_buffer)
        self.first_change_detected = False
        self.last_position = 0  # 上次读取文件的位置

    def initUI(self):
        self.setWindowTitle('AI Assistant')
        self.setGeometry(2000, 100, 800, 1000)

        layout = QVBoxLayout()

        ascii_art = """
     ___       __  
    /   \     |  | 
   /  ^  \    |  | 
  /  /_\  \   |  | 
 /  _____  \  |  | 
/__/     \__\ |__| 
                   """
        self.ascii_label = QLabel(ascii_art, self)
        self.ascii_label.setFont(QFont("Courier", 12))
        self.ascii_label.setAlignment(Qt.AlignCenter)
        layout.addWidget(self.ascii_label)

        self.text_edit = QTextEdit(self)
        self.text_edit.setFont(QFont("Segoe UI Emoji", 14))
        self.text_edit.setReadOnly(True)
        layout.addWidget(self.text_edit)

        self.setLayout(layout)

        QTimer.singleShot(400, self.set_always_on_top)

    def set_always_on_top(self):
        self.setWindowFlags(self.windowFlags() | Qt.WindowStaysOnTopHint)
        self.show()

    def initFileWatcher(self):
        self.file_watcher = QFileSystemWatcher()
        self.file_watcher.addPath(self.filepath)
        self.file_watcher.fileChanged.connect(self.on_file_changed)

    def on_file_changed(self):
        if not self.first_change_detected:
            self.first_change_detected = True
            self.buffer_timer.start(10)  # 每0.01秒处理缓冲区内容
        else:
            self.buffer_timer.start(10)  # 文件再次变化时重新启动缓冲区处理

        try:
            with open(self.filepath, 'r', encoding='utf-8') as file:
                file.seek(self.last_position)
                new_content = file.read()
                self.last_position = file.tell()  # 更新上次读取的位置
                self.buffer += new_content
        except Exception as e:
            print(f"Error reading file: {e}")

    def process_buffer(self):
        if self.buffer:
            self.text_edit.insertPlainText(self.buffer[0])  # 每次输出一个字符
            self.buffer = self.buffer[1:]
            self.text_edit.moveCursor(QTextCursor.End)
        else:
            self.buffer_timer.stop()

if __name__ == '__main__':
    app = QApplication(sys.argv)
    filepath = os.path.join(os.getcwd(), "Model_Response.txt")
    if not os.path.isfile(filepath):
        print(f"File {filepath} does not exist.")
        sys.exit()

    file_watcher = FileWatcher(filepath)
    file_watcher.show()
    sys.exit(app.exec_())
