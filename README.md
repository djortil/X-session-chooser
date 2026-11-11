This simple tool allows you to launch an X session from Linux TTY without having to update your .xinitrc each time.

## Usage:

1. `git clone https://github.com/djortil/X-session-chooser`
2. cd into project directory
3. `gcc -std=c17 -Wall -Wextra -Wpedantic -Wconversion -Wshadow -Wformat=2 -Wstrict-prototypes -std=c17 -g main.c -o program`
4. move into any folder that's in your $PATH variable, such as /home/user/.local/bin (so that you could run it from anywhere).
5. create a file named '.xsessions' in your home directory and write your entries separated by ;.
`xfce4-session;windowmaker;icewm-session;`
