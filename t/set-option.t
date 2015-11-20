use warnings;
use strict;
use utf8;
use FindBin '$Bin';
use Test::More;
my $builder = Test::More->builder;
binmode $builder->output,         ":utf8";
binmode $builder->failure_output, ":utf8";
binmode $builder->todo_output,    ":utf8";
binmode STDOUT, ":encoding(utf8)";
binmode STDERR, ":encoding(utf8)";

use HTML::Valid;
{
    my $warning;
    local $SIG{__WARN__} = sub {
	$warning = "@_";
    };
    my $htv = HTML::Valid->new ();
    $htv->set_option ('jibberjabber', 'stupid monkey value');
    ok ($warning);
    like ($warning, qr/unknown option.*jibberjabber/);
    $warning = undef;
    $htv->set_option ('omit-optional-tags', 1);
    ok (! $warning);
}

done_testing ();
