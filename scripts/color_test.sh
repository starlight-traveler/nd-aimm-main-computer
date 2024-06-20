# Function to print a color block with the color code
print_color() {
  local code=$1
  local bold_code=$2
  echo -e "Code ${code} - Regular: \e[${code}m██████\e[0m  Bold: \e[${bold_code};${code}m██████\e[0m"
}

# Print colors from 30 to 37 (standard colors) and their bold variants from 1;30 to 1;37
for code in {1..90}; do
  print_color $code "1;${code}"
done

# Print colors from 90 to 97 (bright colors) and their bold variants from 1;90 to 1;97
for code in {90..120}; do
  print_color $code "1;${code}"
done

# Reset colors at the end of the script
echo -e "\e[0m"

