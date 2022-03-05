#include <kore/kore.h>
#include <kore/http.h>
#include <kore/pgsql.h>
#include <string.h>
#include "business_objects.h"

int		get_isle(struct http_request *);
int		isle_products_list(struct http_request *);
int		extractApiIsleId(const char* path) ;

int		post_isle(struct http_request *);
int		put_isle(struct http_request *);
int		delete_isle(struct http_request *);

// extract isle_id from url /api/isle/[0-9]+$/...
int		extractApiIsleId(const char* path) {
	int isle_id = -1 ;
	char *tofree = NULL ;
	char *psz = strdup(path);
	if ( NULL == psz ) {
		return isle_id ;
	}
	tofree = psz ;

	char *pToken = NULL ;
	pToken = strsep(&psz,"/") ; // first iteration cursor on api in /api/isle/[0-9]+$
	if ( pToken == NULL ) {
		free(tofree);
		printf("extractApiIsleId error : pToken is null for first iteratio\n");
		return isle_id ;
	}
	pToken = strsep(&psz,"/") ; // second iteration cursor on isle in /api/isle/[0-9]+$
	if ( pToken == NULL ) {
		free(tofree);
		printf("extractApiIsleId error : pToken is null for second iteratio\n");
		return isle_id ;
	}
	pToken = strsep(&psz,"/") ; // third iteration cursor on id ([0-9]+ in /api/isle/[0-9]+$
	if ( pToken == NULL ) {
		free(tofree);
		printf("extractApiIsleId error : pToken is null for third iteratio\n");
		return isle_id ;
	}
	pToken = strsep(&psz,"/") ; // 4iem iteration cursor on id ([0-9]+ in /api/isle/[0-9]+$
	if ( pToken == NULL ) {
		free(tofree);
		printf("extractApiIsleId error : pToken is null for 4iem iteratio\n");
		return isle_id ;
	}
	isle_id = atoi(pToken);
	free(tofree);
	return isle_id ;
}

// post /api/isle
int		post_isle(struct http_request *req) {
	struct kore_buf		buf;
	struct kore_buf		errors;
	struct kore_json	json;
	s_bo_isle			o_bo_isle;

	req->status = HTTP_STATUS_INTERNAL_ERROR;

	kore_buf_init(&buf, 512);
	kore_buf_init(&errors, 512);
	kore_json_init(&json, req->http_body->data, req->http_body->length);

	if (!kore_json_parse(&json)) {
		kore_buf_appendf(&buf, "post /api/isle error : %s\n", kore_json_strerror());
		goto out;
	} else {
		if ( ! json_to_bo_isle(&json, &o_bo_isle, &errors) ) {
			kore_buf_appendf(&buf, "post /api/isle error while unmarshalling json : ");
			kore_buf_append(&buf, errors.data, errors.offset);		   
			goto out;
		} else {
			if ( ! bo_isle_to_json_buffer(&o_bo_isle, &buf, &errors) ) {
				kore_buf_appendf(&buf, "post /api/isle error while marshalling json");
				kore_buf_append(&buf, errors.data, errors.offset);		   
				goto out;				
			} else {				
				// here perform POST
				printf("call post_store by url /api/isle %s\n",buf.data);
	
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

// put /api/isle
int		put_isle(struct http_request *req) {
	struct kore_buf		buf;
	struct kore_buf		errors;
	struct kore_json	json;
	s_bo_isle			o_bo_isle;

	req->status = HTTP_STATUS_INTERNAL_ERROR;

	kore_buf_init(&buf, 512);
	kore_buf_init(&errors, 512);
	kore_json_init(&json, req->http_body->data, req->http_body->length);

	if (!kore_json_parse(&json)) {
		kore_buf_appendf(&buf, "post /api/isle error : %s\n", kore_json_strerror());
		goto out;
	} else {
		if ( ! json_to_bo_isle(&json, &o_bo_isle, &errors) ) {
			kore_buf_appendf(&buf, "post /api/isle error while unmarshalling json : ");
			kore_buf_append(&buf, errors.data, errors.offset);		   
			goto out;
		} else {
			if ( ! bo_isle_to_json_buffer(&o_bo_isle, &buf, &errors) ) {
				kore_buf_appendf(&buf, "post /api/isle error while marshalling json");
				kore_buf_append(&buf, errors.data, errors.offset);		   
				goto out;				
			} else {
				// here perform PUT
				printf("call post_store by url /api/isle %s\n",buf.data);
	
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

// /api/isle/[0-9]+$ with [0-9]+ : (isle_id)
int		delete_isle(struct http_request *req) {
	req->status = HTTP_STATUS_INTERNAL_ERROR;

	int isle_id = extractApiIsleId(req->path);
	if ( isle_id <= 0 ) {
		goto out;
	}
	
	printf("call delete_isle by url /api/isle/[0-9]+ (%s) with id %d\n",req->path,isle_id);
	
	req->status = HTTP_STATUS_OK;

out:
	http_response(req, req->status, NULL, 0);

	return (KORE_RESULT_OK);
}

// /api/isle/[0-9]+$ with [0-9]+ : (isle_id)
int		get_isle(struct http_request *req) {
	req->status = HTTP_STATUS_INTERNAL_ERROR;

	int isle_id = extractApiIsleId(req->path);
	if ( isle_id <= 0 ) {
		goto out;
	}
	
	printf("call get_isle by url /api/isle/[0-9]+ (%s) with id %d\n",req->path,isle_id);
	
	req->status = HTTP_STATUS_OK;

out:
	http_response(req, req->status, NULL, 0);

	return (KORE_RESULT_OK);
}

// /api/isle/[0-9]+/products [0-9]+ : (isle_id)
int		isle_products_list(struct http_request *req) {
	req->status = HTTP_STATUS_INTERNAL_ERROR;

	int isle_id = extractApiIsleId(req->path);
	if ( isle_id <= 0 ) {
		goto out;
	}
	
	printf("call isle_products_list by url /api/isle/[0-9]+/isle (%s) with id %d\n",req->path,isle_id);
	
	req->status = HTTP_STATUS_OK;

out:
	http_response(req, req->status, NULL, 0);

	return (KORE_RESULT_OK);	
}
