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
use Path::Tiny;
my $cfile = "$Bin/../tidy-html5.c";
my %fincludes;
open my $in, "<", $cfile or die "No $cfile: $!";
while (<$in>) {
    if (m!/*\s*(\w+\.h) --!) {
	$fincludes{$1}++;
    }
}
close $in or die $!;
for my $k (keys %fincludes) {
    ok ($fincludes{$k} == 1, "Only included $k once");
}
done_testing ();
