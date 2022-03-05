#ifndef __business_objects__
#define __business_objects__

#include <kore/kore.h>

char* strcpy_with_limit(char *to, const char* from, size_t maxlen) ;

#define STR_SIZE 256

//*******
// store
typedef struct bo_store {
	long		idt ;
	char		name[STR_SIZE] ;
} s_bo_store, *ps_bo_store ;

#define	SQL_INDEX_BO_STORE_IDT		0
#define	SQL_INDEX_BO_STORE_NAME		1

int		json_to_bo_store(struct kore_json* p_json,
						 ps_bo_store p_bo_store,
						 struct kore_buf* p_kore_buf_errors) ;

int		bo_store_to_json_buffer(ps_bo_store p_bo_store,
								struct kore_buf* p_kore_buf,
								struct kore_buf* p_kore_buf_errors);

int		bo_store_to_kore_json(ps_bo_store p_bo_store,
							  struct kore_json_item* p_json_item,
							  struct kore_buf* p_kore_buf_errors);

int		array_bo_store_to_kore_json(s_bo_store** array_of_bo_store,
									long count_array_of_stores,
									struct kore_json_item* p_json_item,
									struct kore_buf* p_kore_buf_errors);

// store
//*******

//*******
// isle
typedef struct bo_isle {
	long		idt ;
	long		store_idt ;
	char		name[STR_SIZE];
	char 		picture[STR_SIZE];
} s_bo_isle , *ps_bo_isle;

int		json_to_bo_isle(struct kore_json* p_json, ps_bo_isle p_bo_isle, struct kore_buf* p_kore_buf_errors) ;
int		bo_isle_to_json_buffer(ps_bo_isle p_bo_isle, struct kore_buf* p_kore_buf, struct kore_buf* p_kore_buf_errors);
// isle
//*******


//********
// product
typedef struct bo_product {
	long		idt ;
	long		isle_idt ;
	char		name[STR_SIZE];
	char 		picture[STR_SIZE];
	double 		price;
} s_bo_product , *ps_bo_product;

int		json_to_bo_product(struct kore_json* p_json, ps_bo_product p_bo_product, struct kore_buf* p_kore_buf_errors) ;
int		bo_product_to_json_buffer(ps_bo_product p_bo_product, struct kore_buf* p_kore_buf, struct kore_buf* p_kore_buf_errors);
// product
//********

#endif /* defined(__business_objects__) */
