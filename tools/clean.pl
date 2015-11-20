#!/home/ben/software/install/bin/perl
use warnings;
use strict;
use utf8;
use FindBin '$Bin';
use Getopt::Long;
GetOptions (
"verbose" => \my $verbose,
);
$Bin =~ m!tools/?$! or die;
chdir "$Bin/../" or die $!;
my @rmfiles = (qw/extra.h tidy-html5.c tidy-html5.h/);
push @rmfiles, <examples/*-out.txt>;
for my $file (@rmfiles) {
    if (-f $file) {
	if ($verbose) {
	    print "$file\n";
	}
	unlink $file or warn "Error unlinking $file: $!";
    }
}
