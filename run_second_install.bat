#install ollama
wsl yes "y" | sudo apt update && sudo apt upgrade
wsl yes "y" | sudo apt install make
wsl yes "y" | sudo apt install curl
wsl yes "y" | sudo apt install git
wsl yes "y" | sudo apt install g++
wsl yes "y" | sudo apt install gcc
wsl yes "y" | sudo apt install jq
wsl curl -fsSL https://ollama.com/install.sh | sh

