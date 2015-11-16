/* Set the file name. */

void SetFilename( TidyDoc tdoc, ctmbstr filename )
{
    TidyDocImpl* impl = tidyDocToImpl( tdoc );
    TY_(SetOptionBool)( impl, TidyEmacs, yes );
    SetOptionValue( impl, TidyEmacsFile, filename );
}

/* Avoid segmentation fault errors which occur due to allocator not being set. */

void CopyAllocator( TidyDoc tdoc, TidyBuffer * buf )
{
    TidyDocImpl* impl = tidyDocToImpl( tdoc );
    buf->allocator = impl->allocator;
}
