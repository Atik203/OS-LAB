#!/bin/bash

# Difference between `sh script.sh` and `./script.sh`:
#
# `sh script.sh` runs the script using the `sh` shell explicitly.
# It does not require the file to have execute permission, because `sh`
# reads the file and runs it.
# Example:
# sh script.sh
#
# `./script.sh` runs the script directly as an executable file.
# For this to work, the script must have execute permission and a valid
# shebang line such as `#!/bin/bash` at the top.
# Example:
# chmod +x script.sh
# ./script.sh
#
# In short:
# `sh script.sh` = run with the `sh` interpreter
# `./script.sh` = execute the file directly using its shebang

