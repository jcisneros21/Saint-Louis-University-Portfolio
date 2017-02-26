Name
====

`longls` - list files in a directory, with details

Usage
=====

`longls [directory1] [directory2] ... [directoryn]`

Description
===========

`longls` lists all files in the directories
passed as arguments, or lists the current directory if there are no
arguments. If there is more than one directory passed as argument,
precede each listing with a separator containing the directory name.

For each file, you can format your output as you wish, but you must
display:

-   The file name.

-   The type of file, if not a regular file.

-   The owner’s name and group (and note if the file is setuid/setgid).

-   The file’s permissions in `rwxrwxrwx` format (exactly as in `ls -l`).

-   The file size in bytes and blocks.

-   The access, modification, and file status change dates.

Here is some sample output:
```
    demo_basic  (directory)
            owner: bryan     group: faculty     rwxr-xr-x
            16 bytes, 6 blocks.
            last access:              Sun Nov 22 04:51:08 2015
            last modification:        Mon Aug 31 13:33:17 2015
            last file status change:  Mon Aug 31 13:33:17 2015
```

Hints
=====

See the `Public/os/bin` folder for a working version. There are some
interesting files in the `/dev` directory, and more in
`Public/os/demo_file/interesting_files`.

Possible file types are: directory, character device, block device,
fifo, symbolic link, socket. There are macros for detecting these types
(`S_ISDIR`, etc.) - see `lstat`(2).

Check return values from all system calls. Even `getpwuid` and
`getgrgid` can return `NULL`.

First write a function that takes a directory name as argument and lists
the names of all files in that directory. Next, write a function that
takes the name of a file and displays the detailed information for that
file. Finally, put it all together by calling the directory lister for
each command line argument.

When you encounter failures, don’t exit. For example, if a directory
fails to open, print an error and continue with the next directory in
the list of arguments. If a file has trouble, list it with an error
notice and continue listing files in the directory.

Useful Man Pages
================

`lstat(2)`

`chdir(2)`

`getcwd(3)`

`opendir(3)`

`readdir(3)`

`closedir(3)`

`getpwuid(3)`

`getgrgid(3)`

`ctime(3)`
