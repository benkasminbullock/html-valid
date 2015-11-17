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

#ifndef CM_UNKNOWN
/* content model shortcut encoding

   Descriptions are tentative.
*/
#define CM_UNKNOWN      0
/* Elements with no content. Map to HTML specification. */
#define CM_EMPTY        (1 << 0)
/* Elements that appear outside of "BODY". */
#define CM_HTML         (1 << 1)
/* Elements that can appear within HEAD. */
#define CM_HEAD         (1 << 2)
/* HTML "block" elements. */
#define CM_BLOCK        (1 << 3)
/* HTML "inline" elements. */
#define CM_INLINE       (1 << 4)
/* Elements that mark list item ("LI"). */
#define CM_LIST         (1 << 5)
/* Elements that mark definition list item ("DL", "DT"). */
#define CM_DEFLIST      (1 << 6)
/* Elements that can appear inside TABLE. */
#define CM_TABLE        (1 << 7)
/* Used for "THEAD", "TFOOT" or "TBODY". */
#define CM_ROWGRP       (1 << 8)
/* Used for "TD", "TH" */
#define CM_ROW          (1 << 9)
/* Elements whose content must be protected against white space movement.
   Includes some elements that can found in forms. */
#define CM_FIELD        (1 << 10)
/* Used to avoid propagating inline emphasis inside some elements
   such as OBJECT or APPLET. */
#define CM_OBJECT       (1 << 11)
/* Elements that allows "PARAM". */
#define CM_PARAM        (1 << 12)
/* "FRAME", "FRAMESET", "NOFRAMES". Used in ParseFrameSet. */
#define CM_FRAMES       (1 << 13)
/* Heading elements (h1, h2, ...). */
#define CM_HEADING      (1 << 14)
/* Elements with an optional end tag. */
#define CM_OPT          (1 << 15)
/* Elements that use "align" attribute for vertical position. */
#define CM_IMG          (1 << 16)
/* Elements with inline and block model. Used to avoid calling InlineDup. */
#define CM_MIXED        (1 << 17)
/* Elements whose content needs to be indented only if containing one 
   CM_BLOCK element. */
#define CM_NO_INDENT    (1 << 18)
/* Elements that are obsolete (such as "dir", "menu"). */
#define CM_OBSOLETE     (1 << 19)
/* User defined elements. Used to determine how attributes wihout value
   should be printed. */
#define CM_NEW          (1 << 20)
/* Elements that cannot be omitted. */
#define CM_OMITST       (1 << 21)

/* If the document uses just HTML 2.0 tags and attributes described
** it as HTML 2.0 Similarly for HTML 3.2 and the 3 flavors of HTML 4.0.
** If there are proprietary tags and attributes then describe it as
** HTML Proprietary. If it includes the xml-lang or xmlns attributes
** but is otherwise HTML 2.0, 3.2 or 4.0 then describe it as one of the
** flavors of Voyager (strict, loose or frameset).
*/

/* unknown */
#define xxxx                   0u

/* W3C defined HTML/XHTML family document types */
#define HT20                   1u
#define HT32                   2u
#define H40S                   4u
#define H40T                   8u
#define H40F                  16u
#define H41S                  32u
#define H41T                  64u
#define H41F                 128u
#define X10S                 256u
#define X10T                 512u
#define X10F                1024u
#define XH11                2048u
#define XB10                4096u

/* proprietary stuff */
#define VERS_SUN            8192u
#define VERS_NETSCAPE      16384u
#define VERS_MICROSOFT     32768u

/* special flag */
#define VERS_XML           65536u

/* HTML5 */
#define HT50              131072u
#define XH50              262144u

/* compatibility symbols */
#define VERS_UNKNOWN       (xxxx)
#define VERS_HTML20        (HT20)
#define VERS_HTML32        (HT32)
#define VERS_HTML40_STRICT (H40S|H41S|X10S)
#define VERS_HTML40_LOOSE  (H40T|H41T|X10T)
#define VERS_FRAMESET      (H40F|H41F|X10F)
#define VERS_XHTML11       (XH11)
#define VERS_BASIC         (XB10)
/* HTML5 */
#define VERS_HTML5         (HT50|XH50)

/* meta symbols */
#define VERS_HTML40        (VERS_HTML40_STRICT|VERS_HTML40_LOOSE|VERS_FRAMESET)
#define VERS_IFRAME        (VERS_HTML40_LOOSE|VERS_FRAMESET)
#define VERS_LOOSE         (VERS_HTML20|VERS_HTML32|VERS_IFRAME)
#define VERS_EVENTS        (VERS_HTML40|VERS_XHTML11)
#define VERS_FROM32        (VERS_HTML32|VERS_HTML40)
#define VERS_FROM40        (VERS_HTML40|VERS_XHTML11|VERS_BASIC)
#define VERS_XHTML         (X10S|X10T|X10F|XH11|XB10|XH50)

/* all W3C defined document types */
#define VERS_ALL           (VERS_HTML20|VERS_HTML32|VERS_FROM40|XH50|HT50)

/* all proprietary types */
#define VERS_PROPRIETARY   (VERS_NETSCAPE|VERS_MICROSOFT|VERS_SUN)

#endif /* ndef CN_UNKNOWN */

typedef struct html_valid_tag {
    const char * name;
    uint versions;
    uint model;
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
//	fprintf (stderr, "%s %u %u\n", tags[i].name, tags[i].versions, tags[i].model);
    }
}

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

/*
	    fprintf (stderr, "Tag %d (%s) has attribute %d (%s)\n",
		     tag_id, tag_defs[tag_id].name, attribute,
		     name);
*/
	    if (! yes_no[attribute]) {
		yes_no[attribute] = name;
//		fprintf (stderr, "Adding name number %d at %d - %s\n",
//			 n_attr, attribute, name);
		n_attr++;
	    }
	    else {
		// Commented out for 5.0.0 since there are several
		// collisions.
//		fprintf (stderr, "%s:%d: Collision %s\n", __FILE__, __LINE__, yes_no[attribute]);
	    }
	}
    }
    * n_attr_ptr = n_attr;
}

void TagAllAttributes (const char ** yes_no)
{
    int i;
    for (i = 0; i < n_attributes; i++) {
	yes_no[i] = attribute_defs[i].name;
    }
}
