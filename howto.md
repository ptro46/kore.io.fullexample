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
```

## l'utilisation dans la fonction handler
```c
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

# test

## store

```bash
# GET /api/store
curl -i -k -X GET  https://127.0.0.1:8888/api/store
call stotes_list by url /api/store

# GET /api/store/[0-9]+$
curl -i -k -X GET  https://127.0.0.1:8888/api/store/1234
call get_store by url /api/store/[0-9]+ (/api/store/1234) with id 1234

# GET /api/store/[0-9]+$/isles
curl -i -k -X GET  https://127.0.0.1:8888/api/store/1234/isles
call store_isles_list by url /api/store/[0-9]+/isle (/api/store/1234/isles) with id 1234

# POST /api/store
curl -i -k -X POST -d '{"idt": 0, "name": "Store1"}' https://127.0.0.1:8888/api/store
call post_store by url /api/store
call post_store by url /api/store {"idt": 0, "name": "Store1"}

# PUT /api/store
curl -i -k -X PUT -d '{"idt" 1, "name": "NewStore1"}}' https://127.0.0.1:8888/api/store
call put_store by url /api/store

# DELETE /api/store/[0-9]+$
curl -i -k -X DELETE  https://127.0.0.1:8888/api/store/1234
call delete_store by url /api/store/[0-9]+ (/api/store/1234) with id 1234
```

## isle

```bash
# GET /api/isle/[0-9]+$
curl -i -k -X GET  https://127.0.0.1:8888/api/isle/1234
call get_isle by url /api/isle/[0-9]+ (/api/isle/1234) with id 1234

# GET /api/isle/[0-9]+$/products
curl -i -k -X GET  https://127.0.0.1:8888/api/isle/1234/products
call isle_products_list by url /api/isle/[0-9]+/isle (/api/isle/1234/products) with id 1234

# POST /api/isle
curl -i -k -X POST -d '{"idt": 0, "store_idt": 1, "name": "Isle1", "picture": "pict.jpg"}' https://127.0.0.1:8888/api/isle
call post_isle by url /api/isle
call post_store by url /api/isle {"idt": 0, "store_idt": 1, "name": "Isle1", "picture": "pict.jpg"}

# PUT /api/isle
curl -i -k -X PUT -d '{"idt": 1, "store_idt": 1, "name": "NewIsle1", "picture": "pict.jpg"}' https://127.0.0.1:8888/api/isle
call put_isle by url /api/isle

# DELETE /api/isle/[0-9]+$
curl -i -k -X DELETE  https://127.0.0.1:8888/api/isle/1234
call delete_isle by url /api/isle/[0-9]+ (/api/isle/1234) with id 1234
```

## product

```bash
# GET /api/product/[0-9]+$
curl -i -k -X GET  https://127.0.0.1:8888/api/product/1234
call get_product by url /api/product/[0-9]+ (/api/product/1234) with id 1234

# POST /api/product
curl -i -k -X POST -d '{"idt": 0, "isle_idt": 1, "name": "Product1", "picture": "pict.jpg", "price": 3.14}' https://127.0.0.1:8888/api/product
call post_product by url /api/product
call post_store by url /api/product {"idt": 0, "isle_idt": 1, "name": "Product1", "picture": "pict.jpg", "price": 3.140000}

# PUT /api/product
curl -i -k -X PUT -d '{"idt": 0, "isle_idt": 1, "name": "NewProduct1", "picture": "pict.jpg", "price": 3.14}' https://127.0.0.1:8888/api/product
call put_product by url /api/product

# DELETE /api/product/[0-9]+$
curl -i -k -X DELETE  https://127.0.0.1:8888/api/product/1234
call delete_product by url /api/product/[0-9]+ (/api/product/1234) with id 1234
```
