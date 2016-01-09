#!/home/ben/software/install/bin/perl
use warnings;
use strict;
use Template;
use FindBin '$Bin';
use Perl::Build qw/get_version get_commit/;
use Perl::Build::Pod ':all';
use Deploy qw/do_system older/;
use Getopt::Long;
use Path::Tiny;
BEGIN: {
    use FindBin '$Bin';
    use lib $Bin;
    use HVB ':all';
};

$Bin =~ m!tools/?$! or die;
my $base = path ("$Bin/..");

my $ok = GetOptions (
    'force' => \my $force,
    'verbose' => \my $verbose,
);
if (! $ok) {
    usage ();
    exit;
}
my %inputs = (base => $base);
my $version = get_version (%inputs);
my $commit = get_commit (%inputs);

# Names of the input and output files containing the documentation.

my @inputs = (
    "$base/lib/HTML/Valid.pod.tmpl",
    "$base/lib/HTML/Valid/Tagset.pod.tmpl",
);

# Template toolkit variable holder

my %vars;

$vars{version} = $version;
$vars{commit} = $commit;
$vars{html_tidy_version} = html_tidy_version ();

my $tt = Template->new (
    ABSOLUTE => 1,
    INCLUDE_PATH => [
	$base,
	pbtmpl (),
	"$base/examples",
	"$base/tmpl",
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

my @examples = <$base/examples/*.pl>;
for my $example (@examples) {
    my $output = $example;
    $output =~ s/\.pl$/-out.txt/;
    if (older ($output, $example) || $force) {
	do_system ("perl -I$base/blib/lib -I$base/blib/arch $example > $output", $verbose);
    }
}

#system ("$Bin/make-options.pl") == 0 or die "make-options.pl failed";

for my $input (@inputs) {
    my $module = $input;
    $module =~ s/\.pod.*$//;
    $module =~ s!^.*lib/!!;
    $module =~ s!/!::!g;
    $vars{module} = $module;
    my $output = $input;
    $output =~ s/\.tmpl$//;
    if (-f $output) {
	chmod 0644, $output or die $!;
    }
    $tt->process ($input, \%vars, $output, binmode => 'utf8')
        or die '' . $tt->error ();
    chmod 0444, $output or die $!;
}

exit;

sub usage
{
    print <<EOF;
--verbose
--force
EOF
}
