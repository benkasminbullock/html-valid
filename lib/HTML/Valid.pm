package HTML::Valid;
require Exporter;
@ISA = qw(Exporter);
@EXPORT_OK = qw//;
%EXPORT_TAGS = (
    all => \@EXPORT_OK,
);
use warnings;
use strict;
use Carp;
our $VERSION = '0.00_01';
require XSLoader;
XSLoader::load ('HTML::Valid', $VERSION);

sub new
{
    my $htv = html_valid_new ();
    bless $htv;
    return $htv;
}


1;
