void do_source(char *file,  /*the file to be sourced*/
			   char *redir, /*the filename for redirection, if any*/
			   int   back,  /*flag for backgorund execution*/
			   int   reperr); /*report errors in the file open?*/
void do_string(char *instr, char *redir, int back);
void do_str(char *instr);
void do_command(char *name, char *param, int intr, char *file, int back);
