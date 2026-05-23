#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <sys/types.h>


struct products
{
   char name[100];
   char description[500];
   float price;
   int item_count;
};

int main ()
{
   int fd_server[2]; // Server writes to client
   int fd_client[2]; // Client writes to server
   int client, number = -1, availability = -1;
   int sold_items=0, i, j, got_num = -1;
   int unsuccessful_requests=0, successful_requests=0, total_requests=0;
   int item_requests[20]={0}, sold_item_of[20]={0};
   float total_profit=0.0, product_price=0.0;
   ssize_t nread;  
   
   
   struct products catalog[] =
        { 
           {"Item 0", "description 0", 9.90, 2 },
           {"Item 1", "description 1", 11.90, 2 },
           {"Item 2", "description 2", 8.90, 2 },
           {"Item 3", "description 3", 9.00, 2 },
           {"Item 4", "description 4", 5.00, 2 },
           {"Item 5", "description 5", 7.90, 2 },
           {"Item 6", "description 6", 6.90, 2 },
           {"Item 7", "description 7", 4.90, 2 },
           {"Item 8", "description 8", 3.90, 2 },
           {"Item 9", "description 9", 2.90, 2 },
           {"Item 10", "description 10", 14.40, 2 },
           {"Item 11", "description 11", 30.50, 2 },
           {"Item 12", "description 12", 23.90, 2 },
           {"Item 13", "description 13", 27.60, 2 },
           {"Item 14", "description 14", 18.20, 2 },
           {"Item 15", "description 15", 40.00, 2 },
           {"Item 16", "description 16", 33.90, 2 },
           {"Item 17", "description 17", 26.70, 2 },
           {"Item 18", "description 18", 6.70, 2 },
           {"Item 19", "description 19", 60.50, 2 }
        };
        
   for(j = 0; j < 20; j++)  // Print the products, their description, price and availability
   {  
      printf("%-8s\t", catalog[j].name);
      printf("%-16s\t", catalog[j].description);
      printf("%.2f\t", catalog[j].price);  
      printf("%d\n", catalog[j].item_count);     
   } 

   for (client = 0; client < 5; client++) // For 5 clients
   {
      for(i = 0; i < 10; i++) // For 10 orders each client
      { 
        srand(time(NULL)); // Initialization of random numbers
        
        if (pipe(fd_server) < 0)
        {
           printf("Error number: %d\n", errno);
           perror("Error Description");
           return(-1);
        }
      
        if (pipe(fd_client) < 0)
        {
           printf("Error number: %d\n", errno);
           perror("Error Description");
           return(-2);
        } 
           printf("\n----\n");
           printf("[Client %d] ==> Order %d/10.\n", client+1, i+1);
           printf("----\n\n");
           
           int pid = fork();
           
           if (pid < 0) 
           {
              printf("Error number: %d\n", errno);
              perror("Error Description");
              return(0);
      	   }  
    
     	   if (pid == 0) // Child process / client 
      	   {
              close(fd_client[0]); // Closing fd_client's read 
              close(fd_server[1]); // Closing fd_server's write
              
              number = (rand() % 20); // Random number 0-19 for the product
              
              printf("[Client %d] Wants item number = %d.\n", client+1, number);  
              
              if(write(fd_client[1], &number, sizeof(number)) < 0)
              {  
                 perror("Message from write [Client].");
                 exit(-3);
              }
              else if(write(fd_client[1], &number, sizeof(number)) > 0)
              {
                 printf("\n[Client %d] ==> Number %d written to server.\n", client+1, number);
              }

              printf("[Client %d] ==> Finished writing. Now we read from server\n", client+1);
              
              nread = read(fd_server[0], &availability, sizeof(availability)); 
              // Client reads if availability is 0 or 1 from server 
              if(nread == -1)
              {
                 printf("\n Read error [Client].\n");
                 exit(-4);
              }
              printf("[Client %d] ==> Got an answer for availability = %d.\n", client+1, availability);
              
              close(fd_client[1]); // Closing fd_client's write
             
              if ( availability > 0) // If availability > 0 then there are products available
              {
                 printf("[Client %d] ==> Answer from Server, you can buy the item.\n", client+1);
   
                 nread = read(fd_server[0], &product_price, sizeof(product_price)); // Reads the price of the product
                 if(nread == -1)
                 {
                    printf("\n Read error [Client].\n");
                    exit(-5);
                 }
                 
                 printf("[Client %d] ==> Read price = %.2f.\n", client+1, product_price);
              }
              else // If availability <= 0 then the product is not available
              {    
                 printf("[Client %d] ==> Answer from Server, there are no items available.\n", client+1); // Printing to the client 
              }
              
              close(fd_server[0]); // Closing fd_server's read
              exit(0);
           }
           else // Parent process / server
      	   {  
      	      close(fd_server[0]); // Closing fd_server's read
      	      close(fd_client[1]); // Closing fd_client's write
              
              nread = read(fd_client[0], &got_num, sizeof(got_num)); // Reads the product number
              if(nread == -1) // Read error
              {
                 printf("\n Read error [Server].\n");
                 exit(-6);
              }  

              printf("[Server] ==> Read the client's request. Item number %d.\n", got_num);
              
              total_requests = total_requests + 1; // Total requests.  
              item_requests[got_num]= item_requests[got_num] + 1; // Number of requests for a specific product
              
              if(catalog[got_num].item_count > 0) // If item_count is enough
              {
                 availability = 1;
                 successful_requests = successful_requests + 1; // Total successful requests
                // item_requests[got_num]= item_requests[got_num] + 1; // Number of requests for a specific product
                 sold_item_of[got_num]= sold_item_of[got_num] +1; // Total amount of this product that got sold
                 catalog[got_num].item_count = catalog[got_num].item_count - 1;
                 // We assume that each client is making an order for 1 item
                 printf("[Server] ==> The item is available.\n");     
              }
              else // If item_count is not enough
              {
                 availability = 0; 
                 unsuccessful_requests = unsuccessful_requests + 1; // Total unsuccessful requests
                 printf("[Server] ==> The item is not available.\n");
              }
              
              if(write(fd_server[1], &availability, sizeof(availability)) < 0)
              {  // Sending to client the result of availability (unless it is <0,
               // then we go to if)
                 printf("\nWrite error [Server].\n");
                 exit(-7);
              }
              else
              {
                 printf("[Server] ==> Sent availability = %d to client.\n", availability);
                 sleep(1);
              }
              if(availability == 1) // If the product exists then we send the price to client
              {
                 printf("[Server] ==> Writing price = %.2f to client.\n", catalog[got_num].price);
                 
                 if(write(fd_server[1], &catalog[got_num].price, sizeof(catalog[got_num].price)) < 0)
                 {  
                    perror("Message from write [Client].");
                    exit(-8);
                 }
                 total_profit = total_profit + catalog[got_num].price; // Total profit
                 sold_items = sold_items + 1; // Total sold products         
              }
              
              close(fd_server[1]); // Closing fd_server's write
              close(fd_client[0]); // Closing fd_client's read
              
              sleep(1); // Wait 1 second

              waitpid(pid, NULL, 0); // Waits for client to finish so we don't have multiple clients 
                                // at the same time
              if (total_requests >= 50) // When requests finish, we show the results
              {
                 printf("\n");
                 for ( j = 0; j < 20; j++)
                 {
                    printf("%-16s\t", catalog[j].description);
                    printf("Item requests = %d\t", item_requests[j]);
                    printf("Item sold = %d times\n", sold_item_of[j]);
                 }
                 printf("\nSold items: %d.\n", sold_items);  
                 printf("Total requests: %d.\n", total_requests);
       		 printf("Successful requests: %d.\n", successful_requests);
        	 printf("Unsuccessful requests: %d.\n", unsuccessful_requests);
        	 printf("Total profit: %.2f\n", total_profit); 
       	      } 
      	   }
      	} 
     }  
        return 0; 
   }