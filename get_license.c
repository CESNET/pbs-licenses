#include <stdio.h>
#include <stdlib.h>
#include <pbs_cache_api.h>
#include <sqlite3.h> 

//gcc get_license.c -o get_license -lpbscache -l sqlite3 -g -ggdb

int get_available(char* license_name) {
	long long int v1 = -1, v2 = -1;
    char* value;
    char *i1, *i2;
    value = pbs_cache_get_local(license_name,"dynamic_resources");

    // value = "123456789	1;0"
    if (value != NULL) {
        v1 = strtoll(value,&i1,10);
        v2 = strtoll(i1,&i2,10);
        return v2;
    }

    return 0;
}

int get_used_license(char* license_name) {
    char* sqlite_db="/var/spool/pbs/license.db";
    char sql[300];

    sqlite3 *db;
    sqlite3_stmt *res;
    int rc;
    int used = 0;

    rc = sqlite3_open(sqlite_db, &db);

    if( rc != SQLITE_OK ) {
        //error
        return 0;
    }

    snprintf(sql, sizeof sql, "SELECT SUM(value) FROM licenses WHERE license_name='%s'", license_name);
    //printf("%s\n", sql);
    
    rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);

    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    rc = sqlite3_step(res);

    if (rc == SQLITE_ROW) {
        used = sqlite3_column_int(res, 0);
    } else {
        used = 0;
    }

    sqlite3_close(db);
    return used;
}
 
int main(int argc, char* argv[]) {
    int avail = 0;
    int used = 0;
    int res = 0;

    if (argc != 2) {
        printf("usage %s license_name\n", argv[0]);
        return(0);
    }

    char * license_name = argv[1];

    avail = get_available(license_name);

    used = get_used_license(license_name);
    
    res = avail-used;
    
    if (res > 0)
        printf("%d\n", res);
    else
        printf("0\n");
    
    
    return 0;
}
