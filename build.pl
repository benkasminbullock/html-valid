#!/home/ben/software/install/bin/perl
use warnings;
use strict;
use Perl::Build;
perl_build (
    pre => './make-c-file.pl',
    pod => ['lib/HTML/Valid.pod',],
    clean => './clean.pl',
);
exit;
