# Concurrent remote compiler with C programming language

Features
  This project compile c programs remotly

  Structure called "program" to save program information as:
        Filename
        Source path
        Path
        Response
        Status
  
  Structure called "requests" to save request information as:
        User id
        Request Order
        
  The structure is shared by shared memory technique with the follow process:
       Main
       Server
       Compiler
       
  The main process forks 2 times, and your childs are:
      Server
      Compiler
  
  The compiler child process has 2 threads:
      Compiler thread
      Runner thread
   
   The compiler thread takes care of compile remote programs and saves the state in the structure "program"
   
   The runner thread takes care of execute remote programs and saves the state in the structure "program"
   
   The server accepts connections with sockets techniques in concurrent way with threads
   
   The write process in the structure is sinchronized by semaphores techniques
   
   All programs that are sent remotly are saves in local disk, compiled and executed locally
   
   The client sent commands to the server to do the actions
   
   The client can sent program source files to compile remotly



Bulding
  - cd src
  - make
 
 Running
  - cd src
  - ./run
  - ./client
Testing
  - cd src
  - ./test <client numbers>
  - killall -s SIGALRM client
