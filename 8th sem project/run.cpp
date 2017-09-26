 #include <windows.h>
  #include <wininet.h>
  #include <iostream>


  #define ERROR_OPEN_FILE       10
  #define ERROR_MEMORY          11
  #define ERROR_SIZE            12
  #define ERROR_INTERNET_OPEN   13
  #define ERROR_INTERNET_CONN   14
  #define ERROR_INTERNET_REQ    15
  #define ERROR_INTERNET_SEND   16

  using namespace std;

  int main()
  {
     // Local variables
     static char *filename   = "test.txt";   //Filename to be loaded
     static char *type       = "image/jpg";
     static char boundary[]  = "pippo";            //Header boundary
     static char nameForm[]  = "uploadedfile";     //Input form name
     static char iaddr[]     = "localhost";        //IP address
     static char url[]       = "phh.php";         //URL

     char hdrs[255];                  //Headers
     char * buffer;                   //Buffer containing file + headers
     char * content;                  //Buffer containing file
     FILE * pFile;                    //File pointer
     long lSize;                      //File size
     size_t result;                   


     // Open file
     pFile = fopen ( filename , "rb" );
     if (pFile==NULL) return ERROR_OPEN_FILE;

     // obtain file size:
     fseek (pFile , 0 , SEEK_END);
     lSize = ftell (pFile);
     rewind (pFile);

     // allocate memory to contain the whole file:
     content = (char*) malloc (sizeof(char)*lSize);
     if (content == NULL) return ERROR_MEMORY;

     // copy the file into the buffer:
     result = fread (content,1,lSize,pFile);
     if (result != lSize) return ERROR_SIZE;

     // terminate
     fclose (pFile);

     //allocate memory to contain the whole file + HEADER
     buffer = (char*) malloc (sizeof(char)*lSize + 2048);

     //print header
     sprintf(hdrs,"Content-Type: multipart/form-data; boundary=%s",boundary);
     sprintf(buffer,"--%s\r\nContent-Disposition: form-data; name=\"%s\"; filename=\"%s\"\r\n",boundary,nameForm,filename);
     sprintf(buffer,"%sContent-Type: %s\r\n\r\n",buffer,type);
     sprintf(buffer,"%s%s\r\n",buffer,content);
     sprintf(buffer,"%s--%s--\r\n",buffer,boundary);

     //Open internet connection
     HINTERNET hSession = InternetOpen("WinSock",INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
     if(hSession==NULL) return ERROR_INTERNET_OPEN;

     HINTERNET hConnect = InternetConnect(hSession, iaddr,INTERNET_DEFAULT_HTTP_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 1);
     if(hConnect==NULL) return ERROR_INTERNET_CONN;

     HINTERNET hRequest = HttpOpenRequest(hConnect, (const char*)"POST",url, NULL, NULL, (const char**)"*/*\0", 0, 1);
     if(hRequest==NULL) return ERROR_INTERNET_REQ;

     BOOL sent= HttpSendRequest(hRequest, hdrs, strlen(hdrs), buffer, strlen(buffer));
     if(!sent) return ERROR_INTERNET_SEND;

     //close any valid internet-handles
     InternetCloseHandle(hSession);
     InternetCloseHandle(hConnect);
     InternetCloseHandle(hRequest);

     return 0;
  }
