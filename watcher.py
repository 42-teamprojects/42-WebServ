from watchdog.observers import Observer
from watchdog.events import FileSystemEventHandler
import subprocess
import os
import signal
import time

EXE = "webserv"

class MyHandler(FileSystemEventHandler):
    def __init__(self, process):
        self.process = process
        self.last_modified = 0
        self.exclude_extensions = ['.o', '']  # Add any other file extensions to exclude

    def on_modified(self, event):
        if event.is_directory:
            return
        _, file_extension = os.path.splitext(event.src_path)
        if file_extension.lower() in self.exclude_extensions:
            return
        current_time = time.time()
        if current_time - self.last_modified > 1:  # Add a 1-second delay
            self.last_modified = current_time
            print(f'File {event.src_path} has been modified. Recompiling and restarting...')
            self.process.send_signal(signal.SIGINT)  # Send interrupt signal to the process
            self.process.wait()  # Wait for the process to finish
            compile_result = subprocess.run(['make'])  # Recompile
            if compile_result.returncode == 0:
                self.process = subprocess.Popen(['./' + EXE])  # Start the new process

def start_watcher():
    initial_compile = subprocess.run(['make'])
    if initial_compile.returncode != 0:
        print("Initial compilation failed. Exiting.")
        return

    process = subprocess.Popen(['./' + EXE])

    event_handler = MyHandler(process)
    observer = Observer()
    observer.schedule(event_handler, path='.', recursive=True)
    observer.start()
    
    try:
        while True:
            time.sleep(1)  # Add a delay to avoid rapid recompilation
    except KeyboardInterrupt:
        observer.stop()
    
    observer.join()

if __name__ == "__main__":
    start_watcher()
