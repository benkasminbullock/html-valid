# HTML Valid Build
package HVB;
use parent Exporter;
our @EXPORT_OK = qw/html_tidy_version/;
our %EXPORT_TAGS = (all => \@EXPORT_OK);
use warnings;
use strict;
use utf8;
use Carp;
use Path::Tiny;
my $dir = __FILE__;
$dir =~ s!/[^/]+$!!;
my $config = path ("$dir/../config.txt");
die unless -f $config;
my @lines = $config->lines ();
my %vars;
for (@lines) {
    if (/^(.+)\s*:\s*(.*)/) {
	$vars{$1}=$2;
    }
}
sub html_tidy_version
{
    return $vars{'html-tidy-version'};
}

1;
