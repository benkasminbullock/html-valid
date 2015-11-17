#!/home/ben/software/install/bin/perl
use warnings;
use strict;
use Template;
use FindBin '$Bin';
use Perl::Build 'get_version';
use Perl::Build::Pod ':all';
use Deploy qw/do_system older/;
use Getopt::Long;
my $ok = GetOptions (
    'force' => \my $force,
    'verbose' => \my $verbose,
);
if (! $ok) {
    usage ();
    exit;
}

my $version = get_version ();

# Names of the input and output files containing the documentation.

my $pod = 'Create.pod';
my @inputs = (
    "$Bin/lib/HTML/Valid.pod.tmpl",
    "$Bin/lib/HTML/Valid/Tagset.pod.tmpl",
);

# Template toolkit variable holder

my %vars;

$vars{version} = $version;
$vars{html_tidy_version} = '5.0.0';

my $tt = Template->new (
    ABSOLUTE => 1,
    INCLUDE_PATH => [
	$Bin,
	pbtmpl (),
	"$Bin/examples",
	"$Bin/tmpl",
    ],
    ENCODING => 'UTF8',
    FILTERS => {
        xtidy => [
            \& xtidy,
            0,
        ],
    },
    STRICT => 1,
);

my @examples = <$Bin/examples/*.pl>;
for my $example (@examples) {
    my $output = $example;
    $output =~ s/\.pl$/-out.txt/;
    if (older ($output, $example) || $force) {
	do_system ("perl -I$Bin/blib/lib -I$Bin/blib/arch $example > $output", $verbose);
    }
}

for my $input (@inputs) {
    my $output = $input;
    $output =~ s/\.tmpl$//;
    $tt->process ($input, \%vars, $output, binmode => 'utf8')
        or die '' . $tt->error ();
}

exit;

sub usage
{
print <<EOF;
--verbose
--force
EOF
}
