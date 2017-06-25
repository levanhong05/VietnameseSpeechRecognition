#!/usr/bin/perl
# Creates a list of all combinations of monophones,
# left and right biphones, and triphones.  This
# assume sp is context free, sil is context independent,
# and all others are context dependent.
#
# Copyright 2005 by Keith Vertanen

#use strict;

if ( @ARGV != 2 ) {
    print "$0 <monophone list without sp> \n"; 
    exit(1);
}

my $listFile;
my @mono;

($listFile,$fulllist) = @ARGV;

my $line;
open(IN, $listFile);
open(OUT, ">$fulllist");

my $monoCount = 0;

# Output all monophones including sp
while($line = <IN>) {
    $line =~ s/[\n\r]//g;

    if (length($line) > 0) {
        $mono[$monoCount] = $line;
        $monoCount++;
    }
}

$mono[$monoCount] = "sp";
$monoCount++;

close(IN);

print OUT "sil\n";
print OUT "sp\n";

# Triphones
my $i;
my $j;
my $k;

for ($i = 0; $i < $monoCount; $i++) {
    for ($j = 0; $j < $monoCount; $j++) {
        if ($mono[$j] !~ /sil/) {
            for ($k = 0; $k < $monoCount; $k++) {
                print OUT $mono[$i] . "-" . $mono[$j] . "+" . $mono[$k] . "\n";
            }
        }
    }
}


close(OUT);
