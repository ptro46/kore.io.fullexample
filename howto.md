# Create project
kodev create magasin-rest

# Step per Step

## routing

```bash
    # create one store with payload {"idt": 0, "name": "Store1"}
    route ^/api/store$ {
        handler post_store
        methods post
    }

    # delete one store, uri contains store_id 
    route ^/api/store/[0-9]+$ {
        handler delete_store
        methods delete
    }

    # get one store, uri contains store_id 
    route ^/api/store/[0-9]+$ {
        handler get_store
    }

    # get isles from one store, uri contains store_id 
    route ^/api/store/[0-9]+/isles$ {
        handler store_isles_list
    }
```

## handlers

### post_store

```c
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

    // init pgsql struct
    kore_pgsql_init(&sql);
    if ( kore_pgsql_setup(&sql, "db", KORE_PGSQL_SYNC) ) {
        // parse json
        if ( kore_json_parse(&json)) {
            // build struct bo_store ( business_objects.h) from json struct
            if ( json_to_bo_store(&json, &o_bo_store, &errors) ) {
                // execute query with parameters
                if ( kore_pgsql_query_params(&sql, "insert into magasin(nom) values($1) returning idt,nom",0, nParams, o_bo_store.name, 0, 0) ) {
                    // execute pg cursor to get one row
                    if ( for_one_tuple(&sql, &bo_store, bo_store_tuple_copy_one_tuple) ) {
                        // convert struct to json
                        if ( bo_store_to_kore_json(&bo_store, json_created, &errors) ) {
                            // all is ok, fill buf returned with bo_store as json
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

    // send http response, payload containt bo_store created in json
    http_response(req, req->status, buf.data, buf.offset);

    // clean
    kore_pgsql_cleanup(&sql);
    kore_buf_cleanup(&errors);
    kore_buf_cleanup(&buf);
    kore_json_cleanup(&json);
    kore_json_item_free(json_created);

    return  KORE_RESULT_OK;
}
```

### delete_store

```c
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

    // init pgsql struct
    kore_pgsql_init(&sql);
    
    // extract store_id from url
    int store_id = extract_api_store_id(req->path);
    if ( store_id > 0 ) {
        sprintf(sIdt,"%d",store_id);
        if ( kore_pgsql_setup(&sql, "db", KORE_PGSQL_SYNC) ) {
            // execute query with parameters
            if ( kore_pgsql_query_params(&sql, "delete from magasin where idt=$1 returning idt,nom",0, nParams, sIdt, 0, 0) ) {
                // execute pg cursor to get one row, if no row delete did nothgin
                if ( for_one_tuple(&sql, &bo_store, bo_store_tuple_copy_one_tuple) ) {
                    // convert struct to json
                    if ( bo_store_to_kore_json(&bo_store, json, &errors) ) {
                        // all is ok, fill buf returned with deleted bo_store as json
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

    // send http response, payload containt bo_store created in json
    http_response(req, req->status, buf.data, buf.offset);

    // clean
    kore_pgsql_cleanup(&sql);
    kore_buf_cleanup(&buf);
    kore_buf_cleanup(&errors);
    kore_json_item_free(json);

    return KORE_RESULT_OK;
}
```

### get_store

```c
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

    // init pgsql struct
    kore_pgsql_init(&sql);

    // extract store_id from url
    int store_id = extract_api_store_id(req->path);
    if ( store_id > 0 ) {
        sprintf(sIdt,"%d",store_id);
        if ( kore_pgsql_setup(&sql, "db", KORE_PGSQL_SYNC) ) {
            // execute query with parameters
            if ( kore_pgsql_query_params(&sql, "SELECT idt,nom FROM magasin where idt=$1",0, nParams, sIdt, 0, 0) ) {
                // execute pg cursor to get one row, if no row store_id is not valid
                if ( for_one_tuple(&sql, &bo_store, bo_store_tuple_copy_one_tuple) ) {
                    // convert struct to json
                    if ( bo_store_to_kore_json(&bo_store, json, &errors) ) {
                        // all is ok, fill buf returned with deleted bo_store as json
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

    // send http response, payload containt bo_store created in json
    http_response(req, req->status, buf.data, buf.offset);

    // clean
    kore_pgsql_cleanup(&sql);
    kore_buf_cleanup(&buf);
    kore_buf_cleanup(&errors);
    kore_json_item_free(json);

    return KORE_RESULT_OK;
}
```

### store_isles_list

```c
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

    // extract store_id from url
    int store_id = extract_api_store_id(req->path);
    if ( store_id > 0 ) {
        sprintf(sIdt,"%d",store_id);
        if ( kore_pgsql_setup(&sql, "db", KORE_PGSQL_SYNC) ) {
            // execute query with parameters
            if ( kore_pgsql_query_params(&sql, "SELECT idt,idt_magasin,nom,nom_image FROM rayon where idt_magasin=$1",0, nParams, sIdt, 0, 0) ) {
                // execute pg cursor to get all rows
                // alloc array of stores with callback bo_isle_array_allocator (db_functions.[hc])
                // append datas to array with callback bo_isle_tuple_copy_from_array (db_functions.[hc])
                for_each_tuples(&sql,
                                (void**)&array_of_isles,
                                &count_array_of_isles,
                                bo_isle_array_allocator,
                                bo_isle_tuple_copy_from_array);

                // convert struct to json
                if ( array_bo_isle_to_kore_json(&array_of_isles, count_array_of_isles, json, &errors) ) {
                    // all is ok, fill buf returned with array of bo_isle
                    req->status = HTTP_STATUS_OK;
                    kore_json_item_tobuf(json, &buf);
                } else {
                    kore_buf_append(&buf, errors.data, errors.offset) ;
                }

                // free array of isles allocated by bo_isle_array_allocator callback in for_each_tuples
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

    
    // send http response, payload containt bo_store created in json
    http_response(req, req->status, buf.data, buf.offset);

    // clean
    kore_pgsql_cleanup(&sql);
    kore_buf_cleanup(&buf);
    kore_buf_cleanup(&errors);
    kore_json_item_free(json);

    return KORE_RESULT_OK;
}
```

# postgresql

## init

### linux particularity

It is necessary to start by authorizing the syscall getdents getdents64 by a global call.


```C
#include <kore/kore.h>
#include <kore/http.h>
#include <kore/pgsql.h>
#include <kore/seccomp.h>

int		init(int);
int		page(struct http_request *);

KORE_SECCOMP_FILTER("app",KORE_SYSCALL_ALLOW(getdents),KORE_SYSCALL_ALLOW(getdents64))

int		init(int state) {
	int ret ;
	
	ret = kore_pgsql_register("db","user=kore password=korepwd dbname=magasin host=10.1.1.24 sslmode=disable");
	if ( ret == KORE_RESULT_OK ) {
		printf("database postgres db is register\n");
	} else {
		printf("database postgres db is not register\n");
	}
	return ret;
}
```


# tests

## store

### GET /api/store

#### Curl
```bash
curl -i -k -X GET  https://127.0.0.1:8888/api/store
HTTP/1.1 200 OK
server: kore (master-23d762d6)
connection: keep-alive
keep-alive: timeout=20
strict-transport-security: max-age=31536000; includeSubDomains
content-length: 87

{"stores":["idt":1,"name":"Lille","idt":2,"name":"Gourdon","idt":7,"name":"NewStore7"]}
```

#### backend outputs
```bash
call get_store by url /api/store/[0-9]+ (/api/store/1) with id 1
call stotes_list by url /api/store
```

### GET /api/store/[0-9]+$
#### Curl
```bash
curl -i -k -X GET  https://127.0.0.1:8888/api/store/1
HTTP/1.1 200 OK
server: kore (master-23d762d6)
connection: keep-alive
keep-alive: timeout=20
strict-transport-security: max-age=31536000; includeSubDomains
content-length: 24

{"idt":1,"name":"Lille"}
```

#### backend outputs
```bash
call get_store by url /api/store/[0-9]+ (/api/store/1) with id 1
```

### GET /api/store/[0-9]+$/isles
#### Curl
```bash
curl -i -k -X GET  https://127.0.0.1:8888/api/store/1/isles
HTTP/1.1 200 OK
server: kore (master-23d762d6)
connection: keep-alive
keep-alive: timeout=20
strict-transport-security: max-age=31536000; includeSubDomains
content-length: 489

{"isles":["idt":3,"store_idt":1,"name":"Epicerie","picture":"epicerie","idt":4,"store_idt":1,"name":"Fruits","picture":"fruits","idt":5,"store_idt":1,"name":"Légumes","picture":"legumes","idt":6,"store_idt":1,"name":"Hi-Fi","picture":"hifi","idt":7,"store_idt":1,"name":"Hygiène","picture":"hygiene","idt":8,"store_idt":1,"name":"Papétrie","picture":"papetrie","idt":9,"store_idt":1,"name":"Viandes","picture":"viandes","idt":19,"store_idt":1,"name":"NewIsle1","picture":"newpict.jpg"]}
```

#### backend outputs
```bash
call store_isles_list by url /api/store/[0-9]isle
```

### POST /api/store
#### Curl
```bash
curl -i -k -X POST -d '{"idt": 0, "name": "Store1"}' https://127.0.0.1:8888/api/store
HTTP/1.1 200 OK
server: kore (master-23d762d6)
connection: keep-alive
keep-alive: timeout=20
strict-transport-security: max-age=31536000; includeSubDomains
content-length: 25

{"idt":9,"name":"Store1"}
```

#### backend outputs
```bash
call post_store by url /api/store {"idt":9,"name":"Store1"}
```

### PUT /api/store
#### Curl
```bash
curl -i -k -X PUT -d '{"idt": 9, "name": "NewStore1"}' https://127.0.0.1:8888/api/store
HTTP/1.1 200 OK
server: kore (master-23d762d6)
connection: keep-alive
keep-alive: timeout=20
strict-transport-security: max-age=31536000; includeSubDomains
content-length: 28

{"idt":9,"name":"NewStore1"}
```

#### backend outputs
```bash
call put_store by url /api/store {"idt":9,"name":"NewStore1"}
```

## isle

### GET /api/isle/[0-9]+$
#### Curl
```bash
curl -i -k -X GET  https://127.0.0.1:8888/api/isle/4
HTTP/1.1 200 OK
server: kore (master-23d762d6)
connection: keep-alive
keep-alive: timeout=20
strict-transport-security: max-age=31536000; includeSubDomains
content-length: 58

{"idt":4,"store_idt":1,"name":"Fruits","picture":"fruits"}
```

#### backend outputs
```bash
call get_isle by url /api/isle/[0-9]+ (/api/isle/4) with id 4
```

### GET /api/isle/[0-9]+$/products
#### Curl
```bash
curl -i -k -X GET  https://127.0.0.1:8888/api/isle/4/products
HTTP/1.1 200 OK
server: kore (master-23d762d6)
connection: keep-alive
keep-alive: timeout=20
strict-transport-security: max-age=31536000; includeSubDomains
content-length: 266

{"products":["idt":9,"isle_idt":4,"name":"1Kg de Bananes","picture":"banane","price":0.000000,"idt":10,"isle_idt":4,"name":"1Kg de Clémentines","picture":"clementine","price":0.000000,"idt":11,"isle_idt":4,"name":"1Kg de Pommes","picture":"pomme","price":0.000000]}
```

#### backend outputs
```bash
call isle_products_list by url /api/isle/[0-9]isle
```

### POST /api/isle
#### Curl
```bash
curl -i -k -X POST -d '{"idt": 0, "store_idt": 1, "name": "Isle1", "picture": "pict.jpg"}' https://127.0.0.1:8888/api/isle
TTP/1.1 200 OK
server: kore (master-23d762d6)
connection: keep-alive
keep-alive: timeout=20
strict-transport-security: max-age=31536000; includeSubDomains
content-length: 60

{"idt":20,"store_idt":1,"name":"Isle1","picture":"pict.jpg"}
```

#### backend outputs
```bash
call post_isle by url /api/isle {"idt":20,"store_idt":1,"name":"Isle1","picture":"pict.jpg"}
```

### PUT /api/isle
#### Curl
```bash
curl -i -k -X PUT -d '{"idt": 20, "store_idt": 1, "name": "NewIsle1", "picture": "new-pict.jpg"}' https://127.0.0.1:8888/api/isle
HTTP/1.1 200 OK
server: kore (master-23d762d6)
connection: keep-alive
keep-alive: timeout=20
strict-transport-security: max-age=31536000; includeSubDomains
content-length: 67

{"idt":20,"store_idt":1,"name":"NewIsle1","picture":"new-pict.jpg"}
```

#### backend outputs
```bash
call put_isle by url /api/isle {"idt":20,"store_idt":1,"name":"NewIsle1","picture":"new-pict.jpg"}
```

## product

### GET /api/product/[0-9]+$
#### Curl
```bash
curl -i -k -X GET  https://127.0.0.1:8888/api/product/9
HTTP/1.1 200 OK
server: kore (master-23d762d6)
connection: keep-alive
keep-alive: timeout=20
strict-transport-security: max-age=31536000; includeSubDomains
content-length: 82

{"idt":9,"isle_idt":4,"name":"1Kg de Bananes","picture":"banane","price":4.000000}
```

#### backend outputs
```bash
call get_product by url /api/product/[0-9]+ (/api/product/9) with id 9
```

### POST /api/product
#### Curl
```bash
curl -i -k -X POST -d '{"idt": 0, "isle_idt": 4, "name": "Product1", "picture": "pict.jpg", "price": 3.14}' https://127.0.0.1:8888/api/product
HTTP/1.1 200 OK
server: kore (master-23d762d6)
connection: keep-alive
keep-alive: timeout=20
strict-transport-security: max-age=31536000; includeSubDomains
content-length: 79

{"idt":43,"isle_idt":4,"name":"Product1","picture":"pict.jpg","price":3.140000}
```

#### backend outputs
```bash
call post_product by url /api/product {"idt":43,"isle_idt":4,"name":"Product1","picture":"pict.jpg","price":3.140000}
```

### PUT /api/product
#### Curl
```bash
curl -i -k -X PUT -d '{"idt": 43, "isle_idt": 4, "name": "NewProduct1", "picture": "new-pict.jpg", "price": 6.28}' https://127.0.0.1:8888/api/product
HTTP/1.1 200 OK
server: kore (master-23d762d6)
connection: keep-alive
keep-alive: timeout=20
strict-transport-security: max-age=31536000; includeSubDomains
content-length: 86

{"idt":43,"isle_idt":4,"name":"NewProduct1","picture":"new-pict.jpg","price":6.280000}
```

#### backend outputs
```bash
call put_product by url /api/product {"idt":43,"isle_idt":4,"name":"NewProduct1","picture":"new-pict.jpg","price":6.280000}
```

## Deletes

### DELETE /api/product/[0-9]+$
#### Curl
```bash
curl -i -k -X DELETE  https://127.0.0.1:8888/api/product/43
HTTP/1.1 200 OK
server: kore (master-23d762d6)
connection: keep-alive
keep-alive: timeout=20
strict-transport-security: max-age=31536000; includeSubDomains
content-length: 86

{"idt":43,"isle_idt":4,"name":"NewProduct1","picture":"new-pict.jpg","price":6.280000}
```

#### backend outputs
```bash
call delete_product by url /api/product/[0-9]+ (/api/product/43) with id 43
```

### DELETE /api/isle/[0-9]+$
#### Curl
```bash
curl -i -k -X DELETE  https://127.0.0.1:8888/api/isle/20
HTTP/1.1 200 OK
server: kore (master-23d762d6)
connection: keep-alive
keep-alive: timeout=20
strict-transport-security: max-age=31536000; includeSubDomains
content-length: 67

{"idt":20,"store_idt":1,"name":"NewIsle1","picture":"new-pict.jpg"}
```

#### backend outputs
```bash
call delete_isle by url /api/isle/[0-9]+ (/api/isle/20) with id 20
```

### DELETE /api/store/[0-9]+$
#### Curl
```bash
curl -i -k -X DELETE  https://127.0.0.1:8888/api/store/9
HTTP/1.1 200 OK
server: kore (master-23d762d6)
connection: keep-alive
keep-alive: timeout=20
strict-transport-security: max-age=31536000; includeSubDomains
content-length: 28

{"idt":9,"name":"NewStore1"}
```

#### backend outputs
```bash
call delete_store by url /api/store/[0-9]+ (/api/store/9) with id 9
```

