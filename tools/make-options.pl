#!/home/ben/software/install/bin/perl
use warnings;
use strict;
use utf8;
use HTML::Entities;
use Deploy 'do_system';
use Table::Readable 'read_table';
use FindBin '$Bin';
if (! -f "$Bin/../tidy-html5.o") {
    die "No tidy-html5.o";
}
do_system ("chdir $Bin;cc -o get-option-doc -I ../ ../tidy-html5.o get-option-doc.c");
my $options = "$Bin/options.txt";
do_system ("$Bin/get-option-doc > $options");
my @options = read_table ($options);
@options = sort {$a->{name} cmp $b->{name}} @options;
open my $out, ">:encoding(utf8)", "$Bin/../tmpl/options.pod" or die $!;
for (@options) {
my $doc = $_->{doc};
# Turn HTML into POD
$doc =~ s!<br />!\n\n!g;
$doc =~ s!<code>([^<>]*?)</code>!C<$1>!g;
$doc =~ s!<code>(.*?)</code>!C<<$1>>!g;
$doc =~ s!<em>(.*?)</em>!I<$1>!g;
$doc =~ s!<strong>(.*?)</strong>!B<$1>!g;
$doc =~ s!<a\s*href\s*=\s*"(.*?)"\s*>(.*?)</a>!L<$2|$1>!gsm;
if ($doc =~ /[^CIBL]<(.*)>/) {
    warn "HTML tag <$1> remains.\n";
}
# Remove HTML entities.
$doc = decode_entities ($doc);
print $out <<EOF;
=item $_->{name}

    \$htv->set_option ('$_->{name}', <value>);

Type: $_->{type}

Default: $_->{default}

$doc

EOF

}
close $out or die $!;
exit;
