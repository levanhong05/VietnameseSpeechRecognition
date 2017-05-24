#!/usr/bin/perl -w

my ($hmmdefs1, $macros1, $hmmdefs2, $macros2, $streaminfo, $vecsize);

# check usage
if (@ARGV != 4) {
  print "usage: $0 sourceHmmdef destHmmdef sourceMacros destMacros\n";
  exit(0);
}

# read in command line arguments
($hmmdefs1, $hmmdefs2, $macros1, $macros2) = @ARGV;

use File::Copy;

copy("$hmmdefs1","$hmmdefs2");
copy("$macros1","$macros2");

 $okprint = "FALSE";
 $silprint = "FALSE";

#string variable for storing sp model
$sp = "";

 unless (open(FILE, "$hmmdefs2")) {
   die ("can't open $hmmdefs2");
 }
 
 
 while ($line = <FILE>) {
  chop ($line);
  
 if ($line =~ /STREAMINFO/) {
   $streaminfo = $line;
 }

 if ($line =~ /VECSIZE/) {
   $vecsize = $line;
 }

 if ($line =~ /"sil"/) {
   $okprint = "TRUE";
   $sp .= "~h \"sp\"\n";
   $sp .= "<BEGINHMM>\n";
   $sp .= "<NUMSTATES> 3\n";
   $sp .= "<STATE> 2\n";
 }

 if ($okprint eq "TRUE" && ($line =~ /<STATE> 3/)) {
   $silprint = "TRUE";
   $line = <FILE>;
   chop ($line);
   #printf NEWFILE "$line\n";
 } 

 if ($okprint eq "TRUE" && ($line =~ /<STATE> 4/)) {
   $silprint = "FALSE";
 } 

 if ($okprint eq "TRUE" && ($line =~ /TRANSP/)) {
   $silprint = "FALSE";
   $sp .= "<TRANSP> 3\n";
   $sp .= " 0.000000e+00 5.000000e-01 5.000000e-01\n";
   $sp .= " 0.000000e+00 5.000000e-01 5.000000e-01\n";
   $sp .= " 0.000000e+00 0.000000e+00 0.000000e+00\n";
 } 

 if ($okprint eq "TRUE" && ($line =~ /ENDHMM/)) {
     $sp .= "$line\n";
   $silprint = "FALSE";
   $okprint = "FALSE";
 } 


 if ($silprint eq "TRUE") {
    $sp .= "$line\n";
 }

}

close(FILE);

unless (open(FILE, ">>$hmmdefs2")) {
  die ("can't open $hmmdefs2");
}

print FILE "$sp";

close(FILE);
 
 

