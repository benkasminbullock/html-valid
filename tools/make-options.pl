#!/home/ben/software/install/bin/perl
use warnings;
use strict;
use utf8;
use HTML::Entities;
use Deploy 'do_system';
use Table::Readable 'read_table';
use FindBin '$Bin';
use JSON::Create 'create_json';
use boolean;

# options we don't want.

my $reject_re = 

qr/
      # Disable all "encoding" stuff, including "bom".
      encoding
  |
      bom
  |
      # Invalid option, causes segmentation faults if one tries to use
      # it.
      ^doctype-mode$
  |
      # Disable all "file" stuff.
      file
  |
      # Disable keep-time option and other time-related
      time
  |
      # This disables one file option.
      ^write-back$
  /x;

if (! -f "$Bin/../tidy-html5.o") {
    die "No tidy-html5.o";
}
do_system ("chdir $Bin;cc -o get-option-doc -I ../ ../tidy-html5.o get-option-doc.c");
my $options = "$Bin/options.txt";
do_system ("$Bin/get-option-doc > $options");
my @options = read_table ($options);
for (@options) {
    if ($_->{name} =~ $reject_re) {
	print "Rejecting $_->{name}.\n";
    }
}
@options = grep {$_->{name} !~ $reject_re} @options;
@options = sort {$a->{name} cmp $b->{name}} @options;
open my $out, ">:encoding(utf8)", "$Bin/../tmpl/options.pod" or die $!;
for (@options) {
    my $doc = $_->{doc};
    # Turn HTML into POD
    $doc =~ s!<br\s*/>!\n\n!g;
    $doc =~ s!<code>(.*?)</code>!C<< $1 >>!g;
    $doc =~ s!<em>(.*?)</em>!I<$1>!g;
    $doc =~ s!<strong>(.*?)</strong>!B<$1>!g;
    $doc =~ s!<a\s*href\s*=\s*"(.*?)"\s*>\s*(.*?)</a>!L<$2|$1>!gsm;
#    if ($doc =~ /[^CIBL]<(.*)>/ ) {
#	warn "HTML tag <$1> remains.\n";
#    }
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
my %ok_options;
for (@options) {
    $ok_options{$_->{name}} = true;
}
my $jc = JSON::Create->new ();
$jc->bool ('boolean');
my $json = $jc->run (\%ok_options);
open my $jout, ">:encoding(utf8)", "$Bin/../lib/HTML/Valid/ok-options.json"
    or die $!;
print $jout $json;
close $jout or die $!;    
exit;
