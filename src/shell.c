#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <dirent.h>
#include <fcntl.h>

#include <stdio.h>

#ifndef __WIN32__
// we need param.h to compile on *BSD
#include <sys/param.h>
#include <sys/mount.h>
#endif

#ifdef __GLIBC__
#ifndef MNT_DETACH
#define MNT_DETACH      0x00000002
#endif
#endif

#ifdef __WIN32__
#include <io.h>
#else
#include <unistd.h>
#include <sys/wait.h>
#include <sys/reboot.h>
#endif

#include "ibaard_shell.h"
#include "ibaard_types.h"
#include "ibaard_log.h"
#include "ibaard_fs.h"
#include "ibaard.h"

#ifndef O_LARGEFILE
#define O_LARGEFILE  0100000
#endif

#ifndef __linux
#define MS_MGC_VAL 0
#endif

typedef struct {
    char *name;
    void(*func)(int argc, char **argv);
    int minArgs;
    int maxArgs;
    char *usage;
} ash_cdefs;

struct {
    int fancyprompt;
} ash_settings;

static void ash_cat(int argc, char **argv){
  int i,len,fd;
  char buf[1024];
  for (i=1;i<argc;i++){
    if ((fd=open(argv[i], O_RDONLY))==-1) {
      emsg("cat: ", strerror(errno), "\n", 0);
      break;
    }
    while ((len=read(fd,buf,1024))){
      if (len<0) {
        emsg("cat: ", strerror(errno), "\n", 0);
        close(fd);
        break;
      }
      buf[len]='\0';
      write(1,buf,strlen(buf));
    }
    close(fd);
  }
}

static void ash_cd(int argc, char **argv){
  (void) argc;
  if (chdir(argv[1]))
    emsg("cd: ", strerror(errno), "\n", 0);
}

static void ash_clear(int argc, char **argv){
  (void) argc;
  (void) argv;
  write(1,"\e[H\e[J",6);
}

static void ash_cp(int argc, char **argv){
  int src, dst=0, len;
  char buf[1024];
  if (argc > 3) {
    // in this case last arg as to be a directory.
    // not yet supported.
    emsg("Copying multiple files is not yet supported\n", 0);
    return;
  } else {
    if ((src=open(argv[1],O_RDONLY|O_LARGEFILE))==-1){
      emsg("cp: ", strerror(errno), "\n", 0);
      goto cleanup;
    }
    if ((dst=open(argv[2],O_WRONLY|O_CREAT|O_TRUNC|O_LARGEFILE,0666))==-1){
      emsg("cp: ", strerror(errno), "\n", 0);
      goto cleanup;
    }

    while((len=read(src,buf,1024))){
      if (len<0) {
        emsg("cp: ", strerror(errno), "\n", 0);
        goto cleanup;
      }
      buf[len]='\0';
      write(dst,buf,strlen(buf));
    }
    cleanup:
    close(src);
    close(dst);
  }
}

static void ash_exit(int argc, char **argv){
  (void) argc;
  (void) argv;
  // we handle exit in the while-loop. Just a dummy.
}

// TODO: sorted output
static void ash_ls(int argc, char **argv){
  char *dirname;
  struct dirent *e;
  DIR *d;
  int i;

  for (i=0;i<argc;i++){
    if (argc==1) dirname=".";
    else if (i==0) continue;
    else dirname=argv[i];

    if (argc>2) lmsg(dirname, ":\n", 0);

    if ((d=opendir(dirname))==NULL) {
      if (errno==ENOTDIR) {
        lmsg(dirname, "\n", 0);
        continue;
      } else
        emsg("ls: ", dirname, ": ", strerror(errno), "\n", 0);
      continue;
    }

    while ((e=readdir(d))){
      lmsg(e->d_name, "\n", 0);
    }
    closedir(d);
  }
}

static void ash_mkdir(int argc, char **argv){
  mode_t mode=0777;
  int i;
  for (i=1;i<argc;i++){
    if
#ifdef __WIN32__
      (mkdir(argv[i]))
#else
      (mkdir(argv[i],mode))
#endif
        emsg("mkdir: ", argv[i], ": ", strerror(errno), "\n", 0);
  }
}

static void ash_mv(int argc, char **argv){
  if (argc > 3) {
    // in this case last arg has to be a directory.
    // not yet supported.
    emsg("Moving multiple files is not yet supported\n", 0);
    return;
  } else {
    if (rename(argv[1],argv[2])){
      emsg("mv: ", strerror(errno), "\n", 0);
      return;
    }
  }
}

static void ash_ni(int argc, char **argv){
#ifdef __GNUC__
  (void) argc;
#endif
  emsg(argv[0], ": not yet implemented\n", 0);
}

#ifdef __linux__
static void ash_poweroff(int argc, char **argv){
#ifdef __GNUC__
  (void) argc;
  (void) argv;
#endif
  reboot(RB_POWER_OFF);
}
#endif

static void ash_pwd(int argc, char **argv){
  char *buf=NULL;
#ifdef __GNUC__
  (void) argc;
  (void) argv;
#endif

  lmsg(xgetcwd(buf),"\n",0);
}

#ifdef __linux__
static void ash_reset(int argc, char **argv){
#ifdef __GNUC__
  (void) argc;
  (void) argv;
#endif
  reboot(RB_AUTOBOOT);
}
#endif

static void ash_rm(int argc, char **argv){
  int i;
  for (i=1;i<argc;i++){
    if (unlink(argv[i])){
      emsg("rm: ", argv[i], ": ", strerror(errno), "\n", 0);
    }
  }
}

static void ash_rmdir(int argc, char **argv){
  int i;
  for (i=1;i<argc;i++){
    if (rmdir(argv[i]))
      emsg("rmdir: ", argv[i], ": ", strerror(errno), "\n", 0);
  }
}

#ifndef __WIN32__

static void ash_mount(int argc, char **argv){
  (void) argc;
  if (argv[3]==NULL) return;

  if (mount(argv[1], argv[2], argv[3], MS_MGC_VAL, "")){
    emsg("mount: ", argv[1], " on ", argv[2], " as ", argv[3], " failed: ", strerror(errno), "\n", 0);
  }
}

static void ash_umount(int argc, char **argv){
  (void) argc;

  if (umount(argv[1])) {
#ifdef __linux__
    emsg("umount: umounting ", argv[1], " failed, umounting with MNT_DETACH\n", 0);
    if (umount2(argv[1], MNT_DETACH))
      emsg("umount: unmounting ", argv[1], " failed: ", strerror(errno), "\n", 0);
#else
    emsg("umount: unmounting ", argv[1], " failed: ", strerror(errno), "\n", 0);
#endif
  }
}
#endif

static void ash_set(int argc, char **argv){
  (void) argc;
  (void) argv;
  if (!strcmp(argv[1], "fancyprompt")) {
    if (argv[2]!=NULL) ash_settings.fancyprompt=atoi(argv[2]);
  }
}

// end of command part, start of `management' part
static void ash_help(int argc, char **argv);

static ash_cdefs ash_commands[] = {
  {"cat", ash_cat, 2, 99, "[FILE]... - concatenate files and print on stdout"},
  {"cd", ash_cd, 1, 2, "[DIR] - changes the directory to DIR"},
  {"clear", ash_clear, 1, 1, "- clear the terminal screen"},
  {"cp", ash_cp, 3, 99, "SOURCE DEST - copy SOURCE to DEST"},
  {"dd", ash_ni, 2, 6, "if=<name> of=<name> [bs=n] [count=n] [skip=n] [seek=n]"},
  {"exec", ash_ni, 2, 2, "FILE - replaces the current shell with FILE"},
  {"exit", ash_exit, 1, 1, "- exits the shell"},
  {"help", ash_help, 1, 1, "- print this help"},
  {"ls", ash_ls, 1, 99, "[DIR] - list contents of DIR or current directory"},
  {"mkdir", ash_mkdir, 2, 99, "[DIR]... - creates one or more directories"},
#ifndef __WIN32__
  {"mount", ash_mount, 4, 5, "DEVICE MOUNTPOINT FSTYPE - mount device on mountpoint"},
#endif
  {"mv", ash_mv, 3, 99, "SOURCE DEST - move SOURCE to DEST"},
#ifdef __linux__
  {"poweroff", ash_poweroff, 1, 1, "- switch the system off"},
#endif
  {"pwd", ash_pwd, 1, 1, "- print name of current directory"},
#ifndef __WIN32__
  {"umount", ash_umount, 2, 2, "MOUNTPOINT - umount"},
#endif
#ifdef __linux__
  {"reset", ash_reset, 1, 1, "- reboot the system"},
#endif
  {"rm", ash_rm, 2, 99, "FILE - removes one or more files"},
  {"rmdir", ash_rmdir, 2, 99, "DIR - removes one or more directories"},
  {"set", ash_set, 2, 3, "OPTION VALUE - sets ash-options"},
  {NULL, 0, 0, 0, NULL}
};

static void ash_help(int argc, char **argv){
  ash_cdefs *cdef;
#ifdef __GNUC__
  (void) argc;
  (void) argv;
#endif

  lmsg("AardShell v0.1\n",0);
  lmsg("You can use the following built-in commands:\n\n",0);
  for (cdef = ash_commands; cdef->name; cdef++) {
    lmsg(cdef->name, " ", cdef->usage, "\n", 0);
  }
}

static int ash_shellcmd(char *cmd){
// FIXME fork, wait and execve need te be replaced on windows
  int argc;
  ash_cdefs *cdef;
  char **argv;

  if (cmd[strlen(cmd)-1] == '\n')
    cmd[strlen(cmd)-1] = '\0';

  while(1){
    if (cmd[strlen(cmd)-1] == ' ')
      cmd[strlen(cmd)-1] = '\0';
    else break;
  }

  if (strlen(cmd)==0) return 0;

  //cmd[strlen(cmd)] = '\0';

  // split string into args; FIXME ;)
  argv=split(cmd, ' ', &argc, 0, 0);

  for (cdef = ash_commands; cdef->name != NULL; cdef++) {
    if (strcmp(cdef->name, argv[0]) == 0) break;
  }

  if (cdef->name == NULL) {
    pid_t pid;
    pid = fork();
    if (pid < 0 ) return -1;
    if (pid > 0) {
      wait (NULL);
    } else {
      // FIXME: properly terminate args
      if (execvp(argv[0], argv)==-1)
        emsg(argv[0], ": ", strerror(errno), "\n", 0);
      exit(0);
    }
    return 0;
  }

  if ((argc < cdef->minArgs) || (argc > cdef->maxArgs)){
    emsg("Usage: ", argv[0], " ", cdef->usage, "\n", 0);
    return 0;
  }
  cdef->func(argc, argv);
  return 0;
}

static int ash_read(char buf[512]){
  int i=0;
  if ((i=read(0,buf,512))==-1) return -1;
  buf[i]='\0';
  return 0;
}

int ash_init(char* info){
  char buf[512];

  emsg(info, 0);
  ash_settings.fancyprompt=0;

  while(strcmp(buf,"exit")){
    if (ash_settings.fancyprompt)
      lmsg(xgetcwd(buf), "> ", 0);
    else
      lmsg("> ", 0);
    ash_read(buf);
    ash_shellcmd(buf);
  }
  return 0;
}
