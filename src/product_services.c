#include <kore/kore.h>
#include <kore/http.h>
#include <kore/pgsql.h>
#include <string.h>
#include "business_objects.h"
#include "db_functions.h"

int        extract_api_product_id(const char* path) ;

int        get_product(struct http_request *);
int        post_product(struct http_request *);
int        put_product(struct http_request *);
int        delete_product(struct http_request *);

// extract product_id from url /api/product/[0-9]+$/...
int        extract_api_product_id(const char* path) {
    int product_id = -1 ;
    char *tofree = NULL ;

    char *psz = strdup(path);
    if ( NULL == psz ) {
        return product_id ;
    }
    tofree = psz ;

    char *pToken = NULL ;
    for(int i=0;i<4;i++) {
        pToken = strsep(&psz,"/") ; // first iteration cursor on api in /api/product/[0-9]+$
        if ( pToken == NULL ) {
            free(tofree);
            printf("extract_api_product_id error : pToken is null for %d iteratio\n",i);
            return product_id ;
        }
    }
    product_id = atoi(pToken);
    free(tofree);
    return product_id ;
}

// post /api/product
int        post_product(struct http_request *req) {
    struct kore_pgsql        sql;
    struct kore_buf          buf;
    struct kore_buf          errors;
    struct kore_json         json;
    struct kore_json_item*   json_created;
    s_bo_product             o_bo_product;
    s_bo_product             bo_product;
    int                      nParams=4;
    char                     sIdtIsle[14];
    char                     sPrice[14];

    req->status = HTTP_STATUS_INTERNAL_ERROR;

    kore_buf_init(&buf, 512);
    kore_buf_init(&errors, 512);
    kore_json_init(&json, req->http_body->data, req->http_body->length);
    json_created = kore_json_create_object(NULL, NULL);

    kore_pgsql_init(&sql);
    if ( kore_pgsql_setup(&sql, "db", KORE_PGSQL_SYNC) ) {
        if ( kore_json_parse(&json)) {
            if ( json_to_bo_product(&json, &o_bo_product, &errors) ) {
                sprintf(sIdtIsle,"%ld",o_bo_product.isle_idt);
                sprintf(sPrice,"%lf",o_bo_product.price);
                if ( kore_pgsql_query_params(&sql,
                                             "insert into produit(idt_rayon,nom,nom_image,prix) values($1,$2,$3,$4) returning idt,idt_rayon,nom,nom_image,prix",
                                             0,
                                             nParams,
                                             sIdtIsle,0,0,
                                             o_bo_product.name,0,0,
                                             o_bo_product.picture,0,0,
                                             sPrice,0,0) ) {
                    // execute pg cursor
                    if ( for_one_tuple(&sql, &bo_product, bo_product_tuple_copy_one_tuple) ) {
                        // convert struct to json
                        if ( bo_product_to_kore_json(&bo_product, json_created, &errors) ) {
                            req->status = HTTP_STATUS_OK;
                            kore_json_item_tobuf(json_created, &buf);
                        } else {
                            kore_buf_append(&buf, errors.data, errors.offset) ;
                        }
                    } else {
                        kore_buf_appendf(&buf, "no data return\n");
                    }

                    printf("call post_product by url /api/product %s\n",buf.data);
                } else {
                    kore_pgsql_logerror(&sql);
                }     
            } else {
                kore_buf_appendf(&buf, "post /api/product error while unmarshalling json : ");
                kore_buf_append(&buf, errors.data, errors.offset);           
            }        
        } else {
            kore_buf_appendf(&buf, "post /api/product error : %s\n", kore_json_strerror());
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

// put /api/product
int        put_product(struct http_request *req) {
    struct kore_pgsql       sql;
    struct kore_buf         buf;
    struct kore_buf         errors;
    struct kore_json        json;
    struct kore_json_item*  json_created;
    s_bo_product            o_bo_product;
    s_bo_product            bo_product;
    int                     nParams=4;
    char                    sIdt[14];
    char                    sPrice[14];

    req->status = HTTP_STATUS_INTERNAL_ERROR;

    kore_buf_init(&buf, 512);
    kore_buf_init(&errors, 512);
    kore_json_init(&json, req->http_body->data, req->http_body->length);
    json_created = kore_json_create_object(NULL, NULL);

    kore_pgsql_init(&sql);
    if ( kore_pgsql_setup(&sql, "db", KORE_PGSQL_SYNC) ) {
        if ( kore_json_parse(&json)) {
            if ( json_to_bo_product(&json, &o_bo_product, &errors) ) {
                sprintf(sIdt,"%ld",o_bo_product.idt);
                sprintf(sPrice,"%lf",o_bo_product.price);
                if ( kore_pgsql_query_params(&sql,
                                             "update produit set nom=$1,nom_image=$2,prix=$3 where idt=$4 returning idt,idt_rayon,nom,nom_image,prix",
                                             0,
                                             nParams,
                                             o_bo_product.name,0,0,
                                             o_bo_product.picture,0,0,
                                             sPrice,0,0,
                                             sIdt,0,0) ) {
                    // execute pg cursor
                    if ( for_one_tuple(&sql, &bo_product, bo_product_tuple_copy_one_tuple) ) {
                        // convert struct to json
                        if ( bo_product_to_kore_json(&bo_product, json_created, &errors) ) {
                            req->status = HTTP_STATUS_OK;
                            kore_json_item_tobuf(json_created, &buf);
                        } else {
                            kore_buf_append(&buf, errors.data, errors.offset) ;
                        }
                    } else {
                        kore_buf_appendf(&buf, "no data return\n");
                    }

                    printf("call put_product by url /api/product %s\n",buf.data);
                } else {
                    kore_pgsql_logerror(&sql);
                }     
            } else {
                kore_buf_appendf(&buf, "put /api/product error while unmarshalling json : ");
                kore_buf_append(&buf, errors.data, errors.offset);           
            }        
        } else {
            kore_buf_appendf(&buf, "put /api/product error : %s\n", kore_json_strerror());
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

// /api/product/[0-9]+$ with [0-9]+ : (product_id)
int        delete_product(struct http_request *req) {
    struct kore_pgsql       sql;
    s_bo_product            bo_product;
    struct kore_json_item*  json;
    struct kore_buf         buf;
    struct kore_buf         errors;
    int                     nParams=1;
    char                    sIdt[14];

    req->status = HTTP_STATUS_INTERNAL_ERROR;

    kore_buf_init(&buf, 1024);
    kore_buf_init(&errors, 1024);
    json = kore_json_create_object(NULL, NULL);

    kore_pgsql_init(&sql);

     int product_id = extract_api_product_id(req->path);
    if ( product_id > 0 ) {
        sprintf(sIdt,"%d",product_id);
        if ( kore_pgsql_setup(&sql, "db", KORE_PGSQL_SYNC) ) {
            if ( kore_pgsql_query_params(&sql,
                                         "delete from produit where idt=$1 returning idt,idt_rayon,nom,nom_image,prix",
                                         0,
                                         nParams,
                                         sIdt, 0, 0) ) {
                // execute pg cursor
                if ( for_one_tuple(&sql, &bo_product, bo_product_tuple_copy_one_tuple) ) {
                    // convert struct to json
                    if ( bo_product_to_kore_json(&bo_product, json, &errors) ) {
                        req->status = HTTP_STATUS_OK;
                        kore_json_item_tobuf(json, &buf);
                    } else {
                        kore_buf_append(&buf, errors.data, errors.offset) ;
                    }
                } else {
                    kore_buf_appendf(&buf, "no data return\n");
                }
                printf("call delete_product by url /api/product/[0-9]+ (%s) with id %d\n",req->path,product_id);
            } else {
                kore_pgsql_logerror(&sql);
            }
        } else {
            kore_pgsql_logerror(&sql);
        }
    } else {
        kore_buf_appendf(&buf, "product_id must be > 0\n");
    }
    http_response(req, req->status, buf.data, buf.offset);

    kore_pgsql_cleanup(&sql);
    kore_buf_cleanup(&buf);
    kore_buf_cleanup(&errors);
    kore_json_item_free(json);

    return KORE_RESULT_OK;
}

// /api/product/[0-9]+$ with [0-9]+ : (product_id)
int        get_product(struct http_request *req) {
    struct kore_pgsql        sql;
    s_bo_product             bo_product;
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

     int product_id = extract_api_product_id(req->path);
    if ( product_id > 0 ) {
        sprintf(sIdt,"%d",product_id);
        if ( kore_pgsql_setup(&sql, "db", KORE_PGSQL_SYNC) ) {
            if ( kore_pgsql_query_params(&sql,
                                         "SELECT idt,idt_rayon,nom,nom_image,prix FROM produit where idt=$1",
                                         0,
                                         nParams,
                                         sIdt, 0, 0) ) {
                // execute pg cursor
                if ( for_one_tuple(&sql, &bo_product, bo_product_tuple_copy_one_tuple) ) {
                    // convert struct to json
                    if ( bo_product_to_kore_json(&bo_product, json, &errors) ) {
                        req->status = HTTP_STATUS_OK;
                        kore_json_item_tobuf(json, &buf);
                    } else {
                        kore_buf_append(&buf, errors.data, errors.offset) ;
                    }
                } else {
                    kore_buf_appendf(&buf, "no data return\n");
                }
                printf("call get_product by url /api/product/[0-9]+ (%s) with id %d\n",req->path,product_id);
            } else {
                kore_pgsql_logerror(&sql);
            }
        } else {
            kore_pgsql_logerror(&sql);
        }
    } else {
        kore_buf_appendf(&buf, "product_id must be > 0\n");
    }
    http_response(req, req->status, buf.data, buf.offset);

    kore_pgsql_cleanup(&sql);
    kore_buf_cleanup(&buf);
    kore_buf_cleanup(&errors);
    kore_json_item_free(json);

    return KORE_RESULT_OK;
}
