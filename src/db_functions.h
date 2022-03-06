#ifndef __db_functions__
#define __db_functions__

#include <kore/kore.h>
#include <kore/pgsql.h>

typedef void (*pf_array_tuples_allocator)(void** array_of_tuples, long count_array_of_tuples);
typedef void (*pf_tuple_copy_from_array)(struct kore_pgsql* p_sql, int tuple_index, void** array_of_tuples);
typedef void (*pf_tuple_copy_one_tuple)(struct kore_pgsql* p_sql, int tuple_index, void* p_tuple);

void   for_each_tuples(struct kore_pgsql*            p_sql,
                       void**                        array_of_tuples,
                       long*                         p_count_array_of_tuples,
                       pf_array_tuples_allocator     pf_allocator,
                       pf_tuple_copy_from_array      pf_copy);

int    for_one_tuple(struct kore_pgsql*         p_sql,
                     void*                      p_tuple,
                     pf_tuple_copy_one_tuple    pf_copy);

void   bo_store_array_allocator(void** array_of_tuples, long count_array_of_tuples);
void   bo_store_tuple_copy_from_array(struct kore_pgsql* p_sql, int tuple_index, void** array_of_tuples);
void   bo_store_tuple_copy_one_tuple(struct kore_pgsql* p_sql, int tuple_index, void* p_tuple);

void   bo_isle_array_allocator(void** array_of_tuples, long count_array_of_tuples);
void   bo_isle_tuple_copy_from_array(struct kore_pgsql* p_sql, int tuple_index, void** array_of_tuples);
void   bo_isle_tuple_copy_one_tuple(struct kore_pgsql* p_sql, int tuple_index, void* p_tuple);

void   bo_product_array_allocator(void** array_of_tuples, long count_array_of_tuples);
void   bo_product_tuple_copy_from_array(struct kore_pgsql* p_sql, int tuple_index, void** array_of_tuples);
void   bo_product_tuple_copy_one_tuple(struct kore_pgsql* p_sql, int tuple_index, void* p_tuple);

#endif /* defined(__db_functions__) */
