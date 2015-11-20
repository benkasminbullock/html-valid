#!/home/ben/software/install/bin/perl
use warnings;
use strict;
use Perl::Build;
perl_build (
    pre => './tools/make-c-file.pl',
    make_pod => './tools/make-pod.pl',
# pod => [
# 	'lib/HTML/Valid.pod',
# 	'lib/HTML/Valid/Tagset.pod',
#     ],
    clean => './tools/clean.pl',
);
exit;
