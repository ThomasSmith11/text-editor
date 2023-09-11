A simple text editor with limited functionality to learn to write in C++

Tested on a 2013 Macbook Pro running Linux Mint 21.1 and a 2020 Macbook Pro running MacOS Ventura 13.3.1 (macOS requires changing KEY_BACKSPACE in inputHandler to 127 and KEY_SUP and KEY_SDOWN may need to be determined and replaced in the same file, I used cat -v on linux, but that may be different on mac.).

Has functionality for editing and saving files, as well as searching, cutting, copying, and pasting.

If you wish to try it out, clone the repository, cd into the cloned repo, and run make.  Then run ./TextEditor path/to/file/you/wish/to/edit
