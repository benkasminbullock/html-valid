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
use JSON::Parse 'json_file_to_perl';
our $VERSION = '0.01';
require XSLoader;
XSLoader::load ('HTML::Valid', $VERSION);

sub new
{
    my ($class, %options) = @_;
    my $htv = html_valid_new ();
    bless $htv;
    for my $k (keys %options) {
	$htv->set_option ($k, $options{$k});
    }
    return $htv;
}

sub read_ok_options
{
    my $ok_options_file = __FILE__;
    $ok_options_file =~ s!Valid\.pm$!Valid/ok-options.json!;
    return json_file_to_perl ($ok_options_file);
}

my $ok_options;

sub set_option
{
    my ($htv, $option, $value) = @_;
    $option =~ s/_/-/g;
    if (! $ok_options) {
	$ok_options = read_ok_options ();
    }
    if ($ok_options->{$option}) {
	$htv->set_option_unsafe ($option, $value);
    }
    else {
	warn "Unknown or disallowed option $option";
    }
}

1;
