#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

// function to execute a command on a file
void execute(const char *command, const char *file_path, const char *new_name) 
{
  pid_t pid = fork(); // fork new process

  if (pid == 0) 
  { // child process
    char *args[4] = {NULL, NULL, NULL, NULL};

    args[0] = (char *)command;
    args[1] = (char *)file_path;
    
    if (strcmp(command, "mv") == 0 && new_name != NULL) 
    {args[2] = (char *)new_name;}

    execvp(command, args);
    _exit(EXIT_FAILURE); // exit if execvp fails
  } 
  else if (pid < 0) 
  {printf("fork failed");} 
  else 
  { 
    int status;
    waitpid(pid, &status, 0); // parent process waits for child
  }
}

// recursive function to read the subdirectory
void read_sub(char *sub_dir, const char *name, int mmin, ino_t inum, int willDelete, const char *execCmd, const char *newName) 
{
  DIR *sub_dp = opendir(sub_dir); // open a directory stream
  struct dirent *sub_dirp; 
  struct stat buf;

  if (sub_dp != NULL) 
  {
    while ((sub_dirp = readdir(sub_dp)) != NULL) 
    {
      // skip "." and ".."
      if (strcmp(sub_dirp->d_name, ".") == 0 || strcmp(sub_dirp->d_name, "..") == 0)
      {continue;}

      // construct the full path for each entry
      char *temp_full_path = malloc(sizeof(char) * 2000);
      strcpy(temp_full_path, sub_dir);
      strcat(temp_full_path, "/");
      strcat(temp_full_path, sub_dirp->d_name);

      // try opening path as a directory
      DIR *subsubdp = opendir(temp_full_path);
      
      if (subsubdp != NULL)
      { // if its a directory
        closedir(subsubdp); // close directory stream
        read_sub(temp_full_path, name, mmin, inum, willDelete, execCmd, newName); // recursively read directory
      } 
      else
      { // if its a file, get file info
        if (stat(temp_full_path, &buf) != 0)
        {
          free(temp_full_path);
          continue;
        }

        // check if file matches specified criteria
        int nameMatch = (name == NULL) || strstr(sub_dirp->d_name, name);
        int timeMatch = (mmin == -1) || (mmin > 0 && (difftime(time(NULL), buf.st_mtime) / 60 == mmin)) || (mmin < 0 && (difftime(time(NULL), buf.st_mtime) / 60 <= -mmin));
        int inodeMatch = (inum == 0) || (buf.st_ino == inum);
        
        if (nameMatch && timeMatch && inodeMatch)
        {
           printf("%s\n", temp_full_path); // print the file path
           
          if (willDelete) 
          {remove(temp_full_path);}
                    
          if (execCmd != NULL) 
          {
            if (strcmp(execCmd, "mv") == 0) 
            {execute(execCmd, temp_full_path, newName);} // for 'mv', use newName
            else 
            {execute(execCmd, temp_full_path, NULL);} // for other commands, NULL
          }
        }
      }
      free(temp_full_path); // free memory allocated for path
    }
    closedir(sub_dp); // close directory stream
  } 
  else 
  {printf("cannot open directory: %s\n", sub_dir);}
}

int main(int argc, char **argv) 
{
  char *dir = ".";
  char *name = NULL;
  int mmin = -1;
  ino_t inum = 0;
  int willDelete = 0;
  char *execCmd = NULL;
  char *newName = NULL; // variable for new name in 'mv' command
  int i;
	  
  // parse arguments
  for (i = 1; i < argc; i++) 
  {
    if (strcmp(argv[i], "-name") == 0) 
    {name = argv[++i];} 
    else if (strcmp(argv[i], "-mmin") == 0) 
    {mmin = atoi(argv[++i]);} 
    else if (strcmp(argv[i], "-inum") == 0) 
    {inum = (ino_t)atoll(argv[++i]);} 
    else if (strcmp(argv[i], "-delete") == 0) 
    {willDelete = 1;} 
    else if (strcmp(argv[i], "-exec") == 0) 
    {
      execCmd = argv[++i];
      // check if next argument is mv command
      if (strcmp(execCmd, "mv") == 0 && i + 1 < argc) 
      {newName = argv[++i];}  // get newName for 'mv'
    } 
    else 
    {dir = argv[i];} // set directory to search
  }

  read_sub(dir, name, mmin, inum, willDelete, execCmd, newName); // begin processing directory
  return 0; 
}
