#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>
#define MAX 10000

bool help;
bool L, D, G, I, P, S, T, U;
bool l;
char dirname[1024] = ".";
char execname[1024];

void parse(int argc, char* argv[]) {
	char str[128];
	strcpy(execname, argv[0]);
	help = L = D = G = I = P = S = T = U = false;
	l = false;
	char c;
	while ((c = getopt (argc, argv, "hLdgipstul:")) != -1)
		switch (c) {
			case 'h':
				help = true;
				break;
			case 'L':
				L = true;
				break;
			case 'd':
				D = true;
				break;
			case 'g':
				G = true;
				break;
			case 'i':
				I = true;
				break;
			case 'p':
				P = true;
				break;
			case 's':
				S = true;
				break;
			case 't':
				T = true;
				break;
			case 'u':
				U = true;
				break;
			case 'l':
				l = true;
				break;
			default:
				sprintf(str, "%s Error", execname);
				perror(str);
		}
	if (optind < argc) strcpy(dirname, argv[optind]);
	int i;
	for (i = 1; i < argc; i++) {
		if (argv[i][0] != '-') {
			strcpy(dirname, argv[i]);
			continue;
		}
		if (argv[i][1] == 'h') help = true;
		if (argv[i][1] == 'L') L = true;
		if (argv[i][1] == 'd') D = true;
		if (argv[i][1] == 'g') G = true;
		if (argv[i][1] == 'i') I = true;
		if (argv[i][1] == 'p') P = true;
		if (argv[i][1] == 's') S = true;
		if (argv[i][1] == 't') T = true;
		if (argv[i][1] == 'u') U = true;
		if (argv[i][1] == 'l') l = true;
	}
}

char* list[MAX];
int hd, tl;

void insert(char str[]) {
	list[tl] = malloc(strlen(str) * sizeof(char));
	strcpy(list[tl++], str);
}
char* pop() {
	return list[hd++];
}

char* formatdate(char* str, time_t val)
{
        strftime(str, 36, "%d.%m.%Y %H:%M:%S", localtime(&val));
        return str;
}

char* permissions(char *file) {
    struct stat st;
    char *modeval = malloc(sizeof(char) * 9 + 1);
    stat(file, &st);
    mode_t perm = st.st_mode;
    modeval[0] = (perm & S_IRUSR) ? 'r' : '-';
    modeval[1] = (perm & S_IWUSR) ? 'w' : '-';
    modeval[2] = (perm & S_IXUSR) ? 'x' : '-';
    modeval[3] = (perm & S_IRGRP) ? 'r' : '-';
    modeval[4] = (perm & S_IWGRP) ? 'w' : '-';
    modeval[5] = (perm & S_IXGRP) ? 'x' : '-';
    modeval[6] = (perm & S_IROTH) ? 'r' : '-';
    modeval[7] = (perm & S_IWOTH) ? 'w' : '-';
    modeval[8] = (perm & S_IXOTH) ? 'x' : '-';
    modeval[9] = '\0';
    return modeval;
}

char* type(char* file) {
	struct stat st;
	stat(file, &st);
	if (S_ISBLK(st.st_mode)) return "block";
	if (S_ISCHR(st.st_mode)) return "character";
	if (S_ISDIR(st.st_mode)) return "directory";
	if (S_ISFIFO(st.st_mode)) return "pipe or FIFO";
	if (S_ISLNK(st.st_mode)) return "symbolic link";
	if (S_ISREG(st.st_mode)) return "regular";
	if (S_ISSOCK(st.st_mode)) return "socket";
	return "file";
}

short cntLink(char *file) {
    struct stat st;
    stat(file, &st);
    return st.st_nlink;
}

short getUID(char *file) {
    struct stat st;
    stat(file, &st);
    return st.st_uid;
}

short getGID(char *file) {
    struct stat st;
    stat(file, &st);
    return st.st_gid;
}
char* getSize(char *file) {
	struct stat st;
    stat(file, &st);
	char* ret;
	char suffix = '\0';
	ret = malloc(sizeof(char) * 128);
	int sz = st.st_size;
	if (sz > 1024) {
		suffix = 'K';
		sz /= 1024;
	}
	if (sz > 1024) {
		suffix = 'M';
		sz /= 1024;
	}
	if (sz > 1024) {
		suffix = 'G';
		sz /= 1024;
	}
	sprintf(ret, "%d%c", sz, suffix);
	return ret;
}

char* getModf(char* path) {
	struct stat st;
	char *ret;
	ret = malloc(sizeof(char)*128);
    stat(path, &st);
	return formatdate(ret, st.st_mtime);
}

void traverse(char *name)
{
	struct stat st;
	if (stat(name, &st) == -1) {
		char str[128];
		sprintf(str, "%s Error", execname);
		perror(str);
		return;
	}
	hd = tl = 0;
	insert(name);
	while (hd < tl) {
		char* dname = pop();
	    DIR *dir;
	    struct dirent *entry;
	    if (!(dir = opendir(dname))) {
	        continue;
		}

	    while ((entry = readdir(dir)) != NULL) {
		    char path[1024];
		    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
		        continue;
		    snprintf(path, sizeof(path), "%s/%s", dname, entry->d_name);
		    insert(path);
			if (l) {
				printf("%s ", permissions(path));
				printf("%d ", cntLink(path));
				printf("%d ", getUID(path));
				printf("%d ", getGID(path));
				printf("%s ", getSize(path));
				printf("%s ", getModf(path));
			} else {
				if (T) printf("%s ", type(path));
				if (P) printf("%s ", permissions(path));
				if (I) printf("%d ", cntLink(path));
				if (U) printf("%d ", getUID(path));
				if (G) printf("%d ", getGID(path));
				if (S) printf("%s ", getSize(path));
				if (D) printf("%s ", getModf(path));
			}
		    printf("%s\n", path);
	    }
	    closedir(dir);
	}
}

int main(int argc, char* argv[]) {
	parse(argc, argv);
	if (help) {
		printf("Usage: filename [-h] [-L -d -g -i -p -s -t -u | -l] [dirname]\n\
Traversing Directories by using Breadth First Search\n\
\n\
	The options are to be interpreted as follows:\n\
	 h 			: Print a help message and exit.\n\
	 L 			: Follow symbolic links, if any. Default will be to not follow symbolic links.\n\
	 t 			: Print information on file type.\n\
	 p 			: Print permission bits as rwxrwxrwx. \n\
	 i			: Print the number of links to file in inode table. \n\
	 u			: Print the UID associated with the file. \n\
	 g			: Print the GID associated with the file.\n\
	 s			: Print the size of file in bytes. If the size is larger than 1K, indicate the size in KB with a suffix K; if the size is larger than 1M, indicate the size in MB with a suffix M; if the size is larger than 1G, indicate the size in GB with a suffix G\n\
	 d			: Show the time of last modification\n\
	 l			: This option will be used to print information on the file as if the options tpiugs are all specified.\n\
	 dirname	: Directory name to explore. If the user does not specify dirname, run the command using current directory\n\
");
		return 0;
	}
	if (L) {
		char buff[1024];
		ssize_t len = readlink(dirname, buff, sizeof(buff)-1);
		if (len != -1) {
			buff[len] = '\0';
		} else {
			char str[128];
			sprintf(str, "%s Error", execname);
			perror(str);
			return 0;
		}
		strcpy(dirname, buff);
	}
	traverse(dirname);
	return 0;
}
