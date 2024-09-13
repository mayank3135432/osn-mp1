[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https:/classroom.github.com/a/Qiz9msrr)
# mini-project-1-template
  
  
    
Details:  
Specification 2:  
1. MAX value for input lenght and most other stuff is set to 4096  
2. if using semicolon in a command, make sure to leave a space  
    echo hello ; reveal ..  # valid command  
    echo hello;reveal ..  # invalid command  
3. same thing with ampersands  
        sleep 5 & echo hello    # valid command  
        sleep 5&echo hello    # invalid command  
4. Same thing with pipes and redirection, must be space seperated.  
    
      
Specification 3:  
1. The previous directory '-' is left undefined at the beginning of each shell session.  
2. if multiple directory, hop changes directory sequentially, ultimately ending up at the last directory  
3. In case of a command such as 'hop t1 -' , the previous directory is NOT redefined to t1  


Specification 5:  
1. erroneous commands are also stored in log.  
2. history file will be created at the home directory if not present already.  
3. history file is preserved across different shell sessions.  
4. If you run a.out from a different directory (different home), a different history file will be maintained.  
5. commands containing log will not be stored in history.  
      
Specification 6:  
1. Running interactive commands such as vim, htop ...etc in background (using ampersand '&') is NOT SUPPORTED.  

Specification 8:  
1. seek does not go through symlinks.  
1. seek does not take in '-' like hop does.  

Specification 9:  
1. alias is implemeted as a command that can be run/ Everytime a new alias is defined, an alias - command pair stored in a struct. This struct is initially empty at thestart of every shell session.  
(Access time for alias is linear which is not the best compared to constant time when using a hashmap but it works nonetheless.)  
2. .myshrc file is run line by line at the beginning of every shell session.  
custom functions are NOT SUPPORTED.  

Specification 10,11,12:  
1. background processes with '&' will not work with piped/redirected commands since user commands are not supported to run in the background.  
2. pipe cannot handle data of size above a maximum limit (about 4100 lines).  

Specification 13:  
1. Works by looking at the /proc directory and printing the processes whose parent is the shell.  
2. Again, the implementation is not the best but it works.  

Specification 17:  
1. iMan does not print the http headers.  
2. iMan also prints html tags.  
  
Instructions:  
to compile, run make  

Changes made in Part A files:  
1. reveal command now prints contents in lexicographic order  
2. reveal command now also accepts files as argument (similar to ls in bash.)  
3. Background support withdrawn for user commands (non system commands eg. hop, reveal, proclore, ping... etc)  

General Details:  
1) Error messages of system commands are also shown  