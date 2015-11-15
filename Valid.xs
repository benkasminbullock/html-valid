#include "EXTERN.h"
#include "perl.h"
#include "XSUB.h"
#include "ppport.h"

#include "html-valid-perl.c"

typedef html_valid_t * HTML__Valid;

MODULE=HTML::Valid PACKAGE=HTML::Valid

PROTOTYPES: DISABLE

BOOT:
	/* HTML__Valid_error_handler = perl_error_handler; */

