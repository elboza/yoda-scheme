/*
 * author: Fernando Iazeolla
 * license: GPLv2
 */
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<stdarg.h>
#include<getopt.h>
#include "main.h"
#include "object.h"
#include "stream.h"
#include "read.h"
#include "env.h"
#include "eval.h"
#include "print.h"
#include "repl.h"


struct m_action{
	int file;
	int shell;
	int stdin;
	int exec;
};
void log_d(char *s){
	if(!s) return;
	if(!DEBUG) return;
	printf("%s\n",s);
}
void die(char *s){
	printf("%s\n",s);
	exit(1);
}
void reset_actions(struct m_action *action){
	action->shell=0;
	action->file=0;
	action->stdin=0;
	action->exec=0;
}
void usage()
{
	printf("%s v%s\n",PROGNAME,VERSION);
	printf("\nUSAGE: ");
	printf("%s [options] [file]\n",TARGETNAME);
	printf("valid options:\n");
	printf("-i              --shell         interactive (shell mode)\n");
	printf("-h              --help          show this help\n");
	printf("-v              --version       prints scheme version number\n");
	printf("-s              --stdin         read prog from stdin\n");
	printf("-f file         --file          read file\n");
	printf("-x 'prog'       --exec          read & execute prog\n");
	exit(1);
}
void usage_b()
{
	printf("%s v%s (c) Fernando Iazeolla \n",PROGNAME,VERSION);
	printf("for help type: %s --help\n",TARGETNAME);
}
void parse_args(int argc,char **argv,struct m_action *action,stream_t *stream)
{
	int c;
	reset_actions(action);
	while (1)
	{
		static struct option long_options[] =
		{
			{"exec",required_argument,0,'x'},
			{"shell",no_argument,0,'i'},
			{"help",no_argument,0,'h'},
			{"version",no_argument,0,'v'},
			{"stdin",no_argument,0,'s'},
			{"file",required_argument,0,'f'},
			{0,0,0,0,}
			
		};
		int option_index = 0;
		c = getopt_long (argc, argv, "x:ihvsf:",long_options, &option_index);
		if (c == -1) break;
		switch(c)
		{
			case 'i':
				action->shell=1;
				break;
			case 'v':
				usage_b();
				break;
			case 's':
				action->stdin=1;
				break;
			case 'f':
				action->file=1;
				stream->filename=optarg;
				break;
			case 'x':
				action->exec=1;
				stream->ptr=optarg;
				break;
			case 'h':
			case '?':
				usage();
				break;
			default:
				usage_b();
				break;
		}
		
	}
}
void shell(stream_t *s){
	printf("entering interactive-shell mode... ");
	printf("(type :h for help).\n");
	repl(s);
}

int main(int argc,char **argv)
{
	struct m_action action;
	stream_t stream;
	object_t *exp;
	init_stream(&stream);
	parse_args(argc,argv,&action,&stream);
	init_env();
	if(argc<2) usage_b();
	if(optind<argc) {
		if(action.file){
			printf("argument incongruence input prog. see %s -h\n",TARGETNAME);
			reset_actions(&action);
		}
		stream.filename=argv[optind];
		action.file=1;
	}
	if(action.stdin+action.file+action.shell+action.exec>1){
		printf("argument incongruence input prog. see %s -h\n",TARGETNAME);
		reset_actions(&action);
	}
	
	if(action.exec){
		open_stream(&stream,stream.ptr,TSTREAM_STR);
		while (!is_eof_object(exp = read_sx(&stream))) {
			//		exp = read(stream);
			//write_sx(NULL, eval(exp, the_global_environment));
			eval(exp, the_global_environment);
		}
	}
	if(action.stdin){
		open_stream(&stream,NULL,TSTREAM_STDIN);
		while (!is_eof_object(exp = read_sx(&stream))) {
			//		exp = read(stream);
			//write_sx(NULL, eval(exp, the_global_environment));
			eval(exp, the_global_environment);
		}
	}
	if(action.file)
	{
		printf("file: %s\n",stream.filename);
		open_stream(&stream,stream.filename,TSTREAM_FILE);
		while (!is_eof_object(exp = read_sx(&stream))) {
			//		exp = read(stream);
			//write_sx(NULL, eval(exp, the_global_environment));
			eval(exp, the_global_environment);
		}
	}
	if(action.shell)
	{
		shell(&stream);
	}
	
	close_stream(&stream);
	log_d("Bye.");
	return 0;
}
