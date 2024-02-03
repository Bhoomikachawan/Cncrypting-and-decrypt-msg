#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <openssl/sha.h> 
int gcd(int a, int h)
{
    int temp;
    while (1) {
        temp = a % h;
        if (temp == 0)
            return h;
        a = h;
        h = temp;
    }
}


vector<int>RSA_algorithm(vector<int>v)
{
int p=11;
	int q=7;

	int n=p*q;

	int phi= (p-1)*(q-1);

	int proxy_Pk=2;

	for(int i=2;i<phi;i++)
	{
		if(__gcd(phi,i)==1)
		{
			proxy_Pk=i;
			break;
		}
	}

	int k = 2;

	int inv_pk; // inverse of public key with mod n

    
    for(int i=1;i<n;i++)
    {
    	if((proxy_Pk*i)%n==1)
    	{
    		inv_pk=i;
    		break;
    	}
    }

    int proxy_PrK = (1 + (k * phi))*inv_pk; 


    vector<int>chipher_text(v.size());

    for(int i=0;i<v.size();i++)
    {
    	int temp=  pow(v[i],proxy_Pk);
        temp = fmod(temp, n);

        chipher_text[i]=temp;
    }


}

////////////////////////generating signature

int* signature( int data[])
{
  const int DataLen = 30;
    SHA_CTX shactx;
    byte digest[SHA_DIGEST_LENGTH];

    byte* testdata = new byte[DataLen];
    for (int i=0; i<DataLen; i++) testdata[i] = 0;

    SHA1_Init(&shactx);
    SHA1_Update(&shactx, data, DataLen);
    SHA1_Final(digest, &shactx);
    
    return digest

}


int main(){
  int clientSocket;
  char buffer[1024];
  struct sockaddr_in serverAddr;
  socklen_t addr_size;

  /*---- Create the socket. The three arguments are: ----*/
  /* 1) Internet domain 2) Stream socket 3) Default protocol (TCP in this case) */
  clientSocket = socket(PF_INET, SOCK_STREAM, 0);
  
  /*---- Configure settings of the server address struct ----*/
  /* Address family = Internet */
  serverAddr.sin_family = AF_INET;
  /* Set port number, using htons function to use proper byte order */
  serverAddr.sin_port = htons(7891);
  /* Set IP address to localhost */
  serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  /* Set all bits of the padding field to 0 */
  memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  

  /*---- Connect the socket to the server using the address struct ----*/
  addr_size = sizeof serverAddr;
  connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);

  /*---- Read the message from the server into the buffer ----*/
  recv(clientSocket, buffer, 1024, 0);

  /*---- Print the received message ----*/
  printf("Data received: %s",buffer);
  
  
  
  
  //  sending to userB chipher text recieved by userA
  
  
   vector<int>cipher_text= RSA_algorithm(buffer);
  
  
  int welcomeSocket, newSocket;

  struct sockaddr_in serverAddr;
  struct sockaddr_storage serverStorage;
  socklen_t addr_size;

  /*---- Create the socket. The three arguments are: ----*/
  /* 1) Internet domain 2) Stream socket 3) Default protocol (TCP in this case) */
  welcomeSocket = socket(PF_INET, SOCK_STREAM, 0);
  
  /*---- Configure settings of the server address struct ----*/
  /* Address family = Internet */
  serverAddr.sin_family = AF_INET;
  /* Set port number, using htons function to use proper byte order */
  serverAddr.sin_port = htons(8080);
  /* Set IP address to localhost */
  serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  /* Set all bits of the padding field to 0 */
  memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  

  /*---- Bind the address struct to the socket ----*/
  bind(welcomeSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));

  /*---- Listen on the socket, with 5 max connection requests queued ----*/
  if(listen(welcomeSocket,5)==0)
    printf("Listening\n");
  else
    printf("Error\n");

  /*---- Accept call creates a new socket for the incoming connection ----*/
  addr_size = sizeof serverStorage;
  newSocket = accept(welcomeSocket, (struct sockaddr *) &serverStorage, &addr_size);

  /*---- Send message to the socket of the incoming connection ----*/
  strcpy(buffer,"Hello World\n");
  
  string ciphertext = RSA_algorithm(buffer);
  send(newSocket,buffer,cipher_textsizeof(cipher_text),0);
  
  
  int*  sign = signature(cipherText);
    
   send(newSocket,buffer,cipher_textsizeof(cipher_text),0); 

  return 0;
}
