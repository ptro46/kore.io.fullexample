#include "db_functions.h"
#include "business_objects.h"

//*******************
// generics functions
void    for_each_tuples(struct kore_pgsql*             p_sql,
                        void**                         array_of_tuples,
                        long*                          p_count_array_of_tuples,
                        pf_array_tuples_allocator      pf_allocator,
                        pf_tuple_copy_from_array       pf_copy) {
    int rows,i ;
    rows = kore_pgsql_ntuples(p_sql);
    (*p_count_array_of_tuples) = rows ;
    pf_allocator(array_of_tuples, (*p_count_array_of_tuples));
    for (i = 0; i < rows; i++) {
        pf_copy(p_sql, i, array_of_tuples);
    }
}

int     for_one_tuple(struct kore_pgsql*         p_sql,
                      void*                      p_tuple,
                      pf_tuple_copy_one_tuple    pf_copy) {
    int rows ;
    rows = kore_pgsql_ntuples(p_sql);
    if ( rows > 0 ) {
        pf_copy(p_sql, 0, p_tuple);
    }
    return rows;
}
// generics functions
//*******************

//*******
// store
void   bo_store_array_allocator(void** array_of_tuples, long count_array_of_tuples) {
    (*array_of_tuples) = calloc(count_array_of_tuples, sizeof(s_bo_store));
}

void   bo_store_tuple_copy_from_array(struct kore_pgsql* p_sql, int tuple_index, void** array_of_tuples) {
    s_bo_store**    p_array = (s_bo_store**)array_of_tuples;
    ps_bo_store     p_store = &((*p_array)[tuple_index]);

    bo_store_tuple_copy_one_tuple(p_sql, tuple_index, p_store);
}

void     bo_store_tuple_copy_one_tuple(struct kore_pgsql* p_sql, int tuple_index, void* p_tuple) {
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
// store
//*******


//*******
// isle
void   bo_isle_array_allocator(void** array_of_tuples, long count_array_of_tuples) {
    (*array_of_tuples) = calloc(count_array_of_tuples, sizeof(s_bo_isle));
}

void   bo_isle_tuple_copy_from_array(struct kore_pgsql* p_sql, int tuple_index, void** array_of_tuples) {
    s_bo_isle**    p_array = (s_bo_isle**)array_of_tuples;
    ps_bo_isle  p_isle = &((*p_array)[tuple_index]);

    bo_isle_tuple_copy_one_tuple(p_sql, tuple_index, p_isle);
}

void   bo_isle_tuple_copy_one_tuple(struct kore_pgsql* p_sql, int tuple_index, void* p_tuple) {
    char* p;
    ps_bo_isle p_isle = (ps_bo_isle)p_tuple;

    p = kore_pgsql_getvalue(p_sql, tuple_index, SQL_INDEX_BO_ISLE_IDT) ;
    if ( p != NULL ) {
        p_isle->idt = atol(p);
    }

    p = kore_pgsql_getvalue(p_sql, tuple_index, SQL_INDEX_BO_ISLE_STORE_IDT) ;
    if ( p != NULL ) {
        p_isle->store_idt = atol(p);
    }

    p = kore_pgsql_getvalue(p_sql, tuple_index, SQL_INDEX_BO_ISLE_NAME) ;
    if ( p != NULL ) {
        strcpy(p_isle->name,p);
    }

    p = kore_pgsql_getvalue(p_sql, tuple_index, SQL_INDEX_BO_ISLE_PICTURE) ;
    if ( p != NULL ) {
        strcpy(p_isle->picture,p);
    }
}
// isle
//*******

//********
// product
void   bo_product_array_allocator(void** array_of_tuples, long count_array_of_tuples) {
    (*array_of_tuples) = calloc(count_array_of_tuples, sizeof(s_bo_product));
}

void   bo_product_tuple_copy_from_array(struct kore_pgsql* p_sql, int tuple_index, void** array_of_tuples) {
    s_bo_product**    p_array = (s_bo_product**)array_of_tuples;
    ps_bo_product   p_product = &((*p_array)[tuple_index]);

    bo_isle_tuple_copy_one_tuple(p_sql, tuple_index, p_product);
}

void   bo_product_tuple_copy_one_tuple(struct kore_pgsql* p_sql, int tuple_index, void* p_tuple) {
    char* p;
    ps_bo_product p_product = (ps_bo_product)p_tuple;

    p = kore_pgsql_getvalue(p_sql, tuple_index, SQL_INDEX_BO_PRODUCT_IDT) ;
    if ( p != NULL ) {
        p_product->idt = atol(p);
    }

    p = kore_pgsql_getvalue(p_sql, tuple_index, SQL_INDEX_BO_PRODUCT_ISLE_IDT) ;
    if ( p != NULL ) {
        p_product->isle_idt = atol(p);
    }

    p = kore_pgsql_getvalue(p_sql, tuple_index, SQL_INDEX_BO_PRODUCT_NAME) ;
    if ( p != NULL ) {
        strcpy(p_product->name,p);
    }

    p = kore_pgsql_getvalue(p_sql, tuple_index, SQL_INDEX_BO_PRODUCT_PICTURE) ;
    if ( p != NULL ) {
        strcpy(p_product->picture,p);
    }

    p = kore_pgsql_getvalue(p_sql, tuple_index, SQL_INDEX_BO_PRODUCT_PRICE) ;
    if ( p != NULL ) {
        p_product->price = atof(p);
    }

}
// product
//********
