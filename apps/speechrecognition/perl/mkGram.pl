#!/usr/bin/perl
#
# make a .hed script to clone monophones in a phone list 
# 
# rachel morton 6.12.96


if (@ARGV != 2){
  print "usage: makehed monolist trilist\n\n"; 
  exit (0);
}

($wordlist, $gramout) = @ARGV;

# open word list
open(IN, $wordlist);

# open gram.txt
open(OUT, ">$gramout");
# 
my $line;
my $gram ;
my $flg ;
$flg = 1;
while ($line = <IN>) 
{
    if ($flg)
	{
		$gram = '$digit = '.substr($line,0,length($line)-1);
		$flg  = 0;
	}
	else
	{
		$gram .= "|".substr($line,0,length($line)-1);
	}
}
$gram .= ";\n";
$gram .='(<$digit>)';
print $gram;
printf(OUT "%s\n" ,$gram);