To Run :- = gcc -pthread q1.c
		  = ./a.out
          = Enter input "n"



Q1 :
   tnumpl = total number of players left , tnumref = total number of refree left...

   then using the ratio of tnumpl and tnumpl + tnumref ... it calculates which is more..and then using random number generator 
   one with more probability and less probability is chosen

   then to make changes in tnumpl,tnumref.. mutex lock is used
   and a thread is created for a player or a refree

   at last pthread_join is used to wait for the execution of the thread.
   after generating a player and a refree thread .. they enter Academy ..
   after that .. 
 
   plbuf(player) && refbuf(refree) is incremented ..

   then in meet organiser till there are less than 2 player and 1 refree waiting .. nothing happens.
   if there are 2 p and 1 r then they wait till organizer is free ( this is implemented using pthread_cond_t which is signaled when game gets completed. ) 

   warmup takes 1sec
   adjustEquipment takes 0.5 sec ( so I used usleep(500) as sleep() takes input as int)

   