typedef struct {
    TidyDoc tdoc;
    int n_mallocs;
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

    TidyBuffer tidy_output = {0};
    TidyBuffer tidy_errbuf = {0};

    /* First set these up sanely in case the stuff hits the fan. */

    * output_ptr = & PL_sv_undef;
    * errors_ptr = & PL_sv_undef;

    /* Work around bug where allocator sometimes does not get set. */

    CopyAllocator (htv->tdoc, & tidy_output);
    CopyAllocator (htv->tdoc, & tidy_errbuf);

    html_string = SvPV (html, html_length);
    CALL_TIDY (tidySetErrorBuffer (htv->tdoc, & tidy_errbuf));
    htv->n_mallocs++;
    CALL_TIDY (tidyParseString (htv->tdoc, html_string));
    CALL_TIDY (tidyCleanAndRepair (htv->tdoc));
    CALL_TIDY (tidyRunDiagnostics (htv->tdoc));
    CALL_TIDY (tidySaveBuffer (htv->tdoc, & tidy_output));
    htv->n_mallocs++;

    /* Copy the contents of the buffers into the Perl scalars. */

    output = newSVpv ((char *) tidy_output.bp, tidy_output.size);
    errors = newSVpv ((char *) tidy_errbuf.bp, tidy_errbuf.size);

    /* HTML Tidy randomly segfaults here due to "allocator" not being
       set in some cases, hence the above CopyAllocator fix. */

    tidyBufFree (& tidy_output);
    htv->n_mallocs--;
    tidyBufFree (& tidy_errbuf);
    htv->n_mallocs--;

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
    tidyRelease (htv->tdoc);
    htv->n_mallocs--;
    return html_valid_ok;
}

