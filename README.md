# Command to copy files within server: 
            
    cp /stu/dulichne20/os/fileName fileDestination


# SCP - Copy files from server to your computer

In CMD: 
   
   From Server:  scp username@mopsdev.bw.edu:\~/fileName ./Desktop/ 
   
   To Server:  scp ./Dektop/fileName username@mopsdev.bw.edu:~/       

# Test network connection

Receive : nc -l portNum   (Remember to only use your port number)
Send : nc localhost portNum   (Remember to only use your port number)

Sonny - PortNumber = 1089
Kaoutar - PortNumber = 1080
Dennis - PortNumber = 1081
