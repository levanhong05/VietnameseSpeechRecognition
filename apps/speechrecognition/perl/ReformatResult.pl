#!/usr/bin/perl

my $fileIn;
my $fileOut;
my @line;

($fileIn, $fileOut) = @ARGV;

open(IN, $fileIn) || die ("Unable to open file IN");
open(OUT,">$fileOut");
print OUT "#!MLF!#\n";
while ($line = <IN>)
{
    	@words  = split /\s+/, $line; 
	foreach $word ( @words ){
		if ($word =~ /.mfc/)
		{
			$posEnd = index($word, ".mfc");
			$fileName = substr($word, 0, $posEnd);	
			print OUT '"*/'.$fileName.'.rec"'."\n";
		}
		elsif ($word ne "!ENTER" and $word ne "!EXIT" and $word ne "!!UNK" and $word ne "<s>" and $word ne "</s>")
		{
			print OUT $word."\n";
		}
	}
	print OUT ".\n";

}

close IN;
close OUT;


