#include "db_functions.h"
#include "business_objects.h"

void	for_each_tuples(struct kore_pgsql* 			p_sql,
						void** 						array_of_tuples,
						long* 						p_count_array_of_tuples,
						pf_array_tuples_allocator 	pf_allocator,
						pf_tuple_copy_from_array 	pf_copy) {
	int rows,i ;
	rows = kore_pgsql_ntuples(p_sql);
	(*p_count_array_of_tuples) = rows ;
	pf_allocator(array_of_tuples, (*p_count_array_of_tuples));
	for (i = 0; i < rows; i++) {
		pf_copy(p_sql, i, array_of_tuples);
	}
}

int		for_one_tuple(struct kore_pgsql* 		p_sql,
					  void*						p_tuple,
					  pf_tuple_copy_one_tuple 	pf_copy) {
	int rows ;
	rows = kore_pgsql_ntuples(p_sql);
	if ( rows > 0 ) {
		pf_copy(p_sql, 0, p_tuple);
	}
	return rows;
}

void	bo_store_array_allocator(void** array_of_tuples, long count_array_of_tuples) {
	(*array_of_tuples) = calloc(count_array_of_tuples, sizeof(s_bo_store));
}

void 	bo_store_tuple_copy_from_array(struct kore_pgsql* p_sql, int tuple_index, void** array_of_tuples) {
	s_bo_store**	p_array = (s_bo_store**)array_of_tuples;
	ps_bo_store p_store = &((*p_array)[tuple_index]);

	bo_store_tuple_copy_one_tuple(p_sql, tuple_index, p_store);
}

void 	bo_store_tuple_copy_one_tuple(struct kore_pgsql* p_sql, int tuple_index, void* p_tuple) {
	char* p;
	ps_bo_store p_store = (ps_bo_store)p_tuple;

	p = kore_pgsql_getvalue(p_sql, tuple_index, SQL_INDEX_BO_STORE_IDT) ;
	if ( p != NULL ) {
		p_store->idt = atol(p);
	}

	p = kore_pgsql_getvalue(p_sql, tuple_index, SQL_INDEX_BO_STORE_NAME) ;
	if ( p != NULL ) {
		strcpy(p_store->name,p);
	}
}

