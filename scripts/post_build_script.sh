if [[ "$OSTYPE" != "msys" && "$OSTYPE" != "win32" ]]; then
  Esc=$(printf "\033")
  ColourReset="${Esc}[m"
  ColourBold="${Esc}[1m"
  Red="${Esc}[31m"
  Green="${Esc}[32m"
  Yellow="${Esc}[33m"
  Blue="${Esc}[34m"
  Magenta="${Esc}[35m"
  Cyan="${Esc}[36m"
  White="${Esc}[37m"
  BoldRed="${Esc}[1;31m"
  BoldBlue="${Esc}[1;90m"
  Reset="${Esc}[0m"  # ANSI code for bright and bold green
fi

current_time=$(date +"%Y-%m-%d %H:%M:%S")


message="
${Yellow}================================================================================
${BoldBlue} ____  _____  ______              _       _____  ____    ____  ____    ____
${BoldBlue}|_   \\|_   _||_   _ \`.           / \\     |_   _||_   \\  /   _||_   \\  /   _|
${BoldBlue}  |   \\ | |    | | \`. \ ______  / _ \\      | |    |   \\/   |    |   \\/   |
${BoldBlue}  | |\\ \\| |    | |  | ||______|/ ___ \\     | |    | |\\  /| |    | |\\  /| |
${BoldBlue} _| |_\\   |_  _| |_.' /      _/ /   \\ \\_  _| |_  _| |_\\/_| |_  _| |_\\/_| |_
${BoldBlue}|_____|\\____||______.'      |____| |____||_____||_____||_____||_____||_____|
${Reset}${Yellow}================================================================================${Reset}
${Green}[Success!] Build completed at: ${current_time}${Reset}
${Reset}${Yellow}================================================================================${Reset}
"

echo -e "$message"

