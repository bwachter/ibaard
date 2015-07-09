/**
 * @file edit.c
 * @author Bernd Wachter <bwachter@lart.info>
 * @date 2006-2011
 */

#include "ibaard_cat.h"
#include "ibaard_edit.h"

int edit(char *filename){
  char *editor;

#ifndef _WIN32
  pid_t pid;

  if (getenv("EDITOR") == NULL) editor=strdup("vi");
  else editor=strdup(getenv("EDITOR"));

  if ((pid=fork())==-1){
    return -1;
  }

  if (pid==0){
    execlp("/bin/sh", "sh", "-c", cati(editor, " ", filename, NULL), (char*)NULL);
    exit(-1);
  } else {
    return wait(NULL);
  }
#endif
}
