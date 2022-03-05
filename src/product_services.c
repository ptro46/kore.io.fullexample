#include <kore/kore.h>
#include <kore/http.h>
#include <kore/pgsql.h>
#include <string.h>
#include "business_objects.h"

int		extractApiProductId(const char* path) ;

int		get_product(struct http_request *);
int		post_product(struct http_request *);
int		put_product(struct http_request *);
int		delete_product(struct http_request *);

// extract product_id from url /api/product/[0-9]+$/...
int		extractApiProductId(const char* path) {
	int product_id = -1 ;
	char *tofree = NULL ;
	char *psz = strdup(path);
	if ( NULL == psz ) {
		return product_id ;
	}
	tofree = psz ;

	char *pToken = NULL ;
	pToken = strsep(&psz,"/") ; // first iteration cursor on api in /api/product/[0-9]+$
	if ( pToken == NULL ) {
		free(tofree);
		printf("extractApiProductId error : pToken is null for first iteratio\n");
		return product_id ;
	}
	pToken = strsep(&psz,"/") ; // second iteration cursor on product in /api/product/[0-9]+$
	if ( pToken == NULL ) {
		free(tofree);
		printf("extractApiProductId error : pToken is null for second iteratio\n");
		return product_id ;
	}
	pToken = strsep(&psz,"/") ; // third iteration cursor on id ([0-9]+ in /api/product/[0-9]+$
	if ( pToken == NULL ) {
		free(tofree);
		printf("extractApiProductId error : pToken is null for third iteratio\n");
		return product_id ;
	}
	pToken = strsep(&psz,"/") ; // 4iem iteration cursor on id ([0-9]+ in /api/product/[0-9]+$
	if ( pToken == NULL ) {
		free(tofree);
		printf("extractApiProductId error : pToken is null for 4iem iteratio\n");
		return product_id ;
	}
	product_id = atoi(pToken);
	free(tofree);
	return product_id ;
}

// post /api/product
int		post_product(struct http_request *req) {
	struct kore_buf		buf;
	struct kore_buf		errors;
	struct kore_json	json;
	s_bo_product		o_bo_product;

	req->status = HTTP_STATUS_INTERNAL_ERROR;

	kore_buf_init(&buf, 512);
	kore_buf_init(&errors, 512);
	kore_json_init(&json, req->http_body->data, req->http_body->length);

	if (!kore_json_parse(&json)) {
		kore_buf_appendf(&buf, "post /api/product error : %s\n", kore_json_strerror());
		goto out;
	} else {
		if ( ! json_to_bo_product(&json, &o_bo_product, &errors) ) {
			kore_buf_appendf(&buf, "post /api/product error while unmarshalling json : ");
			kore_buf_append(&buf, errors.data, errors.offset);		   
			goto out;
		} else {
			if ( ! bo_product_to_json_buffer(&o_bo_product, &buf, &errors) ) {
				kore_buf_appendf(&buf, "post /api/product error while marshalling json");
				kore_buf_append(&buf, errors.data, errors.offset);		   
				goto out;				
			} else {
				// here perform POST
				printf("call post_store by url /api/product %s\n",buf.data);
	
				req->status = HTTP_STATUS_OK;
			}
		}
	}

out:
	http_response(req, 200, buf.data, buf.offset);

	kore_buf_cleanup(&errors);
	kore_buf_cleanup(&buf);
	kore_json_cleanup(&json);

	return (KORE_RESULT_OK);
}

// put /api/product
int		put_product(struct http_request *req) {
	struct kore_buf		buf;
	struct kore_buf		errors;
	struct kore_json	json;
	s_bo_product		o_bo_product;

	req->status = HTTP_STATUS_INTERNAL_ERROR;

	kore_buf_init(&buf, 512);
	kore_buf_init(&errors, 512);
	kore_json_init(&json, req->http_body->data, req->http_body->length);

	if (!kore_json_parse(&json)) {
		kore_buf_appendf(&buf, "post /api/product error : %s\n", kore_json_strerror());
		goto out;
	} else {
		if ( ! json_to_bo_product(&json, &o_bo_product, &errors) ) {
			kore_buf_appendf(&buf, "post /api/product error while unmarshalling json : ");
			kore_buf_append(&buf, errors.data, errors.offset);		   
			goto out;
		} else {
			if ( ! bo_product_to_json_buffer(&o_bo_product, &buf, &errors) ) {
				kore_buf_appendf(&buf, "post /api/product error while marshalling json");
				kore_buf_append(&buf, errors.data, errors.offset);		   
				goto out;				
			} else {
				// here perform PUT
				printf("call post_store by url /api/product %s\n",buf.data);
	
				req->status = HTTP_STATUS_OK;
			}
		}
	}

out:
	http_response(req, 200, buf.data, buf.offset);

	kore_buf_cleanup(&errors);
	kore_buf_cleanup(&buf);
	kore_json_cleanup(&json);

	return (KORE_RESULT_OK);
}

// /api/product/[0-9]+$ with [0-9]+ : (product_id)
int		delete_product(struct http_request *req) {
	req->status = HTTP_STATUS_INTERNAL_ERROR;

	int product_id = extractApiProductId(req->path);
	if ( product_id <= 0 ) {
		goto out;
	}
	
	printf("call delete_product by url /api/product/[0-9]+ (%s) with id %d\n",req->path,product_id);
	
	req->status = HTTP_STATUS_OK;

out:
	http_response(req, req->status, NULL, 0);

	return (KORE_RESULT_OK);
}

// /api/product/[0-9]+$ with [0-9]+ : (product_id)
int		get_product(struct http_request *req) {
	req->status = HTTP_STATUS_INTERNAL_ERROR;

	int product_id = extractApiProductId(req->path);
	if ( product_id <= 0 ) {
		goto out;
	}
	
	printf("call get_product by url /api/product/[0-9]+ (%s) with id %d\n",req->path,product_id);
	
	req->status = HTTP_STATUS_OK;

out:
	http_response(req, req->status, NULL, 0);

	return (KORE_RESULT_OK);
}
