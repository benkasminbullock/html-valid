/* ------------------------------------------------------- */

/* The following routines are not part of HTML Tidy, they are
   additions for the HTML::Valid Perl module. */

/* Set the file name. */

void SetFilename( TidyDoc tdoc, ctmbstr filename )
{
    TidyDocImpl* impl = tidyDocToImpl( tdoc );
    TY_(SetOptionBool)( impl, TidyEmacs, yes );
    SetOptionValue( impl, TidyEmacsFile, filename );
}

/* Avoid segmentation fault errors which occur due to allocator not
   being set. */

void CopyAllocator( TidyDoc tdoc, TidyBuffer * buf )
{
    TidyDocImpl* impl = tidyDocToImpl( tdoc );
    buf->allocator = impl->allocator;
}

#ifdef HEADER

typedef struct html_valid_tag {
    const char * name;
    unsigned int versions;
    unsigned int model;
}
html_valid_tag_t;

#endif /* def HEADER */

/* We have to subtract 1 from this due to the use of a NULL in the
   final line of tag_defs. */

const int n_html_tags = (sizeof (tag_defs) / sizeof (Dict) - 1);
const int n_attributes = (sizeof (attribute_defs) / sizeof (Attribute) - 1);

/* Export the tag information. */

void TagInformation (html_valid_tag_t * tags)
{
    int i;
    for (i = 0; i < n_html_tags; i++) {
	tags[i].name = tag_defs[i].name;
	tags[i].versions = tag_defs[i].versions;
	tags[i].model = tag_defs[i].model;
    }
}

/* Given a tag id (offset in tag_defs array) and a version bitmap in
   "version", put the attributes for that tag and version into
   "yes_no" where applicable, and the number of tags into
   "n_attr_ptr". */

void TagAttributes (unsigned int tag_id, unsigned int version,
		    const char ** yes_no, int * n_attr_ptr)
{
    int i;
    Dict * tag_def;
    AttrVersion const * attrvers;
    int n_attr;
    if (tag_id >= n_html_tags) {
	fprintf (stderr, "%s:%d: bad tag id %d\n",
		 __FILE__, __LINE__, tag_id);
	return;
    }
    tag_def = & tag_defs[tag_id];
    attrvers = tag_def->attrvers;
    n_attr = 0;
    /* Blank out all the attributes. */
    for (i = 0; i < n_attributes; i++) {
	yes_no[i] = 0;
    }
    for (i = 0; attrvers[i].attribute != TidyAttr_UNKNOWN; i++) {
	if (version & attrvers[i].versions) {
	    int attribute;
	    char * name;
	    attribute = attrvers[i].attribute;

	    /* First check that the number "attribute" makes sense. */

	    if (attribute >= n_attributes || attribute < 0) {
		fprintf (stderr, "%s:%d: Attribute %d of %d "
			 "has attribute id %d "
			 "overflowing the maximum value %d.\n",
			 __FILE__, __LINE__,
			 i, tag_id, attribute, n_attributes);
		return;
	    }
	    name = attribute_defs[attribute].name;

	    if (! yes_no[attribute]) {
		yes_no[attribute] = name;
		n_attr++;
	    }
	}
    }
    * n_attr_ptr = n_attr;
}

/* Make a list of all of the attributes. */

void TagAllAttributes (const char ** yes_no)
{
    int i;
    for (i = 0; i < n_attributes; i++) {
	yes_no[i] = attribute_defs[i].name;
    }
}

void get_option_doc (TidyOptionId ti, const char ** doc, const TidyOptionId ** xrefs)
{
    int i;
    for (i = 0; i < sizeof (option_docs) / sizeof (TidyOptionDoc); i++) {
	if (ti == option_docs[i].opt) {
	    * doc = option_docs[i].doc;
	    * xrefs = option_docs[i].links;
	    return;
	}
    }
    * doc = 0;
    * xrefs = 0;
    fprintf (stderr, "no doc for id %d\n", ti);
}

/* Reset the error counters back to zero. This doesn't seem to be
   available in the API, so after parsing a file with an error, the
   next file is listed as having an error too. Thus basically a tdoc
   becomes non-functional after reading a file with an error. It seems
   too obvious a bug to have been neglected, but I cannot find
   anywhere in the API which does this trivial task. */

void reset_doc (TidyDoc tdoc)
{
    TidyDocImpl* impl = tidyDocToImpl( tdoc );
    impl->errors = 0;
    impl->warnings = 0;
    impl->accessErrors = 0;
    impl->docErrors = 0;
}
