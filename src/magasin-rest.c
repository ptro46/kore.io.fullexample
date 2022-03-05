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

