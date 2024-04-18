#install ollama
wsl -d Debian bash -c "sudo apt update && sudo apt upgrade -y;
sudo apt install -y make curl git g++ gcc jq;
curl -fsSL https://ollama.com/install.sh | sudo sh"

