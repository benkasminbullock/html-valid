#!/home/ben/software/install/bin/perl
use warnings;
use strict;
use utf8;
use FindBin '$Bin';
use Path::Tiny;
my $in = '/home/ben/software/tidy-html5-5.0.0/src/attrs.c';
my @lines = path ($in)->lines ();
my %attr2type;
for (@lines) {
    chomp;
    if (/\{\s*TidyAttr_(?:\S+),\s*"([^"]+)",\s*CH_(\S+)\s*\},/) {
	if ($attr2type{$1}) {
	    print "Collision on $1\n";
	}
	$attr2type{$1} = lc $2;
    }
    else {
#	print "ok $_\n";
    }
}

print "my %attr2type = {\n";
for my $k (sort keys %attr2type) {
    print "    '$k' => '$attr2type{$k}',\n";
}
print "};\n";

