Details:  
Specification 2:  
    MAX value for input lenght and most other stuff is set to 4096  
    if using semicolon in a command, make sure to leave a space  
        echo hello ; reveal ..  # valid command  
        echo hello;reveal ..  # invalid command  
    same thing with ampersands  
        sleep 5 & echo hello    # valid command  
        sleep 5&echo hello    # invalid command  
    implemented commands like hop, reveal, ...etc can also be run in background with '&'.  
Specification 3:  
    the previous directory '-' is left undefined at the beginning of each shell session.  
    if multiple directory, hop changes directory sequentially, ultimately ending up at the last directory  
    In case of a command such as 'hop t1 -' , the previous directory is NOT redefined to t1  
Specification 5:  
    erroneous commands are also stored in log.  
    history file will be created at the home directory if not present already.  
    history file is preserved across different shell sessions.  
    If you run a.out from a different directory (different home), a different history file will be maintained.  
    commands containing log will not be stored in history.  
Specification 8:  
    seek does not go through symlinks.  
    seek does not take in '-' like hop does.

                    

Instructions:
to compile, run make

