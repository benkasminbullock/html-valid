use warnings;
use strict;
use utf8;
use FindBin '$Bin';
use Test::More;
my $builder = Test::More->builder;
binmode $builder->output,         ":utf8";
binmode $builder->failure_output, ":utf8";
binmode $builder->todo_output,    ":utf8";
binmode STDOUT, ":encoding(utf8)";
binmode STDERR, ":encoding(utf8)";

use HTML::Valid::Tagset qw/attributes all_attributes/;

run ('a', [qw/onmouseover href/]);
run ('b', [qw/onmouseover/]);

# Run with a version.

my $htmlattrv2 = attributes ('html', standard => 'html2');
ok ($htmlattrv2, "got attributes using standard");
ok (scalar (@$htmlattrv2) == 2, "got two attributes for <html> tag version 2");

my $attributes = all_attributes ();
ok ($attributes, "got all attributes");
ok (ref $attributes eq 'ARRAY', "got array reference");

done_testing ();
exit;

sub run
{
    my ($tag, $expect) = @_;
    my %h;
    my $attr = attributes ($tag);
    ok ($attr, "got attributes for <$tag>");
    ok (ref $attr eq 'ARRAY', "got array reference");
    for (@$attr) {
	$h{$_} = 1;
    }
    for (@$expect) {
	ok ($h{$_}, "got expected attribute $_ for <$tag>");
    }
}
