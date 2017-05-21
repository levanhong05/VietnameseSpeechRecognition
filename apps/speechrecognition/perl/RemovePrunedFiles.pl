#!/usr/bin/perl

# Checks that every file in the script file is actually in the MLF file.
#
# Copyright 2005 by Keith Vertanen
# tuanbul modify to use for vov corpus

use strict;

if ( @ARGV < 2 )
{
    print "$0 <MLF file> <original script file> [output not in]\n"; 
    exit(1);
}

my $MLFFile;
my $scriptFile;
my $posStart;
my $posEnd;
my $line;
my %files;
my $filename;
my $invert;
my $count;
($MLFFile, $scriptFile, $invert) = @ARGV;
open(OUT, ">$invert") || die ("Unable to open $invert file for writing");
# Read in all the filename lines from the MLF
open(IN, $MLFFile);
$count = 0;
while ($line = <IN>) 
{
    $line =~ s/\n//g;
    $posStart = index($line, "*/");
	
	if ($posStart > 0)
    {
		$posEnd = index($line, ".lab");
		if ($posEnd == -1)
		{
		    $posEnd = index($line, ".rec");
		}
		 
		if ($posEnd > 0)
		{
		
		    $filename = substr($line, $posStart + 2, $posEnd - $posStart - 2);			
			#print $filename."\n";
		    $files{$filename} = 1;
			$count++;
		}
    }
	
}
print "Total file in mlf file is $count";
close IN;
$count = 0;
open(IN, $scriptFile);
while ($line = <IN>) 
{
    $line =~ s/\n//g;

	#print $line;
	$posStart = index($line, "Mfc\\")+4;
	$posEnd = rindex($line, ".mfc");
	if ($posEnd > 0)
	{
	    $filename = substr($line, $posStart, $posEnd - $posStart );
		#print $filename;
		if ($files{$filename})
		{
			print $filename."\n";
			$count++;
		    printf(OUT "%s\n" ,$line);
		}
		else
		{
		   print "not found: $filename on $line\n";
		}
    }	
	
}
print "Total train file is $count";
close IN;
close OUT;
