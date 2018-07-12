#include "extern.h"
#include "jsmn.h"
#include "jsmn.c"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int set_config(char *c_file);
char* pull_file(char *fname);
static int set_option(jsmnerr_t ret, jsmntok_t *t, char *config);
static int eval_json(char *config, jsmntok_t *tok_key, jsmntok_t *tok_val, const char *opt_name);
static int jsoneq(const char *json, jsmntok_t *tok, const char *s);

/* Option names must remain in order with config.json 
 * TODO: Dynamically generate this list.
 */
const char* options[] = {"auto open", "pattern match", "case sensitivity", "permission errors", 
                            "search user files", "search system files"};


int set_config(char *c_file)
{
    int i, j, opt;
    char *config;
	jsmn_parser p;
    jsmnerr_t ret;
    jsmntok_t *tp;
	jsmntok_t t[128];  /* We expect no more than 128 tokens */

    tp = t;
    config = pull_file(c_file);

	jsmn_init(&p);
	ret = jsmn_parse(&p, config, strlen(config), t, sizeof(t)/sizeof(t[0]));
	if (ret < 0) {
		printf("Failed to parse JSON: %d\n", ret);
		return 1;
	}

	/* Assume the top-level element is an object */
	if (ret < 1 || t[0].type != JSMN_OBJECT) {
		printf("Object expected\n");
		return 1;
	}

    set_option(ret, tp, config);
    free(config);

    return 0;
}


/* Fill an array with file contents. */
char* pull_file(char *fname)
{
    FILE *fp;
    char *buf;
    size_t sz;

    fp = fopen(fname, "r");
    fseek(fp, 0L, SEEK_END);
    sz = ftell(fp);

    // You are responsible for freeing 'buf' when it is passed to another function
    buf = malloc(sizeof(char) * sz + 1);
    *(buf + sz + 1) = '\0';

    rewind(fp);
    fread(buf, sz, 1, fp);

    if (fclose(fp) != 0)
        perror("fclose");
    return buf;
}


static int set_option(jsmnerr_t ret, jsmntok_t *t, char *config)
{
    int i, j, opt;
    
    /* Loop over all keys of the root object */
	for (i = 1, j = 0; i < ret; i++, j++) {
        if ((opt = eval_json(config, &t[i], &t[i + 1], options[j])) != 0) {

            // Cases must be listed in order with lines in config.json
            switch(j) {
                case 0:
                    option.openf = (opt) ? 1 : 0;
                    break;
                case 1:
                    option.grep = (opt) ? 1 : 0;
                    break;
                case 2:
                    option.csens = (opt) ? 1 : 0;
                    break;
                case 3:
                    option.perm = (opt) ? 1 : 0;
                    break;
                case 4:
                    option.home = (opt) ? 1 : 0;
                    break;
                case 5:
                    option.sys = (opt) ? 1 : 0;
                    break;
                default:

                    // Will only occur if for loop fails to iterate through 'j'
                    printf("Error log: set_config() in config.c failed.\n");
                    exit(1);
            }
        }
        i++; // Skip to next key, value pair (&t[i], &t[i+1])
	}
    return 0;
}


static int eval_json(char *config, jsmntok_t *tok_key, jsmntok_t *tok_val, const char *opt_name)
{
    int ret = 0;
    if (jsoneq(config, tok_key, opt_name) == 0) {
            if (!strncmp("true", config + tok_val->start, tok_val->end-tok_val->start))
			    ret++;
            else if (!strncmp("false", config + tok_val->start, tok_val->end-tok_val->start))
                ret--;
            else
                printf("Error log: Invalid value for \"%s\" in config.json\n", opt_name);
        }
    return ret;
}


static int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
	if (tok->type == JSMN_STRING && (int) strlen(s) == tok->end - tok->start &&
			strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
		return 0;
	}
	return -1;
}
