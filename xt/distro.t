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
use warnings;
use strict;
use Test::More;
use FindBin '$Bin';

# Check that OPTIMIZE is not defined in Makefile.PL.

my $file = "$Bin/../Makefile.PL";
open my $in, "<", $file or die $!;
while (<$in>) {
    if (/OPTIMIZE|-Wall/) {
	like ($_, qr/^\s*#/, "Commented out optimize/-Wall in Makefile.PL");
    }
}
close $in or die $!;
ok (1);
done_testing ();
