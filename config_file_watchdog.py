from watchdog.observers import Observer
from watchdog.events import FileSystemEventHandler
import time

class ChangeHandler(FileSystemEventHandler):
    def on_modified(self, event):
        if event.src_path == "data/config/hardwareConfig.json":
            print("Config file changed. Running validation...")
            subprocess.run(["python", "scripts/validate_esp32_hardware_config.py"])

if __name__ == "__main__":
    event_handler = ChangeHandler()
    observer = Observer()
    observer.schedule(event_handler, path='.', recursive=False)
    observer.start()

    try:
        while True:
            time.sleep(1)
    except KeyboardInterrupt:
        observer.stop()

    observer.join()
