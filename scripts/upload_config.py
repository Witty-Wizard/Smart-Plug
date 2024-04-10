Import("env")
# Define the custom target for building and uploading LittleFS filesystem
target_buildfs = "buildfs"
target_uploadfs = "uploadfs"


# Define the function to handle the buildfs target
def buildfs(target, source, env):
    # Run the platformio buildfs command to build the LittleFS filesystem
    env.Execute("platformio run --target buildfs")


# Define the function to handle the uploadfs target
def uploadfs(target, source, env):
    # Run the platformio uploadfs command to upload the LittleFS filesystem
    env.Execute("platformio run --target uploadfs")


# Hook into the pre-action of the upload target
env.AddPreAction("upload", buildfs)
env.AddPreAction("upload", uploadfs)
