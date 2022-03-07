#include <kore/kore.h>
#include <kore/http.h>
#include <kore/pgsql.h>
#include <string.h>
#include "business_objects.h"
#include "db_functions.h"

int        extract_api_isle_id(const char* path) ;

int        get_isle(struct http_request *);
int        isle_products_list(struct http_request *);

int        post_isle(struct http_request *);
int        put_isle(struct http_request *);
int        delete_isle(struct http_request *);

// extract isle_id from url /api/isle/[0-9]+$/...
int        extract_api_isle_id(const char* path) {
    int isle_id = -1 ;
    char *tofree = NULL ;

    char *psz = strdup(path);
    if ( NULL == psz ) {
        return isle_id ;
    }
    tofree = psz ;

    char *pToken = NULL ;
    for(int i=0;i<4;i++) {
        pToken = strsep(&psz,"/") ; // first iteration cursor on api in /api/isle/[0-9]+$
        if ( pToken == NULL ) {
            free(tofree);
            printf("extract_api_isle_id error : pToken is null for %d iteratio\n",i);
            return isle_id ;
        }
    }
    isle_id = atoi(pToken);
    free(tofree);
    return isle_id ;
}

// post /api/isle
int        post_isle(struct http_request *req) {
    struct kore_pgsql        sql;
    struct kore_buf          buf;
    struct kore_buf          errors;
    struct kore_json         json;
    struct kore_json_item*   json_created;
    s_bo_isle                o_bo_isle;
    s_bo_isle                bo_isle;
    int                      nParams=3;
    char                     sIdtMagasin[14];

    req->status = HTTP_STATUS_INTERNAL_ERROR;

    kore_buf_init(&buf, 512);
    kore_buf_init(&errors, 512);
    kore_json_init(&json, req->http_body->data, req->http_body->length);
    json_created = kore_json_create_object(NULL, NULL);

    kore_pgsql_init(&sql);
    if ( kore_pgsql_setup(&sql, "db", KORE_PGSQL_SYNC) ) {
        if ( kore_json_parse(&json)) {
            if ( json_to_bo_isle(&json, &o_bo_isle, &errors) ) {
                sprintf(sIdtMagasin,"%ld",o_bo_isle.store_idt);
                if ( kore_pgsql_query_params(&sql,
                                             "insert into rayon(idt_magasin,nom,nom_image) values($1,$2,$3) returning idt,idt_magasin,nom,nom_image",
                                             0,
                                             nParams,
                                             sIdtMagasin,0,0,
                                             o_bo_isle.name,0,0,
                                             o_bo_isle.picture,0,0) ) {
                    // execute pg cursor
                    if ( for_one_tuple(&sql, &bo_isle, bo_isle_tuple_copy_one_tuple) ) {
                        // convert struct to json
                        if ( bo_isle_to_kore_json(&bo_isle, json_created, &errors) ) {
                            req->status = HTTP_STATUS_OK;
                            kore_json_item_tobuf(json_created, &buf);
                        } else {
                            kore_buf_append(&buf, errors.data, errors.offset) ;
                        }
                    } else {
                        kore_buf_appendf(&buf, "no data return\n");
                    }

                    printf("call post_isle by url /api/isle %s\n",buf.data);
                } else {
                    kore_pgsql_logerror(&sql);
                }     
            } else {
                kore_buf_appendf(&buf, "post /api/isle error while unmarshalling json : ");
                kore_buf_append(&buf, errors.data, errors.offset);           
            }        
        } else {
            kore_buf_appendf(&buf, "post /api/isle error : %s\n", kore_json_strerror());
        }    
    } else {
        kore_pgsql_logerror(&sql);
    }
    
    http_response(req, req->status, buf.data, buf.offset);

    kore_pgsql_cleanup(&sql);
    kore_buf_cleanup(&errors);
    kore_buf_cleanup(&buf);
    kore_json_cleanup(&json);
    kore_json_item_free(json_created);

    return  KORE_RESULT_OK;
}

int        put_isle(struct http_request *req) {
    struct kore_pgsql       sql;
    struct kore_buf         buf;
    struct kore_buf         errors;
    struct kore_json        json;
    struct kore_json_item*  json_created;
    s_bo_isle               o_bo_isle;
    s_bo_isle               bo_isle;
    int                     nParams=3;
    char                    sIdt[14];

    req->status = HTTP_STATUS_INTERNAL_ERROR;

    kore_buf_init(&buf, 512);
    kore_buf_init(&errors, 512);
    kore_json_init(&json, req->http_body->data, req->http_body->length);
    json_created = kore_json_create_object(NULL, NULL);

    kore_pgsql_init(&sql);
    if ( kore_pgsql_setup(&sql, "db", KORE_PGSQL_SYNC) ) {
        if ( kore_json_parse(&json)) {
            if ( json_to_bo_isle(&json, &o_bo_isle, &errors) ) {
                sprintf(sIdt,"%ld",o_bo_isle.idt);
                if ( kore_pgsql_query_params(&sql,
                                             "update rayon set nom=$1,nom_image=$2 where idt=$3 returning idt,idt_magasin,nom,nom_image",
                                             0,
                                             nParams,
                                             o_bo_isle.name,0,0,
                                             o_bo_isle.picture,0,0,
                                             sIdt,0,0 ) ) {
                    // execute pg cursor
                    if ( for_one_tuple(&sql, &bo_isle, bo_isle_tuple_copy_one_tuple) ) {
                        // convert struct to json
                        if ( bo_isle_to_kore_json(&bo_isle, json_created, &errors) ) {
                            req->status = HTTP_STATUS_OK;
                            kore_json_item_tobuf(json_created, &buf);
                        } else {
                            kore_buf_append(&buf, errors.data, errors.offset) ;
                        }
                    } else {
                        kore_buf_appendf(&buf, "no data return\n");
                    }

                    printf("call put_isle by url /api/isle %s\n",buf.data);
                } else {
                    kore_pgsql_logerror(&sql);
                }     
            } else {
                kore_buf_appendf(&buf, "put /api/isle error while unmarshalling json : ");
                kore_buf_append(&buf, errors.data, errors.offset);           
            }        
        } else {
            kore_buf_appendf(&buf, "put /api/isle error : %s\n", kore_json_strerror());
        }    
    } else {
        kore_pgsql_logerror(&sql);
    }
    
    http_response(req, req->status, buf.data, buf.offset);

    kore_pgsql_cleanup(&sql);
    kore_buf_cleanup(&errors);
    kore_buf_cleanup(&buf);
    kore_json_cleanup(&json);
    kore_json_item_free(json_created);

    return  KORE_RESULT_OK;
}

// /api/isle/[0-9]+$ with [0-9]+ : (isle_id)
int        delete_isle(struct http_request *req) {
    struct kore_pgsql        sql;
    s_bo_isle                bo_isle;
    struct kore_json_item*   json;
    struct kore_buf          buf;
    struct kore_buf          errors;
    int                      nParams=1;
    char                     sIdt[14];

    req->status = HTTP_STATUS_INTERNAL_ERROR;

    kore_buf_init(&buf, 1024);
    kore_buf_init(&errors, 1024);
    json = kore_json_create_object(NULL, NULL);

    kore_pgsql_init(&sql);

     int isle_id = extract_api_isle_id(req->path);
    if ( isle_id > 0 ) {
        sprintf(sIdt,"%d",isle_id);
        if ( kore_pgsql_setup(&sql, "db", KORE_PGSQL_SYNC) ) {
            if ( kore_pgsql_query_params(&sql,
                                         "delete from rayon where idt=$1 returning idt,idt_magasin,nom,nom_image",
                                         0,
                                         nParams,
                                         sIdt, 0, 0) ) {
                // execute pg cursor
                if ( for_one_tuple(&sql, &bo_isle, bo_isle_tuple_copy_one_tuple) ) {
                    // convert struct to json
                    if ( bo_isle_to_kore_json(&bo_isle, json, &errors) ) {
                        req->status = HTTP_STATUS_OK;
                        kore_json_item_tobuf(json, &buf);
                    } else {
                        kore_buf_append(&buf, errors.data, errors.offset) ;
                    }
                } else {
                    kore_buf_appendf(&buf, "no data return\n");
                }
                printf("call delete_isle by url /api/isle/[0-9]+ (%s) with id %d\n",req->path,isle_id);
            } else {
                kore_pgsql_logerror(&sql);
            }
        } else {
            kore_pgsql_logerror(&sql);
        }
    } else {
        kore_buf_appendf(&buf, "isle_id must be > 0\n");
    }
    http_response(req, req->status, buf.data, buf.offset);

    kore_pgsql_cleanup(&sql);
    kore_buf_cleanup(&buf);
    kore_buf_cleanup(&errors);
    kore_json_item_free(json);

    return KORE_RESULT_OK;
}

// /api/isle/[0-9]+$ with [0-9]+ : (isle_id)
int        get_isle(struct http_request *req) {
    struct kore_pgsql        sql;
    s_bo_isle                bo_isle;
    struct kore_json_item*   json;
    struct kore_buf          buf;
    struct kore_buf          errors;
    int                      nParams=1;
    char                     sIdt[14];

    req->status = HTTP_STATUS_INTERNAL_ERROR;

    kore_buf_init(&buf, 1024);
    kore_buf_init(&errors, 1024);
    json = kore_json_create_object(NULL, NULL);

    kore_pgsql_init(&sql);

     int isle_id = extract_api_isle_id(req->path);
    if ( isle_id > 0 ) {
        sprintf(sIdt,"%d",isle_id);
        if ( kore_pgsql_setup(&sql, "db", KORE_PGSQL_SYNC) ) {
            if ( kore_pgsql_query_params(&sql,
                                         "SELECT idt,idt_magasin,nom,nom_image FROM rayon where idt=$1",
                                         0,
                                         nParams,
                                         sIdt, 0, 0) ) {
                // execute pg cursor
                if ( for_one_tuple(&sql, &bo_isle, bo_isle_tuple_copy_one_tuple) ) {
                    // convert struct to json
                    if ( bo_isle_to_kore_json(&bo_isle, json, &errors) ) {
                        req->status = HTTP_STATUS_OK;
                        kore_json_item_tobuf(json, &buf);
                    } else {
                        kore_buf_append(&buf, errors.data, errors.offset) ;
                    }
                } else {
                    kore_buf_appendf(&buf, "no data return\n");
                }
                printf("call get_isle by url /api/isle/[0-9]+ (%s) with id %d\n",req->path,isle_id);
            } else {
                kore_pgsql_logerror(&sql);
            }
        } else {
            kore_pgsql_logerror(&sql);
        }
    } else {
        kore_buf_appendf(&buf, "isle_id must be > 0\n");
    }
    http_response(req, req->status, buf.data, buf.offset);

    kore_pgsql_cleanup(&sql);
    kore_buf_cleanup(&buf);
    kore_buf_cleanup(&errors);
    kore_json_item_free(json);

    return KORE_RESULT_OK;
}

// /api/isle/[0-9]+/products [0-9]+ : (isle_id)
int        isle_products_list(struct http_request *req) {
    struct kore_pgsql        sql;
    s_bo_product*            array_of_products;
    long                     count_array_of_products;
    struct kore_json_item*   json;
    struct kore_buf          buf;
    struct kore_buf          errors;
    int                      nParams=1;
    char                     sIdt[14];
    
    req->status = HTTP_STATUS_INTERNAL_ERROR;

    kore_buf_init(&buf, 1024);
    kore_buf_init(&errors, 1024);
    json = kore_json_create_object(NULL, NULL);
    kore_pgsql_init(&sql);

    int isle_id = extract_api_isle_id(req->path);
    if ( isle_id > 0 ) {
        sprintf(sIdt,"%d",isle_id);
        if ( kore_pgsql_setup(&sql, "db", KORE_PGSQL_SYNC) ) {
            if ( kore_pgsql_query_params(&sql,
                                         "SELECT idt,idt_rayon,nom,nom_image,prix FROM produit where idt_rayon=$1",
                                         0,
                                         nParams,
                                         sIdt, 0, 0) ) {
                // execute pg cursor, alloc array of stores with callbac bo_store_array_allocator, append datas to array with callback bo_store_tuple_copy
                for_each_tuples(&sql,
                                (void**)&array_of_products,
                                &count_array_of_products,
                                bo_product_array_allocator,
                                bo_product_tuple_copy_from_array);

                // convert struct to json
                if ( array_bo_product_to_kore_json(&array_of_products, count_array_of_products, json, &errors) ) {
                    req->status = HTTP_STATUS_OK;
                    kore_json_item_tobuf(json, &buf);
                } else {
                    kore_buf_append(&buf, errors.data, errors.offset) ;
                }
    
                free(array_of_products);
                printf("call isle_products_list by url /api/isle/[0-9]isle \n");
            } else {
                kore_pgsql_logerror(&sql);
            }
        } else {
            kore_pgsql_logerror(&sql);
        }
    } else {
        kore_buf_appendf(&buf, "isle_id must be > 0\n");
    }
    http_response(req, req->status, buf.data, buf.offset);

    kore_pgsql_cleanup(&sql);
    kore_buf_cleanup(&buf);
    kore_buf_cleanup(&errors);
    kore_json_item_free(json);

    return KORE_RESULT_OK;
}
