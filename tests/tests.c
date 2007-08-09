#include "tests.h" 

int int_count(int l[]) {
	int x = 0;

	while (l[x] != -2  ) x++;

	return x;
}

int string_count(char *sl[]) {
	int x = 0;
	
	while (sl[x] != NULL) {
		x++;
	}
	
	return x;
}

TEST_RESULT *create_test_result() {
	TEST_RESULT *r = calloc(sizeof(struct _test_result),1); 
	
	clog(CL_DEBUG, "CREATED TEST_RESULT"); 
	r->log = g_ptr_array_new(); 
	
	return r; 
}

void destroy_test_result(TEST_RESULT *r) {
	LOG_LINE *l; 
	GPtrArray *log; 
	
	log = r->log; 
		
	if (r->name != NULL) { 
		free(r->name); 
	}

	while( log->len > 0 ) {	
		l=g_ptr_array_index(log, 0);
			if (l->text != NULL) free(l->text); 
			if (l->file != NULL) free(l->file); 
			if (l->function != NULL) free(l->function);
			free(l); 
		g_ptr_array_remove_index_fast(r->log, 0);	// and finally remove the stale l from the array  
	} 
	
	// destroy the list 
	g_ptr_array_free(r->log, TRUE); 
	
	// and finally the result itself 
	free(r); 	
}


void test_log(TEST_RESULT *r, int level, const char *file, int line, const char *func,  const char *fmt, ...) {
static char buffer1[BUFFSIZE];	
	
	memset(buffer1,0,BUFFSIZE);	// ensure a ZERO filled buffer
	
	LOG_LINE *l = calloc(sizeof(struct log_line), 1);

	va_list ap; 

	va_start(ap, fmt);
		vsnprintf(buffer1, BUFFSIZE, fmt , ap); 
	va_end(ap); 

	r->counts[level]++;		// sort each log entry by level/type 
	r->lines++;			// count all lines. 
		
	time(&l->t);
	l->type = level; 
	l->text = strdup(buffer1);
	l->function = strdup(func); 
	l->line = line;
	l->file = strdup(file); 
	// TODO: allow to write to file. 
	
	claro_log(level, "TEST APP", (char*)file, line, func, "%s", l->text);  
		
	g_ptr_array_add(r->log, l); 
}
