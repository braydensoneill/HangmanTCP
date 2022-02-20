# Hangman_TCP
Client/server play hangman using TCP

---SERVER---
Open a terminal to act as a server and compile using this command:
  gcc hangserver_iter.c DieWithMessage.c -o hang_server

You can get the server running using this command:
  ./hang_server 4096
  4096 is just an example of a port number, you can use your own

---CLIENT---
Open a second terminal to act as a client and compile using this command:
  gcc hangclient.c DieWithMessage.c -o hang_client
  
You can get the client running using this command:
  ./hang_client 10.0.2.15 4096
  This is an example IP address, use whatever address the server is on (can be found using ifconfig)
  Use whatever port number you assigned to the server previously
  
Open as many terminals as you wish to act as multiple clients playing at the same time
  
  
