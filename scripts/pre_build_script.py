Import("env")
import subprocess

def before_build(source, target, env):
    print("Running config validation...")
    result = subprocess.run(["python", "scripts/validate_esp32_hardware_config.py"], capture_output=True, text=True)
    print(result.stdout)
    if result.returncode != 0:
        raise RuntimeError("Validation failed!")

env.AddPreAction("buildprog", before_build)
