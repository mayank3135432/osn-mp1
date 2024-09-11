#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "constants.h"
#include "iman.h"


char* iman_UTIL(const char *command) {
    const char* host = "man.he.net";
    int port = 80;
    struct hostent *server;
    struct sockaddr_in server_addr;
    char request[BUFFER_SIZE];
    char response[BUFFER_SIZE];
    int sockfd, bytes_received;
    char* manual = (char*)malloc(BUFFER_SIZE*sizeof(char));
    manual[0]=0;
    // Resolve hostname to IP address
    server = gethostbyname(host);
    if (server == NULL) {
        fprintf(stderr, "Error: No such host found.\n");
        exit(EXIT_FAILURE);
    }

    //make socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        fprintf(stderr, ""RED"Error opening socket"RESET"");
        exit(EXIT_FAILURE);
    }

    //server address struct
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    memcpy(&server_addr.sin_addr.s_addr, server->h_addr, server->h_length);

    // Connect to the server
    if (connect(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        fprintf(stderr, ""RED"Error connecting to server"RESET"");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Request for man page
    snprintf(request, sizeof(request),
             "GET /?topic=%s&section=all HTTP/1.1\r\n"
             "Host: %s\r\n"
             "User-Agent: iMan/1.0\r\n"
             "Connection: close\r\n\r\n", command, host);

    // Send the HTTP request
    if (send(sockfd, request, strlen(request), 0) < 0) {
        fprintf(stderr, ""RED"Error sending request"RESET"");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Read the response and print after skipping HTTP headers
    int header_end = 0;
    while ((bytes_received = recv(sockfd, response, sizeof(response) - 1, 0)) > 0) {
        response[bytes_received] = '\0'; // Null-terminate the response
        if (!header_end){
            //printf("%s", response);
            strcat(manual, response);
        }
    }

    if (bytes_received < 0) {
        fprintf(stderr, ""RED"Error reading from socket"RESET"");
    }

    // Close the socket
    close(sockfd);
    //printf("%s",manual);
    return manual;
}
char* filterstuff(char* haystack, char* needle){
    char* whatyouwant = (char*)malloc(BUFFER_SIZE*sizeof(char));
    whatyouwant[0]=0;
    
    whatyouwant = strstr(haystack, needle);
    if(whatyouwant == NULL){
        return NULL;
    }
    //printf("thatis: %d\n", whatyouwant);
    return(whatyouwant+strlen(needle));
}
int iman(char** tokens){
    if(tokens[1]==NULL){
        fprintf(stderr, ""RED"iman: no input"RESET"\n");
        return 1;
    }
    char* X = iman_UTIL(tokens[1]);
    char* Y = filterstuff(X, "\n\n");
    printf("%s\n",Y);
    return 0;
}

