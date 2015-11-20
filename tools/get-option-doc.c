/* This program extracts the documentation for the options from the C
   library. */

#include <stdio.h>
#include <ctype.h>
#include <stdarg.h>
#include "tidy-html5.h"

int main ()
{
    TidyIterator pos;
    TidyDoc tdoc;
    int count;
    count = 0;
    tdoc = tidyCreate();    
    pos = tidyGetOptionList( tdoc );
    while (1) {
	TidyOption to;
	TidyOptionType tot;
	int ti;
	const char * name;
	const char * option_default;
	int option_default_int;
	Bool option_default_bool;
	to = tidyGetNextOption (tdoc, & pos);
	if (to == 0) {
	    break;
	}
	tot = tidyOptGetType (to);
	ti = tidyOptGetId (to);
	name = tidyOptGetName (to);
	printf ("name: %s\n", name);
	printf ("id: %d\n", ti);
	switch (tot) {
	case TidyString:
	    printf ("type: string\n");
	    option_default = tidyOptGetDefault (to);
	    if (option_default) {
		printf ("default: %s\n", option_default);
	    }
	    else {
		printf ("default: <empty>\n");
	    }
	    break;
	case TidyInteger:
	    printf ("type: integer\n");
	    option_default_int = tidyOptGetDefaultInt (to);
	    printf ("default: %d\n", option_default_int);
	    break;
	case TidyBoolean:
	    printf ("type: true or false\n");
	    option_default_bool = tidyOptGetDefaultInt (to);
	    if (option_default_bool == 0) {
		printf ("default: false\n");
	    }
	    else {
		printf ("default: true\n");
	    }
	    break;
	default:
	    fprintf (stderr, "# Unknown value for option type %d.\n", tot);
	    exit (1);
	}
	const char * doc;
	const TidyOptionId * xrefs;
	get_option_doc (ti, & doc, & xrefs);
	if (doc) {
	    printf ("%%%%doc:\n%s\n%%%%\n", doc);
	    if (xrefs) {
		printf ("xrefs:");
		int j;
		for (j = 0; xrefs[j]; j++) {
		    printf (" %d", xrefs[j]);
		}
	    }
	    printf ("\n");
	}
	else {
	    printf ("doc: undocumented\n");
	}
	printf ("\n");
    }

    return 0;
}
