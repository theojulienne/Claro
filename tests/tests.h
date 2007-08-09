#ifndef __TESTS_H 
	#define __TESTS_H 
	
#include <stdio.h>
#include <stdlib.h>  
#include <stdarg.h>
#include <string.h> 

#include <sys/stat.h>	 
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <glib.h> 
 
#include <claro/base.h>
#include <claro/graphics.h>

#define BUFFSIZE  				512		// for tlog function
#define tlog(l,f,args...)		test_log(r,l,__FILE__,__LINE__,__FUNCTION__,f, ## args )
	
enum {
	TF_NONE, 
	TF_SKIP, 
};

enum {
	TR_ERROR 			= -1,
	TR_SKIPPED 		= 0,  
	TR_OK			= 1,
};

enum {
	TL_ALL = 0,
	TL_DEBUG, 
	TL_INFO, 
	TL_WARNING, 
	TL_ERROR, 
	TL_CRITICAL, 
	TL_LAST,
};

typedef struct _test_result TEST_RESULT; 
struct _test_result {
	char *name; 
	int result; 					// -1 errors, 0 skipped, 1 ok

		 
	
	int level; 					// debug level 
	int lines; 
	int counts[TL_LAST];
		
	GPtrArray *log;
 
}; 

typedef struct log_line LOG_LINE; 
struct log_line {
	int type;			

	time_t t; 
 	 
	char *file;
	char *function; 
	int line; 

	char *text; 
};

int			int_count(int l[]);
int			string_count(char *sl[]); 

TEST_RESULT 	*create_test_result();
void			destroy_test_result(); 

void			test_log(TEST_RESULT *r, int level, const char *file, int line, const char *func,  const char *fmt, ...); 


#endif // __TESTS_H
