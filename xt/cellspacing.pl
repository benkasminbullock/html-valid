#!/home/ben/software/install/bin/perl
use warnings;
use strict;
use utf8;
use FindBin '$Bin';
use HTML::Valid::Tagset qw/attributes/;
my $atr = attributes ('table');
my %atr;
@atr{@$atr} = (1) x (@$atr);
for my $field (qw/cellspacing onmouseover/) {
if ($atr{$field}) {
    print "$field OK\n";
}
else {
    print "No $field.\n";
}
}


