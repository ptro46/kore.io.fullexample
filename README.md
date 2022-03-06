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

# Create project
kodev create magasin-rest

# gestion du cas avec un parametre dans l'url (int)
exemples
```bash
curl -i -k -X GET  https://127.0.0.1:8888/api/store/1234
curl -i -k -X GET  https://127.0.0.1:8888/api/store/1234/isle
```

## il faut commencer par parametrer les routes
```bash
	route ^/api/store/[0-9]+$ {
		handler get_store
	}

	route ^/api/store/[0-9]+/isle$ {
		handler store_isles_list
	}
```

## ensuite faire une fonction qui va aller chercher l'id dans l'url par strsep (strtok)
```c
// extract store_id from url /api/store/[0-9]+$/...
int		extract_api_store_id(const char* path) {
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
```

## l'utilisation dans la fonction handler
```c
// /api/store/[0-9]+$ with [0-9]+ : (magasin_id)
int		get_store(struct http_request *req) {
	req->status = HTTP_STATUS_INTERNAL_ERROR;

	int store_id = extract_api_store_id(req->path);
	if ( store_id <= 0 ) {
		goto out;
	}
	
	printf("call get_store by url /api/store/[0-9]+ (%s) with id %d\n",req->path,store_id);
	
	req->status = HTTP_STATUS_OK;

out:
	http_response(req, req->status, NULL, 0);

	return (KORE_RESULT_OK);
}
```

## exemple d'appel

```bash
call get_store by url /api/store/[0-9]+ (/api/store/1234) with id 1234
call store_isles_list by url /api/store/[0-9]+/isle (/api/store/1234/isle) with id 1234
```

# meme URL avec POST / GET / PUT / DELETE


## il faut commencer par parametrer les routes
```bash
	route ^/api/store$ {
		handler stores_list
		methods get
	}

	route ^/api/store$ {
		handler post_store
		methods post
	}
```

## Ensuite coder les fonctions
```C
// get /api/store
int		stores_list(struct http_request *req) {
	req->status = HTTP_STATUS_INTERNAL_ERROR;

	printf("call stotes_list by url /api/store \n");
	
	req->status = HTTP_STATUS_OK;

//out:
	http_response(req, req->status, NULL, 0);

	return (KORE_RESULT_OK);
}

// post /api/store
int		post_store(struct http_request *req) {
	req->status = HTTP_STATUS_INTERNAL_ERROR;

	printf("call post_store by url /api/store \n");
	
	req->status = HTTP_STATUS_OK;

//out:
	http_response(req, req->status, NULL, 0);

	return (KORE_RESULT_OK);
}
```

## Tester
```bash
curl -i -k -X GET  https://127.0.0.1:8888/api/store
curl -i -k -X POST -d '{"foo":{"bar": "Hello world"}}' https://127.0.0.1:8888/api/store
```

## Resultat
```bash
call stotes_list by url /api/store
call post_store by url /api/store
```

# Exemple complet

## routing
```bash
	route ^/api/store$ {
		handler stores_list
		methods get
	}

	route ^/api/store$ {
		handler post_store
		methods post
	}

	route ^/api/store$ {
		handler put_store
		methods put
	}

	route ^/api/store/[0-9]+$ {
		handler delete_store
		methods delete
	}

	route ^/api/store/[0-9]+$ {
		handler get_store
	}
```

## Code
```C
int		stores_list(struct http_request *);
int		get_store(struct http_request *);
int		extractApiStoreId(const char* path) ;

int		post_store(struct http_request *);
int		put_store(struct http_request *);
int		delete_store(struct http_request *);

// extract store_id from url /api/store/[0-9]+$/...
int		extractApiStoreId(const char* path) {
	int store_id = -1 ;
	char *tofree = NULL ;
	char *psz = strdup(path);
	if ( NULL == psz ) {
		return store_id ;
	}
	tofree = psz ;

	char *pToken = NULL ;
	pToken = strsep(&psz,"/") ; // first iteration cursor on api in /api/store/[0-9]+$
	if ( pToken == NULL ) {
		free(tofree);
		printf("get_store error : pToken is null for first iteratio\n");
		return store_id ;
	}
	pToken = strsep(&psz,"/") ; // second iteration cursor on store in /api/store/[0-9]+$
	if ( pToken == NULL ) {
		free(tofree);
		printf("get_store error : pToken is null for second iteratio\n");
		return store_id ;
	}
	pToken = strsep(&psz,"/") ; // third iteration cursor on id ([0-9]+ in /api/store/[0-9]+$
	if ( pToken == NULL ) {
		free(tofree);
		printf("get_store error : pToken is null for third iteratio\n");
		return store_id ;
	}
	pToken = strsep(&psz,"/") ; // 4iem iteration cursor on id ([0-9]+ in /api/store/[0-9]+$
	if ( pToken == NULL ) {
		free(tofree);
		printf("get_store error : pToken is null for 4iem iteratio\n");
		return store_id ;
	}
	store_id = atoi(pToken);
	free(tofree);
	return store_id ;
}

// get /api/store
int		stores_list(struct http_request *req) {
	req->status = HTTP_STATUS_INTERNAL_ERROR;

	printf("call stotes_list by url /api/store \n");
	
	req->status = HTTP_STATUS_OK;

//out:
	http_response(req, req->status, NULL, 0);

	return (KORE_RESULT_OK);
}

// post /api/store
int		post_store(struct http_request *req) {
	req->status = HTTP_STATUS_INTERNAL_ERROR;

	printf("call post_store by url /api/store \n");
	
	req->status = HTTP_STATUS_OK;

//out:
	http_response(req, req->status, NULL, 0);

	return (KORE_RESULT_OK);
}

// post /api/store
int		put_store(struct http_request *req) {
	req->status = HTTP_STATUS_INTERNAL_ERROR;

	printf("call put_store by url /api/store \n");
	
	req->status = HTTP_STATUS_OK;

//out:
	http_response(req, req->status, NULL, 0);

	return (KORE_RESULT_OK);
}

// /api/store/[0-9]+$ with [0-9]+ : (magasin_id)
int		delete_store(struct http_request *req) {
	req->status = HTTP_STATUS_INTERNAL_ERROR;

	int store_id = extractApiStoreId(req->path);
	if ( store_id <= 0 ) {
		goto out;
	}
	
	printf("call delete_store by url /api/store/[0-9]+ (%s) with id %d\n",req->path,store_id);
	
	req->status = HTTP_STATUS_OK;

out:
	http_response(req, req->status, NULL, 0);

	return (KORE_RESULT_OK);
}

// /api/store/[0-9]+$ with [0-9]+ : (magasin_id)
int		get_store(struct http_request *req) {
	req->status = HTTP_STATUS_INTERNAL_ERROR;

	int store_id = extractApiStoreId(req->path);
	if ( store_id <= 0 ) {
		goto out;
	}
	
	printf("call get_store by url /api/store/[0-9]+ (%s) with id %d\n",req->path,store_id);
	
	req->status = HTTP_STATUS_OK;

out:
	http_response(req, req->status, NULL, 0);

	return (KORE_RESULT_OK);
}
```

## test
```bash
curl -i -k -X GET  https://127.0.0.1:8888/api/store
curl -i -k -X POST -d '{"foo":{"bar": "Hello world"}}' https://127.0.0.1:8888/api/store
curl -i -k -X PUT -d '{"foo":{"bar": "Hello world"}}' https://127.0.0.1:8888/api/store
curl -i -k -X GET  https://127.0.0.1:8888/api/store/1234
curl -i -k -X DELETE  https://127.0.0.1:8888/api/store/1234

call stotes_list by url /api/store
call post_store by url /api/store
call put_store by url /api/store
call get_store by url /api/store/[0-9]+ (/api/store/1234) with id 1234
call delete_store by url /api/store/[0-9]+ (/api/store/1234) with id 1234
```

# Connection a la base postgres

## Fonction init

### Particularite Linux (seccomp) 

Attention, il faut commencer par autoriser les syscall getdents getdents64 par un appel global.

### Parametrage de la base dans la fonction init


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

