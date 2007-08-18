// This tests the functionality of claro_list. 
//	TEST 1: CREATION		(claro_list_create())
//	TEST 1: APPENDING	(claro_list_append(list,s))	 
//	TEST 2: REMOVING 	(claro_list_remove(list,s))				 
//	TEST 3: CHANGING 	(claro_list_set_item(list, x, s))
//	TEST 4: INSERTING	(claro_list_insert(list,x,s) 
// 
//
// CLARO_LIST_GET_ITEM(list,x) && CLARO_LIST_COUNT(list) 
//	These are tested throughout all the above tests.   If these didn't work,
//	none of the other tests would work.
//
// Functions not implemented yet in claro_list.c but is set to test here when 
// these functions get implemented. 
//    
// INSERT TEST
// 	change the flag in the tests[] structure from TF_SKIP to TF_NONE or 0 (both will do) to enable this test.   
//	Insert test assumes claro_list_insert(list,index,string) will be defined.
//
//	*NOTE*	This insert test has not been tested!  If this test is enabled, expect 
//				to have bugs. 
//

#include "tests.h"

#define FILL_LIST(s)		fill_list(r, s, list)	
#define COMPARE_LIST(s)		compare_list(r, s, list)
#define DUMP_LIST(l)		dump_list(r, l) 	


TEST_RESULT *test_claro_list_create(void);
TEST_RESULT *test_claro_list_append(void);
TEST_RESULT *test_claro_list_set_item(void);
TEST_RESULT *test_claro_list_remove(void);
TEST_RESULT *test_claro_list_insert(void);

enum {
	TEST_CREATE = 0, 
	TEST_APPEND, 
	TEST_SET, 
	TEST_REMOVE, 
	TEST_INSERT, 
	TEST_LAST, 
};

struct {
	int type; 
	char *name;
	
	int f; 
	
	TEST_RESULT *(*func)(void);
	TEST_RESULT *result;
} tests[] = {
	{ TEST_CREATE,		"claro_list_create()",	TF_NONE,	test_claro_list_create	, NULL }, 
	{ TEST_APPEND,		"claro_list_append()",	TF_NONE,	test_claro_list_append	, NULL }, 
	{ TEST_SET,			"claro_list_set()",			TF_NONE,	test_claro_list_set_item	, NULL },
	{ TEST_REMOVE,		"claro_list_remove()",	TF_NONE,	test_claro_list_remove	, NULL },
	{ TEST_INSERT, 	"claro_list_insert()",	TF_NONE,	test_claro_list_insert	, NULL },
	{ TEST_LAST,		NULL, 								TF_SKIP, 	NULL									, NULL },
};


char *raw_list[] = {  
			"abc",
			"def",
			"ghi",
			"jkl",
			"mno",	
			"pqr",	
			"stu",	
			"vwx",	
			"yz",
			"somestringa",	
			"somestringb",	
			"somestringc",	
			NULL 
		};


static void fill_list(TEST_RESULT *r, char *source[], claro_list_t *list) {
	int x = 0; 
	
	tlog(TL_DEBUG, "BEGIN %s()", __FUNCTION__);
		
	tlog(TL_DEBUG, "Source list Count: %d  LIST Count: %d", string_count(source), claro_list_count(list)); 	
	while(source[x]!=NULL) {
		tlog(TL_DEBUG, "APPENDING %s", source[x]);
		 
		claro_list_append(list, source[x]);
		x++;
	}
	
	tlog(TL_DEBUG, "END %s()", __FUNCTION__); 
}

static int compare_list(TEST_RESULT *r, char *source[], claro_list_t *list) {
	int x=0, t=0;
	char *s; 
	tlog(TL_DEBUG, "BEGIN %s()", __FUNCTION__);
	tlog(TL_DEBUG, "SOURCE.count = %d LIST.count = %d", string_count(source), claro_list_count(list)); 
	
	x=0; 
	while(source[x] != NULL) { 
		s = claro_list_get_item(list,x); 

		t=strncmp(source[x],s, strlen(source[x]));
  
		tlog(t==0 ? TL_DEBUG : TL_ERROR, "R(%3d) = %-15s %s L(%3d) = %-15s",	x, source[x], t == 0 ? "==" : "!=" , x, s);
		
		x++;
	}
	
	tlog(TL_DEBUG, "SOURCE %s LIST. (%d)", r->counts[TL_ERROR] > 0 ? "!=" : "==", r->counts[TL_ERROR]);  
	tlog(TL_DEBUG, "END %s()", __FUNCTION__);
	
	return r->counts[TL_ERROR] == 0;
} 

static void dump_list(TEST_RESULT *r, claro_list_t *list) {
	int x; 
	int c = claro_list_count(list); 
	
	tlog(TL_DEBUG, "BEGIN %s()", __FUNCTION__);
	
	tlog(TL_DEBUG, "CURRENT LIST: COUNT=%d",c);
	 
	if (c >0 )
		for(x=0; x < c;x++) 
			tlog(TL_DEBUG, "LIST[%d] = '%s'" ,x, claro_list_get_item(list, x));   

	tlog(TL_DEBUG, "END %s()", __FUNCTION__);	
}


TEST_RESULT *test_claro_list_create() {
	TEST_RESULT *r; 
 
	claro_list_t *list = NULL;
		
	r = tests[TEST_CREATE].result = create_test_result();

	tlog(TL_DEBUG, "BEGIN TEST %s", __FUNCTION__);
		
	list = claro_list_create(); 

	if (list == NULL) tlog(TL_ERROR, "Could not create claro_list.");
	
	claro_list_destroy(list); 
	
	tlog(TL_DEBUG, "END TEST %s -- ", __FUNCTION__, r->counts[TL_ERROR]==0 ? "SUCCESSFULLY" : "FAILED");
	
	return r;  
}

TEST_RESULT *test_claro_list_append() {
	TEST_RESULT *r; 
	int x = 0, c = 0;
	
	claro_list_t *list = NULL;
			
	r = tests[TEST_APPEND].result = create_test_result();

	tlog(TL_DEBUG, "BEGIN TEST %s", __FUNCTION__);
		
	list = claro_list_create();
	FILL_LIST(raw_list); 
	
	c = claro_list_count(list); 
	x = string_count(raw_list);

	if (c!=x) tlog(CL_ERROR, "LIST.count (%d)  != RAW_LIST.count (%d)", c,x); 
		
	COMPARE_LIST(raw_list); 
	
	claro_list_destroy(list); 
	
	tlog(TL_DEBUG, "END TEST %s -- ", __FUNCTION__, r->counts[TL_ERROR]==0 ? "SUCCESSFULLY" : "FAILED");
	
	return r; 
}

TEST_RESULT *test_claro_list_set_item() {
	int x = 0, i = 0;
	claro_list_t *list = NULL;
	
	TEST_RESULT *r;
	
	char *result_list[] = { 
			"changed1",
			"changed5",
			"ghi",
			"jkl",
			"changed3",	
			"pqr",	
			"stu",	
			"vwx",	
			"yz",
			"somestringa",	
			"changed4",	
			"changed2",	
			NULL 
	};
 	
	char *change_list[] = { 
		"changed1",
		"changed2",
		"changed3",
		"changed4",
		"changed5",
		NULL
	};
	
	int index[] = { 0,11,4,10,1, -2 }; 	
		
	// first, last, middle, second last, second 		
			

	
	r = tests[TEST_SET].result = create_test_result();

	tlog(TL_DEBUG, ".. BEGIN TEST %s", __FUNCTION__);
		
	list = claro_list_create();
	
	FILL_LIST(raw_list); 

	x=0; 
	
	while ( (i=index[x])!=-2) {
		tlog(TL_DEBUG, "SETTING LIST[%d] to %s", i, change_list[x]); 
		claro_list_set_item(list, i, change_list[x]);
		x++; 
	} 
	
	COMPARE_LIST(result_list); 

	claro_list_destroy(list); 
	
	tlog(TL_DEBUG, "END TEST %s -- ", __FUNCTION__, r->counts[TL_ERROR]==0 ? "SUCCESSFULLY" : "FAILED");
	
	return r; 
}

TEST_RESULT *test_claro_list_remove() {
	int  x = 0, i = 0;
	TEST_RESULT *r;
	
	claro_list_t *list = NULL;
	
	char *s, *result_list[] = {  
													"def",
													"jkl",
													"pqr",	
													"stu",	
													"vwx",	
													"yz",
													"somestringb",	
													NULL 
												};
			
	int index[] = { 0,10,3,7,1, -2 };		// first, last, middle, second last, second
	

		
	r = tests[TEST_REMOVE].result = create_test_result();
	
	tlog(TL_DEBUG, "BEGIN TEST %s", __FUNCTION__);
			
	list = claro_list_create();
	FILL_LIST(raw_list);  
		
	x=0; 
	
	while ( (i=index[x])!=-2) {
		s=claro_list_get_item(list, i); 
		
		if (!claro_list_remove(list, s)) {
			tlog(TL_ERROR, "Failed to remove '%s' at index %d", s, i); 
			// since we wouldn't get right results if this failed.. we exit now.. 
			goto error_out;
		}
				
		x++; 
	} 
	
	COMPARE_LIST(result_list); 

error_out:
	claro_list_destroy(list); 
	
	tlog(TL_DEBUG, "END TEST %s -- ", __FUNCTION__, r->counts[TL_ERROR]==0 ? "SUCCESSFULLY" : "FAILED");
	
	return r; 
}

	
TEST_RESULT *test_claro_list_insert() {
	int  x = 0;
	TEST_RESULT *r;
	
	claro_list_t *list = NULL;

	
	char *result_list[] = {
			"inserted1",
			"abc",
			"def",
			"ghi",
			"jkl",
			"mno",	
			"pqr",	
			"stu",	
			"vwx",	
			"yz",
			"somestringa",	
			"somestringb",
			"somestringc",
			"inserted3",
			"inserted2",	
			NULL 
		};
		
const struct { 
			char *text;
			int index;  
		} insert_list[] = {
			{"inserted1", 0 },		// first 
			{"inserted2", 12 },		// last 
			{"inserted3", 12 },		// second last 
//			{"inserted3", 5 },		// second 
//			{"inserted3", 5 },		// somewhere in the middle 
			{	NULL, -1 },
		};

		
	r = tests[TEST_INSERT].result = create_test_result();
	
	tlog(TL_DEBUG, "BEGIN TEST %s", __FUNCTION__);
			
	list = claro_list_create();
	FILL_LIST(raw_list);  
		
	x=-1; 
	
	while (insert_list[++x].text != NULL) {
		tlog(TL_DEBUG, "Inserting %s at %d", insert_list[x], x); 
		claro_list_insert(list, insert_list[x].index, insert_list[x].text);
	}
	
	COMPARE_LIST(result_list); 

	claro_list_destroy(list); 
	
	tlog(TL_DEBUG, "END TEST %s -- ", __FUNCTION__, r->counts[TL_ERROR]==0 ? "SUCCESSFULLY" : "FAILED");
	
	return r; 
}

static void _list_foreach_func(const void * val, void * arg)
{
    clog(CL_DEBUG, "%d [%x]\n", val, arg);
}

int main() 
{
	int c = 0, x = 0; 
	claro_list_t * list;

	claro_base_init(); 
	
	log_fd_set_level( CL_DEBUG, stderr );
	
	clog(CL_DEBUG, "%s running using Claro!", __FILE__ );

//	clog(CL_INFO, "Tests for CLARO_LIST");
		
    // foreach test code hacked in

    list = claro_list_create();

    claro_list_append(list, 1);

    claro_list_append(list, 2);

    claro_list_append(list, 3);

    claro_list_foreach(list, _list_foreach_func, 0xdeadbeef);
    
    claro_list_destroy(list);

	c = 0; x=-1;	
	while (tests[++x].type != TEST_LAST) {
		if (tests[x].f == TF_SKIP) {
			clog(CL_INFO, "Skipping TEST %s", tests[x].name); 
			continue; 
		}
		
		tests[x].result = (*tests[x].func)();
		 
		clog(CL_INFO, "TEST: %20s %s (CRITICAL=%d ERRORS=%d WARNINGS=%d INFO=%d DEBUG=%d ANY=%d)", 
				tests[x].name, 
				tests[x].result->counts[TL_ERROR] > 0 ? "failed" : "successful",
				tests[x].result->counts[TL_CRITICAL], 
				tests[x].result->counts[TL_ERROR],
				tests[x].result->counts[TL_WARNING],
				tests[x].result->counts[TL_INFO],
				tests[x].result->counts[TL_DEBUG],
				tests[x].result->counts[TL_ALL]
												
				);
		
				c+= tests[x].result->counts[TL_ERROR];
				c+= tests[x].result->counts[TL_CRITICAL];
	}
	
	if (c==0) {
		clog(CL_INFO, "All executed tests completed successfully."); 
	} else {
		clog(CL_INFO, "Not all tests completed successfully. A total of %d errors or critical errors occurred.", c); 
	}
	

	claro_shutdown();
	 
	return c; 
}
