# Installation Guide for WSL, Debian, ollama, and other prerequisite software

This guide details how to use two separate batch files, `run_first_install.bat` and `run_second_install.bat`, to install the Windows Subsystem for Linux (WSL) with Debian and various tools including ollama.

## Prerequisites
- **Operating System**: Windows 10 or later, with updates installed.
- **Permissions**: Administrator rights on your Windows machine are required to execute WSL installation commands.

## Steps to Install

### 1. Running `run_first_install.bat`
The `run_first_install.bat` file automates the installation of WSL and the Debian Linux distribution:
- **Right-click** on `run_first_install.bat` and select **"Run as administrator"** to start the installation process. This is essential as installing WSL requires administrative privileges. Windows 11 users can simply double-click to run the file.

### 2. Running `run_second_install.bat`
After setting up WSL and Debian, execute `run_second_install.bat` to install necessary tools such as ollama, curl, git, make, g++, gcc, and jq:
- **Right-click** on `run_second_install.bat` and select **"Run as administrator"**. This step ensures all installations are handled with the necessary permissions.

### 3. User Interactions
- **During Debian Installation**: When prompted, create a username and password for your new Debian user account. This information will be necessary for sudo operations within Debian.

### 4. Following On-Screen Instructions
Follow any on-screen prompts to complete the setup through both batch files. These will guide you through the initial WSL and Debian setup and then the installation of the development tools and ollama.

## Post-Installation
Access your Debian WSL environment:
- **Via the Start Menu**: Search for "Debian" and open the app.
- **Via Command Line**: Enter `wsl` or `wsl -d Debian` in the command prompt to start using Debian.

## Troubleshooting
- **WSL Not Recognized**: Ensure your Windows has the latest updates installed.
- **Permission Issues**: Confirm you have administrative rights and run both batch files as administrator.

For additional help or more detailed instructions on using WSL, refer to the [official Microsoft WSL installation guide](https://docs.microsoft.com/en-us/windows/wsl/install).

## What's next?

After the installation, open the WSL terminal by typing `wsl` into a command prompt, navigate to the project directory, move into the server directory, and type `make run` to compile and run the server. Next, proceed to the `CURL_README.md` to learn how to interact with the web service.

