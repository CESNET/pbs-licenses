#include <stdio.h>
#include <stdlib.h>
#include <pbs_cache_api.h>
#include <sqlite3.h> 
#include <string.h>

//gcc get_license.c -o get_license -lpbscache -l sqlite3 -g -ggdb

long long int *get_cache(char* license_name) {
    char* value;
    char *i1, *i2;
    long long int *v = calloc(3, sizeof(long long int));

    if (v == NULL) {
        return v;
    }

    value = pbs_cache_get_local(license_name,"dynamic_resources");

    // value = "123456789	1;0"
    if (value != NULL) {
        v[0] = strtoll(value,&i1,10);
        v[1] = strtoll(i1,&i2,10);
        if (*i2 == ';' && strlen(++i2) > 0) {
            v[2] = strtoll(i2,NULL,10);
        } else {
            v[2] = 0;
        }
        return v;
    }

    return v;
}

int get_reserved(char* license_name) {
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
    int reserved = 0;
    int used = 0;
    int not_used_yet = 0;
    int res = 0;
    long long int *cache;

    if (argc != 2) {
        printf("usage %s license_name\n", argv[0]);
        return(0);
    }

    char * license_name = argv[1];

    cache = get_cache(license_name);
    if (cache == NULL) {
        return 0;
    }

    avail = cache[1];
    used = cache[2];
    free(cache);

    reserved = get_reserved(license_name);

    not_used_yet = reserved - used;

    if (not_used_yet < 0) {
        not_used_yet = 0;
    }

    res = avail - not_used_yet;
    
    if (res > 0)
        printf("%d\n", res);
    else
        printf("0\n");
    
    return 0;
}
