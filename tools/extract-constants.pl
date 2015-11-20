#!/home/ben/software/install/bin/perl
use warnings;
use strict;
use utf8;
use FindBin '$Bin';
use Path::Tiny;
use C::Tokenize ':all';
my $in = '/home/ben/software/tidy-html5-5.0.0/src/lexer.h';
die unless -f $in;
my $stuff = path ($in)->slurp ();
my %values;
print <<EOF;
use constant {
EOF
while ($stuff =~ /($cpp_re)/g) {
    my $cpp = $1;
    chomp $cpp;
    if ($cpp =~ /^#define\s*([A-Z0-9_]+)\s+(.*)/) {
	my $const = $1;
	my $value = $2;
	if ($const !~ /unknown/i && $value !~ /xxxx/) {
	    $value =~ s/u$//;
	    for my $k (sort {length ($b) <=> length ($a)} keys %values) {
		$value =~ s/$k/$values{$k}/g;
	    }
	    print "$const => $value,\n";
	    $values{$const} = $value;
	    next;
	}
    }
    if ($cpp =~ /define/) {
	# Keep a track of rejected macros
	print "# Rejected $cpp\n";
    }
}
print <<EOF;
};
EOF
