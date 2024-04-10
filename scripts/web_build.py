Import("env")
import os
import subprocess


def run_npm_commands(folder_path):

    os.chdir(folder_path)

    install_process = subprocess.run(
        ["npm", "install"], shell=True, capture_output=True, text=True
    )

    build_process = subprocess.run(
        ["npm", "run", "build"], shell=True, capture_output=True, text=True
    )


def npm_pre_build(target, source, env):
    folder_path = "./website"  # Specify the folder path
    run_npm_commands(folder_path)


# Add a pre-action hook to execute npm commands before build
env.AddPreAction("buildprog", npm_pre_build)
