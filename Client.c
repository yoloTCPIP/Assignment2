#include "inet.h"

#define BUFSIZE 1024
void menu();
void createFolder();
void createFile();
void CopyCurrFolder();
void CopyAnothFolder();
void deleteFile();
void ViewFile();
void DownloadFile();
void UploadFile();

int main(int argc, char**argv) {  	
 struct sockaddr_in addr, cl_addr;    
 int sockfd, ret, buf;    
 char buffer[BUFSIZE];  
 char choice[10];  
 struct hostent * server;  
 char * serverAddr;  
 sigset_t set1, set2;
 sigfillset(&set1);  
 sigfillset(&set2);
 sigdelset(&set2, SIGINT);	
 sigdelset(&set2, SIGQUIT);
 sigprocmask(SIG_SETMASK, &set1, (void*)0); 
 if (argc < 2) {  
  printf("usage: client < ip address >\n");  
  exit(1);    
 }  
  
 serverAddr = argv[1];   
   
 sockfd = socket(AF_INET, SOCK_STREAM, 0);    
 if (sockfd < 0) {    
  printf("Error creating socket!\n");    
  exit(1);    
 }    
 printf("Socket created...\n");     
  
 memset(&addr, 0, sizeof(addr));    
 addr.sin_family = AF_INET;    
 addr.sin_addr.s_addr = inet_addr(serverAddr);  
 addr.sin_port = SERV_TCP_PORT;       
  
 ret = connect(sockfd, (struct sockaddr *) &addr, sizeof(addr));    
 if (ret < 0) {    
  printf("Error connecting to the server!\n");    
  exit(1);    
 }    
 printf("Connected to the server...\n");    

 memset(buffer, 0, BUFSIZE);  
 do{ 
    menu();
    printf("Please enter the message to sent to server [TYPE /q to EXIT]\n==> ");  
    gets(buffer);
    buf = atoi(buffer);
    switch (buf){
   	case 1:
	    createFolder();
	    break;
   	case 2:
	    createFile();
	    break;
   	case 3:
	    CopyCurrFolder();
	    break;
   	case 4:
	    CopyAnothFolder();
	    break;
   	case 5:
	    deleteFile();
	    break;
   	case 6:
	    ViewFile();
	    break;
   	case 7:
	    DownloadFile();
	    break;
   	case 8:
	    UploadFile();
	    break;
	case 9:
 	    printf("\nThank you! Please visit again...\n\n");    
	    return;
	default:
	    printf("\nInvalid Input\n\n");
    }
    sendto(sockfd, buffer, BUFSIZE, 0, (struct sockaddr *) &addr, sizeof(addr)); 
 
    printf("\n");  
 } while(strcmp(buffer,"/q"));
 
 if (buffer == "/q")
   sigprocmask(SIG_UNBLOCK, &set1, (void*)0); //explain
 close(sockfd);
 fcloseall();    
 return 0;     
}   // for main function

void menu(){
  printf("__________________________________________________\n\n");
  printf("_____   WELCOME TO FILE REPOSITORY SYSTEM  _______\n\n");    
  printf("__________________________________________________\n\n");  
  printf("1. To create a folder. \n");
  printf("2. To create a file. \n");
  printf("3. Copy existing file to current location.\n");
  printf("4. Copy existing file to another directory.\n");
  printf("5. To delete a file.\n");
  printf("6. To list all the file. \n");
  printf("7. To download the file.\n");
  printf("8. To upload the file.\n");
  printf("9. Exit.\n\n");
}

void createFolder(){
 char create_folder_name[30];
 char create_folder_path[30];
 printf("--------- Create New Folder ---------\n\n");
 strcpy(create_folder_path, "/home/esther/Desktop/client/");
 printf("Please enter the folder name  \n==> ");
 gets(create_folder_name);
 strcat(create_folder_path,create_folder_name);
 mkdir(create_folder_path,0700);
 printf("Folder created successfully!");
}

void createFile(){
 FILE *fp;
 char slash[2];
 char create_file_name[30];
 char create_file_path[30];
 char ChooseOptPath_createFile[2];
 char FolderName_createFile[30];
 strcpy(slash, "/");
 strcpy(create_file_path, "/home/esther/Desktop/client/");
 printf("---------Create New File---------\n\n");
 /*Choose path start*/
 printf("=== Choose the Location === \n");
 printf("1. Add to current Location. \n");
 printf("2. Add to another Location. \n");
 printf("Location ==> ");
 gets(ChooseOptPath_createFile);
 
 int Pathbuf;
 Pathbuf = atoi(ChooseOptPath_createFile);
 if (Pathbuf == 1){
   printf("\nEnter File Name  \n==> ");
   gets(create_file_name);
   strcat(create_file_path, create_file_name);
   fp = fopen(create_file_path,"w+");
   sleep(2);
   printf("File created successfully!");
 }
 else if (Pathbuf == 2){
   printf("Enter Folder Name  \n==> ");
   gets(FolderName_createFile);
   strcat(create_file_path, FolderName_createFile);
   printf("Enter File Name  \n==> ");
   gets(create_file_name);
   strcat(create_file_path, slash);
   strcat(create_file_path, create_file_name);
   fp = fopen(create_file_path,"w+");
   sleep(2);
   printf("File created successfully!");
 }
 else{
   printf("Invalid Input ! \nPlease try again.");
 }
}

void CopyCurrFolder(){
 DIR *d;
 struct dirent *dir;
 FILE *fp1, *fp2;
 int bytelength1;
 char ch1;
 char booleanCurr[10];
 char copy_cur_name[30],copy_cur_changfname[30];
 char copy_cur_path[30],copy_cur_path_changfname[30];
 strcpy(booleanCurr, "0");
 strcpy(copy_cur_path, "/home/esther/Desktop/client/");
 printf("---------Copy to Current Directory---------\n\n");
 printf("=== The List of File === \n");
   d = opendir("/home/esther/Desktop/client/");
   if (d){
     while ((dir = readdir(d)) != NULL)
       printf("|   %s\n",dir->d_name);
     printf("====================\n");
     closedir(d);
   }

 printf("Enter file name to be copied \n==>");
 gets(copy_cur_name);
 strcat(copy_cur_path, copy_cur_name);
 if ((fp1 = fopen(copy_cur_path,"r")) == NULL){
   do{
        printf("\nFile cannot be opened ! \nPlease try again.");
	strcpy(booleanCurr, "1");
	break;
   }while(booleanCurr=="1");
 }
 else{
   strcpy(copy_cur_path_changfname, "/home/esther/Desktop/client/");
   printf("\nEnter New File Name \n==>");
   gets(copy_cur_changfname);
   strcat(copy_cur_path_changfname, copy_cur_changfname);
   printf("[Client] Copying %s ...\n",copy_cur_changfname);
   sleep(2);

   fp2 = fopen(copy_cur_path_changfname,"w+");	//open a txt copy the content from fp1   //fp2 is client side
   fseek(fp1, 0L, SEEK_END); 
   bytelength1 = ftell(fp1);
   fseek(fp1, 0L, SEEK_SET); 
   while (bytelength1--){
    	ch1 = fgetc(fp1);  
    	fputc(ch1, fp2);
   }
   printf("File copied successfully!");
 }
}

void CopyAnothFolder(){
 DIR *d;
 struct dirent *dir;
 FILE *fp3, *fp4, *fp5;
 int bytelength2;
 char ch2,slash[2];
 char booleanAnoth[10];
 char copy_anoth_name[30],copy_anoth_changfname[30];
 char copy_anoth_path[30],copy_anoth_path_changfname[30];
 char folderName[30]; 
 strcpy(slash, "/");
 strcpy(booleanAnoth, "0");
 strcpy(copy_anoth_path, "/home/esther/Desktop/client/");
 printf("---------Copy to Another Directory---------\n\n");
 printf("=== The List of File === \n");
   d = opendir("/home/esther/Desktop/client/");
   if (d){
     while ((dir = readdir(d)) != NULL)
       printf("|   %s\n",dir->d_name);
     printf("====================\n");
     closedir(d);
   }

 printf("Enter file name to be copied \n==>");
 gets(copy_anoth_name);
 strcat(copy_anoth_path, copy_anoth_name);
 if ((fp3 = fopen(copy_anoth_path,"r")) == NULL){
   do{
        printf("\nFile does not exist ! \nPlease try again.");
	strcpy(booleanAnoth, "1");
	break;
   }while(booleanAnoth=="1");
 }
 else{
   strcpy(copy_anoth_path_changfname, "/home/esther/Desktop/client/");
   printf("\nEnter the Folder Name to be copy to \n==>");
   gets(folderName);
   strcat(copy_anoth_path_changfname, folderName);
   if ((fp5 = fopen(copy_anoth_path_changfname,"r")) == NULL){
     do{
        printf("Folder does not exist ! \nPlease try again.");
	strcpy(booleanAnoth, "1");
	break;
     }while(booleanAnoth=="1");
   }
   else{
     printf("\nEnter New File Name \n==>");
     gets(copy_anoth_changfname);

     strcat(copy_anoth_path_changfname, slash);
     strcat(copy_anoth_path_changfname, copy_anoth_changfname);
     printf("[Client] Copying %s ...\n",copy_anoth_name);
     sleep(2);
   
     fp4 = fopen(copy_anoth_path_changfname,"w+");	//open a txt copy the content from fp1   //fp2 is client side
     fseek(fp3, 0L, SEEK_END); 
     bytelength2 = ftell(fp3);
     fseek(fp3, 0L, SEEK_SET); 
     while (bytelength2--){
    	ch2 = fgetc(fp3);  
    	fputc(ch2, fp4);
     }
     printf("File copied successfully!");
   }
 }
}

void deleteFile(){
 DIR *d;
 struct dirent *dir;
 FILE * fp6, * fpcheckFpath, * fp6_1;
 int delete_v;
 int Pathbuf;
 char slash[2];
 char file_delete[30];
 char file_delete_path[30];
 char booleanDelete[2];
 char ChooseOptPath_deleteFile[2];
 char FolderName_deleteFile[30];
 strcpy(booleanDelete, "0");
 strcpy(slash, "/");
 printf("\n-------- Delete File ---------\n\n");
 /*Choose path start*/
 printf("=== Choose the Location === \n");
 printf("1. Current Location. \n");
 printf("2. Another Location. \n");
 printf("Location ==> ");
 gets(ChooseOptPath_deleteFile);

 Pathbuf = atoi(ChooseOptPath_deleteFile);
 if (Pathbuf == 1){
   strcpy(file_delete_path, "/home/esther/Desktop/client/");
   printf("===The List of File=== \n");
   d = opendir("/home/esther/Desktop/client/");
   if (d){
     while ((dir = readdir(d)) != NULL)
       printf("|   %s\n",dir->d_name);
     printf("====================\n");
     closedir(d);
   }
   printf("Enter file name to delete the file\n==> ");
   gets(file_delete);
   strcat(file_delete_path, file_delete);
   if ((fp6 = fopen(file_delete_path,"r")) == NULL){// to check the server
     do{
       printf("Folder does not exist ! \nPlease try again.");
       strcpy(booleanDelete, "1");
       break;
     }while(booleanDelete == "1");
   }
   else{
     delete_v = remove(file_delete_path);
     if (delete_v == 0){
       printf("File [%s] from Client is deleting !\n",file_delete);
       sleep(1);
       printf("File deleted successfully!\n");
     }
     else
       printf("Error deleting file!.\n");
   }
 }
 else if (Pathbuf == 2){
   strcpy(file_delete_path, "/home/esther/Desktop/client/");
   printf("Enter Folder Name  \n==> ");
   gets(FolderName_deleteFile);
   strcat(file_delete_path, FolderName_deleteFile);
   if ((fpcheckFpath = fopen(file_delete_path,"r")) == NULL){
     do{
       printf("Folder does not exist ! \nPlease try again.");
       strcpy(booleanDelete, "1");
       break;
     }while(booleanDelete == "1");
   }
   else{
     printf("Enter file name to delete the file\n==> ");
     gets(file_delete);
     strcat(file_delete_path, slash);
     strcat(file_delete_path, file_delete);
     if ((fp6_1 = fopen(file_delete_path,"r")) == NULL){// to check the server
       do{
       puts(file_delete_path);
         printf("File does not exist ! \nPlease try again.");
         strcpy(booleanDelete, "1");
         break;
       }while(booleanDelete == "1");
     }
     else{
       puts(file_delete_path);
       delete_v = remove(file_delete_path);
       if (delete_v == 0){
         printf("File [%s] from Client is deleting !\n",file_delete);
         sleep(1);
         printf("File deleted successfully!\n");
       }
       else
         printf("Error deleting file!.\n");  
     } 
   }
 }
 else
   printf("Invalid Input ! \nPlease try again.");
}

void ViewFile(){
 DIR *d;
 struct dirent *dir;
 FILE * fpView;
 char view_file_path[30], booleanView[2];
 char FolderName_ViewFile[30];
 char ChooseOptPath_ViewFile[30];
 strcpy(booleanView, "0");
 printf("---------List All File---------\n\n");
 /*Choose path start*/
 printf("=== Choose the Locaion === \n");
 printf("1. Current Location. \n");
 printf("2. Another Location. \n ==> ");
 gets(ChooseOptPath_ViewFile);
 printf("\n");
 int bufViewFile;
 bufViewFile = atoi(ChooseOptPath_ViewFile);
 if (bufViewFile == 1){
   strcpy(view_file_path, "/home/esther/Desktop/client/");
   d = opendir(view_file_path);
   if (d){
     while ((dir = readdir(d)) != NULL)
       printf("|   %s\n",dir->d_name);
     closedir(d);
   }
  }
 else if (bufViewFile == 2){
   strcpy(view_file_path, "/home/esther/Desktop/client/");
   printf("Enter Folder Name  \n==> ");
   gets(FolderName_ViewFile);
   printf("\n");
   strcat(view_file_path, FolderName_ViewFile);
   if ((fpView = fopen(view_file_path,"r")) == NULL){	// to check the
     do{
       printf("\nFile cannot be opened ! \nPlease try again.");
       strcpy(booleanView, "1");
       break;
     }while((booleanView=="1"));
   }
   else{
     d = opendir(view_file_path);
     if (d){
       while ((dir = readdir(d)) != NULL)
        printf("|   %s \n",dir->d_name);
       closedir(d);
     }
   }
 }
}

void DownloadFile(){
 DIR *d;
 struct dirent *dir;
 char ch3;
 int bytelength3;
 FILE *fp7, *fp8;
 char download_path_oriname[30], download_path_changname[30];
 char download_file_name[20], download_file_changname[20];
 char booleandowload[2];
 strcpy(booleandowload, "0");

 strcpy(download_path_oriname, "/home/esther/Desktop/server/");
 printf("--------- Download file from server ---------\n\n");
 printf("===The List of File=== \n");
 d = opendir("/home/esther/Desktop/server/");
   if (d){
     while ((dir = readdir(d)) != NULL)
       printf("|   %s\n",dir->d_name);
     printf("====================\n");
     closedir(d);
   }
 printf("Enter file name to download :\n");
 gets(download_file_name);
 strcat(download_path_oriname, download_file_name);
 if ((fp7 = fopen(download_path_oriname,"r")) == NULL){	// to check the server txt exist or not //fp1 is server side
   do{
     printf("\nFile cannot be opened ! \nPlease try again.");
     strcpy(booleandowload, "1");
     break;
   }while((booleandowload=="1"));
 }
 else {	
   strcpy(download_path_changname, "/home/esther/Desktop/client/");
   printf("Download file name change to : \n");
   gets(download_file_changname);
   strcat(download_path_changname,download_file_changname);
   printf("File [%s] downloading from Server !\n",download_file_name);
   sleep(2);
   fp8 = fopen(download_path_changname,"w+");	//open a txt copy the content from fp1   //fp2 is client side
   fseek(fp7, 0L, SEEK_END); 
   bytelength3 = ftell(fp7);
   fseek(fp7, 0L, SEEK_SET); 
   while (bytelength3--){
    	ch3 = fgetc(fp7);  
    	fputc(ch3, fp8);
  }
  printf("File Downloaded successfully!\n");
 }
}


void UploadFile(){
 DIR *d;
 struct dirent *dir;
 char ch4;
 int bytelength4;
 FILE *fp9, *fp10;
 char upload_path_oriname[30], upload_path_changname[30];
 char upload_file_name[30], upload_file_changname[30];
 char booleanupload[2];
 strcpy(booleanupload, "0");

 strcpy(upload_path_oriname, "/home/esther/Desktop/client/");
 printf("--------- Upload file from Client to Server ---------\n\n");
 printf("=== The List of File === \n");
 d = opendir("/home/esther/Desktop/client/");
   if (d){
     while ((dir = readdir(d)) != NULL)
       printf("|   %s\n",dir->d_name);
     printf("====================\n");
     closedir(d);
   }

 printf("Enter file name to upload :\n");
 gets(upload_file_name);
 strcat(upload_path_oriname, upload_file_name);
 if ((fp9 = fopen(upload_path_oriname,"r")) == NULL){	// to check the server txt exist or not //fp1 is server side
   do{
     printf("\nFile does not exist ! \nPlease try again.");
     strcpy(booleanupload, "1");
     break;
   }while((booleanupload=="1"));
 }
 else {	
   strcpy(upload_path_changname, "/home/esther/Desktop/server/");
   printf("Upload file name change to : \n");
   gets(upload_file_changname);
   strcat(upload_path_changname,upload_file_changname);
   printf("File [%s] uploading to Server !\n", upload_file_name);
   sleep(2);
   fp10 = fopen(upload_path_changname,"w+");	//open a txt copy the content from fp1   //fp2 is client side
   fseek(fp9, 0L, SEEK_END); 
   bytelength4 = ftell(fp9);
   fseek(fp9, 0L, SEEK_SET); 
   while (bytelength4--){
    	ch4 = fgetc(fp9);  
    	fputc(ch4, fp10);
  }
  printf("File Uploaded successfully!\n");
 }
}
