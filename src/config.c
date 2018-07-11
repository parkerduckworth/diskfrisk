#include "extern.h"
#include "jsmn.h"
#include "jsmn.c"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TOKENS 256
#define BUFFER_SIZE 1028

// char *config_file = "../config.json";


char* pull_file(char *fname)
{
    FILE *fp;
    char *buf;
    size_t sz;

    fp = fopen(fname, "r");
    fseek(fp, 0L, SEEK_END);
    sz = ftell(fp);

    buf = malloc(sizeof(char) * sz + 1);
    *(buf + sz + 1) = '\0';

    rewind(fp);
    fread(buf, sz, 1, fp);

    if (fclose(fp) != 0)
        perror("fclose");
    return buf;
}

static int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
	if (tok->type == JSMN_STRING && (int) strlen(s) == tok->end - tok->start &&
			strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
		return 0;
	}
	return -1;
}

int set_config(char *c_file)
{
    int i;
	int r;
    char *config;
	jsmn_parser p;
	jsmntok_t t[128]; /* We expect no more than 128 tokens */

    config = pull_file(c_file);

	jsmn_init(&p);
	r = jsmn_parse(&p, config, strlen(config), t, sizeof(t)/sizeof(t[0]));
	if (r < 0) {
		printf("Failed to parse JSON: %d\n", r);
		return 1;
	}

	/* Assume the top-level element is an object */
	if (r < 1 || t[0].type != JSMN_OBJECT) {
		printf("Object expected\n");
		return 1;
	}


	/* Loop over all keys of the root object */
	for (i = 1; i < r; i++) {
		if (jsoneq(config, &t[i], "auto open") == 0) {
            if (!strncmp("true", config + t[i+1].start, t[i+1].end-t[i+1].start))
			    option.openf = 1;
            else if (!strncmp("false", config + t[i+1].start, t[i+1].end-t[i+1].start))
                option.openf = 0;
            else
                printf("Invalid value for \"auto open\" in settings.\n");
			i++;
        }
		if (jsoneq(config, &t[i], "pattern match") == 0) {
            if (!strncmp("true", config + t[i+1].start, t[i+1].end-t[i+1].start))
			    option.grep = 1;
            else if (!strncmp("false", config + t[i+1].start, t[i+1].end-t[i+1].start))
                option.grep = 0;
            else
                printf("Invalid value for \"patttern match\" in settings.\n");
			i++;
        }        
		if (jsoneq(config, &t[i], "case sensitivity") == 0) {
            if (!strncmp("true", config + t[i+1].start, t[i+1].end-t[i+1].start))
			    option.csens = 1;
            else if (!strncmp("false", config + t[i+1].start, t[i+1].end-t[i+1].start))
                option.csens = 0;
            else
                printf("Invalid value for \"case sensitivity\" in settings.\n");
			i++;
        }
		if (jsoneq(config, &t[i], "permission errors") == 0) {
            if (!strncmp("true", config + t[i+1].start, t[i+1].end-t[i+1].start))
			    option.perm = 1;
            else if (!strncmp("false", config + t[i+1].start, t[i+1].end-t[i+1].start))
                option.perm = 0;
            else
                printf("Invalid value for \"permission errors\" in settings.\n");
			i++;
        }
		if (jsoneq(config, &t[i], "search user files") == 0) {
            if (!strncmp("true", config + t[i+1].start, t[i+1].end-t[i+1].start))
			    option.home = 1;
            else if (!strncmp("false", config + t[i+1].start, t[i+1].end-t[i+1].start))
                option.home = 0;
            else
                printf("Invalid value for \"search user files\" in settings.\n");
			i++;
        }
		if (jsoneq(config, &t[i], "search system files") == 0) {
            if (!strncmp("true", config + t[i+1].start, t[i+1].end-t[i+1].start))
			    option.sys = 1;
            else if (!strncmp("false", config + t[i+1].start, t[i+1].end-t[i+1].start))
                option.sys = 0;
            else
                printf("Invalid value for \"search system files\" in settings.\n");
			i++;
        }

        /* Error catch for invalid key */
        //  if () {
		// 	printf("Unexpected key: %.*s\n", t[i].end-t[i].start,
		// 			config + t[i].start);
		// }
	}

    free(config);
    return 0;
}
