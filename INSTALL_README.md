# Installation Guide for WSL, Debian, ollama, and prerequisite software via `install.bat`

This guide provides detailed instructions on how to use the `install.bat` batch file to install the Windows Subsystem for Linux (WSL) with Debian and to execute a shell script that installs Ollama and requires superuser privileges.

## Prerequisites
- **Operating System**: Windows 10 or later, with updates installed.
- **Permissions**: Administrator rights on your Windows machine are required to execute WSL installation commands.

## Steps to Install

### 1. Running `install.bat`
The `install.bat` file automates the installation of WSL and the Debian Linux distribution, and runs the `install.sh` script that installs the Ollama application. Here are the included commands:

### 2. Executing the Batch File
- **Right-click** on `install.bat` and select **"Run as administrator"** to start the installation process. Running as administrator is essential as installing WSL requires administrative privileges. If you are in windows 11 instead you should just be able to run the bat file by double clicking on it

### 3. User Interactions
- **During Debian Installation**: When prompted, enter a username and password for your new Debian user account. These credentials will be used for future sudo operations within Debian.
  
- **During Script Execution**: The `install.sh` script will likely prompt you for the sudo password. Enter the password you set up for the Debian user account. This script will proceed to install Ollama.

### 4. Following On-Screen Instructions
After initiating the batch file, follow any on-screen prompts to complete the setup. These will guide you through the WSL installation, the configuration of Debian, and the installation of Ollama.

## Post-Installation
After the installation, you can access your Debian WSL environment:
- **Via the Start Menu**: Search for "Debian" and open the app.
- **Via Command Line**: Type `wsl` or `wsl -d Debian` in your command prompt to start interacting with Debian.

## Troubleshooting
- **WSL Not Recognized**: Confirm that you have the latest Windows updates installed.
- **Permission Issues**: Ensure you run `install.bat` as an administrator.
- **Sudo Password Prompts**: Remember that the sudo password is the one you set during the Debian user account creation.

For additional help or more detailed instructions on using WSL, visit the [official Microsoft WSL installation guide](https://docs.microsoft.com/en-us/windows/wsl/install).

## What's next?

If you have successfully completed the instructions please open the project in the wsl terminal byt typing wsl into a terminal, and cding to the appropriate directory
Then cd into the server directory, and type 'make run' to install, and run
Then move onto the CURL_README.md, and follow the instructions there to learn how to interact with the web service
