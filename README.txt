Run this command in terminal to compile your source
	gcc os.c -o bt
	
To create a object file type "Make" and enter

*** make sure you have a gcc compiler to compile your source file ***

To execution run following command in terminal
	./bt [OPTION][DIRNAME]
	
For help screen "./bt -h"

Usage: filename [-h] [-L -d -g -i -p -s -t -u | -l] [dirname]\n\
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

Version Control : /classes/OS/madiredd/maddiredd.1/log
