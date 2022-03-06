#include <kore/kore.h>
#include <kore/http.h>
#include <kore/pgsql.h>
#include <string.h>
#include "business_objects.h"
#include "db_functions.h"

int        extract_api_store_id(const char* path) ;

int        stores_list(struct http_request *);
int        get_store(struct http_request *);
int        store_isles_list(struct http_request *);

int        post_store(struct http_request *);
int        put_store(struct http_request *);
int        delete_store(struct http_request *);


// extract store_id from url /api/store/[0-9]+$/...
int        extract_api_store_id(const char* path) {
    int store_id = -1 ;
    char *tofree = NULL ;

    char *psz = strdup(path);
    if ( NULL == psz ) {
        return store_id ;
    }
    tofree = psz ;

    char *pToken = NULL ;
    for(int i=0;i<4;i++) {
        pToken = strsep(&psz,"/") ; // 4iem iteration cursor on id ([0-9]+ in /api/store/[0-9]+$
        if ( pToken == NULL ) {
            free(tofree);
            printf("get_store error : pToken is null for %d iteratio\n",i);
            return store_id ;
        }
    }
    store_id = atoi(pToken);
    free(tofree);
    return store_id ;
}

// get /api/store
int        stores_list(struct http_request *req) {
    struct kore_pgsql        sql;
    s_bo_store*              array_of_stores;
    long                     count_array_of_stores;
    struct kore_json_item*   json;
    struct kore_buf          buf;
    struct kore_buf          errors;
    
    req->status = HTTP_STATUS_INTERNAL_ERROR;

    kore_buf_init(&buf, 1024);
    kore_buf_init(&errors, 1024);
    json = kore_json_create_object(NULL, NULL);

    kore_pgsql_init(&sql);
    if ( kore_pgsql_setup(&sql, "db", KORE_PGSQL_SYNC) ) {
        if ( kore_pgsql_query(&sql, "SELECT idt,nom FROM magasin") ) {
            // execute pg cursor, alloc array of stores with callbac bo_store_array_allocator, append datas to array with callback bo_store_tuple_copy
            for_each_tuples(&sql,
                            (void**)&array_of_stores,
                            &count_array_of_stores,
                            bo_store_array_allocator,
                            bo_store_tuple_copy_from_array);

            // convert struct to json
            if ( array_bo_store_to_kore_json(&array_of_stores, count_array_of_stores, json, &errors) ) {
                req->status = HTTP_STATUS_OK;
                kore_json_item_tobuf(json, &buf);
            } else {
                kore_buf_append(&buf, errors.data, errors.offset) ;
            }
    
            free(array_of_stores);
            printf("call stotes_list by url /api/store \n");
        } else {
            kore_pgsql_logerror(&sql);
        }
    } else {
        kore_pgsql_logerror(&sql);
    }

    http_response(req, req->status, buf.data, buf.offset);

    kore_pgsql_cleanup(&sql);
    kore_buf_cleanup(&buf);
    kore_buf_cleanup(&errors);
    kore_json_item_free(json);

    return KORE_RESULT_OK;
}

// post /api/store
int        post_store(struct http_request *req) {
    struct kore_pgsql        sql;
    struct kore_buf          buf;
    struct kore_buf          errors;
    struct kore_json         json;
    struct kore_json_item*   json_created;
    s_bo_store               o_bo_store;
    s_bo_store               bo_store;
    int                      nParams=1;

    req->status = HTTP_STATUS_INTERNAL_ERROR;

    kore_buf_init(&buf, 512);
    kore_buf_init(&errors, 512);
    kore_json_init(&json, req->http_body->data, req->http_body->length);
    json_created = kore_json_create_object(NULL, NULL);

    kore_pgsql_init(&sql);
    if ( kore_pgsql_setup(&sql, "db", KORE_PGSQL_SYNC) ) {
        if ( kore_json_parse(&json)) {
            if ( json_to_bo_store(&json, &o_bo_store, &errors) ) {
                if ( kore_pgsql_query_params(&sql, "insert into magasin(nom) values($1) returning idt,nom",0, nParams, o_bo_store.name, 0, 0) ) {
                    // execute pg cursor
                    if ( for_one_tuple(&sql, &bo_store, bo_store_tuple_copy_one_tuple) ) {
                        // convert struct to json
                        if ( bo_store_to_kore_json(&bo_store, json_created, &errors) ) {
                            req->status = HTTP_STATUS_OK;
                            kore_json_item_tobuf(json_created, &buf);
                        } else {
                            kore_buf_append(&buf, errors.data, errors.offset) ;
                        }
                    } else {
                        kore_buf_appendf(&buf, "no data return\n");
                    }

                    printf("call post_store by url /api/store %s\n",buf.data);
                } else {
                    kore_pgsql_logerror(&sql);
                }     
            } else {
                kore_buf_appendf(&buf, "post /api/store error while unmarshalling json : ");
                kore_buf_append(&buf, errors.data, errors.offset);           
            }        
        } else {
            kore_buf_appendf(&buf, "post /api/store error : %s\n", kore_json_strerror());
        }    
    } else {
        kore_pgsql_logerror(&sql);
    }
    
    http_response(req, 200, buf.data, buf.offset);

    kore_pgsql_cleanup(&sql);
    kore_buf_cleanup(&errors);
    kore_buf_cleanup(&buf);
    kore_json_cleanup(&json);
    kore_json_item_free(json_created);

    return  KORE_RESULT_OK;
}

// put /api/store
int        put_store(struct http_request *req) {
    struct kore_pgsql         sql;
    struct kore_buf           buf;
    struct kore_buf           errors;
    struct kore_json          json;
    struct kore_json_item*    json_created;
    s_bo_store                o_bo_store;
    s_bo_store                bo_store;
    int                       nParams=2;
    char                      sIdt[14];

    req->status = HTTP_STATUS_INTERNAL_ERROR;

    kore_buf_init(&buf, 512);
    kore_buf_init(&errors, 512);
    kore_json_init(&json, req->http_body->data, req->http_body->length);
    json_created = kore_json_create_object(NULL, NULL);

    kore_pgsql_init(&sql);
    if ( kore_pgsql_setup(&sql, "db", KORE_PGSQL_SYNC) ) {
        if ( kore_json_parse(&json) ) {
            if ( json_to_bo_store(&json, &o_bo_store, &errors) ) {
                sprintf(sIdt,"%ld",o_bo_store.idt);
                if ( kore_pgsql_query_params(&sql,
                                             "update magasin set nom=$1 where idt=$2 returning idt,nom",
                                             0,
                                             nParams,
                                             o_bo_store.name, 0, 0,
                                             sIdt, 0, 0) ) {
                    // execute pg cursor
                    if ( for_one_tuple(&sql, &bo_store, bo_store_tuple_copy_one_tuple) ) {
                        // convert struct to json
                        if ( bo_store_to_kore_json(&bo_store, json_created, &errors) ) {
                            req->status = HTTP_STATUS_OK;
                            kore_json_item_tobuf(json_created, &buf);
                        } else {
                            kore_buf_append(&buf, errors.data, errors.offset) ;
                        }
                    } else {
                        kore_buf_appendf(&buf, "no data return\n");
                    }

                    printf("call put_store by url /api/store %s\n",buf.data);
                } else {
                    kore_pgsql_logerror(&sql);
                }     
            } else {
                kore_buf_appendf(&buf, "put /api/store error while unmarshalling json : ");
                kore_buf_append(&buf, errors.data, errors.offset);           
            }
        } else {
            kore_buf_appendf(&buf, "put /api/store error : %s\n", kore_json_strerror());
        }
    } else {
        kore_pgsql_logerror(&sql);
    }
    
    http_response(req, 200, buf.data, buf.offset);

    kore_pgsql_cleanup(&sql);
    kore_buf_cleanup(&errors);
    kore_buf_cleanup(&buf);
    kore_json_cleanup(&json);
    kore_json_item_free(json_created);

    return KORE_RESULT_OK;
}

// /api/store/[0-9]+$ with [0-9]+ : (magasin_id)
int        delete_store(struct http_request *req) {
    struct kore_pgsql        sql;
    s_bo_store               bo_store;
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

    int store_id = extract_api_store_id(req->path);
    if ( store_id > 0 ) {
        sprintf(sIdt,"%d",store_id);
        if ( kore_pgsql_setup(&sql, "db", KORE_PGSQL_SYNC) ) {
            if ( kore_pgsql_query_params(&sql, "delete from magasin where idt=$1 returning idt,nom",0, nParams, sIdt, 0, 0) ) {
                // execute pg cursor
                if ( for_one_tuple(&sql, &bo_store, bo_store_tuple_copy_one_tuple) ) {
                    // convert struct to json
                    if ( bo_store_to_kore_json(&bo_store, json, &errors) ) {
                        req->status = HTTP_STATUS_OK;
                        kore_json_item_tobuf(json, &buf);
                    } else {
                        kore_buf_append(&buf, errors.data, errors.offset) ;
                    }
                } else {
                    kore_buf_appendf(&buf, "no data return\n");
                }
                printf("call delete_store by url /api/store/[0-9]+ (%s) with id %d\n",req->path,store_id);
            } else {
                kore_pgsql_logerror(&sql);
            }
        } else {
            kore_pgsql_logerror(&sql);
        }
    } else {
        kore_buf_appendf(&buf, "store_id must be > 0\n");
    }

    http_response(req, req->status, buf.data, buf.offset);

    kore_pgsql_cleanup(&sql);
    kore_buf_cleanup(&buf);
    kore_buf_cleanup(&errors);
    kore_json_item_free(json);

    return KORE_RESULT_OK;
}

// /api/store/[0-9]+$ with [0-9]+ : (magasin_id)
int        get_store(struct http_request *req) {
    struct kore_pgsql        sql;
    s_bo_store               bo_store;
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

    int store_id = extract_api_store_id(req->path);
    if ( store_id > 0 ) {
        sprintf(sIdt,"%d",store_id);
        if ( kore_pgsql_setup(&sql, "db", KORE_PGSQL_SYNC) ) {
            if ( kore_pgsql_query_params(&sql, "SELECT idt,nom FROM magasin where idt=$1",0, nParams, sIdt, 0, 0) ) {
                // execute pg cursor
                if ( for_one_tuple(&sql, &bo_store, bo_store_tuple_copy_one_tuple) ) {
                    // convert struct to json
                    if ( bo_store_to_kore_json(&bo_store, json, &errors) ) {
                        req->status = HTTP_STATUS_OK;
                        kore_json_item_tobuf(json, &buf);
                    } else {
                        kore_buf_append(&buf, errors.data, errors.offset) ;
                    }
                } else {
                    kore_buf_appendf(&buf, "no data return\n");
                }
                printf("call get_store by url /api/store/[0-9]+ (%s) with id %d\n",req->path,store_id);
            } else {
                kore_pgsql_logerror(&sql);
            }
        } else {
            kore_pgsql_logerror(&sql);
        }
    } else {
        kore_buf_appendf(&buf, "store_id must be > 0\n");
    }

    http_response(req, req->status, buf.data, buf.offset);

    kore_pgsql_cleanup(&sql);
    kore_buf_cleanup(&buf);
    kore_buf_cleanup(&errors);
    kore_json_item_free(json);

    return KORE_RESULT_OK;
}

int        store_isles_list(struct http_request *req) {
    struct kore_pgsql        sql;
    s_bo_isle*               array_of_isles;
    long                     count_array_of_isles;
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

    int store_id = extract_api_store_id(req->path);
    if ( store_id > 0 ) {
        sprintf(sIdt,"%d",store_id);
        if ( kore_pgsql_setup(&sql, "db", KORE_PGSQL_SYNC) ) {
            if ( kore_pgsql_query_params(&sql, "SELECT idt,idt_magasin,nom,nom_image FROM rayon where idt_magasin=$1",0, nParams, sIdt, 0, 0) ) {
                // execute pg cursor, alloc array of stores with callbac bo_store_array_allocator, append datas to array with callback bo_store_tuple_copy
                for_each_tuples(&sql,
                                (void**)&array_of_isles,
                                &count_array_of_isles,
                                bo_isle_array_allocator,
                                bo_isle_tuple_copy_from_array);

                // convert struct to json
                if ( array_bo_isle_to_kore_json(&array_of_isles, count_array_of_isles, json, &errors) ) {
                    req->status = HTTP_STATUS_OK;
                    kore_json_item_tobuf(json, &buf);
                } else {
                    kore_buf_append(&buf, errors.data, errors.offset) ;
                }
    
                free(array_of_isles);
                printf("call store_isles_list by url /api/store/[0-9]isle \n");
            } else {
                kore_pgsql_logerror(&sql);
            }
        } else {
            kore_pgsql_logerror(&sql);
        }
    } else {
        kore_buf_appendf(&buf, "store_id must be > 0\n");
    }
    http_response(req, req->status, buf.data, buf.offset);

    kore_pgsql_cleanup(&sql);
    kore_buf_cleanup(&buf);
    kore_buf_cleanup(&errors);
    kore_json_item_free(json);

    return KORE_RESULT_OK;
}
