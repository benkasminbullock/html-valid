typedef struct {
    TidyDoc tdoc;
    TidyBuffer output;
    TidyBuffer errbuf;
    int n_mallocs;
    /* HTML Tidy cannot work out whether it has allocated a buffer by
       itself, we have to do that for it, so these track whether we
       can call the HTML Tidy freeing routines or not. Calling the
       buffer freeing routines with unallocated buffers (zeroed
       buffers) causes segmentation violation errors. */
    unsigned output_allocated : 1;
    unsigned errbuf_allocated : 1;
}
html_valid_t;

typedef enum html_valid_status {
    html_valid_ok,
    html_valid_memory_failure,
    html_valid_tidy_error,
}
html_valid_status_t;

#if 0

static int
perl_error_handler (const char * file, int line_number, const char * msg, ...)
{
    va_list args;
    va_start (args, msg);
    vcroak (msg, & args);
    va_end (args);
    return 0;
}

#endif /* 0 */

static html_valid_status_t
html_valid_create (html_valid_t * htv)
{
    htv->tdoc = tidyCreate ();
    htv->n_mallocs++;
    return html_valid_ok;
}

/* This is "example.c". */

#define CALL_TIDY(x) {					\
	int rc;						\
	rc = x;						\
	if (rc < 0) {					\
	    warn ("Error %d from tidy library", rc);	\
	    return html_valid_tidy_error;		\
	}						\
    }


static html_valid_status_t
html_valid_run (html_valid_t * htv, SV * html,
		SV ** output_ptr, SV ** errors_ptr)
{
    const char * html_string;
    STRLEN html_length;
    SV * output;
    SV * errors;
    /* First set these up sanely in case the stuff hits the fan. */
    * output_ptr = & PL_sv_undef;
    * errors_ptr = & PL_sv_undef;
    html_string = SvPV (html, html_length);
    CALL_TIDY (tidySetErrorBuffer (htv->tdoc, & htv->errbuf));
    htv->errbuf_allocated = 1;
    htv->n_mallocs++;
    CALL_TIDY (tidyParseString (htv->tdoc, html_string));
    CALL_TIDY (tidyCleanAndRepair (htv->tdoc));
    CALL_TIDY (tidyRunDiagnostics (htv->tdoc));
    CALL_TIDY (tidySaveBuffer (htv->tdoc, & htv->output));
    htv->output_allocated = 1;
    htv->n_mallocs++;
    output = newSVpv ((char *) htv->output.bp, htv->output.size);
    errors = newSVpv ((char *) htv->errbuf.bp, htv->errbuf.size);
    /* These are not our mallocs, they are Perl's mallocs, so we don't
       increase htv->n_mallocs for these. After we return them, we no
       longer take care of these. */
    * output_ptr = output;
    * errors_ptr = errors;
    return html_valid_ok;
}

static html_valid_status_t
html_valid_destroy (html_valid_t * htv)
{
    if (htv->output_allocated) {
	tidyBufFree (& htv->output);
	htv->n_mallocs--;
	htv->output_allocated = 0;
    }
    if (htv->errbuf_allocated) {
	tidyBufFree (& htv->errbuf);
	htv->n_mallocs--;
	htv->errbuf_allocated = 0;
    }
    tidyRelease (htv->tdoc);
    htv->n_mallocs--;
    return html_valid_ok;
}

