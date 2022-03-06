#include "business_objects.h"


char* strcpy_with_limit(char *to, const char* from, size_t maxlen) {
    char* p = NULL;
   
    if ( strlen(from) < maxlen ) {
        p = strcpy(to, from);
    } else {
        p = strncpy(to, from, maxlen-1);
        to[maxlen-1] = 0 ;
    }
    return p;
}

//*******
// store
// return 0 if error
// curl -i -k -X POST -d '{"idt": 0, "name": "Store1"}' https://127.0.0.1:8888/api/store
int    json_to_bo_store(struct kore_json*   p_json,
                        ps_bo_store         p_bo_store,
                        struct kore_buf*    p_kore_buf_errors) {
    int ret = 1 ;
    struct kore_json_item* item;

    item = kore_json_find_integer_u64(p_json->root, "idt") ;
    if ( item != NULL ) {
        p_bo_store->idt = item->data.u64 ;
    } else {
        kore_buf_appendf(p_kore_buf_errors, "can not find idt in store json\n");
        ret = 0 ;
    }

    item = kore_json_find_string(p_json->root, "name") ;
    if ( item != NULL ) {
        strcpy_with_limit(p_bo_store->name, item->data.string, sizeof(p_bo_store->name)) ;
    } else {
        kore_buf_appendf(p_kore_buf_errors, "can not find name in store json\n");
        ret = 0 ;
    }

    return ret ;    
}

// return 0 if error
int    bo_store_to_json_buffer(ps_bo_store         p_bo_store,
                               struct kore_buf*    p_kore_buf,
                               struct kore_buf*    p_kore_buf_errors) {
    int ret = 1 ;
    kore_buf_appendf(p_kore_buf, "{\"idt\": %d, \"name\": \"%s\"}", p_bo_store->idt, p_bo_store->name);
    return ret ;    
}


int    bo_store_to_kore_json(ps_bo_store                p_bo_store,
                             struct kore_json_item*     p_json_item,
                             struct kore_buf*           p_kore_buf_errors) {
    int ret = 1 ;
    if ( NULL == kore_json_create_integer_u64(p_json_item, "idt", p_bo_store->idt) ) {
        kore_buf_appendf(p_kore_buf_errors, "can not create idt as integer_u64 in json object\n");
        return 0;
    }
        
    if ( NULL == kore_json_create_string(p_json_item, "name",p_bo_store->name) ) {
        kore_buf_appendf(p_kore_buf_errors, "can not create name as string in json object\n");
        return 0;
    }
    return ret ;    
}

int    array_bo_store_to_kore_json(s_bo_store**             array_of_bo_store,
                                   long                     count_array_of_stores,
                                   struct kore_json_item*   p_json_item,
                                   struct kore_buf*         p_kore_buf_errors) {
    int ret = 1 ;
    struct kore_json_item* array ;

    array = kore_json_create_array(p_json_item, "stores");
    if ( NULL == array ) {
        kore_buf_appendf(p_kore_buf_errors, "can not create stores as array in json object\n");
        return 0;        
    }
    for(int i=0; i<count_array_of_stores; i++) {
        if ( ! bo_store_to_kore_json(&((*array_of_bo_store)[i]), array, p_kore_buf_errors) ) {
            return 0;
        }
    }    
    return ret ;    
}
// store
//*******

//*******
// isle
// curl -i -k -X POST -d '{"idt": 0, "store_idt": 1, "name": "Isle1", "picture": "pict.jpg"}' https://127.0.0.1:8888/api/isle
int    json_to_bo_isle(struct kore_json*     p_json,
                       ps_bo_isle            p_bo_isle,
                       struct kore_buf*      p_kore_buf_errors) {
    int ret = 1 ;
    struct kore_json_item* item;

    item = kore_json_find_integer_u64(p_json->root, "idt") ;
    if ( item != NULL ) {
        p_bo_isle->idt = item->data.u64 ;
    } else {
        kore_buf_appendf(p_kore_buf_errors, "can not find idt in isle json\n");
        ret = 0 ;
    }

    item = kore_json_find_integer_u64(p_json->root, "store_idt") ;
    if ( item != NULL ) {
        p_bo_isle->store_idt = item->data.u64 ;
    } else {
        kore_buf_appendf(p_kore_buf_errors, "can not find store_idt in isle json\n");
        ret = 0 ;
    }

    item = kore_json_find_string(p_json->root, "name") ;
    if ( item != NULL ) {
        strcpy_with_limit(p_bo_isle->name, item->data.string, sizeof(p_bo_isle->name));
    } else {
        kore_buf_appendf(p_kore_buf_errors, "can not find name in isle json\n");
        ret = 0 ;
    }

    item = kore_json_find_string(p_json->root, "picture") ;
    if ( item != NULL ) {
        strcpy_with_limit(p_bo_isle->picture, item->data.string, sizeof(p_bo_isle->picture));
    } else {
        kore_buf_appendf(p_kore_buf_errors, "can not find name in isle json\n");
        ret = 0 ;
    }
        
    return ret ;    
}

// curl -i -k -X POST -d '{"idt": 0, "store_idt": 1, "name": "Isle1", "picture": "pict.jpg"}' https://127.0.0.1:8888/api/isle
int    bo_isle_to_json_buffer(ps_bo_isle         p_bo_isle,
                              struct kore_buf*   p_kore_buf,
                              struct kore_buf*   p_kore_buf_errors) {
    int ret = 1 ;
    kore_buf_appendf(p_kore_buf,
                     "{\"idt\": %d, \"store_idt\": %d, \"name\": \"%s\", \"picture\": \"%s\"}",
                     p_bo_isle->idt,
                     p_bo_isle->store_idt,
                     p_bo_isle->name,
                     p_bo_isle->picture);
    return ret ;    
}

int    bo_isle_to_kore_json(ps_bo_isle p_bo_isle,
                             struct kore_json_item* p_json_item,
                             struct kore_buf* p_kore_buf_errors) {
    int ret = 1 ;
    if ( NULL == kore_json_create_integer_u64(p_json_item, "idt", p_bo_isle->idt) ) {
        kore_buf_appendf(p_kore_buf_errors, "can not create idt as integer_u64 in json object\n");
        return 0;
    }

    if ( NULL == kore_json_create_integer_u64(p_json_item, "store_idt", p_bo_isle->store_idt) ) {
        kore_buf_appendf(p_kore_buf_errors, "can not create store_idt as integer_u64 in json object\n");
        return 0;
    }

    if ( NULL == kore_json_create_string(p_json_item, "name",p_bo_isle->name) ) {
        kore_buf_appendf(p_kore_buf_errors, "can not create name as string in json object\n");
        return 0;
    }

    if ( NULL == kore_json_create_string(p_json_item, "picture",p_bo_isle->picture) ) {
        kore_buf_appendf(p_kore_buf_errors, "can not create picture as string in json object\n");
        return 0;
    }
    return ret ;    
}

int    array_bo_isle_to_kore_json(s_bo_isle**             array_of_bo_isle,
                                  long                    count_array_of_isles,
                                  struct kore_json_item*  p_json_item,
                                  struct kore_buf*        p_kore_buf_errors) {
    int ret = 1 ;
    struct kore_json_item* array ;

    array = kore_json_create_array(p_json_item, "isles");
    if ( NULL == array ) {
        kore_buf_appendf(p_kore_buf_errors, "can not create isles as array in json object\n");
        return 0;        
    }
    for(int i=0; i<count_array_of_isles; i++) {
        if ( ! bo_isle_to_kore_json(&((*array_of_bo_isle)[i]), array, p_kore_buf_errors) ) {
            return 0;
        }
    }
    return ret ;    
}
// isle
//*******

//********
// product
// curl -i -k -X POST -d '{"idt": 0, "isle_idt": 1, "name": "Product1", "picture": "pict.jpg", "price": 3.14}' https://127.0.0.1:8888/api/product
int    json_to_bo_product(struct kore_json*     p_json,
                          ps_bo_product         p_bo_product,
                          struct kore_buf*      p_kore_buf_errors) {
    int ret = 1 ;
    struct kore_json_item* item;

    item = kore_json_find_integer_u64(p_json->root, "idt") ;
    if ( item != NULL ) {
        p_bo_product->idt = item->data.u64 ;
    } else {
        kore_buf_appendf(p_kore_buf_errors, "can not find idt in product json\n");
        ret = 0 ;
    }

    item = kore_json_find_integer_u64(p_json->root, "isle_idt") ;
    if ( item != NULL ) {
        p_bo_product->isle_idt = item->data.u64 ;
    } else {
        kore_buf_appendf(p_kore_buf_errors, "can not find isle_idt in product json\n");
        ret = 0 ;
    }

    item = kore_json_find_string(p_json->root, "name") ;
    if ( item != NULL ) {
        strcpy_with_limit(p_bo_product->name, item->data.string, sizeof(p_bo_product->name));
    } else {
        kore_buf_appendf(p_kore_buf_errors, "can not find name in product json\n");
        ret = 0 ;
    }

    item = kore_json_find_string(p_json->root, "picture") ;
    if ( item != NULL ) {
        strcpy_with_limit(p_bo_product->picture, item->data.string, sizeof(p_bo_product->picture));
    } else {
        kore_buf_appendf(p_kore_buf_errors, "can not find picture in product json\n");
        ret = 0 ;
    }

    item = kore_json_find_number(p_json->root, "price") ;
    if ( item != NULL ) {
        p_bo_product->price = item->data.number ;
    } else {
        kore_buf_appendf(p_kore_buf_errors, "can not find price in product json\n");
        ret = 0 ;
    }

    return ret ;    
}

// curl -i -k -X POST -d '{"idt": 0, "isle_idt": 1, "name": "Product1", "picture": "pict.jpg", "price": 3.14}' https://127.0.0.1:8888/api/product
int    bo_product_to_json_buffer(ps_bo_product     p_bo_product,
                                 struct kore_buf*  p_kore_buf,
                                 struct kore_buf*  p_kore_buf_errors) {
    int ret = 1 ;
    kore_buf_appendf(p_kore_buf,
                     "{\"idt\": %d, \"isle_idt\": %d, \"name\": \"%s\", \"picture\": \"%s\", \"price\": %lf}",
                     p_bo_product->idt,
                     p_bo_product->isle_idt,
                     p_bo_product->name,
                     p_bo_product->picture,
                     p_bo_product->price);
    return ret ;    
}

int    bo_product_to_kore_json(ps_bo_product             p_bo_product,
                               struct kore_json_item*    p_json_item,
                               struct kore_buf*          p_kore_buf_errors) {
    int ret = 1 ;
    if ( NULL == kore_json_create_integer_u64(p_json_item, "idt", p_bo_product->idt) ) {
        kore_buf_appendf(p_kore_buf_errors, "can not create idt as integer_u64 in json object\n");
        return 0;
    }

    if ( NULL == kore_json_create_integer_u64(p_json_item, "isle_idt", p_bo_product->isle_idt) ) {
        kore_buf_appendf(p_kore_buf_errors, "can not create isle_idt as integer_u64 in json object\n");
        return 0;
    }

    if ( NULL == kore_json_create_string(p_json_item, "name",p_bo_product->name) ) {
        kore_buf_appendf(p_kore_buf_errors, "can not create name as string in json object\n");
        return 0;
    }

    if ( NULL == kore_json_create_string(p_json_item, "picture",p_bo_product->picture) ) {
        kore_buf_appendf(p_kore_buf_errors, "can not create picture as string in json object\n");
        return 0;
    }

    if ( NULL == kore_json_create_number(p_json_item, "price", p_bo_product->price) ) {
        kore_buf_appendf(p_kore_buf_errors, "can not create price as number in json object\n");
        return 0;
    }
    return ret ;    
}

int    array_bo_product_to_kore_json(s_bo_product**         array_of_bo_product,
                                     long                   count_array_of_products,
                                     struct kore_json_item* p_json_item,
                                     struct kore_buf*       p_kore_buf_errors) {
    int ret = 1 ;
    struct kore_json_item* array ;

    array = kore_json_create_array(p_json_item, "products");
    if ( NULL == array ) {
        kore_buf_appendf(p_kore_buf_errors, "can not create products as array in json object\n");
        return 0;        
    }
    for(int i=0; i<count_array_of_products; i++) {
        if ( ! bo_product_to_kore_json(&((*array_of_bo_product)[i]), array, p_kore_buf_errors) ) {
            return 0;
        }
    }
    return ret ;    
}
// product
//********
