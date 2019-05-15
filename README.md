# Deanna Liu Homework 3 #

## Part II

#### Program 1: Fibonacci
* Returns the last digit of the Nth Fibonacci Number.

```c
$ make fib
$ ./fib 16
```

#### Program 2: Simple Shell
* Makeshift copy of the Linux Shell.
```
$ make sshell
$ ./simple_shell
shell> ls -a
shell> echo $PATH
shell> date
```

#### Program 3: Signal Block
* Print numbers [1-10] from the child process. It waits 1 second before printing out the next number. This program only accepts **run** and **exit**
```c
$ make sigblock
$ ./shell
shell> run
shell> exit
```

## Part III 

```c
$ make
$ ./artist_manager
shell> help
shell> date
shell> hire [N]
shell> fire [X]
shell> fireall
shell> assign [X]
shell> withdraw [X]
shell> list
shell> exit
```

* arguments are held in an array called `my_argv` which is like a copy of main's `*argv[]` 
* begin the program by filling the array `my_argv` with the commands.
   - method called `fill_argv()` fills the arg_v, separating the user input by a space. 
* catch `CTRL+C` and when it does, it properly exits the program.
* `help` prints all the possible commands and a short information about them.
* `date` prints the date.
* `hire [N]` where N is an integer. Creates N number of "processes" or "artists". The PID and the Status is stored in a linked list called `artists` in `artist_ds.c`
* `fire [X]` where X is the PID (process ID) - can be found by using `list`. This basically kills or "fires" the process/"artist". Data about them is removed from `artists` as well. 
   1. If X's status is `ASSIGNED` then withdraw them first.
* `fireall` "Fires" or kills all the processes/"artists". Data about them is removed from `artists` as well.
   1. If the current process status is `ASSIGNED` then withdraw them first.
* `assign [X]` where X is the PID (process ID) - can be found by using `list`. This basically changes the status of the currently still running processes/artists whose status is `WAITING` that they have a job now. Their status' are updated in the linked-list. 
    1. prints out **"ARTIST X IS ASSIGNED TO A JOB"** when successful. Their status is changed to `ASSIGNED`
    2. prints out **"ARTIST X IS ALREADY ASSIGNED TO A JOB"** if you try to assign an artist that has already been assigned.
    3. prints out **"ARTIST X DOES NOT EXIST"** if you try to assign an artist that you have not hired (not in the database)
* `withdraw [X]` where X is the PID (process ID) - can be found by using `list`. This basically changes the status of the currently still running processes/artists whose status is `ASSIGNED` that they don't have a job now. Their status' are updated in the linked-list. 
    1. prints out **"ARTIST X IS WITHDRAWN FROM A JOB"** when successful. Their status is changed to `WAITING`
    2. prints out **"ARTIST X CANNOT BE WITHDRAWN FROM A NONEXISTENT JOB"** if you try to withdraw an artist whose status is `WAITING`
    3. prints out **"ARTIST X DOES NOT EXIST"** if you try to withdraw an artist that you have not hired (not in the database)`
* `list` - prints out all the current artists/processes in the database. 
* `exit` - fires all of the employees. If the current process status is `ASSIGNED` then withdraw them first. Cleans up database. Flush. Byebye.

### Data Structure
* `artists` is a linked-list 
   - Has 3 attributes:
       1. PID
       2. Status
       3. Pointer to NEXT

* `my_argv` is an array that holds the command and if there is a number that follows it, then it will be in the next index.
# Artist-Manager
