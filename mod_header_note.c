/* Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * mod_header_note.c
 *
 * Copyright 2014 Hiroyuki Kusu <kusu0806@gmail.com>
 * Author: Hiroyuki Kusu <kusu0806@gmail.com>
 *
 * mod_header_note is Apache 2.2 module that rewrites to Apache-note the value of a HTTP-response-header.
 *
 */

#include "httpd.h"
#include "http_config.h"
#include "http_protocol.h"
#include "ap_config.h"

#include "util_filter.h"
#include "apr_tables.h"
#include "apr_strings.h"
#include "http_log.h"

#define THIS_FILTER_NAME "HEADER_NOTE_OUTPUT_FILTER"
#define HEADER_PREFIX    "x_hdrnt_"

static int callback_set_notes(void *data, const char *key, const char *val) {
    request_rec *req = (request_rec *)data;
    char *lowered_key;

    lowered_key = apr_pstrdup(req->pool, (char *)key);
    ap_str_tolower(lowered_key);

    if (strncmp(lowered_key, HEADER_PREFIX, strlen(HEADER_PREFIX)) == 0) {
        apr_table_set(req->notes, key, val);
    }

    return 1;
}

static int callback_remove_headers_out(void *data, const char *key, const char *val) {
    request_rec *req = (request_rec *)data;
    char *lowered_key;

    lowered_key = apr_pstrdup(req->pool, (char *)key);
    ap_str_tolower(lowered_key);

    if (strncmp(lowered_key, HEADER_PREFIX, strlen(HEADER_PREFIX)) == 0) {
        apr_table_unset(req->headers_out, key);
    }

    return 1;
}

static apr_status_t header_note_output_filter(ap_filter_t *f, apr_bucket_brigade *in) {
    request_rec *req = f->r;

    if (APR_BRIGADE_EMPTY(in)) {
        return APR_SUCCESS;
    }

    if (!apr_is_empty_table(req->headers_out)) {
        apr_table_do(callback_set_notes, req, req->headers_out, NULL);
    }

    if (!apr_is_empty_table(req->notes)) {
        apr_table_do(callback_remove_headers_out, req, req->notes, NULL);
    }

    ap_remove_output_filter(f);

    return ap_pass_brigade(f->next, in);
}

static void header_note_register_hooks(apr_pool_t *p)
{
    /* output filter */
    ap_register_output_filter(THIS_FILTER_NAME, header_note_output_filter, NULL, AP_FTYPE_RESOURCE);
}

/* Dispatch list for API hooks */
module AP_MODULE_DECLARE_DATA header_note_module = {
    STANDARD20_MODULE_STUFF,
    NULL,                       /* create per-dir    config structures */
    NULL,                       /* merge  per-dir    config structures */
    NULL,                       /* create per-server config structures */
    NULL,                       /* merge  per-server config structures */
    NULL,                       /* table of config file commands       */
    header_note_register_hooks  /* register hooks                      */
};
