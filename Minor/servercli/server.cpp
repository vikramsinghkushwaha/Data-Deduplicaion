#include<iostream>
// ./server <port>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h> 
#include <fcntl.h>
#include<fstream>
#include<sstream>
#include <map>
#include "../test/listfile.h"
 

#include <sys/stat.h>

#include <sys/sendfile.h>



int main(int argc, char *argv[])
{
    string loc = "/home/vicky/socket/Minor/servercli";
    listfiles(loc);
    int listenfd = 0, connfd = 0,k;
    struct sockaddr_in serv_addr , client; 

    char sendBuff[1025];
    time_t ticks; 

    listenfd = socket(AF_INET, SOCK_STREAM, 0);        
    
    memset(&serv_addr, '0', sizeof(serv_addr));
    memset(sendBuff, '0', sizeof(sendBuff)); 

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);             
    serv_addr.sin_port = htons(atoi(argv[1]));                     

    k = bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));        


    k = listen(listenfd, 10); 

    
    

    // connfd = accept(listenfd, (struct sockaddr*)NULL,NULL);      

    socklen_t client_size = sizeof(sockaddr_in);
    connfd = accept(listenfd, (struct sockaddr*)&client, &client_size);      

    
    printf("connected to the client\n");
    // printf("IP address is: %s\n", inet_ntoa(client.sin_addr));
    // printf("port is: %d\n", (int) ntohs(client.sin_port));
    char buf[100],command[5],filename[20],ext[20],lscommand[20];      
    int size,i,filehandle;
    struct stat obj;
    int already_exits = 0;
    int overwrite_choice = 1;
    char *pos;
    // while loop start for all opreations 
    string client_addr_str = inet_ntoa(client.sin_addr);
    client_addr_str += ".txt";
    cout<< client_addr_str<<endl;
    map<string, string> client_details;
    if( access( ( loc + "/clientdetails/" +client_addr_str).c_str(), F_OK ) != -1 ){
        ifstream ifs { loc + "/clientdetails/" +client_addr_str};
        boost::archive::text_iarchive iarch(ifs);
        iarch >> client_details;
    }
    else{
        fstream filecreating;
        filecreating.open(loc + "/clientdetails/" +client_addr_str,ios::out );
        filecreating.close();
    }



    while(1)                    
    {   
        
        recv(connfd,buf,100,0);
        sscanf(buf,"%s",command);              
//----------- for put command -------------------------//

        if(!strcmp(command,"put"))
        {
            int c = 0, len;
            char *f;
            
            sscanf(buf+strlen(command), "%s", filename);       
            i = 1;
            // check file already exits or not 

            // if( access( filename, F_OK ) != -1 )
            // {
            //     already_exits = 1;
            //     send(connfd,&already_exits,sizeof(int),0);             
            // } 
            // else 
            // {
            //     already_exits = 0;
            //      send(connfd,&already_exits,sizeof(int),0);             
            // }
            // send(connfd,&already_exits,sizeof(int),0);
            // recv(connfd,&overwrite_choice,sizeof(int),0);               

            // case of overwrite 
            // if(already_exits==1 && overwrite_choice == 1)
            // {
            //     filehandle = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 644);        
            //     recv(connfd, &size, sizeof(int), 0);
            //     f = (char*) malloc(size);
            //     recv(connfd, f, size, 0);              
            //     c = write(filehandle, f, size);            
            //     close(filehandle);              
            //     send(connfd, &c, sizeof(int), 0);           

            // }
            // else if(already_exits == 0 && overwrite_choice == 1)           
            // {
                char filename1[20];
                sscanf(buf+strlen(command), "%s", filename1);
                // filename1[strlen(filename1)] ='0';
                // strcat(filename1,inet_ntoa(client.sin_addr));
                recv(connfd, &size, sizeof(int), 0);
                f = (char*) malloc(size);
                recv(connfd, f, size, 0);
                filehandle = open(filename1, O_CREAT | O_EXCL | O_WRONLY, 0666);
                c = write(filehandle, f, size);
                close(filehandle);
                ifstream inFile;
                inFile.open(filename1);
                stringstream strStream;
                strStream << inFile.rdbuf();
                string outString = strStream.str();
                string outputsha = sha256(outString);
                string fname_( filename);
                
                client_details[fname_] = outputsha;


                send(connfd, &c, sizeof(int), 0);
                ofstream ofs {loc + "/clientdetails/" +client_addr_str};
                boost::archive::text_oarchive oarch(ofs);
                oarch << client_details;
            // }
            
            
        } // ending of put option



//--------------------quit command----------------------------------------//
        else if(!strcmp(command, "quit"))
        {
            printf("FTP server quitting..\n");
            i = 1;
            send(connfd, &i, sizeof(int), 0);           
            exit(0);
        }//ending of quit option 

    }
}
