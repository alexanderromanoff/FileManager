# FileManager
A simple imitation of the Linux terminal providing the fucntional to navigate the filesystem and obtain information about it. 
Created as a project

## How to build
After downloading the files, open the terminal in the directory with the project and simply run
```
make
```
You may also run this prompt and get the same result
```
g++ main.cpp src/controller.cpp -o manager
```

## How to use
Open the terminal and run
```
./filemanager
```
This will start the FileManager in the same termianl.
It's used as a regular linux terminal. You will see the welocme prompt stating the current directory.
You can execute several standard linux commands: 

### pwd
Prints the current directory. Takes no arguments.

### ls
Prints the contents of the given directory. May take one argument - the path to a directory.
If no argument is passed, the command will print the contents of the current directory.
The directories are printed with a slash symbol '/' in the end.

### stat
Prints the number of files, directories and the total number of entries in the given directory. May take one argument - the path to a directory.
If no argument is passed, the command will be executed at the current directory.

### cd
Changes the current directory to the given one. May take one argument - the path to a new directory.
If no argument is passed, the current directory will be set to home. You may also run ```cd ~``` and it will aslo be set to home.

### exit
Quits the FileManager, goes back to shell. Ignores any given arguments.

###### Note:
The flags aren't provided.
If the commands are used with invalid arguments or on the directories with limited permissions, the warning will appear.

## Usage example
<img width="786" height="537" alt="image" src="https://github.com/user-attachments/assets/f536b68c-c28b-466e-bea7-afd670bb0953" />

